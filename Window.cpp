#include "Window.h"

const char* window_title = "GLFW Starter Project";
Cube * cube;
Water * water;
GLint shaderProgram;
GLuint waterShaderProgram;

BezierPatches * bezierPatches;
GLint bpShaderProgram;


// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "../shader.vert"
#define FRAGMENT_SHADER_PATH "../shader.frag"
#define BP_VERTEX_SHADER_PATH "../bpShader.vert"
#define BP_FRAGMENT_SHADER_PATH "../bpShader.frag"

#define WATER_VERTEX_SHADER_PATH "../watershader.vert"
#define WATER_FRAGMENT_SHADER_PATH "../watershader.frag"

// Default camera parameters
glm::vec3 Window::cam_pos = glm::vec3(0.0f, 20.0f, 0.0f);
glm::vec3 Window::cam_look_at = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3  Window::cam_up = glm::vec3(1.0f, 0.0f, 0.0f);

int Window::width;
int Window::height;

glm::mat4 Window::P;
glm::mat4 Window::V;
double prexpos, preypos, prerightx, prerighty;


void Window::initialize_objects()
{

	cube = new Cube();

    water = new Water();
	//Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
    waterShaderProgram = LoadShaders(WATER_VERTEX_SHADER_PATH, WATER_FRAGMENT_SHADER_PATH);

	bezierPatches = new BezierPatches();

	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	bpShaderProgram = LoadShaders(BP_VERTEX_SHADER_PATH, BP_FRAGMENT_SHADER_PATH);

}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete(cube);
    delete(water);
	delete(bezierPatches);
	glDeleteProgram(shaderProgram);
    glDeleteProgram(waterShaderProgram);
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
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use the shader of programID
    glUseProgram(shaderProgram);

    //Render the cube
    cube->draw(shaderProgram);
    
    //glUseProgram(waterShaderProgram);
    //water->draw(waterShaderProgram);

	// Render the cube
	//cube->draw(shaderProgram);
	
	//Render the bezierPatches
	glUseProgram(bpShaderProgram);
	bezierPatches->draw(bpShaderProgram);


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
			Window::cam_pos = glm::rotate(glm::mat4(1.0f), deg, res)*glm::vec4(Window::cam_pos, 1.0);
			Window::cam_up = glm::rotate(glm::mat4(1.0f), deg, res)*glm::vec4(Window::cam_up, 1.0);
			V = glm::lookAt(Window::cam_pos, Window::cam_look_at, Window::cam_up);
		}
		prexpos = xpos;
		preypos = ypos;
	}
	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
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
}

glm::vec3 Window::trackmap(double x, double y)
{
	glm::vec3 result((2.0f * x - width) / width, (height - 2.0f*y) / height, 0.0f);
	float length = glm::length(result);
	if (length > 1.0f)
	{
		length = 1.0;
	}
	result[2] = sqrt(1.001 - length*length);
	return glm::normalize(result);
}

void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		unsigned char pix[4];
		glfwGetCursorPos(window, &prexpos, &preypos);
		//glReadPixels(prexpos, Window::height - preypos, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &pix);
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		glfwGetCursorPos(window, &prerightx, &prerighty);
	}
}

/*    --------------------  water  render ---------------------- */
 
