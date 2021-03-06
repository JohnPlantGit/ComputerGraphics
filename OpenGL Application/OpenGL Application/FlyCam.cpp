#include "FlyCam.h"
#include "GLFW/glfw3.h"
#include "glm/gtx/quaternion.hpp"

FlyCam::FlyCam()
{
	m_up = glm::vec3(0, 1, 0);
	SetPerspective(45, 16 / 9.f, 0.1, 1000.f);
}

FlyCam::~FlyCam()
{
}

// Moves the flycam according to the key input
void FlyCam::Update()
{
	glm::vec3 position = GetPosition();
	glm::vec3 movement(0);

	float speed = 0;
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_LEFT_SHIFT))
	{
		speed = 1;
	}
	else
	{
		speed = 0.1f;
	}
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_W))
	{
		movement.z = -speed;
	}
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_S))
	{
		movement.z = speed;
	}
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_A))
	{
		movement.x = -speed;
	}
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_D))
	{
		movement.x = speed;
	}
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_LEFT_CONTROL))
	{
		movement.y = -speed;
	}
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_SPACE))
	{
		movement.y = speed;
	}


	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_UP))
	{
		m_pitch += 0.01;
	}
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_DOWN))
	{
		m_pitch -= 0.01;
	}
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_LEFT))
	{
		m_yaw += 0.01;
	}
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_RIGHT))
	{
		m_yaw -= 0.01;
	}

	glm::quat rotation(glm::vec3(m_pitch, m_yaw, 0));

	position += rotation * movement;
	
	glm::mat4 positionMatrix(1);
	positionMatrix[3] = glm::vec4(position.x, position.y, position.z, 1);

	m_worldTransform = positionMatrix * glm::toMat4(rotation);

	m_viewTransform = glm::inverse(m_worldTransform);

	UpdateTransform();
}
