//
//  BezierCurve.hpp
//  LastHope
//
//  Created by Ruben Renteria on 11/16/19.
//  Copyright © 2019 Ruben Renteria. All rights reserved.
//

#ifndef BezierCurve_hpp
#define BezierCurve_hpp

#include <stdio.h>
#include <vector>
#include "Geometry.hpp"

class BezierCurve : public Geometry {
protected:
    std::vector<glm::vec3> cntrlPnts, points, constants, anchorPts, cntrlPntColors, pntColors, apprxPnts;
    std::vector<glm::ivec2> indicies;
    GLuint selected;
    GLuint vaos[4];
    bool isBoosted = false;
    
public:
    bool getBoostedState(){ return isBoosted; }
    void setBoosted(bool incoming){ isBoosted = incoming; }
    BezierCurve(std::vector<glm::vec3> newPoints);
    glm::vec3 getPoint(GLfloat t);
    void setCntrlPnts(std::vector<glm::vec3> inPts);
    std::vector<glm::vec3> getCntrlPnts(){ return cntrlPnts; }
    void update(GLuint idx, glm::vec3 adjustment);
    void draw(GLuint program, glm::mat4 C, FrustumG& cam);
    void extracted(GLfloat &factor, GLfloat increment, unsigned int sampleNum);
    
    void setPointsAlongCurve(unsigned int sampleNum);
    void setVABOs();
    void setIndicies();
};


#endif /* BezierCurve_hpp */
