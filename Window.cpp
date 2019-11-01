#include "Window.h"
int Window::width;
int Window::height;

const char* Window::windowTitle = "GLFW Starter Project";

// Objects to display.
Cube * Window::cube;
PointCloud * Window::cubePoints;

// The object currently displaying.
Node * Window::currentObj;
Geometry * Window::bodyGeo;
Geometry * Window::headGeo;
Geometry * Window::limbGeo;
Geometry * Window::antennaGeo;
Geometry * Window::eyeGeo;
Geometry * Window::bunnyGeo;
Geometry * Window::sphereGeo;
Transform * Window::rightLeg;
Transform * Window::leftLeg;
Transform * Window::rightArm;
Transform * Window::boundingSphere;
PointCloud * Window::bear;
PointCloud * Window::dragon;
PointCloud * Window::lightSphere;

int Window::updateCounter = 0;

bool oscillator = false;
bool temp = true;
bool boundToggle = false;
bool demoToggle = false;


glm::mat4 Window::projection; // Projection matrix.

int Window::activeMvmnt = 0;
glm::vec3 Window::lastPoint = glm::vec3(0,0,0);
#define ROT_SCALE 50
#define SCALE_SCALE .01

glm::vec3 lightPosition = glm::vec3(5.0, 5.0, 5.0);
glm::vec3 lightColor = glm::vec3(0.8,0.8,.8);

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

Transform * Window::satellite;
Transform * Window::satelliteArmy;
FrustumG Window::camera(eye, center, up);

bool Window::initializeObjects()
{
	// Create a cube of size 5.
	cube = new Cube(5.0f);
	// Create a point cloud consisting of cube vertices.
    
    // REMEMBER THIS IS NORMAL VIEWING MODE
    glUniform1i(glGetUniformLocation(program, "normalColoring"), 1);
    
    // initialize the camera to keep track
    camera.setCamDef(eye, center, up);

    // Creates pointclouds of the OBJ files
    #ifdef __APPLE__
//    bunnyGeo = new Geometry("res/bunny.obj");
    bodyGeo = new Geometry("res/robot-parts-2018/body_s.obj");
    headGeo = new Geometry("res/robot-parts-2018/head_s.obj");
    limbGeo = new Geometry("res/robot-parts-2018/limb_s.obj");
    eyeGeo = new Geometry("res/robot-parts-2018/eyeball_s.obj");
    antennaGeo = new Geometry("res/robot-parts-2018/antenna_s.obj");
    sphereGeo = new Geometry("res/sphere.obj");
    #else
    bear = new PointCloud(".\\res\\bear.obj", 10);
    dragon = new PointCloud(".\\res\\dragon.obj", 10);
    lightSphere = new PointCloud(".\\res\\sphere.obj", 10);
    #endif
    satellite = new Transform(glm::mat4(1));
    satelliteArmy = new Transform(glm::translate(glm::vec3(0,-5,0)));
    Transform * headMat = new Transform(glm::translate(glm::vec3(0,1.2,0)));
    Transform * leftArm = new Transform(glm::translate(glm::vec3(-1.2,0,0)));
    rightArm = new Transform(glm::translate(glm::vec3(1.2,0,0)));
    rightLeg = new Transform(glm::translate(glm::vec3(.6,-1.4,0)));
    leftLeg = new Transform(glm::translate(glm::vec3(-.6,-1.4,0)));
    Transform * rightEye = new Transform(glm::translate(glm::vec3(0.4,1.6,.8)));
    Transform * leftEye = new Transform(glm::translate(glm::vec3(-0.4,1.6,.8)));
    
    glm::mat4 antennaFix = glm::scale(glm::vec3(.33,.33,.33));
    antennaFix *= glm::rotate(glm::radians(30.f), glm::vec3(0,0,-1));
    Transform * rightAntenna = new Transform(glm::translate(glm::vec3(.2,2,0)) * antennaFix);
    
    antennaFix = glm::scale(glm::vec3(.33,.33,.33));
    antennaFix *= glm::rotate(glm::radians(30.f), glm::vec3(0,0,1));
    Transform * leftAntenna = new Transform(glm::translate(glm::vec3(-.2,2,0)) * antennaFix);
    
    
    rightLeg->update(glm::rotate(glm::radians(7.5f), glm::vec3(1,0,0)));
    leftLeg->update(glm::rotate(glm::radians(-7.5f), glm::vec3(1,0,0)));
    
	// Set cube to be the first to display
    satellite->addChild(bodyGeo);
    satellite->addChild(headMat);
    satellite->addChild(rightArm);
    satellite->addChild(leftArm);
    satellite->addChild(rightLeg);
    satellite->addChild(leftLeg);
    satellite->addChild(rightEye);
    satellite->addChild(leftEye);
    satellite->addChild(rightAntenna);
    satellite->addChild(leftAntenna);
    
    leftArm->addChild(limbGeo);
    rightArm->addChild(limbGeo);
    
    rightLeg->addChild(limbGeo);
    leftLeg->addChild(limbGeo);
    
    rightEye->addChild(eyeGeo);
    leftEye->addChild(eyeGeo);
    
    rightAntenna->addChild(antennaGeo);
    leftAntenna->addChild(antennaGeo);
    
    headMat->addChild(headGeo);
    
    float myScale = 2.8;
    boundingSphere = new Transform(glm::scale(glm::vec3(myScale, myScale, myScale)));
    boundingSphere->addChild(sphereGeo);
    boundingSphere->shouldRender = false;
    satellite->addChild(boundingSphere);
    satellite->setBoundingSphere(glm::vec3(0,0,0), 2.8f);
    
    
    Transform * holder;
    std::vector<Transform*> mySats;
    for(int i = 0; i < 100; i++){
        holder = new Transform(glm::mat4(1));
        mySats.push_back(holder);
    }
    for(Transform * sat : mySats){
        satelliteArmy->addChild(sat);
        sat->addChild(satellite);
    }
    int tempCount = 0;
    for(int i = -5; i < 5; i++){
        for(int j = -5; j < 5; j++){
            mySats.at(tempCount)->update(glm::translate(glm::vec3(i*5,0,j*5)));
            tempCount++;
        }
    }
    
	currentObj = satelliteArmy;
    glUniform1i(glGetUniformLocation(program, "matType"), 0);

	return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
	delete cube;
    delete bunnyGeo;
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

GLfloat Window::fov, Window::ratio, Window::nearDist, Window::farDist, Window::Hnear, Window::Wnear, Window::Hfar, Window::Wfar;

glm::vec3 Window::normalTop, Window::normalBottom, Window::normalRight, Window::normalLeft, Window::normalFront, Window::normalBack;

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
    
    fov = glm::radians(60.0);
    ratio = double(width) / double(height);
    nearDist = 1.0f;
    farDist = 1000.f;
    Hnear = 2 * glm::tan(fov / 2) * nearDist;
    Wnear = Hnear * ratio;
    Hfar = 2 * glm::tan(fov/2) * farDist;
    Wfar = Hfar * ratio;
    
    glm::vec3 d = glm::normalize(eye - center);
    glm::vec3 right = glm::cross((eye - center), up);
    glm::vec3 nc = eye + d * nearDist;
    glm::vec3 fc = eye + d * farDist;
    glm::vec3 a = (nc + (right * Wnear) * .5f) - eye;
    a = glm::normalize(a);
    normalRight = glm::cross(a, up);
    
    a = (nc + (-right * Wnear) * .5f) - eye;
    a = glm::normalize(a);
    normalLeft = glm::cross(a,up);
    
    a = (nc + (up * Hnear) * .5f) - eye;
    a = glm::normalize(a);
    normalTop = glm::cross(right,a);
    
    a = (nc + (-up * Hnear) * .5f) - eye;
    a = glm::normalize(a);
    normalBottom = glm::cross(a,right);
    
    normalFront = -d;
    normalBack = d;
    
    camera.setCamDef(eye, center, up);
    camera.setCamIntervals(fov, ratio, nearDist, farDist);
    
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height); 
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size.
	glViewport(0, 0, width, height);

	// Set the projection matrix.
	Window::projection = glm::perspective(fov, ratio, nearDist, farDist);
    
    
}

void Window::idleCallback()
{
    glm::mat4 limbRot(1);
    glm::mat4 limbRot2(1);
    updateCounter++;
    
    if(updateCounter % 300 == 0){
        oscillator = false;
    }
    else if(updateCounter % 150 == 0){
        oscillator = true;
    }
    
    limbRot = glm::translate(limbRot, glm::vec3(0,1,0));
    limbRot2 = glm::translate(limbRot2, glm::vec3(0,1,0));
    if(oscillator){
        limbRot = glm::rotate(limbRot, glm::radians(.1f), glm::vec3(1,0,0));
        limbRot2 = glm::rotate(limbRot2, glm::radians(.1f), glm::vec3(-1,0,0));
    }
    else{
        limbRot = glm::rotate(limbRot, glm::radians(.1f), glm::vec3(-1,0,0));
        limbRot2 = glm::rotate(limbRot2, glm::radians(.1f), glm::vec3(1,0,0));
    }
    limbRot = glm::translate(limbRot, glm::vec3(0,-1,0));
    limbRot2 = glm::translate(limbRot2, glm::vec3(0,-1,0));
    
    rightArm->update(limbRot);
    rightLeg->update(limbRot);
    leftLeg->update(limbRot2);
	// Perform any updates as necessary. 
	satellite->update(glm::mat4(1));
}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	// Specify the values of the uniform variables we are going to use.
//	glm::mat4 model = currentObj->getModel();
//	glm::vec3 color = currentObj->getColor();
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
//	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
//	glUniform3fv(colorLoc, 1, glm::value_ptr(color));
    glUniform3f(glGetUniformLocation(program, "pointLights[0].position"), lightPosition.x, lightPosition.y, lightPosition.z);
    glUniform3f(glGetUniformLocation(program, "pointLights[0].color"), lightColor.x,  lightColor.y,  lightColor.z);
    glUniform1f(glGetUniformLocation(program, "pointLights[0].constant"), 1.0f);
    glUniform1f(glGetUniformLocation(program, "pointLights[0].linear"), 0.09);
    glUniform1f(glGetUniformLocation(program, "pointLights[0].quadratic"), 0.01);
    
	// Render the object.
	currentObj->draw(program, glm::mat4(1), camera);
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
	
	// Check for a key press.
    if (action == GLFW_PRESS)
	{
		switch (key)
		{
            case GLFW_KEY_ESCAPE:
                // Close the window. This causes the program to also terminate.
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            case GLFW_KEY_1:
                // Set currentObj to cube
//                currentObj = cube;
                break;
            case GLFW_KEY_2:
                // Set currentObj to cubePoints
//                currentObj = cubePoints;
                break;
            case GLFW_KEY_F1:
//                currentObj = bunnyGeo;
                glUniform1i(glGetUniformLocation(program, "matType"), 0);
                break;
            case GLFW_KEY_F2:
//                currentObj = dragon;
                glUniform1i(glGetUniformLocation(program, "matType"), 1);
                break;
            case GLFW_KEY_F3:
//                currentObj = bear;
                glUniform1i(glGetUniformLocation(program, "matType"), 2);
                break;
            case GLFW_KEY_P:
                if(mods == GLFW_MOD_SHIFT){
//                    currentObj->updatePointSize(1);
                }
                else {
//                    currentObj->updatePointSize(-1);
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
            case GLFW_KEY_B:
                if(boundToggle){
                    boundToggle = false;
                    boundingSphere->shouldRender = true;
                }
                else{
                    boundToggle = true;
                    boundingSphere->shouldRender = false;
                }
                break;
            case GLFW_KEY_C:
                if(satellite->cullingActive){
                    satellite->cullingActive = false;
                }
                else{
                    satellite->cullingActive = true;
                }
                break;
            case GLFW_KEY_D:
                if(demoToggle){
                    demoToggle = false;
                    projection = glm::perspective(fov + .6f, ratio, nearDist, farDist);
                }
                else{
                    demoToggle = true;
                    projection = glm::perspective(fov, ratio, nearDist, farDist);
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
            if(rotAxis.length() != 0){
//                view = glm::rotate(glm::radians(rotAngle), rotAxis) * view;
                center = center - direction * (float)ROT_SCALE;
                camera.setCamDef(eye, center, up);
                
                view = glm::lookAt(eye, center, up);
            }
//                currentObj->spin(rotAngle, rotAxis);
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

float temporary = 0.f;
void Window::scrollCallback(GLFWwindow *window, double xOffset, double yOffset){
//    yOffset++;
    fov += (float)yOffset * SCALE_SCALE;
    camera.setCamIntervals(fov, ratio, nearDist, farDist);
    
    projection = glm::perspective(fov, ratio, nearDist, farDist);
}
