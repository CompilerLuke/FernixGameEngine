#include "Camera.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <iostream>
#include "Input.h"
#include "Render.h"

#include "metalib/meta.h"

DEFTYPE(Camera, Entity,
	MEMBER(Camera, MovementSpeed, float),
	MEMBER(Camera, MouseSensitivity, float),
	MEMBER(Camera, Zoom, float),
);

extern Input input;

Camera::Camera(float SCR_WIDTH, float SCR_HEIGHT, glm::vec3 position, glm::vec3 up, float yaw, float pitch) : Entity(), Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
	projection = glm::perspective(glm::radians(Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	//updateCameraVectors();
	this->position = position;
}

Camera::Camera(float SCR_WIDTH, float SCR_HEIGHT, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Entity(), Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	Position = glm::vec3(posX, posY, posZ);
	WorldUp = glm::vec3(upX, upY, upZ);
	Yaw = yaw;
	Pitch = pitch;
	projection = glm::perspective(glm::radians(Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	position = Position;
	//updateCameraVectors();
}

void Camera::Init() {
	updateCameraVectors();
}

void Camera::Update() {
	float movementSpeed = MovementSpeed;
	if (input.keyDown(GLFW_KEY_LEFT_SHIFT)) {
		movementSpeed *= 2.0;
	}
	float deltaTime = ctx->deltaTime;
	float velocity = movementSpeed * deltaTime;
	if (input.keyDown(GLFW_KEY_W)) {
		position += Front * velocity;
	}
	if (input.keyDown(GLFW_KEY_S))
		position -= Front * velocity;
	if (input.keyDown(GLFW_KEY_A))
		position  -= Right * velocity;
	if (input.keyDown(GLFW_KEY_D))
		position += Right * velocity;

	if (!RunningGame()) {
		if (input.rightClicking()) {
			if (firstRightClick) {
				OffsetOfYaw = Yaw;
				OffsetOfPitch = Pitch;
				firstRightClick = false;
				input.mouse_offset.x = 0;
				input.mouse_offset.y = 0; 
				input.captureMouse(true);
			}
		}
		else {
			if (!firstRightClick) {				
				input.captureMouse(false);
			}				
			
			firstRightClick = true;
			return;
		}
	}
	else {
		firstRightClick = true;
	}
	
	float xoffset = input.mouse_offset.x;
	float yoffset = input.mouse_offset.y;

	Yaw = (xoffset * MouseSensitivity) + OffsetOfYaw;
	Pitch = (yoffset * MouseSensitivity) + OffsetOfPitch;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped

	if (Pitch > 89.0f) {
		Pitch = 89.0f;
	}
	if (Pitch < -89.0f)
		Pitch = -89.0f;
	if (Yaw > 360.0f) {
		Yaw -= 360.0f;
		input.mouse_offset.x -= 360 * MouseSensitivity;
	}
	if (Yaw < -360.0f) {
		Yaw += 360.0f;
		input.mouse_offset.x += 360 * MouseSensitivity;
	}

	rotation = glm::angleAxis(glm::radians(90.f), glm::vec3(Pitch / 90.0f, Yaw / 90.0f, 0.f));
	
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
	return glm::lookAt(position, position + Front, Up);
}

glm::mat4 Camera::GetProjectionMatrix() {
	return projection;
}