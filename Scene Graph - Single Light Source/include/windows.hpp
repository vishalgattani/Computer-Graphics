

void func2(GLFWwindow* window, int button, int action, int mods){
    static_cast<Controller*>(glfwGetWindowUserPointer(window))->handleClick(window, button, action, mods);
}

class Window{
private:
    GLFWwindow* window;
public:
    GLFWwindow* get();
    Window(const char*);
	Window();
	~Window();
    void swapBuffer();
    bool quit();
    void makeCurrnetContext();
};

Window::~Window(){
	glfwTerminate();
}

void Window::makeCurrnetContext(){
    glfwMakeContextCurrent(window);
}

bool Window::quit(){
    return glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0;
}

void Window::swapBuffer(){
    glfwSetMouseButtonCallback(window, func2);
    glfwSwapBuffers(window);
    glfwPollEvents();
}

GLFWwindow* Window::get(){
    return window;
}

Window::Window(){

}

Window::Window(const char* name){
    if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		exit(0);
    }
    
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  
    
    
    window = glfwCreateWindow( 1024, 768, name, NULL, NULL);
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

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwPollEvents();
    //glfwSetCursorPos(window, 1024/2, 768/2);
    //glClearColor(0.698f, 0.1765f, 0.134f, 0.2f);
	glClearColor(0.0f, 0.0f, 0.70f, 0.0f);
	
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS); 
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glShadeModel(GL_SMOOTH);

}
