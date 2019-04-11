#include "Camera.h"

Camera::Camera()
{
	m_worldTransform = glm::mat4(1);
	m_viewTransform = glm::mat4(1);
	m_projectionTransform = glm::mat4(1);
	m_projectionViewTransform = glm::mat4(1);
}

Camera::~Camera()
{
}

// Get the position vector out of the world transform 
glm::vec3 Camera::GetPosition()
{
	glm::vec3 position = m_worldTransform[3];
	return position;
}

// use the glm::perspective function to get the new projectionTransform
void Camera::SetPerspective(float fieldOfView, float aspectRatio, float near, float far)
{
	m_projectionTransform = glm::perspective(fieldOfView, aspectRatio, near, far);
}

// use the glm::lookat to get the new view transform
void Camera::SetLookAt(glm::vec3 from, glm::vec3 to, glm::vec3 up)
{
	m_viewTransform = glm::lookAt(from, to, up);
}

// set the position vector in the world transform
void Camera::SetPosition(glm::vec3 position)
{
	m_worldTransform[3] = glm::vec4(position.x, position.y, position.z, 1);
}

// update the projection * view transform
void Camera::UpdateTransform()
{
	m_projectionViewTransform = m_projectionTransform * m_viewTransform;
}
