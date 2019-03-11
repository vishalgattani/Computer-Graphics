#pragma once
#define GLM_ENABLE_EXPERIMENTAL
// Include GLM

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
using namespace glm;

#include <GLFW/glfw3.h>
#include <iostream>
#include <utils.hpp>

class Controller{
private:
    GLFWwindow* window;
    glm::mat4 ViewMatrix;
    glm::mat4 ProjectionMatrix;
    glm::mat4 RotationMatrix ;
    glm::mat4 ScaleMatrix ;
    glm::mat4 TranslateMatrix;
    
    bool mode ;
    glm::vec3 position;
    float horizontalAngle;
    float verticalAngle;
    float initialFoV;
    float speed;
    float mouseSpeed;
    float axis_x,axis_y,axis_z,turn_degrees;
    float scal;
    float trans_x,trans_y,trans_z;
    float factor;
    
public:
    Controller(GLFWwindow* win);
    void computeMatricesFromInputs();
    glm::mat4 getProjectionMatrix();
    glm::mat4 getViewMatrix();
    bool getMode();
    glm::mat4 getRotationMatrix();
    glm::mat4 getScaleMatrix();
    glm::mat4 getTranslateMatrix();

};

