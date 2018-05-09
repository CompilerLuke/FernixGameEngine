#include "Camera.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include "Input.h"
#include <iostream>

extern Input input;

Camera::Camera(float SCR_WIDTH, float SCR_HEIGHT, glm::vec3 position, glm::vec3 up, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
	projection = glm::perspective(glm::radians(Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	updateCameraVectors();
	this->transform.position = position;
}

Camera::Camera(float SCR_WIDTH, float SCR_HEIGHT, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	Position = glm::vec3(posX, posY, posZ);
	WorldUp = glm::vec3(upX, upY, upZ);
	Yaw = yaw;
	Pitch = pitch;
	projection = glm::perspective(glm::radians(Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	this->transform.position = Position;
	updateCameraVectors();
}

void Camera::Update() {
	float deltaTime = ctx->deltaTime;
	float velocity = MovementSpeed * deltaTime;
	if (input.keyDown(GLFW_KEY_W)) {
		this->transform.position += Front * velocity;
	}
	if (input.keyDown(GLFW_KEY_S))
		this->transform.position -= Front * velocity;
	if (input.keyDown(GLFW_KEY_A))
		this->transform.position  -= Right * velocity;
	if (input.keyDown(GLFW_KEY_D))
		this->transform.position += Right * velocity;

	float xoffset = input.mouse_offset.x;
	float yoffset = input.mouse_offset.y;

	Yaw = xoffset * MouseSensitivity;
	Pitch = yoffset * MouseSensitivity;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped

	if (Pitch > 89.0f)
		Pitch = 89.0f;
	if (Pitch < -89.0f)
		Pitch = -89.0f;

	this->transform.rotation = glm::angleAxis(glm::radians(90.f), glm::vec3(Pitch / 90.0f, Yaw / 90.0f, 0.f));
	
	// Update Front, Right and Up Vectors using the updated Euler angles
	updateCameraVectors();
}

/*
// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(float yoffset)
{
	if (Zoom >= 1.0f && Zoom <= 45.0f)
		Zoom -= yoffset;
	if (Zoom <= 1.0f)
		Zoom = 1.0f;
	if (Zoom >= 45.0f)
		Zoom = 45.0f;
}
*/

void Camera::updateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = glm::normalize(glm::cross(Right, Front));
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(this->transform.position, this->transform.position + Front, Up);
}

glm::mat4 Camera::GetProjectionMatrix() {
	return projection;
}