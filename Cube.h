#ifndef _CUBE_H_
#define _CUBE_H_

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
const int size = 256;
const int width = 16;
const int height = 16;
const int tall = 8;
class Cube
{
	//float image[size][size][3];
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uv;
	//std::vector<
public:
	
	
	glm::mat4 toWorld;
	Cube();
	~Cube();
	unsigned char* loadPPM(const char* filename, int& width, int& height);
	void draw(GLuint shaderProgram);
	void update();
	void spin(float);
	// These variables are needed for the shader program
	GLuint VBO, VAO, EBO,TXO;
	GLuint uProjection, uModelview;
};

// Define the coordinates and indices needed to draw the cube. Note that it is not necessary
// to use a 2-dimensional array, since the layout in memory is the same as a 1-dimensional array.
// This just looks nicer since it's easy to tell what coordinates/indices belong where.
/*const GLfloat vertices[8][3] = {
	// "Front" vertices
	{-8.0, -0.5,  8.0}, {8.0, -0.5,  8.0}, {8.0,  0.5,  8.0}, {-8.0,  0.5,  8.0},
	// "Back" vertices
	{-8.0, -0.5, -8.0}, {8.0, -0.5, -8.0}, {8.0,  0.5, -8.0}, {-8.0,  0.5, -8.0}
};*/

// Note that GL_QUADS is deprecated in modern OpenGL (and removed from OSX systems).
// This is why we need to draw each face as 2 triangles instead of 1 quadrilateral
/*const GLuint indices[6][6] = {
	// Front face
	{0, 1, 2, 2, 3, 0},
	// Top face
	{1, 5, 6, 6, 2, 1},
	// Back face
	{7, 6, 5, 5, 4, 7},
	// Bottom face
	{4, 0, 3, 3, 7, 4},
	// Left face
	{4, 5, 1, 1, 0, 4},
	// Right face
	{3, 2, 6, 6, 7, 3}
};*/

#endif

