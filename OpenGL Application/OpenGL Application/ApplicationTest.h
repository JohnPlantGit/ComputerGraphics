#pragma once
#include "Application.h"
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>
#include "FlyCam.h"
class ApplicationTest : public Application
{
public:
	ApplicationTest();
	~ApplicationTest();

	virtual bool Startup();
	virtual bool Update();
	virtual void Draw();
	virtual void Shutdown();

private:
	float m_cameraRotation;
	glm::vec3 m_positions[2];
	glm::quat m_rotations[2];

	FlyCam* m_camera;
};

