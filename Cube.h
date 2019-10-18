#ifndef _CUBE_H_
#define _CUBE_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>

#include "Object.h"

class Cube : public Object
{
private:
	GLuint vao;
	GLuint vbos[2];
public:
	Cube(float size);
	~Cube();

	void draw();
	void update();

	void spin(float deg);
    void spin(float deg, glm::vec3 rotAxis);
    void updatePointSize(GLfloat size);
    void scale(GLfloat factor);
    
};

#endif

