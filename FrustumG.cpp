//
//  FrustumG.cpp
//  LastHope
//
//  Created by Ruben Renteria on 11/1/19.
//  Copyright © 2019 Ruben Renteria. All rights reserved.
//



#include "FrustumG.hpp"



FrustumG::FrustumG(glm::vec3 newEye, glm::vec3 newCenter, glm::vec3 newUp){
    eye = newEye;
    center = newCenter;
    up = newUp;
}


bool FrustumG::sphereCheck(glm::vec3 position, GLfloat radius){
    float distance = 0;
    bool result = true;
    glm::vec3 nc = eye + glm::normalize(eye - center) * nearDist;
    glm::vec3 fc = eye + glm::normalize(eye - center) * farDist;
    
    /**
    for(auto face : faces){
        distance = glm::dot((position - eye), face);
        if(distance < radius){
            std::cout << true << std::endl;
        }
        else{
            std::cout << false << std::endl;
        }
    }
    
    distance = glm::dot((position - fc), normalBack);
    if(distance < radius)
        std::cout << 1 << std::endl;
    else std::cout << "0\n";
    
    distance = glm::dot((position - nc), normalFront);
    if(distance < radius)
        std::cout << "1\n";
    else std::cout << "0\n";
    
    std::cout << std::endl;
    /**/
    for(auto face : faces){
        distance = glm::dot((position - eye), face);
        if(distance < radius)
            return true;
        else{
            result = false;
        }
    }
    distance = glm::dot((position - nc), normalFront);
    if(distance < radius)
        return true;
    else{
        result = false;
    }
    
    distance = glm::dot((position - fc), normalBack);
    if(distance < radius)
        return true;
    else{
        result = false;
    }
    
    return result;
}

void FrustumG::setCamIntervals(GLfloat newFov, GLfloat newRatio, GLfloat newNearDist, GLfloat newFarDist){
    
    fov = newFov;
    ratio = newRatio;
    nearDist = newNearDist;
    farDist = newFarDist;
    
    Hnear = 2 * glm::tan(fov / 2) * nearDist;
    Wnear = Hnear * ratio;
    Hfar = 2 * glm::tan(fov/2) * farDist;
    Wfar = Hfar * ratio;
    
    glm::vec3 d = glm::normalize(eye - center);
    glm::vec3 right = glm::cross((eye - center), up);
    glm::vec3 nc = eye + d * nearDist;
    glm::vec3 fc = eye + d * farDist;
    glm::vec3 a = (nc + (right * Wnear) * .5f) - eye;
    a = glm::normalize(a);
    normalRight = glm::cross(a, up);
    
    a = (nc + (-right * Wnear) * .5f) - eye;
    a = glm::normalize(a);
    normalLeft = glm::cross(up,a);
    
    a = (nc + (up * Hnear) * .5f) - eye;
    a = glm::normalize(a);
    normalTop = glm::cross(right,a);
    
    a = (nc + (-up * Hnear) * .5f) - eye;
    a = glm::normalize(a);
    normalBottom = glm::cross(a,right);
    
    normalFront = -d;
    normalBack = -d;
    
    faces.clear();
    faces.push_back(normalTop);
    faces.push_back(normalBottom);
    faces.push_back(normalRight);
    faces.push_back(normalLeft);
    
}

void FrustumG::setCamDef(glm::vec3 newEye, glm::vec3 newCenter, glm::vec3 newUp){
    eye = newEye;
    center = newCenter;
    up = newUp;
}
