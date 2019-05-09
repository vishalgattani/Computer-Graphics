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
#include "SOIL/SOIL.h"
#include <glm/glm.hpp>
#include "../include/objloader.hpp"

#define NONE 0
#define CYLINDER 1
#define SPHERE 2
#define CUBE 3

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

OBJLoader::OBJLoader(){
	minx = miny = minz = 0;
    maxx = maxy = maxz = 0;
    minArea = 100;
    maxArea = 0; 

}
OBJLoader::~OBJLoader(void){
    //glDeleteBuffers(1, &vertexbuffer);
	//glDeleteBuffers(1, &uvbuffer);
	//glDeleteBuffers(1, &normalbuffer);

    //cout << "Cleanup..." << endl;
}

bool OBJLoader::loadOBJ(	const char * path, int texType){ 
	printf("Loading PLY file : %s...\n", path);
	textureType = texType;

	//std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	//std::vector<glm::vec3> temp_vertices; 
	//std::vector<glm::vec3> temp_normals;
	
	ifstream file(path);
    bool past_header = false;
    bool flag = false;
    string line,temp1,temp2;
    unsigned long long size;
    int count = 0;
	float dummy;
    float temp_num;
    istringstream ss;

    if ( !file.is_open() ){
        cout << "file could not be opened" << endl;
        exit(1);
    }

    //int noOfVertices, noOfFaces;

	while (true)
	{
		getline(file, line);
		istringstream liness (line);
		liness >> temp1;
		
		if(temp1.compare(string("end_header"))==0)
			break;
		
		if (temp1.compare(string("element")) == 0)
		{
			liness >> temp1;
			liness >> size;
			
			if (temp1.compare(string("vertex")) == 0)
			{
				noOfVertices = size;
				flag = true;
			}
			else if (temp1.compare(string("face")) == 0)
			{
				noOfFaces = size;
				flag = false;
			}	
		}
		else if (temp1.compare(string("property")) == 0 && flag)
			count++;			
	}
	
	for (int i = 0; i<noOfVertices; i++)
	{
		glm::vec3 vertex;
		
		
		double t;
        file >> t;
		vertex.x = t;
		file >> t;
		vertex.y = t;
		file >> t;
		vertex.z = t;
		temp_vertices.push_back(vertex);

        
		
		minx = std::min(minx, vertex.x);
        miny = std::min(miny, vertex.y);
        minz = std::min(minz, vertex.z);
        maxx = std::max(maxx, vertex.x);
        maxy = std::max(maxy, vertex.y);
        maxz = std::max(maxz, vertex.z);		
		
		

		for (int j = 3; j < count; j++)
			file >> dummy;
	}

	centre[0] = (minx + maxx)/2;
    centre[1] = (miny + maxy)/2;	
    centre[2] = (minz + maxz)/2;
	
    for (int i = 0; i<noOfVertices; i++)
    {
        if(textureType==CUBE){
            glm::vec2 vertexuv;
            vertexuv.x = temp_vertices[i].z;
            vertexuv.y = temp_vertices[i].y;
            temp_vertexuv.push_back(vertexuv);
        }

        else if(textureType==CYLINDER){
            double theta = atan((temp_vertices[i].y-centre[1]/(temp_vertices[i].x-centre[0])));
            glm::vec2 vertexuv;
            vertexuv.x = ((theta-1.57)/3.14);
            vertexuv.y = 1 - ((temp_vertices[i].y - miny)/(maxy-miny));
            temp_vertexuv.push_back(vertexuv);
        }

        else if(textureType==SPHERE){
            //double theta = atan((temp_vertices[i].y-centre[1]/(temp_vertices[i].x-centre[0])));
            glm::vec2 vertexuv;
            vertexuv.x = -(0.5 + ((temp_vertices[i].x - centre[0])/(maxx-minx)));
            vertexuv.y = -(0.5 + ((temp_vertices[i].y - centre[1])/(maxy-miny)));
            temp_vertexuv.push_back(vertexuv);
        }
        else if(textureType==NONE){
            double theta = atan((temp_vertices[i].y-centre[1]/(temp_vertices[i].x-centre[0])));
            glm::vec2 vertexuv;
            vertexuv.x = -(0.5 + ((temp_vertices[i].x - centre[0])/((maxx-minx+1))));//* 1/(centre[2]);
            vertexuv.y = -(0.5 + ((temp_vertices[i].y - centre[1])/((maxy-miny+1))));//* 1/(centre[2]);
            temp_vertexuv.push_back(vertexuv);
        }
    }

	int j, num_sides;
	for (int i = 0; i<noOfFaces; i++)
	{
		int jib;
        file >> jib;
        //cout << jib;
        unsigned int vertexIndex[jib];
        for (int j = 0; j < jib; j++)
		{
			file >> vertexIndex[j];
			vertexIndices.push_back(vertexIndex[j]);
		}
	}

	size = 0.0;
    if(size < maxx-minx)
            size = maxx-minx;
    if(size < maxy-miny)
        size = maxy-miny;
    if(size < maxz-minz)
            size = maxz-minz;
    scale = 2.0 / size;

    for(int i = 0; i < temp_vertices.size(); i++){
        temp_vertices[i].x = scale * (temp_vertices[i].x - minx) - 1.0;
        temp_vertices[i].y = scale * (temp_vertices[i].y - minx) - 1.0;
        temp_vertices[i].z = scale * (temp_vertices[i].z - minx) - 1.0;
        //cerr << temp_vertices[i].x << " | " << temp_vertices[i].y << " | " << temp_vertices[i].z << " | " << endl;
    }

    minx = miny = minz = 10000;
    maxx = maxy = maxz = -10000;
    for (int i=0; i<temp_vertices.size(); i++) {
            minx = std::min(minx, temp_vertices[i].x);
            maxx = std::max(maxx, temp_vertices[i].x);
            miny = std::min(miny, temp_vertices[i].y);
            maxy = std::max(maxy, temp_vertices[i].y);
            minz = std::min(minz, temp_vertices[i].z);
            maxz = std::max(maxz, temp_vertices[i].z);
    }
    mx = -(maxx+minx)/2;
	my = -(maxy+miny)/2;

	
    setNormals();
    loadValues();
    computeIn();
    //setupBuffers();
    return true;

}

void OBJLoader::setProgramID(GLuint ID){
    prgmID = ID;
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
        maxArea = std::max(area,maxArea);
        minArea = std::min(area,minArea);
        
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
    //for(int j=0;j<noOfFaces;j++){
        for(int i = 0; i < vertexIndices.size(); i++){
            vec3 vertex,color,normal;
            vec2 texturecoordinate;
            color = vec3(0.1, 0.1, 0.1);
            vertex = temp_vertices[vertexIndices[i]];
            texturecoordinate = temp_vertexuv[vertexIndices[i]];
            normal = temp_normals[i];

            //cout << normal.x << "," << normal.y << "," << normal.z << endl;

          
            out_normals.push_back(normal);
            out_vertices.push_back(vertex);
            out_colors.push_back(color);
            out_uv.push_back(texturecoordinate);
            
        }   
        addBoundingBox(); 
    //}
    
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
	glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexIndices.size() * sizeof(unsigned int), &vertexIndices[0], GL_STATIC_DRAW);
    glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, out_vertices.size() * sizeof(glm::vec3), &out_vertices[0], GL_STATIC_DRAW);
    
    //glGenBuffers(1, &uvbuffer);
	//glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	//glBufferData(GL_ARRAY_BUFFER, out_colors.size() * sizeof(glm::vec3), &out_colors[0], GL_STATIC_DRAW);
    glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, out_uv.size() * sizeof(glm::vec2), &out_uv[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, out_normals.size() * sizeof(glm::vec3), &out_normals[0], GL_STATIC_DRAW);
    
  



	Texture = texInit("can.jpg");
	// Get a handle for our "myTextureSampler" uniform
	TextureID  = glGetUniformLocation(prgmID, "myTextureSampler");
}


void OBJLoader::Draw(){
	// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to use Texture Unit 0
		glUniform1i(TextureID, 0);

        // 1rst attribute buffer : out_vertices
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
		


        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, out_vertices.size() );    
        glBindVertexArray(0);
		
		
}

GLuint OBJLoader::texInit(char * texture)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glActiveTexture(GL_TEXTURE0);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 
				   GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
				   GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image(texture, &width, &height, 0, SOIL_LOAD_RGB);
	
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
              GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image); 
	return textureID;

}

void OBJLoader::addBoundingBox(){
    
    vec3 p1(minx,miny,minz);
    vec3 p2(minx,miny,maxz);
    vec3 p3(maxx,miny,maxz);
    vec3 p4(maxx,miny,minz);
    vec3 p5(minx,maxy,minz);
    vec3 p6(minx,maxy,maxz);
    vec3 p7(maxx,maxy,maxz);
    vec3 p8(maxx,maxy,minz);

   
    out_vertices.push_back(p1);
    out_colors.push_back(vec3(0,1,0));
    out_vertices.push_back(p2);
    out_colors.push_back(vec3(0,1,0));
    out_vertices.push_back(p1);
    out_colors.push_back(vec3(0,1,0));


    out_vertices.push_back(p2);
    out_colors.push_back(vec3(0,1,0));
    out_vertices.push_back(p3);
    out_colors.push_back(vec3(0,1,0));
    out_vertices.push_back(p2);
    out_colors.push_back(vec3(0,1,0));

    out_vertices.push_back(p3);
    out_colors.push_back(vec3(0,1,0));
    out_vertices.push_back(p3);
    out_colors.push_back(vec3(0,1,0));
    out_vertices.push_back(p4);
    out_colors.push_back(vec3(0,1,0));

    out_vertices.push_back(p4);
    out_colors.push_back(vec3(0,1,0));
    out_vertices.push_back(p4);
    out_colors.push_back(vec3(0,1,0));
    out_vertices.push_back(p1);
    out_colors.push_back(vec3(0,1,0));

    out_vertices.push_back(p1);
    out_colors.push_back(vec3(0,1,0));
    out_vertices.push_back(p1);
    out_colors.push_back(vec3(0,1,0));
    out_vertices.push_back(p5);
    out_colors.push_back(vec3(0,1,0));
    
    out_vertices.push_back(p5);
    out_colors.push_back(vec3(0,1,0));
    out_vertices.push_back(p5);
    out_colors.push_back(vec3(0,1,0));
    out_vertices.push_back(p6);
    out_colors.push_back(vec3(0,1,0));
 
    
    out_vertices.push_back(p6);
    out_colors.push_back(vec3(0,1,0));
    out_vertices.push_back(p6);
    out_colors.push_back(vec3(0,1,0));
    out_vertices.push_back(p2);
    out_colors.push_back(vec3(0,1,0));
    
    out_vertices.push_back(p6);
    out_colors.push_back(vec3(0,1,0));
    out_vertices.push_back(p6);
    out_colors.push_back(vec3(0,1,0));
    out_vertices.push_back(p7);
    out_colors.push_back(vec3(0,1,0));

    out_vertices.push_back(p7);
    out_colors.push_back(vec3(0,1,0));
    out_vertices.push_back(p7);
    out_colors.push_back(vec3(0,1,0));
    out_vertices.push_back(p8);
    out_colors.push_back(vec3(0,1,0));
    
    
    out_vertices.push_back(p8);
    out_colors.push_back(vec3(0,1,0));
    out_vertices.push_back(p8);
    out_colors.push_back(vec3(0,1,0));
    out_vertices.push_back(p5);
    out_colors.push_back(vec3(0,1,0));

    out_vertices.push_back(p7);
    out_colors.push_back(vec3(0,1,0));
    out_vertices.push_back(p7);
    out_colors.push_back(vec3(0,1,0));
    out_vertices.push_back(p3);
    out_colors.push_back(vec3(0,1,0));
    

    out_vertices.push_back(p8);
    out_colors.push_back(vec3(0,1,0));
    out_vertices.push_back(p8);
    out_colors.push_back(vec3(0,1,0));
    out_vertices.push_back(p4);
    out_colors.push_back(vec3(0,1,0));
    

}


