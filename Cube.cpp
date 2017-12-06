#include "Cube.h"
#include "Window.h"
#include <stdio.h>
#include <stdlib.h>
Cube::Cube()
{
	FILE * file;
	char aByte;
	FILE *fp;
	//char hHeightField[256][256];
	
	/*fp = fopen("C:\\Users\\c7ye\\Desktop\\CSE167StarterCode2-master\\heightField.raw.txt", "rb");
	int hWidth = 256;
	int hHeight = 256;
	std::cout << hHeightField << std::endl;

	fread(hHeightField, 1, hWidth*hHeight , fp);
	fclose(fp);
	
	for (int i = 0; i < hWidth; i++) {

		for (int j = 0; j < hHeight;j++) {
			vertices.push_back(glm::vec3((float)i, (hHeightField[i][j] / 128.0f - 1)*height, -(float)j));
			if (i != 0 && j != 0)
			{
				//glm::vec3 temp = glm::vec3((i-1)*256+j-1,i*256+j-1,(i-1)*256);
				//glm::vec3 temp2 = glm::vec3((i - 1) * 256, i * 256 + j - 1, i * 256 + j);
				indices.push_back((i - 1) * size + j - 1);
				indices.push_back(i * size + j - 1);
				indices.push_back((i - 1) * size + j);
				indices.push_back((i - 1) * size + j);
				indices.push_back(i * size + j - 1);
				indices.push_back(i * size + j);

			}
		}
	}*/
	/*file = fopen("C:\\Users\\c7ye\\Desktop\\CSE167StarterCode2-master\\123.ppm", "rb");
	for (int i = 0; i<size; i++) {
		for (int j = 0; j <size; j++) {
			for (int c = 0; c < 3; c++) {
				aByte = fgetc(file);
				image[i][j][c] = aByte+128;
				//std::cout << (int)aByte ;
			}
			//std::cout << std::endl;
		}
	}*/
	int a = size;

	unsigned char* image;
	std::string st("../123.ppm");
	image = loadPPM(st.c_str(), a, a);
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			vertices.push_back(glm::vec3((float)(i - size / 2) / size*height, (image[(i*size+j)*3] / 128.0f - 1)*tall , (float)(j - size / 2) / size*height));
			//std::cout << (float)(i - size / 2) / size*height << std::endl;
			//vertices.push_back(glm::vec3(((float)i-size/2)/size*height,(image[i][j][0]/128.0f-1)*height / size*height,((float)j-size/2) / size*height));
			//vertices.push_back(glm::vec3((float)i / size*width - width / 2, (image[i][j][0] / 128.0f*height) / 128 * height, (float)j / size*height - height / 2));

			//std::cout << (image[i][j][0]/128.0f-1)*height << std::endl;
			if (i != 0 && j != 0)
			{
				//glm::vec3 temp = glm::vec3((i-1)*256+j-1,i*256+j-1,(i-1)*256);
				//glm::vec3 temp2 = glm::vec3((i - 1) * 256, i * 256 + j - 1, i * 256 + j);
				indices.push_back((i - 1) * size + j-1 );
				indices.push_back(i * size + j - 1);
				indices.push_back((i - 1) * size+j);
				indices.push_back((i - 1) * size+j);
				indices.push_back(i * size+j-1);
				indices.push_back(i * size+j);
				uv.push_back(glm::vec2((float)i/size,(float)j/size));
			}
		}
	}
	std::string str("../grassy.ppm");
	unsigned char* tdata = loadPPM(str.c_str(), a, a);
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, tdata);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	
	
	std::cout << vertices.size() << std::endl;
	std::cout << indices.size() << std::endl;
	toWorld = glm::mat4(1.0f);
	// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &TXO);

	// Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
	// Consider the VAO as a container for all your buffers.
	glBindVertexArray(VAO);

	// Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
	// you want to draw, such as vertices, normals, colors, etc.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
	// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*vertices.size(), &vertices[0], GL_STATIC_DRAW);
	// Enable the usage of layout location 0 (check the vertex shader to see what this is)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.
	glBindBuffer(GL_ARRAY_BUFFER, TXO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*uv.size(), &uv[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2* sizeof(GLfloat), (GLvoid*)0);
	// We've sent the vertex data over to OpenGL, but there's still something missing.
	// In what order should it draw those vertices? That's why we'll need a GL_ELEMENT_ARRAY_BUFFER for this.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind the VAO now so we don't accidentally tamper with it.
	// NOTE: You must NEVER unbind the element array buffer associated with a VAO!
	glBindVertexArray(0);
}
unsigned char* Cube::loadPPM(const char* filename, int& width, int& height)
{
	const int BUFSIZE = 128;
	FILE* fp;
	unsigned int read;
	unsigned char* rawData;
	char buf[3][BUFSIZE];
	char* retval_fgets;
	size_t retval_sscanf;
	if ((fp = fopen(filename, "rb")) == NULL)
	{
		std::cerr << "error reading ppm file, could not locate " << filename << std::endl;
		width = 0;
		height = 0;
		return NULL;
	}
	// Read magic number:
	retval_fgets = fgets(buf[0], BUFSIZE, fp);
	// Read width and height:
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');
	retval_sscanf = sscanf(buf[0], "%s %s", buf[1], buf[2]);
	width = atoi(buf[1]);
	height = atoi(buf[2]);

	// Read maxval:F
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');
	// Read image data:
	rawData = new unsigned char[width * height * 3];
	read = fread(rawData, width * height * 3, 1, fp);
	fclose(fp);
	if (read != 1)
	{
		std::cerr << "error parsing ppm file, incomplete data" << std::endl;
		delete[] rawData;
		width = 0;
		height = 0;
		return NULL;
	}
	return rawData;
}
Cube::~Cube()
{
	// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a 
	// large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Cube::draw(GLuint shaderProgram)
{ 
	// Calculate the combination of the model and view (camera inverse) matrices
	glm::mat4 modelview = Window::V * toWorld;
	// We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
	// Consequently, we need to forward the projection, view, and model matrices to the shader programs
	// Get the location of the uniform variables "projection" and "modelview"
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");
	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
	// Now draw the cube. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);
	// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_LINES, 0, 4096);
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
}

void Cube::update()
{
	spin(1.0f);
}

void Cube::spin(float deg)
{
	// If you haven't figured it out from the last project, this is how you fix spin's behavior
	toWorld = toWorld * glm::rotate(glm::mat4(1.0f), 1.0f / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
}

