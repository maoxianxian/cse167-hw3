#define _USE_MATH_DEFINES
#include "window.h"
#include <math.h>
#include "Geometry.h"
#include "Transform.h"
const char* window_title = "GLFW Starter Project";
GLint shaderProgram;
GLint shader2;
Cube *cbe;
// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "../shader.vert"
#define FRAGMENT_SHADER_PATH "../shader.frag"
#define SHADER2_PATH "../shader2.vert"
#define FRAG2_PATH "../shader2.frag"
// Default camera parameters
glm::vec3 cam_pos(0.0f, 0.0f, 20.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

int Window::width;
int Window::height;
double prexpos, preypos, prerightx, prerighty;
int i = 0;
glm::mat4 Window::P;
glm::mat4 Window::V;
Geometry* antenna;
Geometry* body;
Geometry* eyeball;
Geometry* head;
Geometry* limb;
Transform * root;
std::vector<Transform*> robots;
Transform * robotToroot;
void Window::initialize_objects()
{
	
	loadTexture();
	//  the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	shader2 = LoadShaders(SHADER2_PATH, FRAG2_PATH);
	antenna = new Geometry("C:\\Users\\c7ye\\Desktop\\CSE167StarterCode2-master\\antenna.obj");
	body = new Geometry("C:\\Users\\c7ye\\Desktop\\CSE167StarterCode2-master\\body.obj");
	eyeball = new Geometry("C:\\Users\\c7ye\\Desktop\\CSE167StarterCode2-master\\eyeball.obj");
	head = new Geometry("C:\\Users\\c7ye\\Desktop\\CSE167StarterCode2-master\\head.obj");
	limb = new Geometry("C:\\Users\\c7ye\\Desktop\\CSE167StarterCode2-master\\limb.obj");
	robotToroot = new Transform(glm::mat4(1.0f));
	Transform *antennaleftTorobot = new Transform(glm::mat4(1.0f)*glm::scale(glm::mat4(1.0f),glm::vec3(0.3,0.3,0.1)));
	antennaleftTorobot->rotate( glm::vec3(0,0,1),(float)M_PI*3.0f / 2 );
	antennaleftTorobot->addChild(antenna);
	antennaleftTorobot->translate(2, -1, 0);
	robotToroot->addChild(antennaleftTorobot);

	Transform *antennarightTorobot = new Transform(glm::mat4(1.0f)*glm::scale(glm::mat4(1.0f), glm::vec3(0.3,0.3,0.1)));
	antennarightTorobot->rotate(glm::vec3(0, 0, 1), (float)M_PI*3.0f / 2);
	antennarightTorobot->addChild(antenna);
	antennarightTorobot->translate(4,-1,0);
	robotToroot->addChild(antennarightTorobot);

	Transform *headTorobot = new Transform(glm::mat4(1.0f));
	headTorobot->scale(0.1, 0.1, 0.1);
	headTorobot->rotate(glm::vec3(1, 0, 0), 3.0f*M_PI/2 );
	headTorobot->translate(3.0f, -10.0f, 0);
	headTorobot->addChild(head);
	robotToroot->addChild(headTorobot);

	Transform* bodyTorobot = new Transform(glm::mat4(1.0f));
	bodyTorobot->scale(0.1, 0.1, 0.1);
	bodyTorobot->rotate(glm::vec3(1, 0, 0), M_PI / 2);
	bodyTorobot->translate(3, 1.3, 0);
	bodyTorobot->addChild(body);
	robotToroot->addChild(bodyTorobot);

	Transform* eyeballTorobot = new Transform(glm::mat4(1.0f));
	eyeballTorobot->scale(0.1, 0.1, 0.1);
	eyeballTorobot->translate(3, 0, 2);
	eyeballTorobot->addChild(eyeball);
	robotToroot->addChild(eyeballTorobot);

	Transform* leftarmTorobot = new Transform(glm::mat4(1.0f));
	leftarmTorobot->scale(0.1, 0.09, 0.1);
	leftarmTorobot->translate(3.5, -3, -4);
	leftarmTorobot->addChild(limb);
	robotToroot->addChild(leftarmTorobot);

	Transform* rightarmTorobot = new Transform(glm::mat4(1.0f));
	rightarmTorobot->scale(0.1, 0.09, 0.1);
	rightarmTorobot->rotate(glm::vec3(1, 0, 0), M_PI / 2);
	rightarmTorobot->translate(8, 0.5, 0.5);
	rightarmTorobot->addChild(limb);
	robotToroot->addChild(rightarmTorobot);

	Transform* leftlegTorobot = new Transform(glm::mat4(1.0f));
	leftlegTorobot->scale(0.1, 0.07, 0.1);
	leftlegTorobot->rotate(glm::vec3(1, 0, 0), M_PI / 2);
	leftlegTorobot->translate(4.5, -3, 0.5);
	leftlegTorobot->addChild(limb);
	robotToroot->addChild(leftlegTorobot);

	Transform* rightlegTorobot = new Transform(glm::mat4(1.0f));
	rightlegTorobot->scale(0.1, 0.1, 0.07);
	rightlegTorobot->translate(6.5, -6.2, -2.5);
	rightlegTorobot->addChild(limb);
	robotToroot->addChild(rightlegTorobot);

	root = new Transform(glm::mat4(1.0f));
	root->addChild(robotToroot);
	cbe= new Cube();
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	glDeleteProgram(shaderProgram);
	delete(antenna);
	delete(body);
	delete(eyeball);
	delete(head);
	delete(limb);
	delete root;
}

unsigned char* Window::loadPPM(const char* filename, int& width, int& height)
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
GLuint Window::loadTexture()
{
	std::vector<std::string> faces
	{
		"../right.ppm",
			"../left.ppm",
			"../top.ppm",
			"../base.ppm",
			"../back.ppm",
			"../front.ppm"
	};
	GLuint texture;     // storage for one texture
	int twidth, theight;   // texture width/height [pixels]
	unsigned char* tdata;  // texture pixel data
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	for (int i = 0; i < 6; i++)
	{
		tdata = loadPPM(faces[i].c_str(), twidth, theight); //std::cout << tdata << std::endl;
		if (tdata == NULL) return 0;
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata);
	}
	return texture;
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	glfwGetFramebufferSize(window, &width, &height); // In case your Mac has a retina display
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback()
{
	// Call the update function the cube
}
void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		glfwGetCursorPos(window, &prexpos, &preypos);
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		glfwGetCursorPos(window, &prerightx, &prerighty);
	}
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use the shader of programID
	glUseProgram(shaderProgram);
	cbe->draw(shaderProgram);
	glUseProgram(shader2);
	root->draw(shader2,glm::mat4(1.0f));
	// Render the cube	
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		if (xpos != prexpos || ypos != preypos)
		{
			glm::vec3 prevec = trackmap(prexpos, preypos);
			glm::vec3 curvec = trackmap(xpos, ypos);
			glm::vec3 res = glm::cross(prevec, curvec);
			float cos = glm::dot(prevec, curvec) / (glm::length(prevec)*glm::length(curvec));
			if (cos > 1)
			{
				cos = 1;
			}
			float deg = acos(cos);
			cam_pos = glm::rotate(glm::mat4(1.0f), deg, res)*glm::vec4(cam_pos,1.0);
			V = glm::lookAt(cam_pos, cam_look_at, cam_up);

		}
		prexpos = xpos;
		preypos = ypos;
	}
	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}
glm::vec3 Window::trackmap(double x, double y)
{
	glm::vec3 result( (2.0f * x - width) / width, (height- 2.0f*y)/height, 0.0f);
	float length = glm::length(result);
	if (length > 1.0f)
	{
		length = 1.0;
	}
	result[2] = sqrt(1.001 - length*length);
	return glm::normalize(result);
}
void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
	}
	if (action == GLFW_REPEAT)
	{
		
	}
}
