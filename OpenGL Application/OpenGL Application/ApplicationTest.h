#pragma once
#include "Application.h"
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>
#include "FlyCam.h"
#include "Shader.h"
#include "Mesh.h"
#include "OBJMesh.h"
#include "Texture.h"
class ApplicationTest : public Application
{
public:
	ApplicationTest();
	~ApplicationTest();

	virtual bool Startup();
	virtual bool Update();
	virtual void Draw();
	virtual void Shutdown();

	struct Light
	{
		glm::vec3 direction;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};
	struct Material
	{
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};

private:
	float m_cameraRotation;
	glm::vec3 m_positions[2];
	glm::quat m_rotations[2];

	FlyCam* m_camera;

	aie::ShaderProgram m_shader;
	Mesh m_quadMesh;
	glm::mat4 m_quadTransform;
	Mesh m_boxMesh;
	glm::mat4 m_boxTransform;
	aie::OBJMesh m_bunnyMesh;
	aie::Texture m_texture;
	Light m_light;
	glm::vec3 m_ambientLight;
};

