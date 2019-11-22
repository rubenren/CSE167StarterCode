//
//  BezierCurve.cpp
//  LastHope
//
//  Created by Ruben Renteria on 11/16/19.
//  Copyright © 2019 Ruben Renteria. All rights reserved.
//

#define NUM_OF_POINTS 200

#include "BezierCurve.hpp"
/**
    follows equations from the cubic polynomial form to hold constants for later calculation of points
 */
std::vector<glm::vec3> getConstants(std::vector<glm::vec3> &inPoints){
    
    glm::vec3 a = -inPoints.at(0) + 3.f*inPoints.at(1) - 3.f*inPoints.at(2) + inPoints.at(3);
    
    glm::vec3 b = 3.f*inPoints.at(0) - 6.f*inPoints.at(1) + 3.f*inPoints.at(2);
    
    glm::vec3 c = -3.f*inPoints.at(0) + 3.f*inPoints.at(1);
    
    glm::vec3 d = inPoints.at(0);
    
    std::vector<glm::vec3> outPut;
    
    outPut.push_back(a);
    outPut.push_back(b);
    outPut.push_back(c);
    outPut.push_back(d);
    
    return outPut;
    
}

void BezierCurve::setCntrlPnts(std::vector<glm::vec3> inPts){
    cntrlPnts = inPts;
    anchorPts = {inPts.at(0), inPts.at(3)};
    apprxPnts = {inPts.at(1), inPts.at(2)};
    constants = getConstants(cntrlPnts);
    setPointsAlongCurve(NUM_OF_POINTS);
    setIndicies();
    setVABOs();
}

glm::vec3 BezierCurve::getPoint(GLfloat t){
    glm::vec3 res(0,0,0);
    res = constants.at(0) * t * t * t;
    res += constants.at(1) * t * t;
    res += constants.at(2) * t;
    res += constants.at(3);
    return res;
}

void BezierCurve::setPointsAlongCurve(unsigned int sampleNum){
    points.clear();
    GLfloat increment = 1/float(sampleNum);
    GLfloat factor = 0.f;
    glm::vec3 toEnter(0.f,0,0);
    for(int i = 0; i < sampleNum; i++){
        toEnter = getPoint(factor);
        points.push_back(toEnter);
        factor += increment;
    }
}

void BezierCurve::draw(GLuint program, glm::mat4 C, FrustumG& cam){
    setModelMatrix(C);
        
    glUniform3fv(glGetUniformLocation(program, "inColor"), 1, glm::value_ptr(color));
    glCheckError();
    glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glCheckError();
    
    glBindVertexArray(vaos[0]);
    
    // Drawing the line
    glDrawArrays(GL_LINE_STRIP, 0, points.size());
//    glDrawElements(GL_LINE_STRIP, points.size(), GL_UNSIGNED_INT, 0);
    glCheckError();
    
    glBindVertexArray(vaos[1]);
    
    glPointSize(5.0f);
    
    glDrawArrays(GL_POINTS, 0, cntrlPnts.size() - 1);
    glCheckError();

    glBindVertexArray(0);
}

void BezierCurve::update(GLuint idx, glm::vec3 adjustment){
    if(idx < 4 and idx >= 0){
        if(selected != 0 && selected != 3){
            cntrlPntColors.at(selected) = glm::vec3(0,0.6f,0);
        }
        else cntrlPntColors.at(selected) = glm::vec3(0.6f,0,0);
        cntrlPntColors.at(idx) = glm::vec3(0,0,0.6f);
        selected = idx;
        cntrlPnts.at(idx) += adjustment;
        anchorPts = {cntrlPnts.at(0), cntrlPnts.at(3)};
        apprxPnts = {cntrlPnts.at(1), cntrlPnts.at(2)};
    }
    else if(idx == 4){
        cntrlPntColors.at(0) = glm::vec3(.6f,0,0);
        cntrlPntColors.at(3) = glm::vec3(.6f,0,0);
        cntrlPntColors.at(1) = glm::vec3(0,.6f,0);
        cntrlPntColors.at(2) = glm::vec3(0,.6f,0);
        selected = 3;
    }
    constants = getConstants(cntrlPnts);
    setPointsAlongCurve(NUM_OF_POINTS);
    setVABOs();
}

void BezierCurve::setVABOs(){
    glGenVertexArrays(2, vaos);
    glGenBuffers(3, vbos);
    
    // Bind to vertex array
    glBindVertexArray(vaos[0]);
    
    // Bind to vertex buffer to read in points
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    // Pass in data
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(), points.data(), GL_STATIC_DRAW);
    // Create channel to shader
    glEnableVertexAttribArray(0);
    // Tell shader how to read
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    
    /**/
    // Bind to the vao for the control points
    glBindVertexArray(vaos[1]);
    // Bind to second vbo for the control points
    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    // Pass in the data
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * cntrlPnts.size(), cntrlPnts.data(), GL_STATIC_DRAW);
    // Create the shader channel
    glEnableVertexAttribArray(0);
    // Tell shader how to read
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    glCheckError();
    
    
    glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * cntrlPntColors.size(), cntrlPntColors.data(), GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    /**/
    
    glCheckError();
    // Unbind from the VAO.
    glBindVertexArray(0);
    
}

BezierCurve::BezierCurve(std::vector<glm::vec3> newPoints){
    cntrlPnts = newPoints;
    cntrlPntColors = {
        glm::vec3(.6f,0,0),
        glm::vec3(0,.6f,0),
        glm::vec3(0,.6f,0),
        glm::vec3(.6f,0,0)
    };
    selected = 0;
    anchorPts = {newPoints.at(0), newPoints.at(3)};
    apprxPnts = {newPoints.at(1), newPoints.at(2)};
    color = glm::vec3(.0f,.0,.0);
    constants = getConstants(cntrlPnts);
    setPointsAlongCurve(NUM_OF_POINTS);
    for(int i = 0; i < points.size(); i++){
        pntColors.push_back(glm::vec3(0.f,0,0));
    }
    setIndicies();
    setVABOs();
}

void BezierCurve::setIndicies(){
    /**
    for(unsigned int i = 0; i < points.size(); i++){
        indicies.push_back(glm::ivec1(i));
    }
    /**
    for(int i = 0; i < points.size(); i++){
        indicies.push_back(glm::ivec2(i,i+1));
    }
     /**/
}
