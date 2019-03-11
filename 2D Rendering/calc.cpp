#include <bits/stdc++.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath> 

using namespace std;

#define cout(a) cout<<a<<endl

#define clamp(x, lower, upper) (min(upper, max(x, lower)))

#define transparency 0.2
#define spacing 0.005
#define sizeSquare 0.5


GLdouble posX, posY, posZ;
GLdouble newX, newY, newZ;
float centerx,centery;
static GLdouble zoomFactor = 1.0;
int angle_square_animate = -90;
int angle_square_animate_speed = 10;

int angle_calc = 0.0;
int flag_square_animate = 1;
int button_select_index;
int rightclick_count = 0;
int question1;

struct object {
  float v1[2];
  float v2[2];
  float v3[2];
  float v4[2];
} objects[100];

void createVertices(){
	int c=0;
	for(int i = 0; i < 4; i++){
		for(int j=0;j<4;j++){
			objects[c].v1[0] = -1.0+spacing+i*0.5;
			objects[c].v1[1] = 1.0-spacing-j*0.5;
			objects[c].v2[0] = -1.0+spacing+i*0.5;
			objects[c].v2[1] = 0.5+spacing-j*0.5;
			objects[c].v3[0] = -0.5-spacing+i*0.5;
			objects[c].v3[1] = 0.5+spacing-j*0.5;
			objects[c].v4[0] = -0.5-spacing+i*0.5;
			objects[c].v4[1] = 1.0-spacing-j*0.5;
			c++;
		}
	}
}

char calc[16] = {'1','4','7','0','2','5','8','.','3','6','9','=','+','-','*','/'};    

// IDs
float scale = 1.0, x = 0.0, y = 0.0;

// Function to calculate distance 
float calculateDistance(float x1, float y1, float x2, float y2) { 
    // Calculating distance 
    return sqrt(pow(x2 - x1, 2) +  
                pow(y2 - y1, 2) * 1.0); 
}

void square_animate(GLfloat posX,GLfloat posY,GLfloat posZ){
	int scaledownanimate = 12;
    int index=0;
    float temp=1;
    for(int i=0;i<16;i++){
        float a = calculateDistance(posX,posY,(objects[i].v1[0]+objects[i].v3[0])/2,(objects[i].v1[1]+objects[i].v3[1])/2);
		if(a < temp) {temp = a;index=i;}	        
	}
	//button_select_index = index;
    glLoadIdentity();//load identity matrix
    glTranslatef(x, y, 0);
	glRotatef(angle_calc,0,0,1);
    glScalef(scale, scale, scale);
    //translate to the center of the chosen button
	glTranslatef((objects[index].v1[0]+objects[index].v3[0])/2,(objects[index].v1[1]+objects[index].v3[1])/2,0);
	glRotatef( angle_square_animate,0,0,1);

    angle_square_animate+=angle_square_animate_speed;
    if(angle_square_animate >=90){
        flag_square_animate = 0;
        angle_square_animate = 0;
        cout << ">>> Button Pressed: " << calc[index] << endl;
        //cout <<  " Button Index: " << index << endl;
    }
    //translate away from the center of the chosen button to the default
	glTranslatef(-(objects[index].v1[0]+objects[index].v3[0])/2,-(objects[index].v1[1]+objects[index].v3[1])/2,0);//move forward 4 units	
    glBegin(GL_QUADS);
    glColor4f(1/2,1/2,1/2,transparency);
    //glColor4f(((rand() % 100) + 50) / 150.0,((rand() % 100) + 50) / 150.0,((rand() % 100) + 50) / 150.0,transparency);
	glVertex3f(objects[index].v1[0]+scaledownanimate*spacing,objects[index].v1[1]-scaledownanimate*spacing,0.0f);
	glVertex3f(objects[index].v2[0]+scaledownanimate*spacing,objects[index].v2[1]+scaledownanimate*spacing,0.0f);
	glVertex3f(objects[index].v3[0]-scaledownanimate*spacing,objects[index].v3[1]+scaledownanimate*spacing,0.0f);
    glVertex3f(objects[index].v4[0]-scaledownanimate*spacing,objects[index].v4[1]-scaledownanimate*spacing,0.0f);
	glEnd();

}

void chooseCalcButton(GLfloat posX,GLfloat posY,GLfloat posZ){
	int index=0;
    float temp=1;
    for(int i=0;i<16;i++){
        float a = calculateDistance(posX,posY,(objects[i].v1[0]+objects[i].v3[0])/2,(objects[i].v1[1]+objects[i].v3[1])/2);
		if(a < temp) {temp = a;index=i;}	        
	}
	button_select_index = index;
	cout << "INDEX: "<< index << endl;
	centerx = (objects[button_select_index].v1[0]+objects[button_select_index].v2[0]+objects[button_select_index].v3[0]+objects[button_select_index].v4[0])/4;
	centery = (objects[button_select_index].v1[1]+objects[button_select_index].v2[1]+objects[button_select_index].v3[1]+objects[button_select_index].v4[1])/4;
	cout << "Center of Chosen Button: " << centerx << "," << centery << endl;
	rightclick_count = 2;
}

void transformCalcButton(int button_select_index ,GLfloat newX ,GLfloat newY ,GLfloat newZ){//,float centerx, float centery){
	
	float translatex = newX ;
	float translatey = newY ;
	cout << "Translate to " << translatex << "," << translatey << endl;
	
	objects[button_select_index].v1[0] = translatex-sizeSquare/2;
	objects[button_select_index].v1[1] = translatey+sizeSquare/2;
	objects[button_select_index].v2[0] = translatex-sizeSquare/2;
	objects[button_select_index].v2[1] = translatey-sizeSquare/2;
	objects[button_select_index].v3[0] = translatex+sizeSquare/2;
	objects[button_select_index].v3[1] = translatey-sizeSquare/2;
	objects[button_select_index].v4[0] = translatex+sizeSquare/2;
	objects[button_select_index].v4[1] = translatey+sizeSquare/2;

	

	glLoadIdentity();//load identity matrix
    glTranslatef(x, y, 0);
    glRotatef(angle_calc,0,0,1);
    glScalef(scale, scale, scale);
    glTranslatef(translatex,translatey,0);//move forward 4 units
	//glTranslatef(-translatex,-translatey,0);//move forward 4 units
	
	glBegin(GL_QUADS);
    //glColor4f(1,1,1,1);
	glVertex3f(objects[button_select_index].v1[0],objects[button_select_index].v1[1],0.0f);
	glVertex3f(objects[button_select_index].v2[0],objects[button_select_index].v2[1],0.0f);
	glVertex3f(objects[button_select_index].v3[0],objects[button_select_index].v3[1],0.0f);
    glVertex3f(objects[button_select_index].v4[0],objects[button_select_index].v4[1],0.0f);
	glEnd();

	rightclick_count = 0;
}




void GetOGLPos(int x, int y)
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    
 
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );
 
    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
 
    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
 	
    //cout << posX << "," << posY << "," << posZ << endl;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
    	flag_square_animate=1;
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS){
		cout << "<1>" << posX << "," << posY << "," << posZ << endl;
    	rightclick_count = 1;
	}
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS){
		cout << "<2>" << posX << "," << posY << "," << posZ << endl;
    	newX = posX;newY=posY;newZ=posZ;
    	rightclick_count = 3;	
	}

}

void drawCalcButton(int number)
{
	glLoadIdentity();//load identity matrix
    
	float cx = (objects[number].v1[0] + objects[number].v3[0])/2;
	float cy = (objects[number].v1[1] + objects[number].v3[1])/2;
	glTranslatef(cx, cy, 0);//move forward 4 units
    glScalef(scale, scale, scale);
    glRotatef(angle_calc,0,0,1);
    //glTranslatef(cx, cy, 0);//move forward 4 units
    glTranslatef(-cx, -cy, 0);//move forward 4 units
    if(number>=0 && number<4) glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	else if(number>=4 && number<8) glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	else if(number>=8 && number<12) glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	else if(number>=12 && number<16) glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);
	int c = number;
	glVertex3f(objects[c].v1[0],objects[c].v1[1],0.0);
	glVertex3f(objects[c].v2[0],objects[c].v2[1],0.0);
	glVertex3f(objects[c].v3[0],objects[c].v3[1],0.0);
	glVertex3f(objects[c].v4[0],objects[c].v4[1],0.0);


    glEnd();
}



void drawCalc()
{
	glLoadIdentity();//load identity matrix
    
    glTranslatef(x, y, 0);//move forward 4 units
    glScalef(scale, scale, scale);
    glRotatef(angle_calc,0,0,1);
    
	glBegin(GL_QUADS);
	int c=0;
	for(int i=0; i<4; i++){
		for(int j=0;j<4;j++){
			if(i==0) 
				glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
				//glColor4f(((rand() % 100) + 50) / 150.0,((rand() % 100) + 50) / 150.0,((rand() % 100) + 50) / 150.0,1.0);
			else if(i==1) glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
			else if(i==2) glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
			else if(i==3) glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
			
				//glColor4f(((rand() % 100) + 50) / 150.0,((rand() % 100) + 50) / 150.0,((rand() % 100) + 50) / 150.0,1.0);
			
			glVertex3f(objects[c].v1[0],objects[c].v1[1],0.0);
			glVertex3f(objects[c].v2[0],objects[c].v2[1],0.0);
			//glVertex3f(-0.5-0.01+i*0.5,1.0-0.01-j*0.5,0.0);
			
			//glVertex3f(-1.0+0.01+i*0.5,0.5+0.01-j*0.5,0.0);
			glVertex3f(objects[c].v3[0],objects[c].v3[1],0.0);
			glVertex3f(objects[c].v4[0],objects[c].v4[1],0.0);

			c++;
		}
	}
    glEnd();
}


void handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	
	if (key == GLFW_KEY_EQUAL && action == GLFW_PRESS)
	{
		scale += 0.05;
	}

	if (key == GLFW_KEY_MINUS && action == GLFW_PRESS)
	{
		scale -= 0.05;
	}

	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
		x -= 0.05;
	}

	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		x += 0.05;
	}

	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		y += 0.05;
	}

	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		y -= 0.05;
	}

	if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		angle_calc += 2;
	}

	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		angle_calc -= 2;
	}

	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{	
		createVertices();
		cout << endl << "!!! RESET !!!" << endl << endl;
	}

	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
	{	
		question1=1;
	}

	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{	
		question1=0;
	}	

}


int main(void) 
{
	const GLint WIDTH = 600, HEIGHT = 600;

	// Initializing GLFW
	if(!glfwInit())
	{
		cout("GLFW initialization failed.");
		glfwTerminate();
		return 1;
	}

	// Setup GLFW window properties
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Assignment 1", NULL, NULL);
	if(!mainWindow)
	{
		cout("GLFW window creation failed.");
		glfwTerminate();
		return 1;
	}

	// Get buffer size information
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if(glewInit() != GLEW_OK)
	{
		cout("GLEW initialization failed.");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Setup viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	createVertices();
	
	// Loop until window is closed
	while(!glfwWindowShouldClose(mainWindow))
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	    glEnable( GL_BLEND );
		// Get and handle user input
		glfwPollEvents();
		glfwSetKeyCallback(mainWindow, handleKeys);//(window,fn)
		glfwSetMouseButtonCallback(mainWindow, mouse_button_callback);
		// Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		// Clear colour buffer before next frame
		glClear(GL_COLOR_BUFFER_BIT);
		glRasterPos2i(0, 0);
		if(question1==0) drawCalc();
		else if(question1==1) {
			for(int i=0;i<16;i++) drawCalcButton(i);
		}
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		//glutMotionFunc(motion);

		double xpos,ypos;
        glfwGetCursorPos(mainWindow, &xpos, &ypos);
        GetOGLPos(xpos,ypos);
        
		if(flag_square_animate==1) square_animate(posX,posY,posZ);
		if(rightclick_count==1) chooseCalcButton(posX,posY,posZ);
		if(rightclick_count==3) transformCalcButton(button_select_index,newX,newY,newZ);//,centerx,centery);



		glfwSwapBuffers(mainWindow);
	}

	return 0;
}

