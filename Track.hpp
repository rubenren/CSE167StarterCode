//
//  Track.hpp
//  LastHope
//
//  Created by Ruben Renteria on 11/18/19.
//  Copyright © 2019 Ruben Renteria. All rights reserved.
//

#ifndef Track_hpp
#define Track_hpp

#include <stdio.h>
#include "Geometry.hpp"
#include "BezierCurve.hpp"


class Track : public Geometry{
protected:
    
    std::vector<BezierCurve*> theCurves;
    std::vector<glm::vec3> themBars;
    bool shouldRenderBars = true;
    bool isThereFriction = false;
    GLuint selector = 0;
    
public:
    void nextSelect();
    void prevSelect();
    
    void adjustSelected(GLuint axis, GLfloat amount);
    
    glm::vec3 getPoint(GLfloat t);
    
    bool checkBoosted(GLfloat t);
    
    void setBarRender(bool shouldI){ shouldRenderBars = shouldI; }
    
    GLfloat getSlope(GLfloat t);
    
    void toggleBoosted(unsigned int idx);
    
    Track(std::vector<BezierCurve*> inputCurves);
    void replaceCurves(std::vector<BezierCurve*> inputCurves);
    void draw(GLuint program, glm::mat4 C, FrustumG& cam);
    void ensureC1();
    void renderBars(GLuint program, glm::mat4 C);
    void setupBars();
    
};



#endif /* Track_hpp */
