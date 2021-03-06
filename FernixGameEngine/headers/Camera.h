#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include "Entity.h"

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
static const float YAW = -90.0f;
static const float PITCH = 0.0f;
static const float SPEED = 4.0f;
static const float SENSITIVITY = 0.01f;
static const float ZOOM = 45.0f;

class Camera : public Entity
{
public:
	// Camera Attributes
	glm::vec3 Position; //remove attribute
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// Euler Angles
	float Yaw;
	float Pitch;
	float OffsetOfYaw = 0;
	float OffsetOfPitch = 0;

	// Camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	bool firstRightClick = true;

	Camera(float SCR_WIDTH=2000, float SCR_HEIGHT=1800, glm::vec3 position = glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	Camera(float SCR_WIDTH, float SCR_HEIGHT, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetViewMatrix();

	void Update() override;
	void Init() override;
	
private:
	glm::mat4 projection;

	void updateCameraVectors();
};

struct StructType;
extern StructType* CameraType;

#endif
