#include "PointCloud.h"

#define SCREEN_SCALE 5


PointCloud::PointCloud(std::string objFilename, GLfloat pointSize) 
	: pointSize(pointSize)
{
	/* 
	 * TODO: Section 2: Currently, all the points are hard coded below. 
	 * Modify this to read points from an obj file.
	 */
    
    std::ifstream objFile(objFilename);
    
    if(objFile.is_open()){
        std::string lineAtATime;
        std::string temp;
        points.push_back(glm::vec3(0,0,0));
        norms.push_back(glm::vec3(0,0,0));
        faceIs.push_back(glm::ivec3(0,0,0));
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
                glm::ivec3 idx;
                
                ss >> temp;
                idx.x = stoi(temp.substr(0,temp.find("/")));
                ss >> temp;
                idx.y = stoi(temp.substr(0,temp.find("/")));
                ss >> temp;
                idx.z = stoi(temp.substr(0,temp.find("/")));
                faceIs.push_back(idx);
            }
            
        }
        
//        std::cout << faceIs.size() << std::endl << faceIs[0].x << " " << faceIs[0].y << " " << faceIs[0].z << std::endl;
    }
    else{
        std::cerr << "Can't open the file " << objFilename << std::endl;
    }
    
    objFile.close();
    
	/*
	 * TODO: Section 4, you will need to normalize the object to fit in the
	 * screen. 
	 */
    
    centerVertices(points);

	// Set the model matrix to an identity matrix. 
	model = glm::mat4(1);
	// Set the color. 
	color = glm::vec3(1, 0, 0);

    
	// Generate a vertex array (VAO) and a vertex buffer objects (VBO).
	glGenVertexArrays(1, &vao);
	glGenBuffers(2, vbos);
    
    glCheckError();
    
	// Bind to the VAO.
	glBindVertexArray(vao);
    
    glCheckError();
    
	// Bind to the first VBO. We will use it to store the points.
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	// Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(),
		&points[0], GL_STATIC_DRAW);
	// Enable vertex attribute 0. 
	// We will be able to access points through it.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    
    // Bind to the second vbo, storing normals
    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    // Pass in the data
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * norms.size(),
        &norms[0], GL_STATIC_DRAW);
    // Enable vertex attribute 1.
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    
    glCheckError();
    
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * faceIs.size(), faceIs.data(), GL_STATIC_DRAW);
    
    glCheckError();
    
	// Unbind from the VBO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
    
    glCheckError();
    
    
    
}

PointCloud::~PointCloud() 
{
	// Delete the VBO and the VAO.
	glDeleteBuffers(2, vbos);
	glDeleteVertexArrays(1, &vao);
}

void PointCloud::draw()
{
 
    glBindVertexArray(vao);
    
    glCheckError();
    
    glDrawElements(GL_TRIANGLES, 6 * faceIs.size(), GL_UNSIGNED_INT, 0);
    
    glCheckError();
    
    glBindVertexArray(0);
    
    glCheckError();
    
    /**
	// Bind to the VAO.
	glBindVertexArray(vao);
	// Set point size.
	glPointSize(pointSize);
	// Draw points 
	glDrawArrays(GL_POINTS, 0, points.size());
	// Unbind from the VAO.
	glBindVertexArray(0);
    /**/
}

void PointCloud::update()
{
	// Spin the cube by 1 degree.
	// spin(0.1f);
}

void PointCloud::updatePointSize(GLfloat size) 
{
    if(pointSize + size >= 0 && pointSize + size <= 100){
        pointSize += size;
    }
	/*
	 * TODO: Section 3: Implement this function to adjust the point size.
	 */
}

void PointCloud::spin(float deg, glm::vec3 rotAxis)
{
	// Update the model matrix by multiplying a rotation matrix
	model = glm::rotate(glm::radians(deg), rotAxis) * model;
    
}

void PointCloud::centerVertices(std::vector<glm::vec3> & toCenter){
    
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


void PointCloud::scale(GLfloat factor){
    factor += 1;
    model = glm::scale(model, glm::vec3(factor, factor, factor));
}

void PointCloud::translate(glm::vec3 direction){
    model = glm::translate(glm::mat4(1), direction);
}

void printGLError(const char* msg){
    const GLenum err = glGetError();
    if(err != GL_NO_ERROR){
        std::cerr << "OpenGL error: " << msg << ", " << err << std::endl;
    }
}

