#include "Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <iostream>

#include "Input.h"

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
	Input* input = (Input*)glfwGetWindowUserPointer(window);
	input->CursorPosCallback(xpos, ypos);
}

void scrollCallback(GLFWwindow* window, double xpos, double ypos) {
	Input* input = (Input*)glfwGetWindowUserPointer(window);
	input->ScrollCallback(xpos, ypos);
}

Window::Window(const char* title, Input* input, int width, int height, bool vsync, bool fullscreen) 
: title(title), vSync(vsync), SCR_WIDTH(width), SCR_HEIGHT(height), input(input), fullscreen(fullscreen) {
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void Window::Init() {
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------

	if (fullscreen) {
		window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, title, glfwGetPrimaryMonitor(), NULL);
	}
	else {
		window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, title, NULL, NULL);
	}

	if (window == NULL)
	{
		glfwTerminate();
		throw std::runtime_error("Failed to create GLFW window");
	}
	glfwMakeContextCurrent(window);
	
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		throw std::runtime_error("Failed to initialize GLAD");
	}	

	glfwSetWindowUserPointer(window, input);
	
	this->input->Init(this);
	
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetCursorPosCallback(this->window, cursorPosCallback);
	glfwSetScrollCallback(window, scrollCallback);

	std::cout << "Initialized window" << std::endl;


}

bool Window::getKeyDown(int key) {
	return glfwGetKey(this->window, key) == GLFW_PRESS;
}

bool Window::getKeyDown(char key) {
	return glfwGetKey(this->window, key) == GLFW_PRESS;
}

void Window::captureMouse(bool capture) {
	if (capture) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void Window::gameLoop(void(&func)()) {
	while (!glfwWindowShouldClose(window))
	{
		// render
		// ------
		glfwPollEvents();

		func();

		// apply camera

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
	}

	glfwTerminate();
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}




Window::~Window()
{
}
