// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Include file headers
#include "shader.hpp"
#include "controls.hpp"
#include "view.hpp"
#include "objloader.hpp"
#include "geometryshader.hpp"
#include "drawObj.hpp"
#include "windows.hpp"
//#include "new_controller.hpp"

#include "SOIL.h"

#define NONE 0
#define CYLINDER 1
#define SPHERE 2
#define CUBE 3

using namespace glm;
using namespace std;

// lighting
//glm::vec3 lightPos(0.0f, cubeLength, 0.0f);

GLuint texInit(char * texture)
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

unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrComponents;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = SOIL_load_image(faces[i].c_str(), &width, &height, 0, SOIL_LOAD_RGB);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}


int main( void )
{

    
    Window win("Final");
    Controller ctrl(win.get());

    Shader skyboxShader("skybox.vs","skybox.fs");
	
   
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float cubeLength = 1000.0f;
    float skyboxVertices[] = {
        // positions          
        -cubeLength,  cubeLength, -cubeLength,
        -cubeLength, -cubeLength, -cubeLength,
         cubeLength, -cubeLength, -cubeLength,
         cubeLength, -cubeLength, -cubeLength,
         cubeLength,  cubeLength, -cubeLength,
        -cubeLength,  cubeLength, -cubeLength,
        
        -cubeLength, -cubeLength,  cubeLength,
        -cubeLength, -cubeLength, -cubeLength,
        -cubeLength,  cubeLength, -cubeLength,
        -cubeLength,  cubeLength, -cubeLength,
        -cubeLength,  cubeLength,  cubeLength,
        -cubeLength, -cubeLength,  cubeLength,
        
         cubeLength, -cubeLength, -cubeLength,
         cubeLength, -cubeLength,  cubeLength,
         cubeLength,  cubeLength,  cubeLength,
         cubeLength,  cubeLength,  cubeLength,
         cubeLength,  cubeLength, -cubeLength,
         cubeLength, -cubeLength, -cubeLength,
         
        -cubeLength, -cubeLength,  cubeLength,
        -cubeLength,  cubeLength,  cubeLength,
         cubeLength,  cubeLength,  cubeLength,
         cubeLength,  cubeLength,  cubeLength,
         cubeLength, -cubeLength,  cubeLength,
        -cubeLength, -cubeLength,  cubeLength,
        
        -cubeLength,  cubeLength, -cubeLength,
         cubeLength,  cubeLength, -cubeLength,
         cubeLength,  cubeLength,  cubeLength,
         cubeLength,  cubeLength,  cubeLength,
        -cubeLength,  cubeLength,  cubeLength,
        -cubeLength,  cubeLength, -cubeLength,
        
        -cubeLength, -cubeLength, -cubeLength,
        -cubeLength, -cubeLength,  cubeLength,
         cubeLength, -cubeLength, -cubeLength,
         cubeLength, -cubeLength, -cubeLength,
        -cubeLength, -cubeLength,  cubeLength,
         cubeLength, -cubeLength,  cubeLength
    };
    // skybox VAO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // load textures
    // -------------
    vector<std::string> faces1
    {
        "textures/skybox/right.jpg",
        "textures/skybox/left.jpg",
        "textures/skybox/top.jpg",
        "textures/skybox/bottom.jpg",
        "textures/skybox/front.jpg",
        "textures/skybox/back.jpg",
    };
    vector<std::string> faces2
    {
        "textures/skybox1/left.jpg",
        "textures/skybox1/right.jpg",
        "textures/skybox1/top.jpg",
        "textures/skybox1/bottom.jpg",
        "textures/skybox1/front.jpg",
        "textures/skybox1/back.jpg",
    };
    unsigned int cubemapTexture = loadCubemap(faces2);

    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    OBJLoader obj3;
    obj3.loadOBJ("sphere_orig.ply",SPHERE);
    mods.push_back(obj3);

    drawObject* d = new drawObject(vec3(-1.5,0,0) * 3.0f,"./textures/Disco_Ball_001_basecolor.bmp","./textures/Disco_Ball_001_roughness.bmp","./textures/Disco_Ball_001_normal.bmp","./textures/Disco_Ball_001_ambientOcclusion.bmp","rim_frag.shader");
    baseObjects.push_back(d);
    for(int i = 0; i < baseObjects.size(); i++){
        baseObjects[i]->fillBuffers(mods[0]);
    }

    /*
    GLuint vertexbuffer,colorbuffer,uvbuffer,normalbuffer,tangentbuffer,bitangentbuffer;
    unsigned int VAO,VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, obj3.out_vertices.size() * sizeof(glm::vec3), &obj3.out_vertices[0], GL_STATIC_DRAW);
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, obj3.out_colors.size() * sizeof(glm::vec3), &obj3.out_colors[0], GL_STATIC_DRAW);
    glGenBuffers(1, &normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, obj3.out_normals.size() * sizeof(glm::vec3), &obj3.out_normals[0], GL_STATIC_DRAW);
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, obj3.cylindrical_uvs.size() * sizeof(glm::vec2), &obj3.cylindrical_uvs[0], GL_STATIC_DRAW);
    glGenBuffers(1, &tangentbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, tangentbuffer);
    glBufferData(GL_ARRAY_BUFFER, obj3.tangents.size() * sizeof(glm::vec3), &obj3.tangents[0], GL_STATIC_DRAW);
    glGenBuffers(1, &bitangentbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, bitangentbuffer);
    glBufferData(GL_ARRAY_BUFFER, obj3.bitangents.size() * sizeof(glm::vec3), &obj3.bitangents[0], GL_STATIC_DRAW);
    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(4);
    glBindBuffer(GL_ARRAY_BUFFER, tangentbuffer);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(5);
    glBindBuffer(GL_ARRAY_BUFFER, bitangentbuffer);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glBindVertexArray(0);
    */
	
 
    
   

    do{

        

        // render
        // ------
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ctrl.computeMatricesFromInputs();
       
        for(int i = 0; i < baseObjects.size(); i++){
            glUseProgram(baseObjects[i]->programID);
            baseObjects[i]->calcMatrices(ctrl,ctrl.BodyTranslateMatrix ,baseObjects[i]->cached_rotate,ctrl.BodyScaleMatrix);
            baseObjects[i]->draw(mods[0].out_vertices.size());
            glUseProgram(0);
        }
        // bind diffuse map
       

        // draw skybox as last
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.use();
        glm::mat4 projection = ctrl.getProjectionMatrix();
        glm::mat4 viewMatrix = ctrl.getViewMatrix(); // remove translation from the view matrix
        skyboxShader.setMat4("view", viewMatrix);
        skyboxShader.setMat4("projection", projection);
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default



        win.swapBuffer();

       
    }while(win.quit());


}