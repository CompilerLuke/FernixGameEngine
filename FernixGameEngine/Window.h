#pragma once

class GLFWwindow;
class Input;

class Window
{
public:
private:
	const char* title;
	bool vSync;
	bool fullscreen;
	void Init();
	
	GLFWwindow* window;
	Input* input;
public:
	int SCR_WIDTH, SCR_HEIGHT;

	bool getKeyDown(char key);
	bool getKeyDown(int key);

	void captureMouse(bool);
	void gameLoop(void (&func)());

	Window(const char* title, Input* input, int width, int height, bool vsync = true, bool fullscreen = true);
	void Init();
	~Window();
};

