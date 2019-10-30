#include "Window.h"
int Window::width;
int Window::height;

const char* Window::windowTitle = "GLFW Starter Project";

// Objects to display.
Cube * Window::cube;
PointCloud * Window::cubePoints;

// The object currently displaying.
Object * Window::currentObj;
PointCloud * Window::bunny;
PointCloud * Window::bear;
PointCloud * Window::dragon;
PointCloud * Window::lightSphere;

bool temp = true;


glm::mat4 Window::projection; // Projection matrix.

int Window::activeMvmnt = 0;
glm::vec3 Window::lastPoint = glm::vec3(0,0,0);
#define ROT_SCALE 200
#define SCALE_SCALE -.01

glm::vec3 lightPosition = glm::vec3(5.0, 5.0, 5.0);
glm::vec3 lightColor = glm::vec3(0.05,0.02,.8);

glm::vec3 Window::eye(0, 0, 20); // Camera position.
glm::vec3 Window::center(0, 0, 0); // The point we are looking at.
glm::vec3 Window::up(0, 1, 0); // The up direction of the camera.

// View matrix, defined by eye, center and up.
glm::mat4 Window::view = glm::lookAt(Window::eye, Window::center, Window::up);

GLuint Window::program; // The shader program id.
GLuint Window::lampS; // The lamps shader id.

GLuint Window::projectionLoc; // Location of projection in shader.
GLuint Window::viewLoc; // Location of view in shader.
GLuint Window::modelLoc; // Location of model in shader.
GLuint Window::colorLoc; // Location of color in shader.

bool Window::initializeProgram() {
	// Create a shader program with a vertex shader and a fragment shader.
	program = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
    

	// Check the shader program.
	if (!program)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}

	// Activate the shader program.
	glUseProgram(program);
	// Get the locations of uniform variables.
	projectionLoc = glGetUniformLocation(program, "projection");
	viewLoc = glGetUniformLocation(program, "view");
	modelLoc = glGetUniformLocation(program, "model");
	colorLoc = glGetUniformLocation(program, "color");

	return true;
}

bool Window::initializeObjects()
{
	// Create a cube of size 5.
	cube = new Cube(5.0f);
	// Create a point cloud consisting of cube vertices.
	cubePoints = new PointCloud("foo", 100);

    // Creates pointclouds of the OBJ files
    #ifdef __APPLE__
    bunny = new PointCloud("res/bunny.obj", 10);
    bear = new PointCloud("res/bear.obj", 10);
    dragon = new PointCloud("res/dragon.obj", 10);
    lightSphere = new PointCloud("res/sphere.obj", 10);
    #else
    bunny = new PointCloud(".\\res\\bunny.obj", 10);
    bear = new PointCloud(".\\res\\bear.obj", 10);
    dragon = new PointCloud(".\\res\\dragon.obj", 10);
    lightSphere = new PointCloud(".\\res\\sphere.obj", 10);
    #endif
    
	// Set cube to be the first to display
	currentObj = cube;
    glUniform1i(glGetUniformLocation(program, "matType"), 0);
    
    // bunny material options
    bunny->ambience = glm::vec3(.01, .01, .01);
    bunny->diffuse = glm::vec3(0.7568, 0.61424, 0.7568);
    bunny->specular = glm::vec3(0.8,0.8,0.8);
    bunny->shininess = 64;
    
    // dragon material options
    dragon->ambience = glm::vec3(.01, .01, .01);
    dragon->diffuse = glm::vec3(0.8, 0.8, 0.8);
    dragon->specular = glm::vec3(0.8, 0.8, 0.8);
    dragon->shininess = 64;
    
    // bear material options
    bear->ambience = glm::vec3(.01, .01, .01);
    bear->diffuse = glm::vec3(0.0, 0.0, 0.0);
    bear->specular = glm::vec3(0.8, 0.8, 0.8);
    bear->shininess = 64;
    
    // lamp material options
    lightSphere->ambience = lightColor;
    lightSphere->diffuse = glm::vec3(0.0, 0.0, 0.0);
    lightSphere->specular = glm::vec3(0.0, 0.0, 0.0);

	return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
	delete cube;
    delete bunny;
    delete dragon;
    delete bear;
	delete cubePoints;
    
    
    glCheckError();
    

	// Delete the shader program.
	glDeleteProgram(program);
}

GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ 
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height); 
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size.
	glViewport(0, 0, width, height);

	// Set the projection matrix.
	Window::projection = glm::perspective(glm::radians(60.0), 
		double(width) / (double)height, 1.0, 1000.0);
}

void Window::idleCallback()
{
	// Perform any updates as necessary. 
	currentObj->update();
}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    

	// Specify the values of the uniform variables we are going to use.
	glm::mat4 model = currentObj->getModel();
	glm::vec3 color = currentObj->getColor();
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(colorLoc, 1, glm::value_ptr(color));
    glUniform3f(glGetUniformLocation(program, "pointLights[0].position"), lightPosition.x, lightPosition.y, lightPosition.z);
    glUniform3f(glGetUniformLocation(program, "pointLights[0].color"), lightColor.x,  lightColor.y,  lightColor.z);
    glUniform1f(glGetUniformLocation(program, "pointLights[0].constant"), 1.0f);
    glUniform1f(glGetUniformLocation(program, "pointLights[0].linear"), 0.09);
    glUniform1f(glGetUniformLocation(program, "pointLights[0].quadratic"), 0.01);
    
    // populating materials
    std::vector<PointCloud *> temp;
    temp.push_back(bunny);
    temp.push_back(dragon);
    temp.push_back(bear);
    
    std::string holder = "";

    for(int i = 0; i < temp.size(); i++){
        holder = "material[" + std::to_string(i) + "].ambient";
        glUniform3f(glGetUniformLocation(program, holder.c_str()), temp.at(i)->ambience.x, temp.at(i)->ambience.y, temp.at(i)->ambience.z);
        holder = "material[" + std::to_string(i) + "].diffuse";
        glUniform3f(glGetUniformLocation(program, holder.c_str()), temp.at(i)->diffuse.x, temp.at(i)->diffuse.y, temp.at(i)->diffuse.z);
        holder = "material[" + std::to_string(i) + "].specular";
        glUniform3f(glGetUniformLocation(program, holder.c_str()), temp.at(i)->specular.x, temp.at(i)->specular.y, temp.at(i)->specular.z);
        holder = "material[" + std::to_string(i) + "].shininess";
        glUniform1f(glGetUniformLocation(program, holder.c_str()), temp.at(i)->shininess);
        holder = "material[" + std::to_string(i) + "].lamp";
        glUniform1i(glGetUniformLocation(program, holder.c_str()), 0);
    }
    glUniform1i(glGetUniformLocation(program, "material[3].lamp"), 1);
    glCheckError();
    
    
    lightSphere->translate(lightPosition);
	// Render the object.
	currentObj->draw();
//    lightSphere->draw();
    
    glCheckError();
    
	// Gets events, including input such as keyboard and mouse or window resizing.
	glfwPollEvents();
	// Swap buffers.
	glfwSwapBuffers(window);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	/*
	 * TODO: Modify below to add your key callbacks.
	 */
	
	// Check for a key press.	if (action == GLFW_PRESS)
	{
		switch (key)
		{
            case GLFW_KEY_ESCAPE:
                // Close the window. This causes the program to also terminate.
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            case GLFW_KEY_1:
                // Set currentObj to cube
                currentObj = cube;
                break;
            case GLFW_KEY_2:
                // Set currentObj to cubePoints
                currentObj = cubePoints;
                break;
            case GLFW_KEY_F1:
                currentObj = bunny;
                glUniform1i(glGetUniformLocation(program, "matType"), 0);
                break;
            case GLFW_KEY_F2:
                currentObj = dragon;
                glUniform1i(glGetUniformLocation(program, "matType"), 1);
                break;
            case GLFW_KEY_F3:
                currentObj = bear;
                glUniform1i(glGetUniformLocation(program, "matType"), 2);
                break;
            case GLFW_KEY_P:
                if(mods == GLFW_MOD_SHIFT){
                    currentObj->updatePointSize(1);
                }
                else {
                    currentObj->updatePointSize(-1);
                }
                break;
            case GLFW_KEY_N:
                if(temp){
                    glUniform1i(glGetUniformLocation(program, "normalColoring"), 1);
                    temp = false;
                }
                else{
                    glUniform1i(glGetUniformLocation(program, "normalColoring"), 0);
                    temp = true;
                }
                break;
            default:
                break;
		}
	}
}

glm::vec3 Window::trackBallMapping(glm::vec2 point){
    glm::vec3 v;
    float d;
    
    v.x = (2.0f * point.x - width) / width;
    v.y = (height - 2.0f * point.y) / height;
    v.z = 0;
    
    d = glm::length(v);
    
    d = (d < 1.0f) ? d : 1.0f;
    v.z = sqrtf(1.001f - d*d);
    
    v = glm::normalize(v);
    return v;
}

void Window::cursorCallback(GLFWwindow* window, double x, double y){
    glm::vec3 curPoint;
    glm::vec3 direction;
    glm::vec3 rotAxis;
    glm::mat4 trans = glm::mat4(1.0f);
    GLfloat rotAngle;
    curPoint = trackBallMapping(glm::vec2(x,y));
    
    if(activeMvmnt){
        direction = curPoint - lastPoint;
        float velocity = glm::length(direction);
        if(velocity > .001){
            rotAxis = glm::cross(lastPoint, curPoint);
            rotAngle = velocity * ROT_SCALE;
            rotAxis = glm::normalize(rotAxis);
            /**
            std::cout << rotAngle
                << "\t(" << rotAxis.x
                << ", " << rotAxis.y
                << ", " << rotAxis.z
                << ")\n";
            */
            // ensure that the cross product will not be zero
            if(rotAxis.length() != 0)
                currentObj->spin(rotAngle, rotAxis);
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(trans));
            
        }
    }
    
    lastPoint = curPoint;
}

void Window::mouseCallback(GLFWwindow *window, int button, int action, int mods){
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        activeMvmnt = 1;
    }
    else if(action != GLFW_PRESS){
        activeMvmnt = 0;
    }
}

void Window::scrollCallback(GLFWwindow *window, double xOffset, double yOffset){
    currentObj->scale(yOffset * SCALE_SCALE);
}
