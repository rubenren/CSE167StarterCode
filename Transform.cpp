//
//  Transform.cpp
//  LastHope
//
//  Created by Ruben Renteria on 10/29/19.
//  Copyright © 2019 Ruben Renteria. All rights reserved.
//

#include "Transform.hpp"


Transform::Transform(glm::mat4 newM) : M(newM){
    
}

void Transform::draw(GLuint program, glm::mat4 C, FrustumG& cam){
    if(boundingSet && cullingActive){
        if(cam.sphereCheck(M * glm::vec4(boundingCenter,1), radius)){
            shouldRender = true;
        }
        else{
            shouldRender = false;
        }
    }
    if(shouldRender){
        glm::mat4 M_new = C * M;
        for(Node* child : children){
            child->draw(program, M_new, cam);
        }
    }
}

void Transform::update(glm::mat4 adjustment){
    M = adjustment * M;
    setBoundingSphere(boundingCenter, radius);
}

void Transform::addChild(Node* newChild){
    children.push_back(newChild);
}

void Transform::setBoundingSphere(glm::vec3 position, GLfloat radius){
    boundingCenter = position;
    this->radius = radius;
    boundingSet = true;
}
