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
#include "../include/drawObj.hpp"
#include "../include/windows.hpp"
#include "../include/skybox.hpp"
#include "../include/Scene.hpp"
//#include "new_controller.hpp"

#include "../include/SOIL.h"

using namespace glm;
using namespace std;




int main( void )
{
    Window window("Final");
    Controller ctrl(window.get());
    Scene scene;
    do{
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ctrl.computeMatricesFromInputs();
        scene.renderSkyBox( ctrl.getProjectionMatrix(), ctrl.getViewMatrix());
        scene.displayScene();
        scene.render(&ctrl);    
        window.swapBuffer();

    }while(window.quit());


}