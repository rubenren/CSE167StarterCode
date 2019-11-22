//
//  Track.cpp
//  LastHope
//
//  Created by Ruben Renteria on 11/18/19.
//  Copyright © 2019 Ruben Renteria. All rights reserved.
//

#include "Track.hpp"

GLfloat Track::getSlope(GLfloat t){
    glm::vec3 currentPos = getPoint(t);
    glm::vec3 samplerPos = getPoint(t + .0001f);
    GLfloat rise = samplerPos.y - currentPos.y;
    GLfloat run = glm::sqrt(glm::pow(samplerPos.x - currentPos.x, 2) + glm::pow(samplerPos.z - currentPos.z, 2));
    return rise / run;
}

glm::vec3 Track::getPoint(GLfloat t){
    if(t >= 1.f) t = 0.f;
    else if(t < 0.f) t = .9999f;
    GLfloat getThisOne = t * 8.f;
    GLuint slctr = int(getThisOne);
    getThisOne = getThisOne - GLfloat(slctr);
    return theCurves.at(slctr)->getPoint(getThisOne);
}

void Track::ensureC1(){
    std::vector<glm::vec3> temp1;
    std::vector<glm::vec3> temp2;
    glm::vec3 adjustment;
    themBars.clear();
    if(selector%3 != 1){
        for(int i = 0; i < 8; i++){
            temp1 = theCurves.at(i)->getCntrlPnts();
            temp2 = theCurves.at((i+1)%8)->getCntrlPnts();
            temp2[1] = 2.f * temp1.at(3) - temp1.at(2);
            theCurves.at((i+1)%8)->setCntrlPnts(temp2);
            themBars.push_back(temp1.at(2));
            themBars.push_back(temp1.at(3));
            themBars.push_back(temp2.at(0));
            themBars.push_back(temp2.at(1));
        }
    }
    else{
        for(int i = 0; i < 8; i++){
            temp1 = theCurves.at(i)->getCntrlPnts();
            temp2 = theCurves.at((i+1)%8)->getCntrlPnts();
            temp1[2] = 2.f * temp1.at(3) - temp2.at(1);
            theCurves.at(i)->setCntrlPnts(temp1);
            themBars.push_back(temp1.at(2));
            themBars.push_back(temp1.at(3));
            themBars.push_back(temp2.at(0));
            themBars.push_back(temp2.at(1));
        }
    }
}

void Track::setupBars(){
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, vbos);
    
    glBindVertexArray(vao);
    
    
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * themBars.size(), themBars.data(), GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    
    
    glBindVertexArray(0);
}

Track::Track(std::vector<BezierCurve*> inputCurves){
    theCurves = inputCurves;
    ensureC1();
    setupBars();
}

void Track::replaceCurves(std::vector<BezierCurve*> inputCurves){
    theCurves = inputCurves;
}

void Track::draw(GLuint program, glm::mat4 C, FrustumG &cam){
    setModelMatrix(C);
    for(auto curve : theCurves){
        curve->draw(program, model, cam);
    }
    if(shouldRenderBars){
        renderBars(program, model);
    }
}

void Track::renderBars(GLuint program, glm::mat4 C){
    
    glUniform3fv(glGetUniformLocation(program, "inColor"), 1, glm::value_ptr(glm::vec3(0.5f,0.5f,0)));
    glCheckError();
    glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glCheckError();
    
    
    glBindVertexArray(vao);
    
    glDrawArrays(GL_LINES, 0, themBars.size());
    
    glBindVertexArray(0);
    
}

void Track::adjustSelected(GLuint axis, GLfloat amount){
    if(axis == 1){
        theCurves.at(selector / 3)->update(selector%3, glm::vec3(amount,0,0));
        if(selector%3 == 0)
            theCurves.at((selector / 3 - 1) % 8)->update(3, glm::vec3(amount,0,0));
    }
    else if(axis == 2){
        theCurves.at(selector / 3)->update(selector%3, glm::vec3(0,amount,0));
        if(selector%3 == 0)
            theCurves.at((selector / 3 - 1) % 8)->update(3, glm::vec3(0,amount,0));
    }
    else if(axis == 3){
        theCurves.at(selector / 3)->update(selector%3, glm::vec3(0,0,amount));
        if(selector%3 == 0)
            theCurves.at((selector / 3 - 1) % 8)->update(3, glm::vec3(0,0,amount));
    }
    ensureC1();
    setupBars();
}

void Track::nextSelect(){
    theCurves.at(selector / 3)->update(4, glm::vec3(0,0,0));
    selector = (selector + 1) % 24;
    theCurves.at(selector / 3)->update(selector%3, glm::vec3(0,0,0));
}

void Track::prevSelect(){
    theCurves.at(selector / 3)->update(4, glm::vec3(0,0,0));
    if(selector == 0)
        selector = 23;
    else
        selector = selector - 1;
    theCurves.at(selector / 3)->update(selector%3, glm::vec3(0,0,0));
}
