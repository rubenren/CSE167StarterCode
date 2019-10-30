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
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>


class Node {
public:
    virtual void draw(glm::mat4 C) = 0;
    virtual void update() = 0;
};



#endif /* Node_h */
