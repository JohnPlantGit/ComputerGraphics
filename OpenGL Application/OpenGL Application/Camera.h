#pragma once
#include "glm/ext.hpp"
class Camera
{
public:
	Camera();
	~Camera();

	virtual void Update() = 0;
	void SetPerspective(float fieldOfView, float aspectRatio, float near, float far);
	void SetLookAt(glm::vec3 from, glm::vec3 to, glm::vec3 up);
	void SetPosition(glm::vec3 position);
	glm::vec3 GetPosition();
	glm::mat4 GetWorldTransform() { return m_worldTransform; }
	glm::mat4 GetViewTransform() { return m_viewTransform; }
	glm::mat4 GetProjectionTransform() { return m_projectionTransform; }
	glm::mat4 GetProjectionView() { return m_projectionViewTransform; }
protected:
	void UpdateTransform();

	glm::mat4 m_worldTransform;
	glm::mat4 m_viewTransform;
	glm::mat4 m_projectionTransform;
	glm::mat4 m_projectionViewTransform;
};

