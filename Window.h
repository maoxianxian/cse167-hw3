#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <iostream>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include "Cube.h"
#include "shader.h"
#include "Water.h"
#include "BezierPatches.h"


class Window
{
public:
	static int width;
	static int height;
	static glm::mat4 P; // P for projection
	static glm::mat4 V; // V for view
	static void initialize_objects();
	static void clean_up();
	static GLFWwindow* create_window(int width, int height);
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback();
	static void display_callback(GLFWwindow*);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static glm::vec3 trackmap(double x, double y);
	static void mouse_callback(GLFWwindow* window, int button, int action, int mods);
	static glm::vec3 cam_pos;		// e  | Position of camera
	static glm::vec3 cam_look_at;	// d  | This is where the camera looks at
	static glm::vec3 cam_up;
};

#endif
