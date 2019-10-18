//
//  debugger.hpp
//  LastHope
//
//  Created by Ruben Renteria on 10/16/19.
//  Copyright © 2019 Ruben Renteria. All rights reserved.
//

#ifndef debugger_hpp
#define debugger_hpp

#include <stdio.h>
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

GLenum glCheckError_(const char *file, int line);


#define glCheckError() glCheckError_(__FILE__, __LINE__)

#endif /* debugger_hpp */


