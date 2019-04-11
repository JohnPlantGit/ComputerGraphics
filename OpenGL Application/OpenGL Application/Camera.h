#pragma once
#include "glm/ext.hpp"
class Camera
{
public:
	Camera();
	~Camera();

	virtual void Update() = 0;
	// Update the projection transform
	// Params:
	//		fieldOfView: the new fov
	//		aspectRatio: the aspectRatio of the screen
	//		near: the near clipping plane
	//		far: the far clipping plane
	void SetPerspective(float fieldOfView, float aspectRatio, float near, float far);
	// Update the view transform
	// Params:
	//		from: the camera's position
	//		to: the facing direction
	//		up: the up vector
	void SetLookAt(glm::vec3 from, glm::vec3 to, glm::vec3 up);
	// Set the position of the camera
	// Params:
	//		position: the new position
	void SetPosition(glm::vec3 position);

	// Getters
	glm::vec3 GetPosition();
	glm::mat4 GetWorldTransform() { return m_worldTransform; }
	glm::mat4 GetViewTransform() { return m_viewTransform; }
	glm::mat4 GetProjectionTransform() { return m_projectionTransform; }
	glm::mat4 GetProjectionView() { return m_projectionViewTransform; }
protected:
	void UpdateTransform(); // update the projection view transform

	glm::mat4 m_worldTransform;
	glm::mat4 m_viewTransform;
	glm::mat4 m_projectionTransform;
	glm::mat4 m_projectionViewTransform;
};

