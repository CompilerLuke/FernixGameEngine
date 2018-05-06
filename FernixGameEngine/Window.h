#pragma once

struct GLFWwindow;
class Input;

class Window
{
public:
private:
	const char* title;
	bool vSync;
	bool fullscreen;
	
	GLFWwindow* window;
	Input* input;
public:
	int SCR_WIDTH, SCR_HEIGHT;

	bool getKeyDown(char key);
	bool getKeyDown(int key);

	void captureMouse(bool);
	void gameLoop(void (&func)());

	Window(const char* title, Input* input, int width, int height, bool vsync = true, bool fullscreen = false);
	void Init();
	~Window();
};

