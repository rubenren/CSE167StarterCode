#include "PointCloud.h"

#define SCREEN_SCALE 5

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
    
    if(points.empty()) return;  // return if there are no points
    
    // fill it with some values
    std::vector<GLfloat> MinMaxesMid;
    MinMaxesMid.push_back(points[0].x);
    MinMaxesMid.push_back(points[0].x);
    MinMaxesMid.push_back(points[0].y);
    MinMaxesMid.push_back(points[0].y);
    MinMaxesMid.push_back(points[0].z);
    MinMaxesMid.push_back(points[0].z);

    // grab the absolute max and mins in each axis
    for(auto point : points){
        if(point.x < MinMaxesMid.at(0)) MinMaxesMid.at(0) = point.x;
        if(point.x > MinMaxesMid.at(1)) MinMaxesMid.at(1) = point.x;
        if(point.y < MinMaxesMid.at(2)) MinMaxesMid.at(2) = point.y;
        if(point.y > MinMaxesMid.at(3)) MinMaxesMid.at(3) = point.y;
        if(point.z < MinMaxesMid.at(4)) MinMaxesMid.at(4) = point.z;
        if(point.z > MinMaxesMid.at(5)) MinMaxesMid.at(5) = point.z;
    }
    
    // calculate the midpoint (x, y, z)
    MinMaxesMid.push_back((MinMaxesMid.at(0) + MinMaxesMid.at(1)) / 2);
    MinMaxesMid.push_back((MinMaxesMid.at(2) + MinMaxesMid.at(3)) / 2);
    MinMaxesMid.push_back((MinMaxesMid.at(4) + MinMaxesMid.at(5)) / 2);
    
    // translate the points
    for(auto & point : points){
        point.x -= MinMaxesMid.at(6);
        point.y -= MinMaxesMid.at(7);
        point.z -= MinMaxesMid.at(8);
    }
    
    // translate the min and max records for use later, and absolute valueing them
    for(int i = 0; i < 3; i++){
        // all of the minimums
        MinMaxesMid[2 * i] -= MinMaxesMid.at(i + 6);
        // all of the maximums
        MinMaxesMid[2 * i + 1] -= MinMaxesMid.at(i + 6);
        MinMaxesMid[2 * i] *= -1;
        //std::cerr << MinMaxesMid[2*i]  << ", " << MinMaxesMid[2*i+1]<< std::endl;
    }
    
    /* *
    std::cerr << std::endl;
    for(auto i : MinMaxesMid){
        std::cerr <<  i << std::endl;
    }
    std::cerr << std::endl << *std::max(MinMaxesMid.begin(), MinMaxesMid.end() - 4) << std::endl;
    /* */
    
    // calculate the scalar per the max point
    GLfloat scalar = SCREEN_SCALE / *std::max(MinMaxesMid.begin(), MinMaxesMid.end() - 4);
    
    // multiply all of the vectors by the calculated scalar
    for(auto & point : points){
        point *= scalar;
    }
    /*
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
    */
    
}
