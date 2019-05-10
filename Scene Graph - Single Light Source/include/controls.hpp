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
#include "utils.hpp"


class Controller{
public:
    GLFWwindow* window;
    glm::mat4 ViewMatrix;
    glm::mat4 ProjectionMatrix;
    glm::mat4 RotationMatrix ;
    glm::mat4 ScaleMatrix ;
    glm::mat4 TranslateMatrix;
    glm::mat4 RotationMatrix2 ;
    
    glm::mat4 BodyRotationMatrix ;
    glm::mat4 BodyScaleMatrix ;
    glm::mat4 BodyTranslateMatrix ;
    
    bool mode ;
    int m;
    glm::vec4 modes;
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
    glm::vec3 mousePOS;
    
//public:
    int nTextures,getnthTexture=0;
    Controller(GLFWwindow* win);
    void computeMatricesFromInputs();
    glm::mat4 getProjectionMatrix();
    glm::mat4 getViewMatrix();
    bool getMode();
    int getM();
    int getNTextures();
    void setNTextures(int n);
    glm::vec3 getmousepos();
    
    glm::vec4 getmodes();
    glm::mat4 getRotationMatrix();
    glm::mat4 getScaleMatrix();
    glm::mat4 getTranslateMatrix();
    void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
    void handleClick(GLFWwindow* window, int button, int action, int mods);
    void handleDrag(GLFWwindow* window, double cursor_x, double cursor_y);
    
};

