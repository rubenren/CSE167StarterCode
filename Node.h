//
//  Node.h
//  LastHope
//
//  Created by Ruben Renteria on 10/29/19.
//  Copyright © 2019 Ruben Renteria. All rights reserved.
//


#ifndef Node_h
#define Node_h

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "debugger.hpp"
#include "FrustumG.hpp"
#include "stb_image.h"


class Node {
public:
    virtual void draw(GLuint program, glm::mat4 C, FrustumG& cam) = 0;
    virtual void update(glm::mat4 adjustment) = 0;
};



#endif /* Node_h */
