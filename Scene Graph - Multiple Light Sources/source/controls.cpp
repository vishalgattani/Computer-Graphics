#define GLM_ENABLE_EXPERIMENTAL
// Include GLFW
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
using namespace std;
#include <glm/gtx/quaternion.hpp>

#include <glm/ext.hpp>

#include "../include/controls.hpp"


mat4 rotate_custom(vec3 axis,float angle){
	Quaternion quaternion;
	quaternion.CreateFromAxisAngle(axis.x,axis.y,axis.z,angle);
	float *rmatrix = new float[16];
	quaternion.CreateMatrix(rmatrix);
	mat4 bomb = make_mat4(rmatrix);
	bomb = transpose(bomb);
	return bomb;
}

bool Controller::getMode(){
	return mode;
}
int Controller::getM(){
	return m;
}

glm::mat4 Controller::getViewMatrix(){
	return ViewMatrix;
}
glm::mat4 Controller::getProjectionMatrix(){
	return ProjectionMatrix;
}
glm::mat4 Controller::getRotationMatrix(){
	return RotationMatrix;
}
glm::mat4 Controller::getScaleMatrix(){
	return ScaleMatrix;
}
glm::mat4 Controller::getTranslateMatrix(){
	return TranslateMatrix;
}

Controller::Controller(GLFWwindow* win){
    //factor = 1;
	window = win;
    // Initial position : on +Z
    position = glm::vec3( 0, 2, 40 );
    mousePOS = glm::vec3( 0, 0,0 );
    // Initial horizontal angle : toward -Z
    horizontalAngle = 3.14f;
    // Initial vertical angle : none
    verticalAngle = -0.0f;
    // Initial Field of View
    initialFoV = 45.0f;
	speed = 5.0f; // 3 units / second
    mouseSpeed = 0.00002f;
    scal = 1;
    trans_x = 0,trans_y = 0,trans_z = 0;
    turn_degrees = 0;
    axis_x = axis_y = axis_z = 0;
    RotationMatrix = glm::mat4(1.0);
    TranslateMatrix = translate(glm::mat4(1.0),vec3(0,0,0));
    ViewMatrix = mat4(1.0);
	ProjectionMatrix = mat4(1.0);
	RotationMatrix2 = mat4(1.0);
	ScaleMatrix = mat4(1.0);
	
	mode = true;
	m=1;
	modes.x = 0.0f;
	modes.y = 0.0f;
	modes.z = 0.0f;
	modes.w = 0.0f;
}

glm::vec4 Controller::getmodes(){
	return modes;
}
glm::vec3 Controller::getmousepos(){
	return mousePOS;
}

int Controller::getNTextures(){
	return getnthTexture;
}

void Controller::setNTextures(int n){
	nTextures = n;
}

void Controller::handleClick(GLFWwindow* window, int button, int action, int mods){
    double cursor_x, cursor_y, cursor_z;
    glfwGetCursorPos(window, &cursor_x, &cursor_y);

	float win_x, win_y, win_z;
    win_x = cursor_x;
    win_y = 768 - cursor_y;

	glReadPixels(int(win_x), int(win_y), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &win_z);
	
	win_x = win_x/512 - 1;
	win_y = win_y/384 - 1;
	win_z = 2*win_z - 1;
	
	glm::vec3 pos = glm::vec3(win_x, win_y, win_z);
	
	if(button == GLFW_MOUSE_BUTTON_LEFT and action == GLFW_PRESS){
		cout << pos.x << "," << pos.y << "," << pos.z << endl;
	}
	
	else if(action == GLFW_RELEASE){
		;
	}
}



void Controller::computeMatricesFromInputs(){
    //std::cerr << q;
	//int random = rand() % nTextures + 1;
    float *rmatrix = new float[16]  ;
    Quaternion quaternion;
	quaternion.CreateFromAxisAngle(axis_x,axis_y,axis_z,turn_degrees);
	quaternion.CreateMatrix(rmatrix);
	mat4 bomb = make_mat4(rmatrix);
	bomb = transpose(bomb);
	
    

    //RotationMatrix *= bomb;
	
	


	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Reset mouse position for next frame
	//glfwSetCursorPos(window, 1024/2, 768/2);
	
	
	// Compute new orientation
	horizontalAngle += mouseSpeed * float(1024/2 - xpos );
	verticalAngle   += mouseSpeed * float( 768/2 - ypos );
	
	//cout << horizontalAngle << "," << verticalAngle << endl;

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle), 
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
	
	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f), 
		0,
		cos(horizontalAngle - 3.14f/2.0f)
	);

	// down vector
	glm::vec3 down = glm::vec3(
		0,
		sin(verticalAngle - 3.14f/2.0f),
		cos(verticalAngle - 3.14f/2.0f)
	);
	
	// Up vector
	glm::vec3 up = glm::cross( right, direction );

	// Move forward
	if (glfwGetKey( window, GLFW_KEY_KP_ADD ) == GLFW_PRESS){
		position += direction * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey( window, GLFW_KEY_MINUS ) == GLFW_PRESS){
		position -= direction * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
		position += right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
		position -= right * deltaTime * speed;
	}

	if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
		position += down * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
		position -= down * deltaTime * speed;
	}

	if (glfwGetKey( window, GLFW_KEY_N ) == GLFW_PRESS){
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if (glfwGetKey( window, GLFW_KEY_Q ) == GLFW_PRESS){ 
		RotationMatrix *= rotate_custom(vec3(0,1,0),2.5);
	}
	
	if (glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS){
		RotationMatrix *= rotate_custom(vec3(0,1,0),-2.5);
	}

	if (glfwGetKey( window, GLFW_KEY_E ) == GLFW_PRESS){ 
		RotationMatrix *= rotate_custom(vec3(0,0,1),2.5);
	}
	
	if (glfwGetKey( window, GLFW_KEY_R ) == GLFW_PRESS){
		RotationMatrix *= rotate_custom(vec3(0,0,1),-2.5);
	}

	if (glfwGetKey( window, GLFW_KEY_T ) == GLFW_PRESS){ 
		RotationMatrix *= rotate_custom(vec3(1,0,0),2.5);
	}
	
	if (glfwGetKey( window, GLFW_KEY_Y ) == GLFW_PRESS){
		RotationMatrix *= rotate_custom(vec3(1,0,0),-2.5);
	}


	if (glfwGetKey( window, GLFW_KEY_M ) == GLFW_PRESS){
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS){
		trans_x+=0.1;
		vec3 trans = vec3(trans_x,trans_y,trans_z);
		TranslateMatrix = translate(trans) ;

	}

	if (glfwGetKey( window, GLFW_KEY_F ) == GLFW_PRESS){
		trans_y+=0.1;
		vec3 trans = vec3(trans_x,trans_y,trans_z);
		TranslateMatrix = translate(trans) ;

	}

	if (glfwGetKey( window, GLFW_KEY_G ) == GLFW_PRESS){
		trans_z+=0.1;
		vec3 trans = vec3(trans_x,trans_y,trans_z);
		TranslateMatrix = translate(trans) ;

	}

	if (glfwGetKey( window, GLFW_KEY_C ) == GLFW_PRESS){
		trans_x-=0.1;
		vec3 trans = vec3(trans_x,trans_y,trans_z);
		TranslateMatrix = translate(trans) ;
	}

	if (glfwGetKey( window, GLFW_KEY_V ) == GLFW_PRESS){
		trans_y-=0.1;
		vec3 trans = vec3(trans_x,trans_y,trans_z);
		TranslateMatrix = translate(trans) ;

	}

	if (glfwGetKey( window, GLFW_KEY_B ) == GLFW_PRESS){
		trans_z-=0.1;
		vec3 trans = vec3(trans_x,trans_y,trans_z);
		TranslateMatrix = translate(trans) ;
	}

	/*
	if (glfwGetKey( window, GLFW_KEY_1 ) == GLFW_PRESS){
		trans_x+=0.1;
		vec3 trans = vec3(trans_x,trans_y,trans_z);
		TranslateMatrix = translate(trans) ;
	}

	if (glfwGetKey( window, GLFW_KEY_2 ) == GLFW_PRESS){
		trans_x-=0.1;
		vec3 trans = vec3(trans_x,trans_y,trans_z);
		TranslateMatrix = translate(trans) ;
	}

	if (glfwGetKey( window, GLFW_KEY_3 ) == GLFW_PRESS){
		trans_y+=0.1;
		vec3 trans = vec3(trans_x,trans_y,trans_z);
		TranslateMatrix = translate(trans) ;
	}

	if (glfwGetKey( window, GLFW_KEY_4 ) == GLFW_PRESS){
		trans_y-=0.1;
		vec3 trans = vec3(trans_x,trans_y,trans_z);
		TranslateMatrix = translate(trans) ;

	}

	if (glfwGetKey( window, GLFW_KEY_5 ) == GLFW_PRESS){
		trans_z+=0.1;
		vec3 trans = vec3(trans_x,trans_y,trans_z);
		TranslateMatrix = translate(trans) ;

	}

	if (glfwGetKey( window, GLFW_KEY_6 ) == GLFW_PRESS){
		trans_z-=0.1;
		vec3 trans = vec3(trans_x,trans_y,trans_z);
		TranslateMatrix = translate(trans) ;
	}
	*/

	if (glfwGetKey( window, GLFW_KEY_J ) == GLFW_PRESS){
		trans_z+=0.1;
		vec3 trans = vec3(trans_x,trans_y,trans_z);
		BodyTranslateMatrix = translate(trans) ;

	}

	if (glfwGetKey( window, GLFW_KEY_I ) == GLFW_PRESS){
		trans_z-=0.1;
		vec3 trans = vec3(trans_x,trans_y,trans_z);
		BodyTranslateMatrix = translate(trans) ;
	}

	if (glfwGetKey( window, GLFW_KEY_K ) == GLFW_PRESS){
		trans_y+=0.1;
		vec3 trans = vec3(trans_x,trans_y,trans_z);
		BodyTranslateMatrix = translate(trans) ;

	}

	if (glfwGetKey( window, GLFW_KEY_O ) == GLFW_PRESS){
		trans_y-=0.1;
		vec3 trans = vec3(trans_x,trans_y,trans_z);
		BodyTranslateMatrix = translate(trans) ;
	}

	if (glfwGetKey( window, GLFW_KEY_L ) == GLFW_PRESS){
		trans_x+=0.1;
		vec3 trans = vec3(trans_x,trans_y,trans_z);
		BodyTranslateMatrix = translate(trans) ;

	}

	if (glfwGetKey( window, GLFW_KEY_P ) == GLFW_PRESS){
		trans_x-=0.1;
		vec3 trans = vec3(trans_x,trans_y,trans_z);
		BodyTranslateMatrix = translate(trans) ;
	}

	if (glfwGetKey( window, GLFW_KEY_Z ) == GLFW_PRESS){
	
			if(scal<25)
			{
				scal+=0.1;
				vec3 scull = vec3(scal,scal,scal); 
				ScaleMatrix = glm::scale(scull);
				//std::cerr << scal << std::endl;
			}

	}

	if (glfwGetKey( window, GLFW_KEY_X ) == GLFW_PRESS){
		if(scal > 1){
			scal-=0.1;
			vec3 scull = vec3(scal,scal,scal); 
			ScaleMatrix = glm::scale(scull);
		}
	}

	if (glfwGetKey( window, GLFW_KEY_0 ) == GLFW_PRESS){
		m=0;	
		modes.x = 0.0f;
		modes.y = 0.0f;
		modes.z = 0.0f;
		modes.w = 0.0f;

	}
	if (glfwGetKey( window, GLFW_KEY_1 ) == GLFW_PRESS){
		m=1;
		modes.x = 1.0f;
		modes.y = 0.0f;
		modes.z = 0.0f;
		modes.w = 0.0f;
	}
	if (glfwGetKey( window, GLFW_KEY_2 ) == GLFW_PRESS){
		m=2;
		modes.x = 0.0f;
		modes.y = 1.0f;
		modes.z = 0.0f;
		modes.w = 0.0f;
	}
	if (glfwGetKey( window, GLFW_KEY_3 ) == GLFW_PRESS){
		m=3;
		modes.x = 0.0f;
		modes.y = 0.0f;
		modes.z = 1.0f;
		modes.w = 0.0f;
	}
	if (glfwGetKey( window, GLFW_KEY_4 ) == GLFW_PRESS){
		m=4;
		modes.x = 0.0f;
		modes.y = 0.0f;
		modes.z = 0.0f;
		modes.w = 1.0f;
	}
	if (glfwGetKey( window, GLFW_KEY_5 ) == GLFW_PRESS){
		m=5;
		modes.x = 1.0f;
		modes.y = 1.0f;
		modes.z = 1.0f;
		modes.w = 1.0f;
	}
	if (glfwGetKey( window, GLFW_KEY_C ) == GLFW_PRESS){
		;
	}
	float FoV = initialFoV;//
	//FoV = - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	
	//RotationMatrix = ();
	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(initialFoV), 4.0f / 3.0f, 0.1f, 1000.0f);
	//ProjectionMatrix = glm::mat4(1.0);
	// Camera matrix
	ViewMatrix       = glm::lookAt(
								position,           // Camera is here
								position+direction, // and looks here : at the same position, plus "direction"
								up                  // Head is up (set to 0,-1,0 to look upside-down)
						   );
				
	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}


