#pragma once
#ifndef _WINDOW_H_
#define _WINDOW_H_

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <memory>

#include "Object.h"
#include "Cube.h"
#include "PointCloud.h"
#include "shader.h"
#include "Node.h"
#include "Transform.hpp"
#include "Geometry.hpp"

class Window
{
public:
	static int width;
	static int height;
	static const char* windowTitle;
	static Cube* cube;
    static Geometry * bunnyGeo;
    static Geometry * bodyGeo;
    static Geometry * headGeo;
    static Geometry * limbGeo;
    static Geometry * eyeGeo;
    static Geometry * antennaGeo;
    static Geometry * sphereGeo;
    static Transform * rightLeg;
    static Transform * leftLeg;
    static Transform * rightArm;
    static Transform * boundingSphere;
    static PointCloud * bear;
    static PointCloud * dragon;
	static PointCloud * cubePoints;
    static PointCloud * lightSphere;
    static Transform * satellite;
    static Transform * satelliteArmy;
	static Node * currentObj;
    static int updateCounter;
    static int activeMvmnt;
    static FrustumG camera;
    static glm::vec3 lastPoint;
	static glm::mat4 projection;
	static glm::mat4 view;
	static glm::vec3 eye, center, up;
	static GLuint program, projectionLoc, viewLoc, modelLoc, colorLoc, lampS;
    static GLfloat fov, ratio, nearDist, farDist, Hnear, Wnear, Hfar, Wfar;
    static glm::vec3 normalTop, normalBottom, normalRight, normalLeft, normalFront, normalBack;

	static bool initializeProgram();
	static bool initializeObjects();
	static void cleanUp();
	static GLFWwindow* createWindow(int width, int height);
    static glm::vec3 trackBallMapping(glm::vec2 point);
	static void resizeCallback(GLFWwindow* window, int width, int height);
	static void idleCallback();
	static void displayCallback(GLFWwindow*);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void cursorCallback(GLFWwindow* window, double x, double y);
    static void mouseCallback(GLFWwindow* window, int button, int action, int mods);
    static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
};

#endif

