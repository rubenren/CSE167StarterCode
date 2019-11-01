//
//  Geometry.hpp
//  LastHope
//
//  Created by Ruben Renteria on 10/29/19.
//  Copyright © 2019 Ruben Renteria. All rights reserved.
//

#ifndef Geometry_hpp
#define Geometry_hpp

#include <stdio.h>

#include "Node.h"

class Geometry : public Node{
protected:
    std::vector<glm::vec3> points;
    std::vector<glm::vec3> norms;
    std::vector<GLuint> vertexIndicies;
    std::vector<GLuint> normalIndicies;
    std::vector<GLuint> textureIndicies;
    GLuint vao, ebo;
    GLuint vbos[2];
    GLfloat pointSize;
    glm::mat4 model;
    glm::vec3 color;
    
public:
    
    glm::mat4 getModel() { return model; }
    glm::vec3 getColor() { return color; }
    
    Geometry(std::string);
    Geometry();
    
    void draw(GLuint program, glm::mat4 C, FrustumG& cam);
    void update(glm::mat4 adjustment);
    void render(GLuint program);
    void init(std::string);
    void setModelMatrix(glm::mat4 newModel);
};

#endif /* Geometry_hpp */
