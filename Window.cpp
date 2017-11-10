#define _USE_MATH_DEFINES
#include "window.h"
#include <math.h>
#include "Geometry.h"
#include "Transform.h"
#include "robot.h"
#include <vector>
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
glm::mat4 Window::P;
glm::mat4 Window::V;
Geometry* antenna;
Geometry* body;
Geometry* eyeball;
Geometry* head;
Geometry* limb;
Transform * root;
Transform* temp;
Transform *antennaleftTorobot;
Transform *antennarightTorobot;
Transform *headTorobot;
Transform* bodyTorobot;
Transform* eyeballTorobot;
Transform* leftarmTorobot;
Transform* rightarmTorobot;
Transform* leftlegTorobot;
Transform* rightlegTorobot;
int index = 1;
bool building = true;
std::vector<robot*> rob;
std::vector<Transform*> children;
robot* robo;
double lastTime;
int nbFrames = 0;
void Window::initialize_objects()
{
	lastTime= glfwGetTime();
	loadTexture();
	//  the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	shader2 = LoadShaders(SHADER2_PATH, FRAG2_PATH);
	antenna = new Geometry("C:\\Users\\c7ye\\Desktop\\CSE167StarterCode2-master\\antenna.obj");
	body = new Geometry("C:\\Users\\c7ye\\Desktop\\CSE167StarterCode2-master\\body.obj");
	eyeball = new Geometry("C:\\Users\\c7ye\\Desktop\\CSE167StarterCode2-master\\eyeball.obj");
	head = new Geometry("C:\\Users\\c7ye\\Desktop\\CSE167StarterCode2-master\\head.obj");
	limb = new Geometry("C:\\Users\\c7ye\\Desktop\\CSE167StarterCode2-master\\limb.obj");


	
	antennaleftTorobot = new Transform(glm::mat4(1.0f));
	antennaleftTorobot->addChild(antenna);
	children.push_back(antennaleftTorobot);

	antennarightTorobot = new Transform(glm::mat4(1.0f));
	antennarightTorobot->addChild(antenna);
	children.push_back(antennarightTorobot);

	headTorobot = new Transform(glm::mat4(1.0f));
	headTorobot->addChild(head);
	headTorobot->translate(0, 0, -50);
	children.push_back(headTorobot);

	bodyTorobot = new Transform(glm::mat4(1.0f));
	bodyTorobot->translate(0, 0, -100);
	bodyTorobot->addChild(body);
	children.push_back(bodyTorobot);


	eyeballTorobot = new Transform(glm::mat4(1.0f));
	eyeballTorobot->addChild(eyeball);
	children.push_back(eyeballTorobot);

	leftarmTorobot = new Transform(glm::mat4(1.0f));
	leftarmTorobot->addChild(limb);
	children.push_back(leftarmTorobot);

	rightarmTorobot = new Transform(glm::mat4(1.0f));
	rightarmTorobot->addChild(limb);
	children.push_back(rightarmTorobot);

	leftlegTorobot = new Transform(glm::mat4(1.0f));
	leftlegTorobot->addChild(limb);
	children.push_back(leftlegTorobot);

	rightlegTorobot = new Transform(glm::mat4(1.0f));
	rightlegTorobot->addChild(limb);
	children.push_back(rightlegTorobot);
	root = new Transform(glm::mat4(1.0f));
	root->addChild(antennaleftTorobot);
	temp = antennaleftTorobot;/*
	root = new Transform(glm::mat4(1.0f));
	for (int i = 0; i < 50; i++)
	{
		for (int j = 0; j < 50; j++)
		{
			robot* robo = new robot(glm::mat4(1.0f)*glm::translate(glm::mat4(1.0f),glm::vec3(-i*5,-1,-j*5)), antenna, body, eyeball, head, limb);
			rob.push_back(robo);
			root->addChild(robo);
			building = false;
		}
	}*/
	cbe= new Cube();
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	for (int i = 0; i > rob.size(); i++)
	{
		delete rob[i];
	}
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
		P = glm::perspective(45.0f, (float)width / (float)height, 1.0f, 1000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback()
{
	root->update();
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
	
	double currentTime = glfwGetTime();
	nbFrames++;
	if (currentTime - lastTime >= 1.0)
	{
		std::cout << nbFrames / (currentTime - lastTime) << std::endl;
		nbFrames = 0;
		lastTime = currentTime;
	}
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
			if (!building) {
				cam_pos = glm::rotate(glm::mat4(1.0f), deg, res)*glm::vec4(cam_pos, 1.0);
				cam_up = glm::rotate(glm::mat4(1.0f), deg, res)*glm::vec4(cam_up, 1.0);
				V = glm::lookAt(cam_pos, cam_look_at, cam_up);
			}
			else
			{
				temp->rotateafter(res, deg);
			}

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
		else if (key == GLFW_KEY_R)
		{
			if (!mods)
			{
				root->rotate(glm::vec3(0, 1, 0), M_PI / 18);
			}
			else
			{
				root->rotate(glm::vec3(0, 1, 0), -M_PI / 18);
			}
		}
		else if (key == GLFW_KEY_S)
		{
			if (!mods)
			{
				root->scale(0.9,0.9,0.9);
			}
			else
			{
				root->scale(1.1, 1.1, 1.1);
			}
		}
		else if (key == GLFW_KEY_B)
		{
			for (int i = 0; i < rob.size(); i++)
			{
				rob[i]->ball = !rob[i]->ball;
			}
		}
		else if (key == GLFW_KEY_W)
		{
			if (building)
			{
				if (mods)
				{
					temp->translate(0, 0, -1);
				}
				else
				{
					temp->translate(0, 0, 1);
				}
			}
		}
		else if (key == GLFW_KEY_E)
		{
			if (building)
			{
				if (mods)
				{
					temp->translate(0, 1, 0);
				}
				else
				{
					temp->translate(0, -1, 1);
				}
			}
		}
		else if (key == GLFW_KEY_Q)
		{
			if (building)
			{
				if (mods)
				{
					temp->translate(1, 0, 0);
				}
				else
				{
					temp->translate(-1, 0, 0);
				}
			}
		}
		else if (key == GLFW_KEY_A)
		{
			if (building)
			{
				if (mods)
				{
					temp->scale(1.1,1.1,1.1);
				}
				else
				{
					temp->scale(0.9,0.9,0.9);
				}
			}
		}
		else if (key == GLFW_KEY_Z)
		{
			if (building)
			{
				if (mods)
				{
					temp->scale(1.1, 1, 1);
				}
				else
				{
					temp->scale(0.9, 1, 1);
				}
			}
		}
		else if (key == GLFW_KEY_X)
		{
			if (building)
			{
				if (mods)
				{
					temp->scale(1, 1.1, 1);
				}
				else
				{
					temp->scale(1, 0.9, 1);
				}
			}
		}
		else if (key == GLFW_KEY_C)
		{
			if (building)
			{
				if (mods)
				{
					temp->scale(1, 1, 1.1);
				}
				else
				{
					temp->scale(1, 1, 0.9);
				}
			}
		}
		else if (key == GLFW_KEY_ENTER)
		{
			if (building)
			{
				if (index < 9) {
					temp = children[index];
					index++;
					root->addChild(temp);
				}
				else
				{
					building = false;
					
					for (int i = 0; i < children.size(); i++)
					{
						root->removeChild(children[i]);
					}
					for (int i = 0; i < 10; i++)
					{
						for (int j = 0; j < 10; j++)
						{
							robo = new robot(glm::mat4(1.0f)*glm::translate(glm::mat4(1.0f), glm::vec3(-i * 5, -1, -j * 5)), antenna, body, eyeball, head, limb);
							robo->antennaleftTorobot = antennaleftTorobot;
							robo->antennarightTorobot = antennaleftTorobot;
							robo->headTorobot = headTorobot;
							robo->eyeballTorobot = eyeballTorobot;
							robo->bodyTorobot = bodyTorobot;
							robo->leftarmTorobot = leftarmTorobot;
							robo->leftlegTorobot = leftlegTorobot;
							robo->rightarmTorobot = rightarmTorobot;
							robo->rightlegTorobot = rightlegTorobot;
							robo->children.push_back(antennaleftTorobot);
							robo->children.push_back(antennarightTorobot);
							robo->children.push_back(headTorobot);
							robo->children.push_back(eyeballTorobot);
							robo->children.push_back(bodyTorobot);
							robo->children.push_back(leftarmTorobot);
							robo->children.push_back(rightarmTorobot);
							robo->children.push_back(leftlegTorobot);
							robo->children.push_back(rightlegTorobot);
							rob.push_back(robo);
							root->addChild(robo);
						}
					}
				}
			}
		}
		else if (key == GLFW_KEY_V)
		{
			if (!building)
			{
				if (mods)
				{
					for (int i = 0; i < rob.size(); i++)
					{
						rob[i]->speed *= 1.1;
					}
				}
				else
				{
					for (int i = 0; i < rob.size(); i++)
					{
						rob[i]->speed /= 1.1;
					}
				}
			}
		}
		else if (key == GLFW_KEY_N)
		{
			if (!building)
			{
				if (mods)
				{
					for (int i = 0; i < rob.size(); i++)
					{
						rob[i]->max+=2;
					}
				}
				else
				{
					for (int i = 0; i < rob.size(); i++)
					{
						rob[i]->max-=2;
					}
				}
			}
		}
	}
	if (action == GLFW_REPEAT)
	{
 if (key == GLFW_KEY_W)
	{
		if (building)
		{
			if (mods)
			{
				temp->translate(0, 0, -1);
			}
			else
			{
				temp->translate(0, 0, 1);
			}
		}
	}
	else if (key == GLFW_KEY_E)
	{
		if (building)
		{
			if (mods)
			{
				temp->translate(0, 1, 0);
			}
			else
			{
				temp->translate(0, -1, 1);
			}
		}
	}
	else if (key == GLFW_KEY_Q)
	{
		if (building)
		{
			if (mods)
			{
				temp->translate(1, 0, 0);
			}
			else
			{
				temp->translate(-1, 0, 0);
			}
		}
	}
	else if (key == GLFW_KEY_A)
	{
		if (building)
		{
			if (mods)
			{
				temp->scale(1.1, 1.1, 1.1);
			}
			else
			{
				temp->scale(0.9, 0.9, 0.9);
			}
		}
	}
	else if (key == GLFW_KEY_Z)
	{
		if (building)
		{
			if (mods)
			{
				temp->scale(1.1, 1, 1);
			}
			else
			{
				temp->scale(0.9, 1, 1);
			}
		}
	}
	else if (key == GLFW_KEY_X)
	{
		if (building)
		{
			if (mods)
			{
				temp->scale(1, 1.1, 1);
			}
			else
			{
				temp->scale(1, 0.9, 1);
			}
		}
	}
	else if (key == GLFW_KEY_C)
	{
		if (building)
		{
			if (mods)
			{
				temp->scale(1, 1, 1.1);
			}
			else
			{
				temp->scale(1, 1, 0.9);
			}
		}
	}
	else if (key == GLFW_KEY_V)
	{
		if (!building)
		{
			if (mods)
			{
				for (int i = 0; i < rob.size(); i++)
				{
					rob[i]->speed *= 1.1;
				}
			}
			else
			{
				for (int i = 0; i < rob.size(); i++)
				{
					rob[i]->speed /= 1.1;
				}
			}
		}
	}
	else if (key == GLFW_KEY_N)
	{
		if (!building)
		{
			if (mods)
			{
				for (int i = 0; i < rob.size(); i++)
				{
					rob[i]->max += 2;
				}
			}
			else
			{
				for (int i = 0; i < rob.size(); i++)
				{
					rob[i]->max -= 2;
				}
				std::cout << rob[0]->max << std::endl;;
			}
		}
	}
	}
}
