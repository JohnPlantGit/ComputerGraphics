#pragma once
#include "Application.h"
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>
#include "FlyCam.h"
#include "Shader.h"
#include "Mesh.h"
#include "OBJMesh.h"
#include "Texture.h"
#include "RenderTarget.h"

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

	aie::ShaderProgram m_BRDFShader;
	aie::ShaderProgram m_toonShader;
	aie::ShaderProgram m_texturedShader;
	aie::ShaderProgram m_postShader;
	aie::ShaderProgram m_outlineShader;

	Mesh m_fullScreenQuad;
	Mesh m_quadMesh;
	Mesh m_boxMesh;
	glm::mat4 m_boxTransform;
	
	aie::OBJMesh m_spearMesh;
	aie::OBJMesh m_fenceMesh;
	aie::OBJMesh m_pillarMesh;
	aie::OBJMesh m_swordMesh;
	aie::OBJMesh m_barrelMesh;
	aie::OBJMesh m_gunMesh;

	glm::mat4 m_spearTransform;	
	glm::mat4 m_toonSpearTransform;
	glm::mat4 m_outlineSpearTransform;
	glm::mat4 m_barrelTransform;
	glm::mat4 m_gunTransform;

	glm::mat4 m_fenceTransform;

	glm::mat4 m_pillarTransform;

	glm::mat4 m_swordTransform;

	//aie::OBJMesh m_carMesh;
	aie::Texture m_texture;
	Light m_light;
	Light m_light2;
	glm::vec3 m_ambientLight;

	aie::RenderTarget m_renderTarget;
};

