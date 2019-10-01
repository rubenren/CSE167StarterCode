#include "PointCloud.h"

PointCloud::PointCloud(std::string objFilename, GLfloat pointSize) 
	: pointSize(pointSize)
{
	/* 
	 * TODO: Section 2: Currently, all the points are hard coded below. 
	 * Modify this to read points from an obj file.
	 */
    /*
	points = 
	{
		glm::vec3(-2.5, 2.5, 2.5),
		glm::vec3(-2.5, -2.5, 2.5),
		glm::vec3(2.5, -2.5, 2.5),
		glm::vec3(2.5, 2.5, 2.5),
		glm::vec3(-2.5, 2.5, -2.5),
		glm::vec3(-2.5, -2.5, -2.5),
		glm::vec3(2.5, -2.5, -2.5),
		glm::vec3(2.5, 2.5, -2.5)
	};
     */
    
    std::ifstream objFile(objFilename);
    
    if(objFile.is_open()){
        std::string lineAtATime;
        
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
            
        }
    }
    else{
        std::cerr << "Can't open the file " << objFilename << std::endl;
    }
    
    objFile.close();
    
	/*
	 * TODO: Section 4, you will need to normalize the object to fit in the
	 * screen. 
	 */
    
    centerPoints();

	// Set the model matrix to an identity matrix. 
	model = glm::mat4(1);
	// Set the color. 
	color = glm::vec3(1, 0, 0);

	// Generate a vertex array (VAO) and a vertex buffer objects (VBO).
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	// Bind to the VAO.
	glBindVertexArray(vao);

	// Bind to the first VBO. We will use it to store the points.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(),
		points.data(), GL_STATIC_DRAW);
	// Enable vertex attribute 0. 
	// We will be able to access points through it.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Unbind from the VBO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
}

PointCloud::~PointCloud() 
{
	// Delete the VBO and the VAO.
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void PointCloud::draw()
{
	// Bind to the VAO.
	glBindVertexArray(vao);
	// Set point size.
	glPointSize(pointSize);
	// Draw points 
	glDrawArrays(GL_POINTS, 0, points.size());
	// Unbind from the VAO.
	glBindVertexArray(0);
}

void PointCloud::update()
{
	// Spin the cube by 1 degree.
	spin(0.1f);
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

void PointCloud::spin(float deg)
{
	// Update the model matrix by multiplying a rotation matrix
	model = glm::rotate(model, glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
}

void PointCloud::centerPoints(){
    /*
    std::vector<GLfloat> MinMaxes;
    MinMaxes.push_back(points[0].x);
    MinMaxes.push_back(points[0].x);
    MinMaxes.push_back(points[0].y);
    MinMaxes.push_back(points[0].y);
    MinMaxes.push_back(points[0].z);
    MinMaxes.push_back(points[0].z);
    */
    GLfloat xMin = points[0].x;
    GLfloat xMax = points[0].x;
    GLfloat yMin = points[0].y;
    GLfloat yMax = points[0].y;
    GLfloat zMin = points[0].z;
    GLfloat zMax = points[0].z;
    for(auto point : points){
        if(point.x < xMin) xMin = point.x;
        if(point.x > xMax) xMax = point.x;
        if(point.y < yMin) yMin = point.y;
        if(point.y > yMax) yMax = point.y;
        if(point.z < zMin) zMin = point.z;
        if(point.z > zMax) zMax = point.z;
    }
    
    GLfloat xMid = (xMin + xMax) / 2;
    GLfloat yMid = (yMin + yMax) / 2;
    GLfloat zMid = (zMin + zMax) / 2;
    glm::vec3 midPoint = glm::vec3(xMid, yMid, zMid);
    
    for(auto & point : points){
        point.x -= xMid;
        point.y -= yMid;
        point.z -= zMid;
    }
    
    // adjust the mins and maxes to match the center
    xMin -= xMid;
    xMax -= xMid;
    yMin -= yMid;
    yMax -= yMid;
    zMin -= zMid;
    zMax -= zMid;
    
    GLfloat scalar = 1;
    
}
