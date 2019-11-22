//
//  FrustumG.hpp
//  LastHope
//
//  Created by Ruben Renteria on 11/1/19.
//  Copyright © 2019 Ruben Renteria. All rights reserved.
//

#ifndef FrustumG_hpp
#define FrustumG_hpp

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

#include <stdio.h>

class FrustumG {
    protected:
    GLfloat fov, ratio, nearDist, farDist, Hnear, Wnear, Hfar, Wfar;
    glm::vec3 eye, center, up;
    
    public:
    glm::vec3 normalTop, normalBottom, normalRight, normalLeft, normalFront, normalBack;
    std::vector<glm::vec3> faces;
    
    glm::vec3 getEye(){return eye;}
    
    void setCamIntervals(GLfloat newFov, GLfloat newRatio, GLfloat newNearDist, GLfloat newFarDist);
    
    void setCamDef(glm::vec3 newEye, glm::vec3 newCenter, glm::vec3 newUp);
    
    bool sphereCheck(glm::vec3 position, GLfloat radius);
    
    FrustumG(glm::vec3 newEye, glm::vec3 newCenter, glm::vec3 newUp);
};

#endif /* FrustumG_hpp */
