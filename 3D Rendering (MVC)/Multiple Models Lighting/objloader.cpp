#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>
#include <sstream>
#include <fstream>
#include <iostream>
using namespace std;

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "objloader.hpp"

// Very, VERY simple OBJ loader.
// Here is a short list of features a real function would provide : 
// - Binary files. Reading a model should be just a few memcpy's away, not parsing a file at runtime. In short : OBJ is not very great.
// - Animations & bones (includes bones weights)
// - Multiple UVs
// - All attributes should be optional, not "forced"
// - More stable. Change a line in the OBJ file and it crashes.
// - More secure. Change another line and you can inject code.
// - Loading from memory, stream, etc

float findRadiusOfIncircle(float a, float b, float c) 
{ 
  
    // the sides cannot be negative 
    if (a < 0 || b < 0 || c < 0) 
        return -1; 
  
    // semi-perimeter of the circle 
    float p = (a + b + c) / 2; 
  
    // area of the traingle 
    float area = sqrt(p * (p - a) * (p - b) * (p - c)); 
  
    // Radius of the incircle 
    float radius = area / p; 
  
    // Return the radius 
    return radius; 
} 

OBJLoader::OBJLoader(){}
OBJLoader::~OBJLoader(void){
    glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
    cout << "Cleanup..." << endl;
}

bool OBJLoader::loadOBJ(
	const char * path){ 
	printf("Loading PLY file : %s...\n", path);

	//std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	//std::vector<glm::vec3> temp_vertices; 
	//std::vector<glm::vec3> temp_normals;
	
	ifstream file(path);
    bool past_header = false;
    string line,temp1,temp2;
    float temp_num;
    istringstream ss;

    if ( !file.is_open() ){
        cout << "file could not be opened" << endl;
        exit(1);
    }

    int noOfVertices, noOfFaces;

	while( getline(file, line) ){   
        if( line.compare(0,14,"element vertex") == 0 ){
            //cerr << "header vetex read" << endl;
            ss.str(line);
            ss >> temp1 >> temp2 >> noOfVertices;
            cerr << "vertices: " << noOfVertices << endl;
            break;
        }
    }
	while( getline(file, line) ){   
        if( line.compare(0,12,"element face") == 0 ){
            //cerr << "header faces read" << endl;
            ss.str(line);
            ss >> temp1 >> temp2 >> noOfFaces;
            cerr << "faces: " << noOfFaces << endl;
            break;
        }
    }

    while( getline(file, line) ){   
        if( line.compare(0,3,"end") == 0 ){
            cerr << line << endl;
			break;
        }
    }

	for(int i = 0; i < noOfVertices; i++){
        getline(file, line);
        glm::vec3 vertex;
        ss.str(line);
        ss >> vertex.x >> vertex.y >> vertex.z;
        temp_vertices.push_back(vertex);
        //cerr << line << endl;
    }
	//cerr << "\nvertices read = " << temp_vertices.size() << endl;
    
    for(int i = 0; i < noOfFaces; i++ ){
        getline(file, line);
        ss.str(line);
        int jib;
        unsigned int vertexIndex[3];
        ss >> jib >> vertexIndex[0] >> vertexIndex[1] >> vertexIndex[2] ;
        vertexIndices.push_back(vertexIndex[0]);
        vertexIndices.push_back(vertexIndex[1]);
        vertexIndices.push_back(vertexIndex[2]);
    }
    //cerr << "\nfaces read = " << vertexIndices.size() << endl;
    
    setNormals();
    loadValues();
    computeIn();
    setupBuffers();
    return true;

}

void OBJLoader::setNormals(){
    glm::vec3 a,b;
    float magnitude;
    for(int i = 0;i < vertexIndices.size();i+=3 ){
        vec3 cross;
        a = temp_vertices[vertexIndices[i+1]] - temp_vertices[vertexIndices[i]];
        b = temp_vertices[vertexIndices[i+2]] - temp_vertices[vertexIndices[i]]; 
        cross.x = a.y * b.z - b.y * a.z;
        cross.y = a.z * b.x - b.z * a.x;
        cross.z = a.x * b.y - b.x * a.y;
        float area = glm::length(cross);
        
        magnitude = std::sqrt(cross.x * cross.x + cross.y * cross.y + cross.z * cross.z);
        cross /= (magnitude, magnitude, magnitude);

        temp_normals.push_back(cross);
        temp_normals.push_back(cross);
        temp_normals.push_back(cross);

        //if(cross.x!=0)
        //cout << cross.x << " | " << cross.y << " | " << cross.z << " | " << endl;
    }
}



void OBJLoader::loadValues(){
    for(int i = 0; i < vertexIndices.size(); i++){
        vec3 vertex,color,normal;
        color = vec3(0.1, 0.1, 0.1);
        vertex = temp_vertices[vertexIndices[i]];
        normal = temp_normals[i];
        out_normals.push_back(normal);
        out_vertices.push_back(vertex);
        out_colors.push_back(color);
    }
}

void OBJLoader::computeIn(){
    for(int i=0;i < out_vertices.size()/3;i++){
        glm::vec3 A = out_vertices[i*3];
        glm::vec3 B = out_vertices[(i*3)+1];
        glm::vec3 C = out_vertices[(i*3)+2];
        
        glm::vec3 AB = B-A;
        glm::vec3 BC = C-B;
        glm::vec3 CA = A-C;
        
        float c = glm::length(AB);
        float b = glm::length(CA);
        float a = glm::length(BC);
        float p = a+b+c;

        glm::vec4 incenter = vec4(0.0,0.0,0.0,0.0);
        incenter.x = (a*A.x+b*B.x+c*C.x)/p;
        incenter.y = (a*A.y+b*B.y+c*C.y)/p;
        incenter.z = (a*A.z+b*B.z+c*C.z)/p;
        incenter.w = findRadiusOfIncircle(a,b,c);

        incircles.push_back(incenter);
        //cout << " - " << incenter.x << " - " << incenter.y << " - " << incenter.z << " - " << incenter.w << endl;
        
    }
}




void OBJLoader::setupBuffers(){
    // create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexIndices.size() * sizeof(unsigned int), &vertexIndices[0], GL_STATIC_DRAW);
    glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, out_vertices.size() * sizeof(glm::vec3), &out_vertices[0], GL_STATIC_DRAW);
    glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, out_colors.size() * sizeof(glm::vec3), &out_colors[0], GL_STATIC_DRAW);
    glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, out_normals.size() * sizeof(glm::vec3), &out_normals[0], GL_STATIC_DRAW);
    
    // 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			1,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// 3rd attribute buffer : normals
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glVertexAttribPointer(
			2,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

        glBindVertexArray(0);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);    
        //Draw();
}

void OBJLoader::Draw(){
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, out_vertices.size() );
        glBindVertexArray(0);
        glActiveTexture(GL_TEXTURE0);
}



