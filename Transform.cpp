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

void Transform::draw(glm::mat4 C){
    glm::mat4 M_new = C * M;
    for(Node* child : children){
        child->draw(M_new);
    }
}

void Transform::update(){
    
}

void Transform::addChild(Node* newChild){
    children.push_back(newChild);
}
