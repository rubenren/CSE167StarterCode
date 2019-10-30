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
    Transform(glm::mat4 newM);
    void draw(glm::mat4 C);
    void update();
    void addChild(Node* newChild);
};

#endif /* Transform_hpp */
