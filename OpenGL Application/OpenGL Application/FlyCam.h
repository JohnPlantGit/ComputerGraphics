#pragma once
#include "Camera.h"

class FlyCam : public Camera
{
public:
	FlyCam();
	~FlyCam();

	// Allows the camera to move
	virtual void Update();

private:
	float m_speed;
	glm::vec3 m_up;
	float m_pitch = 0;
	float m_yaw = 0;
};

