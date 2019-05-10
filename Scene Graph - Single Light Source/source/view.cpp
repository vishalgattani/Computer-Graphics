
#include "../include/view.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>

View::View(){
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  
    window = glfwCreateWindow( 1024, 768, "Assignment-2", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window\n" );
        getchar();
        glfwTerminate();
        
    }
    glfwMakeContextCurrent(window);
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        //return -1;
    }
    // Ensure we can capture the escape key being pressed below
glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
// Hide the mouse and enable unlimited mouvement
glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


}