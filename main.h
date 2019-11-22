#ifndef _MAIN_H_
#define _MAIN_H_

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/OpenGL.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include <OpenGL/OpenGL.h>


#include <stdlib.h>
#include <stdio.h>
#include "Window.h"

#endif
