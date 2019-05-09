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
#include "../include/shader.hpp"
#include "../include/controls.hpp"
#include "../include/view.hpp"
#include "../include/objloader.hpp"
#include "../include/geometryshader.hpp"

#include "../include/SOIL.h"

#define NONE 0
#define CYLINDER 1
#define SPHERE 2
#define CUBE 3

using namespace glm;
using namespace std;

// lighting
//glm::vec3 lightPos(0.0f, 10.0f, 0.0f);

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


int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}
	View view;
    
    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(view.window, 1024/2, 768/2);

	// Blue background
	glClearColor(0.0f, 0.0f, 0.2f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

	// Cull triangles which normal is not towards the camera
	//glEnable(GL_CULL_FACE);

    Controller ctrl(view.window);

    Shader lightingShader("shaders/StandardShading.vertexshader", "shaders/StandardShading.fragmentshader");
    Shader floorShader("shaders/floor.vs","shaders/floor.fs");
	
    // positions of the point lights
    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f)
        //glm::vec3( 0.7f,  0.2f,  2.0f),
        //glm::vec3( 2.3f, -3.3f, -4.0f),
        //glm::vec3(-4.0f,  2.0f, -12.0f),
        //glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    glm::vec3 spotLightPositions[] = {
        glm::vec3( 4.0f,  4.0f,  4.0f),
        glm::vec3(-4.0f,  4.0f,  4.0f),
        glm::vec3( 4.0f,  4.0f, -4.0f),
        glm::vec3(-4.0f,  4.0f, -4.0f)
        //glm::vec3( 0.0f,  0.0f, -3.0f)
    };
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    float planeVertices[] = {
        // positions          // texture Coords 
         5.0f, -1.01f,  5.0f,  2.0f, 0.0f,
        -5.0f, -1.01f,  5.0f,  0.0f, 0.0f,
        -5.0f, -1.01f, -5.0f,  0.0f, 2.0f,

         5.0f, -1.01f,  5.0f,  2.0f, 0.0f,
        -5.0f, -1.01f, -5.0f,  0.0f, 2.0f,
         5.0f, -1.01f, -5.0f,  2.0f, 2.0f
    };
    // plane VAO
    unsigned int planeVAO, planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    //ply VAO
	unsigned int VAO,VBO, EBO;
	GLuint vertexbuffer,uvbuffer,normalbuffer;
	OBJLoader obj1;
	obj1.loadOBJ("models/cube.ply",CUBE);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, obj1.out_vertices.size() * sizeof(glm::vec3), &obj1.out_vertices[0], GL_STATIC_DRAW);
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, obj1.out_normals.size() * sizeof(glm::vec3), &obj1.out_normals[0], GL_STATIC_DRAW);
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, obj1.out_uv.size() * sizeof(glm::vec2), &obj1.out_uv[0], GL_STATIC_DRAW);
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glBindVertexArray(0);
    

    unsigned int VAO1,VBO1, EBO1;
	GLuint vertexbuffer1,uvbuffer1,normalbuffer1;
    OBJLoader obj2;
	obj2.loadOBJ("models/trashcan.ply",CYLINDER);
	glGenVertexArrays(1, &VAO1);
	glGenBuffers(1, &vertexbuffer1);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer1);
	glBufferData(GL_ARRAY_BUFFER, obj2.out_vertices.size() * sizeof(glm::vec3), &obj2.out_vertices[0], GL_STATIC_DRAW);
	glGenBuffers(1, &normalbuffer1);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer1);
	glBufferData(GL_ARRAY_BUFFER, obj2.out_normals.size() * sizeof(glm::vec3), &obj2.out_normals[0], GL_STATIC_DRAW);
	glGenBuffers(1, &uvbuffer1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer1);
	glBufferData(GL_ARRAY_BUFFER, obj2.out_uv.size() * sizeof(glm::vec2), &obj2.out_uv[0], GL_STATIC_DRAW);
	glBindVertexArray(VAO1);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glBindVertexArray(0);

    
    unsigned int VAO2,VBO2, EBO2;
    GLuint vertexbuffer2,uvbuffer2,normalbuffer2;
    OBJLoader obj3;
    obj3.loadOBJ("models/sphere_orig.ply",SPHERE);
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &vertexbuffer2);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
    glBufferData(GL_ARRAY_BUFFER, obj3.out_vertices.size() * sizeof(glm::vec3), &obj3.out_vertices[0], GL_STATIC_DRAW);
    glGenBuffers(1, &normalbuffer2);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer2);
    glBufferData(GL_ARRAY_BUFFER, obj3.out_normals.size() * sizeof(glm::vec3), &obj3.out_normals[0], GL_STATIC_DRAW);
    glGenBuffers(1, &uvbuffer2);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer2);
    glBufferData(GL_ARRAY_BUFFER, obj3.out_uv.size() * sizeof(glm::vec2), &obj3.out_uv[0], GL_STATIC_DRAW);
    glBindVertexArray(VAO2);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer2);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glBindVertexArray(0);

    unsigned int VAO3,VBO3, EBO3;
    GLuint vertexbuffer3,uvbuffer3,normalbuffer3;
    OBJLoader obj4;
    obj4.loadOBJ("models/beethoven.ply",NONE);
    glGenVertexArrays(1, &VAO3);
    glGenBuffers(1, &vertexbuffer3);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer3);
    glBufferData(GL_ARRAY_BUFFER, obj4.out_vertices.size() * sizeof(glm::vec3), &obj4.out_vertices[0], GL_STATIC_DRAW);
    glGenBuffers(1, &normalbuffer3);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer3);
    glBufferData(GL_ARRAY_BUFFER, obj4.out_normals.size() * sizeof(glm::vec3), &obj4.out_normals[0], GL_STATIC_DRAW);
    glGenBuffers(1, &uvbuffer3);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer3);
    glBufferData(GL_ARRAY_BUFFER, obj4.out_uv.size() * sizeof(glm::vec2), &obj4.out_uv[0], GL_STATIC_DRAW);
    glBindVertexArray(VAO3);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer3);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer3);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer3);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glBindVertexArray(0);
    
    
	
    // first, configure the cube's VAO (and VertexBufferObject)
    unsigned int VertexBufferObject, cubeVertexArrayObject;
    /*
    glGenVertexArrays(1, &cubeVertexArrayObject);
    glGenBuffers(1, &VertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(cubeVertexArrayObject);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
	*/
    // second, configure the light's VAO (VertexBufferObject stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    std::vector<unsigned int> textureNumbers;
    // load textures (we now use a utility function to keep the code more organized)
    // -----------------------------------------------------------------------------
    unsigned int floorTexture1 = texInit("textures/wood.png");
    unsigned int floorTexture2 = texInit("textures/wood4.jpg");
    unsigned int juteBox = texInit("textures/try.jpg");
    textureNumbers.push_back(juteBox);
    unsigned int chessMap = texInit("textures/chess.png");
    textureNumbers.push_back(chessMap);
    unsigned int flowerTexture = texInit("textures/can.jpg");
    textureNumbers.push_back(flowerTexture);
    unsigned int pokeball = texInit("textures/pokeball.jpeg");
    textureNumbers.push_back(pokeball);
    unsigned int masterball = texInit("textures/masterball.png");
    textureNumbers.push_back(masterball);
    unsigned int obama = texInit("textures/obama.jpg");
    textureNumbers.push_back(obama);
    
    unsigned int activeTextureNumber;
    //cout << textureNumbers.size() << endl;

    
    
    // shader configuration
    // --------------------
    lightingShader.use(); 
    lightingShader.setInt("material.diffuse", 0);   
    lightingShader.setInt("material.specular", 1); 
   	glm::vec3 lightPos(4.0f, 4.0f, 4.0f);
    glm::vec3 lightDirectionFront(0.0f, -1.0f, -0.0f);
    glm::vec4 lightbuttonmodes;

    glm::vec3 ambienton(0.0f, 0.0f, 0.0f);
    glm::vec3 ambientoff(0.0f, 0.0f, 0.0f);
    glm::vec3 diffuseon(2.0f,2.0f,2.0f);
    glm::vec3 diffuseoff(0.0f, 0.0f, 0.0f);
    glm::vec3 specularon(1.0f, 1.0f, 1.0f);
    glm::vec3 specularoff(0.0f, 0.0f, 0.0f);

    do{

        

        // render
        // ------
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ctrl.computeMatricesFromInputs();
        int lightbutton = ctrl.getM();
        lightbuttonmodes = ctrl.getmodes();
        ctrl.setNTextures(textureNumbers.size());
        //cout << lightbuttonmodes.x << lightbuttonmodes.y << lightbuttonmodes.z << lightbuttonmodes.w << endl;
        //int getTex = ctrl.getNTextures();
        //cout << "Texture chosen: " << getTex << endl;
        
        lightingShader.use();
        lightingShader.setVec3("viewPos", lightPos);
        lightingShader.setFloat("material.shininess", 32.0f);

        // directional light
        lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        

        // spotLight1
        if(lightbuttonmodes.x){
            lightingShader.setVec3("spotLight[0].position", spotLightPositions[0]);
            lightingShader.setVec3("spotLight[0].direction", lightDirectionFront);
            lightingShader.setVec3("spotLight[0].ambient", 0.0f, 0.0f, 0.0f);
            lightingShader.setVec3("spotLight[0].diffuse", 2.0f, 2.0f, 2.0f);
            lightingShader.setVec3("spotLight[0].specular", 1.0f, 1.0f, 1.0f);
            lightingShader.setFloat("spotLight[0].constant", 1.0f);
            lightingShader.setFloat("spotLight[0].linear", 0.09);
            lightingShader.setFloat("spotLight[0].quadratic", 0.032);
            lightingShader.setFloat("spotLight[0].cutOff", glm::cos(glm::radians(12.5f)));
            lightingShader.setFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(15.0f)));     
        }
        else{
            lightingShader.setVec3("spotLight[0].position", spotLightPositions[0]);
            lightingShader.setVec3("spotLight[0].direction", lightDirectionFront);
            lightingShader.setVec3("spotLight[0].ambient", 0.0f, 0.0f, 0.0f);
            lightingShader.setVec3("spotLight[0].diffuse", 0.0f, 0.0f, 0.0f);
            lightingShader.setVec3("spotLight[0].specular", 0.0f, 0.0f, 0.0f);
            lightingShader.setFloat("spotLight[0].constant", 1.0f);
            lightingShader.setFloat("spotLight[0].linear", 0.09);
            lightingShader.setFloat("spotLight[0].quadratic", 0.032);
            lightingShader.setFloat("spotLight[0].cutOff", glm::cos(glm::radians(12.5f)));
            lightingShader.setFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(15.0f)));     
        }


        // spotLight2
        if(lightbuttonmodes.y){
            lightingShader.setVec3("spotLight[1].position", spotLightPositions[1]);
            lightingShader.setVec3("spotLight[1].direction", lightDirectionFront);
            lightingShader.setVec3("spotLight[1].ambient", 0.0f, 0.0f, 0.0f);
            lightingShader.setVec3("spotLight[1].diffuse", 2.0f, 2.0f, 2.0f);
            lightingShader.setVec3("spotLight[1].specular", 1.0f, 1.0f, 1.0f);
            lightingShader.setFloat("spotLight[1].constant", 1.0f);
            lightingShader.setFloat("spotLight[1].linear", 0.09);
            lightingShader.setFloat("spotLight[1].quadratic", 0.032);
            lightingShader.setFloat("spotLight[1].cutOff", glm::cos(glm::radians(12.5f)));
            lightingShader.setFloat("spotLight[1].outerCutOff", glm::cos(glm::radians(15.0f)));     
        }
        else{
            lightingShader.setVec3("spotLight[1].position", spotLightPositions[1]);
            lightingShader.setVec3("spotLight[1].direction", lightDirectionFront);
            lightingShader.setVec3("spotLight[1].ambient", 0.0f, 0.0f, 0.0f);
            lightingShader.setVec3("spotLight[1].diffuse", 0.0f, 0.0f, 0.0f);
            lightingShader.setVec3("spotLight[1].specular", 0.0f, 0.0f, 0.0f);
            lightingShader.setFloat("spotLight[1].constant", 1.0f);
            lightingShader.setFloat("spotLight[1].linear", 0.09);
            lightingShader.setFloat("spotLight[1].quadratic", 0.032);
            lightingShader.setFloat("spotLight[1].cutOff", glm::cos(glm::radians(12.5f)));
            lightingShader.setFloat("spotLight[1].outerCutOff", glm::cos(glm::radians(15.0f)));        
        }    

        // spotLight3
        if(lightbuttonmodes.z){
            lightingShader.setVec3("spotLight[2].position", spotLightPositions[2]);
            lightingShader.setVec3("spotLight[2].direction", lightDirectionFront);
            lightingShader.setVec3("spotLight[2].ambient", 0.0f, 0.0f, 0.0f);
            lightingShader.setVec3("spotLight[2].diffuse", 2.0f, 2.0f, 2.0f);
            lightingShader.setVec3("spotLight[2].specular", 1.0f, 1.0f, 1.0f);
            lightingShader.setFloat("spotLight[2].constant", 1.0f);
            lightingShader.setFloat("spotLight[2].linear", 0.09);
            lightingShader.setFloat("spotLight[2].quadratic", 0.032);
            lightingShader.setFloat("spotLight[2].cutOff", glm::cos(glm::radians(12.5f)));
            lightingShader.setFloat("spotLight[2].outerCutOff", glm::cos(glm::radians(15.0f)));     
        }
        else{
            lightingShader.setVec3("spotLight[2].position", spotLightPositions[2]);
            lightingShader.setVec3("spotLight[2].direction", lightDirectionFront);
            lightingShader.setVec3("spotLight[2].ambient", 0.0f, 0.0f, 0.0f);
            lightingShader.setVec3("spotLight[2].diffuse", 0.0f, 0.0f, 0.0f);
            lightingShader.setVec3("spotLight[2].specular", 0.0f, 0.0f, 0.0f);
            lightingShader.setFloat("spotLight[2].constant", 1.0f);
            lightingShader.setFloat("spotLight[2].linear", 0.09);
            lightingShader.setFloat("spotLight[2].quadratic", 0.032);
            lightingShader.setFloat("spotLight[2].cutOff", glm::cos(glm::radians(12.5f)));
            lightingShader.setFloat("spotLight[2].outerCutOff", glm::cos(glm::radians(15.0f)));         
        }

        // spotLight4
        if(lightbuttonmodes.w){
            lightingShader.setVec3("spotLight[3].position", spotLightPositions[3]);
            lightingShader.setVec3("spotLight[3].direction", lightDirectionFront);
            lightingShader.setVec3("spotLight[3].ambient", 0.0f, 0.0f, 0.0f);
            lightingShader.setVec3("spotLight[3].diffuse", 2.0f, 2.0f, 2.0f);
            lightingShader.setVec3("spotLight[3].specular", 1.0f, 1.0f, 1.0f);
            lightingShader.setFloat("spotLight[3].constant", 1.0f);
            lightingShader.setFloat("spotLight[3].linear", 0.09);
            lightingShader.setFloat("spotLight[3].quadratic", 0.032);
            lightingShader.setFloat("spotLight[3].cutOff", glm::cos(glm::radians(12.5f)));
            lightingShader.setFloat("spotLight[3].outerCutOff", glm::cos(glm::radians(15.0f)));     
        }
        else{
            lightingShader.setVec3("spotLight[3].position", spotLightPositions[3]);
            lightingShader.setVec3("spotLight[3].direction", lightDirectionFront);
            lightingShader.setVec3("spotLight[3].ambient", 0.0f, 0.0f, 0.0f);
            lightingShader.setVec3("spotLight[3].diffuse", 0.0f, 0.0f, 0.0f);
            lightingShader.setVec3("spotLight[3].specular", 0.0f, 0.0f, 0.0f);
            lightingShader.setFloat("spotLight[3].constant", 1.0f);
            lightingShader.setFloat("spotLight[3].linear", 0.09);
            lightingShader.setFloat("spotLight[3].quadratic", 0.032);
            lightingShader.setFloat("spotLight[3].cutOff", glm::cos(glm::radians(12.5f)));
            lightingShader.setFloat("spotLight[3].outerCutOff", glm::cos(glm::radians(15.0f)));        
        }
        
        // view/projection transformations
        glm::mat4 projectionMatrix = ctrl.getProjectionMatrix();
        glm::mat4 viewMatrix = ctrl.getViewMatrix();
        lightingShader.setMat4("projection", projectionMatrix);
        lightingShader.setMat4("view", viewMatrix);
        // world transformation
        // this makes it to the origin :) 
        glm::mat4 modelMatrix = ctrl.getRotationMatrix()*glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(4.0f, 0.0f, 4.0f));
        lightingShader.setMat4("model", modelMatrix);
        //render cube.ply
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, chessMap);
        //glBindTexture(GL_TEXTURE_2D, getTex);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, obj1.out_vertices.size() );    
        glBindVertexArray(0);

        // light properties changed for the second
        glm::vec3 lightPos2(-4.0f, 4.0f, 4.0f);
        lightingShader.setVec3("light.position", lightPos2);
        modelMatrix = ctrl.getRotationMatrix()*glm::mat4(1.0);
        float rotAngle = 29.85f;
        modelMatrix = glm::rotate(modelMatrix, rotAngle, glm::vec3(4.0f, 0.0f, 0.0f));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-4.0f, 4.0f, 0.0f));
        lightingShader.setMat4("model", modelMatrix);
        //render trashcan.ply
        glBindTexture(GL_TEXTURE_2D, flowerTexture);
        //glBindTexture(GL_TEXTURE_2D, getTex);
        glBindVertexArray(VAO1);
        glDrawArrays(GL_TRIANGLES, 0, obj2.out_vertices.size() );    
        glBindVertexArray(0);


        modelMatrix = ctrl.getRotationMatrix()*glm::mat4(1.0);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-4.0f, 0.0, 4.0f));
        lightingShader.setMat4("model", modelMatrix);
        lightingShader.setVec3("material.specular", 0.95f, 0.95f, 0.95f);
        lightingShader.setFloat("material.shininess", 100.0f);
        //render sphere.ply
        glBindTexture(GL_TEXTURE_2D, masterball);
        //glBindTexture(GL_TEXTURE_2D, getTex);
        glBindVertexArray(VAO2);
        glDrawArrays(GL_TRIANGLES, 0, obj3.out_vertices.size() );    
        glBindVertexArray(0);

        
        modelMatrix = ctrl.getRotationMatrix()*glm::mat4(1.0);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(4.0f, 0.25f, -4.0f));
        lightingShader.setMat4("model", modelMatrix);
        lightingShader.setVec3("material.specular", 0.95f, 0.95f, 0.95f);
        lightingShader.setFloat("material.shininess", 100.0f);
        //render sphere.ply
        glBindTexture(GL_TEXTURE_2D, obama);
        //glBindTexture(GL_TEXTURE_2D, getTex);
        glBindVertexArray(VAO3);
        glDrawArrays(GL_TRIANGLES, 0, obj4.out_vertices.size() );    
        glBindVertexArray(0);


        // bind diffuse map
        floorShader.use();
        glm::mat4 projection = ctrl.getProjectionMatrix();
        viewMatrix = ctrl.getViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);
        floorShader.setMat4("projection", projection);
        floorShader.setMat4("view", viewMatrix);
        // floor
        glBindVertexArray(planeVAO);
        glBindTexture(GL_TEXTURE_2D, floorTexture2);
        model = ctrl.getRotationMatrix()*glm::mat4(1.0f);
        floorShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);



        

       	glfwSwapBuffers(view.window);
        glfwPollEvents();
    }while( glfwGetKey(view.window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(view.window) == 0 );


}