#pragma once

#include <unordered_map>

struct GLFWwindow;
class Input;

class Window
{
public:
	const char* title;
	bool vSync;
	bool fullscreen;
	
	GLFWwindow* window;
	Input* input;

	int SCR_WIDTH, SCR_HEIGHT;

	bool getKeyDown(int key);
	bool getMouseButtonDown(unsigned int key);
	void setMousePos(double x, double y);

	void captureMouse(bool);
	void gameLoop(void (&func)());

	Window(const char* title, Input* input, int width, int height, bool vsync = true, bool fullscreen = false);
	Window() {};
	
	void Init();
	~Window();
};

class StructType;
extern StructType* WindowType;
