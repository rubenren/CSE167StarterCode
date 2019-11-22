#ifndef _POINT_CLOUD_H_
#define _POINT_CLOUD_H_

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <algorithm>

#include "Node.h"
#include "debugger.hpp"

//unsigned int cubemapTexture;

class PointCloud : public Node {
private:
	std::vector<glm::vec3> points;
    std::vector<glm::vec3> norms;
    std::vector<glm::ivec3> faceIs;
	GLuint vao, ebo;
    GLuint vbos[2];
	GLfloat pointSize;
    glm::mat4 model;
    glm::vec3 color;
public:
    glm::vec3 ambience;
    glm::vec3 diffuse;
    glm::vec3 specular;
    GLfloat shininess;
    
    void setModel(glm::mat4 inputMat) { model = inputMat; }
    void setColor(glm::vec3 inputColor) { color = inputColor; }
    
	PointCloud(std::string objFilename, GLfloat pointSize);
	~PointCloud();

	void draw(GLuint program, glm::mat4 C, FrustumG& cam);
    void draw(GLuint program, glm::mat4 C, FrustumG& cam, glm::mat4 inView);
	void update(glm::mat4 adjustment);

	void updatePointSize(GLfloat size);
	void spin(float deg, glm::vec3 rotAxis);
    
    void centerVertices(std::vector<glm::vec3>&);
    void scale(GLfloat factor);
    void translate(glm::vec3 direction);
};

#endif

