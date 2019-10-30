//
//  Geometry.cpp
//  LastHope
//
//  Created by Ruben Renteria on 10/29/19.
//  Copyright © 2019 Ruben Renteria. All rights reserved.
//

#include "Geometry.hpp"
#define SCREEN_SCALE 5

void centerVertices(std::vector<glm::vec3> & toCenter);

void Geometry::draw(glm::mat4 C){
    setModelMatrix(C);
    
}

void Geometry::render(){
    // Bind the VAO
    glBindVertexArray(vao);
    // Draw the points using triangles
    glDrawElements(GL_TRIANGLES, 6 * vertexIndicies.size(), GL_UNSIGNED_INT, 0);
    // Unbind the VAO
    glBindVertexArray(0);
}

void Geometry::init(std::string fileName){
    
    std::ifstream objFile(fileName);
        
    if(objFile.is_open()){
        std::string lineAtATime;
        std::string temp;
        points.push_back(glm::vec3(0,0,0));
        norms.push_back(glm::vec3(0,0,0));
        vertexIndicies.push_back(glm::ivec3(0,0,0));
        normalIndicies.push_back(glm::ivec3(0,0,0));
        textureIndicies.push_back(glm::ivec3(0,0,0));
        while(std::getline(objFile, lineAtATime)){
            std::stringstream ss;
            ss << lineAtATime;
            
            std::string label;
            ss >> label;
            
            if(label == "v"){
                glm::vec3 point;
                ss >> point.x >> point.y >> point.z;
                
                points.push_back(point);
            }
            else if(label == "vn"){
                glm::vec3 norm;
                ss >> norm.x >> norm.y >> norm.z;
                glm::normalize(norm);
                norms.push_back(norm);
            }
            else if(label == "f"){
                size_t current, previous = 0;
                glm::ivec3 vIdx, nIdx, tIdx;
                
                previous = 0;
                ss >> temp;
                current = temp.find("/");
                if(current != std::string::npos){
                    vIdx.x = stoi(temp.substr(previous,current - previous));
                    previous = current+1;
                    current = temp.find("/",previous);
                    
                    tIdx.x = stoi(temp.substr(previous,current - previous));
                    previous = current+1;
                    current = temp.size();
                    
                    nIdx.x = stoi(temp.substr(previous,current - previous));
                }
                
                previous = 0;
                ss >> temp;
                current = temp.find("/");
                if(current != std::string::npos){
                    vIdx.y = stoi(temp.substr(previous,current - previous));
                    previous = current+1;
                    current = temp.find("/",previous);
                    
                    tIdx.y = stoi(temp.substr(previous,current - previous));
                    previous = current+1;
                    current = temp.size();
                    
                    nIdx.y = stoi(temp.substr(previous,current - previous));
                }
                
                previous = 0;
                ss >> temp;
                current = temp.find("/");
                if(current != std::string::npos){
                    vIdx.z = stoi(temp.substr(previous,current - previous));
                    previous = current+1;
                    current = temp.find("/",previous);
                    
                    tIdx.z = stoi(temp.substr(previous,current - previous));
                    previous = current+1;
                    current = temp.size();
                    
                    nIdx.z = stoi(temp.substr(previous,current - previous));
                }
                vertexIndicies.push_back(vIdx);
                textureIndicies.push_back(tIdx);
                normalIndicies.push_back(nIdx);
            }
            
        }
        
//        std::cout << faceIs.size() << std::endl << faceIs[0].x << " " << faceIs[0].y << " " << faceIs[0].z << std::endl;
    }
    else{
        std::cerr << "Can't open the file " << fileName << std::endl;
    }
    
    objFile.close();
    
    centerVertices(points);
    
    // generate vertex array and buffer objects
    glGenVertexArrays(1, &vao);
    glGenBuffers(2, vbos);
    
    // Bind to the VAO to populate
    glBindVertexArray(vao);
    
    // Bind first VBO to bound VAO for data input. vertices for this one
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    // Pass in the points
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(), points.data(), GL_STATIC_DRAW);
    // Create channel for shader
    glEnableVertexAttribArray(0);
    // Tell shader how to read
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);
    
    // Bind the second VBO now. For normals this time
    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * norms.size(), norms.data(), GL_STATIC_DRAW);
    // Creating different channel for shader
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    
    // Generate EBO
    glGenBuffers(1, &ebo);
    // Bind this buffer to the VAO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    // Populate data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * vertexIndicies.size(), vertexIndicies.data(), GL_STATIC_DRAW);
    
    // Unbind the buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind the VAO
    glBindVertexArray(0);
    
}

void centerVertices(std::vector<glm::vec3> & toCenter){
    
    if(toCenter.empty()) return;  // return if there are no points
    
    // fill it with some values
    std::vector<GLfloat> MinMaxesMid;
    MinMaxesMid.push_back(toCenter[0].x);
    MinMaxesMid.push_back(toCenter[0].x);
    MinMaxesMid.push_back(toCenter[0].y);
    MinMaxesMid.push_back(toCenter[0].y);
    MinMaxesMid.push_back(toCenter[0].z);
    MinMaxesMid.push_back(toCenter[0].z);

    // grab the absolute max and mins in each axis
    for(auto vertice : toCenter){
        if(vertice.x < MinMaxesMid.at(0)) MinMaxesMid.at(0) = vertice.x;
        if(vertice.x > MinMaxesMid.at(1)) MinMaxesMid.at(1) = vertice.x;
        if(vertice.y < MinMaxesMid.at(2)) MinMaxesMid.at(2) = vertice.y;
        if(vertice.y > MinMaxesMid.at(3)) MinMaxesMid.at(3) = vertice.y;
        if(vertice.z < MinMaxesMid.at(4)) MinMaxesMid.at(4) = vertice.z;
        if(vertice.z > MinMaxesMid.at(5)) MinMaxesMid.at(5) = vertice.z;
    }
    
    // calculate the midpoint (x, y, z)
    MinMaxesMid.push_back((MinMaxesMid.at(0) + MinMaxesMid.at(1)) / 2);
    MinMaxesMid.push_back((MinMaxesMid.at(2) + MinMaxesMid.at(3)) / 2);
    MinMaxesMid.push_back((MinMaxesMid.at(4) + MinMaxesMid.at(5)) / 2);
    
    // translate the points
    for(auto & vertice : toCenter){
        vertice.x -= MinMaxesMid.at(6);
        vertice.y -= MinMaxesMid.at(7);
        vertice.z -= MinMaxesMid.at(8);
    }
    
    // translate the min and max records for use later, and absolute valueing them
    for(int i = 0; i < 3; i++){
        // all of the minimums
        MinMaxesMid[2 * i] -= MinMaxesMid.at(i + 6);
        // all of the maximums
        MinMaxesMid[2 * i + 1] -= MinMaxesMid.at(i + 6);
        MinMaxesMid[2 * i] *= -1;
    }
    
    
    // calculate the scalar per the max point
    GLfloat scalar = SCREEN_SCALE / *std::max(MinMaxesMid.begin(), MinMaxesMid.end() - 4);
    
    // multiply all of the vectors by the calculated scalar
    for(auto & vertice : toCenter){
        vertice *= scalar;
    }
}
