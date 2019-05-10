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

#include "geometryshader.hpp"

#include "SOIL.h"


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

class Skybox{
	public:
	
	
   	
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    
    // skybox VAO
    unsigned int skyboxVAO, skyboxVBO;
    unsigned int cubemapTexture;
    Skybox();
    ~Skybox();

};

Skybox::Skybox(){
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
	glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    cubemapTexture = loadCubemap(faces1);
}
