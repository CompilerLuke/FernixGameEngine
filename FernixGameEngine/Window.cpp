#include "Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <iostream>

#include "Input.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
	Input* input = (Input*)glfwGetWindowUserPointer(window);
	input->CursorPosCallback(xpos, ypos);
}

void scrollCallback(GLFWwindow* window, double xpos, double ypos) {
	Input* input = (Input*)glfwGetWindowUserPointer(window);
	input->ScrollCallback(xpos, ypos);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Input* input = (Input*)glfwGetWindowUserPointer(window);
	input->KeyCallback(key, action);
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

	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetScrollCallback(window, scrollCallback);


	if (!vSync) {
		glfwSwapInterval(0);
	}
	else {
		glfwSwapInterval(1);
	}	
	
	std::cout << "Initialized window" << std::endl;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

	ImGui_ImplGlfw_InitForOpenGL(window, true);

	ImGui_ImplOpenGL3_Init();

	ImGui_ImplGlfw_SetKeyCallback(window, &keyCallback);

	// Setup style
	ImGui::StyleColorsDark();
	
	
	std::cout << "initialized imgui" << std::endl;

}

bool Window::getKeyDown(int key) {
	return glfwGetKey(this->window, key) == GLFW_PRESS;
}

void Window::setMousePos(double xpos, double ypos) {
	glfwSetCursorPos(this->window, xpos, ypos);
}

bool Window::getMouseButtonDown(unsigned int key) {
	return glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
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
	// Start the ImGui frame


	while (!glfwWindowShouldClose(window))
	{
		// render
		// ------
		glfwPollEvents();	
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		func();	
		input->Clear();
		
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------

		glfwSwapBuffers(window);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
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
