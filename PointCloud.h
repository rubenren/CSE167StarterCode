#ifndef _POINT_CLOUD_H_
#define _POINT_CLOUD_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
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

#include "Object.h"
#include "debugger.hpp"

class PointCloud : public Object
{
private:
	std::vector<glm::vec3> points;
    std::vector<glm::vec3> norms;
    std::vector<glm::ivec3> faceIs;
	GLuint vao, ebo;
    GLuint vbos[2];
	GLfloat pointSize;
public:
    glm::vec3 ambience;
    glm::vec3 diffuse;
    glm::vec3 specular;
    GLfloat shininess;
    
	PointCloud(std::string objFilename, GLfloat pointSize);
	~PointCloud();

	void draw();
	void update();

	void updatePointSize(GLfloat size);
	void spin(float deg, glm::vec3 rotAxis);
    
    void centerVertices(std::vector<glm::vec3>&);
    void scale(GLfloat factor);
    void translate(glm::vec3 direction);
};

#endif

