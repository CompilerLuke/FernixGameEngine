#pragma once

#include <glm/glm.hpp>
#include "render.h"
#include <unordered_map>

class Window;

class Input 
{
public:
	glm::vec2 mouse_position = glm::vec2(0.0f, 0.0f);
	glm::vec2 mouse_offset = glm::vec2(0.0f, 0.0f);
	float scroll_offset = 0;

	bool capturingMouse = false;

	std::unordered_map<int, int> last_frame_keys;
	std::unordered_map<int, int> keys;

	bool firstMouse = true;

	Window* window;

	void captureMouse(bool);
	bool rightClicking();
	bool leftClicking();
	bool keyDown(int key);
	bool keyPressed(int key);
	void setMousePos(double xpos, double ypos);
	
	glm::vec2 toScreenPos(double xpos, double ypos);

	Input();
	void ScrollCallback(double xpos, double ypos);
	void CursorPosCallback(double xpos, double ypos);
	void KeyCallback(int key, int action);
	void Init(Window* window);
	void Clear();

	~Input();
};
