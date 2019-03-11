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
#include <shader.hpp>
#include <controls.hpp>
#include <view.hpp>
#include <objloader.hpp>
#include <geometryshader.hpp>

using namespace glm;
using namespace std;


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
	glEnable(GL_CULL_FACE);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	view.programID = LoadShaders( "StandardShading.vertexshader", "StandardShading.fragmentshader");
	// Get a handle for our "MVP" uniform
	/*
	GLuint MatrixID = glGetUniformLocation(view.programID, "MVP");
	GLuint ProjectionMatrixID = glGetUniformLocation(view.programID, "P"); 
	GLuint ViewMatrixID = glGetUniformLocation(view.programID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(view.programID, "M");
	*/
	// Generating Random Model Matrices
    unsigned int amount = 10;
    glm::mat4* modelMatrices;
    modelMatrices = new glm::mat4[amount];
    srand(glfwGetTime()); // initialize random seed	
    float radius = 15.0;
    float offset = 20.0f;
    for (unsigned int i = 0; i < amount; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        // 1. translation: displace along circle with 'radius' in range [-offset, offset]
        float angle = (float)i / (float)amount * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 1.0f; // keep height of asteroid field smaller compared to width of x and z
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));

        // 2. scale: Scale between 0.05 and 0.25f
        //float scale = (rand() % 20) / 100.0f + 0.05;
        //float scale = 1.0f;
		//model = glm::scale(model, glm::vec3(scale));

        // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
        //float rotAngle = (rand() % 360);
        //model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

        // 4. now add to list of matrices
        modelMatrices[i] = model;
    }

	// configure instanced array
    // -------------------------
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

	Shader shader("9.1.geometry_shader.vs", "9.1.geometry_shader.fs", "9.1.geometry_shader.gs");
	GLuint MatrixID1 = glGetUniformLocation(shader.ID, "MVP");
	GLuint ProjectionMatrixID1 = glGetUniformLocation(shader.ID, "P"); 
	GLuint ViewMatrixID1 = glGetUniformLocation(shader.ID, "V");
	GLuint ModelMatrixID1 = glGetUniformLocation(shader.ID, "M");
	

    
	
	OBJLoader obj1;
	obj1.loadOBJ("beethoven.ply");
  

	unsigned int VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, obj1.incircles.size() * sizeof(glm::vec4), &obj1.incircles[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindVertexArray(0);

	// Get a handle for our "LightPosition" uniform
	glUseProgram(view.programID);
	GLuint LightID = glGetUniformLocation(view.programID, "LightPosition_worldspace");
	// Light Position
	glm::vec3 lightPos = glm::vec3(4,4,4);
	glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

	Controller ctrl(view.window);

	do{

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glUseProgram(view.programID);
		shader.use();
		
		ctrl.computeMatricesFromInputs();
		
		glm::mat4 ProjectionMatrix = ctrl.getProjectionMatrix();
		glm::mat4 ViewMatrix = ctrl.getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);

		// Using the generated Random Model Matrices
		ModelMatrix = ctrl.getTranslateMatrix()*ctrl.getRotationMatrix()*ctrl.getScaleMatrix()*ModelMatrix;	
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		// Send our transformation to the currently bound shader, in the "MVP" uniform
		glUniformMatrix4fv(MatrixID1, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID1, 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniformMatrix4fv(ViewMatrixID1, 1, GL_FALSE, &ViewMatrix[0][0]);
		glUniformMatrix4fv(ProjectionMatrixID1, 1, GL_FALSE, &ProjectionMatrix[0][0]);
		//obj1.Draw();
		glBindVertexArray(VAO);
		glEnable(GL_PROGRAM_POINT_SIZE);
		
		glDrawArrays(GL_POINTS, 0, obj1.incircles.size() * sizeof(glm::vec4));
		
		glfwSwapBuffers(view.window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(view.window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(view.window) == 0 );

	glDeleteProgram(view.programID);
	// Close OpenGL window and terminate GLFW
	glfwTerminate();


	return 0;
}

