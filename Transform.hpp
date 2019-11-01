//
//  Transform.hpp
//  LastHope
//
//  Created by Ruben Renteria on 10/29/19.
//  Copyright © 2019 Ruben Renteria. All rights reserved.
//

#ifndef Transform_hpp
#define Transform_hpp

#include <stdio.h>
#include <list>

#include "Node.h"

class Transform : public Node{
protected:
    glm::mat4 M;
    std::list<Node*> children;
public:
    bool shouldRender = true;
    bool boundingSet = false;
    bool cullingActive = false;
    glm::vec3 boundingCenter;
    GLfloat radius;
    Transform(glm::mat4 newM);
    void draw(GLuint program, glm::mat4 C, FrustumG& cam);
    void update(glm::mat4 adjustment);
    void addChild(Node* newChild);
    void setBoundingSphere(glm::vec3 center, GLfloat radius);
};

#endif /* Transform_hpp */
