#include "ApplicationTest.h"
#include "Gizmos.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "GLFW/glfw3.h"
#include "Input.h"
#include <glm/gtx/transform.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat4;

ApplicationTest::ApplicationTest()
{
}

ApplicationTest::~ApplicationTest()
{
}

bool ApplicationTest::Startup()
{
	aie::Gizmos::create(10000, 10000, 10000, 10000);
	m_cameraRotation = 0;

	m_positions[0] = glm::vec3(10, 0, 0);
	m_positions[1] = glm::vec3(-10, 0, 0);
	m_rotations[0] = glm::quat(glm::vec3(0, 0, 0));
	m_rotations[1] = glm::quat(glm::vec3(0, glm::pi<float>(), 0));

	m_camera = new FlyCam();
	m_camera->SetLookAt(glm::vec3(-10, -10, -10), glm::vec3(10, 10, 10), glm::vec3(0, 1, 0));
	m_camera->SetPosition(glm::vec3(10, 10, 10));

	m_BRDFShader.loadShader(aie::eShaderStage::VERTEX, "../Shaders/simple.vert");
	m_BRDFShader.loadShader(aie::eShaderStage::FRAGMENT, "../Shaders/simple.frag");
	if (m_BRDFShader.link() == false)
	{
		printf("Shader Error: %s\n", m_BRDFShader.getLastError());
		return false;
	}
	m_toonShader.loadShader(aie::eShaderStage::VERTEX, "../Shaders/toon.vert");
	m_toonShader.loadShader(aie::eShaderStage::FRAGMENT, "../Shaders/toon.frag");
	if (m_toonShader.link() == false)
	{
		printf("Toon Shader Error: %s\n", m_toonShader.getLastError());
		return false;
	}
	m_texturedShader.loadShader(aie::eShaderStage::VERTEX, "../Shaders/textured.vert");
	m_texturedShader.loadShader(aie::eShaderStage::FRAGMENT, "../Shaders/textured.frag");
	if (m_texturedShader.link() == false)
	{
		printf("Textured Shader Error: %s\n", m_texturedShader.getLastError());
		return false;
	}

	m_postShader.loadShader(aie::eShaderStage::VERTEX, "../Shaders/post.vert");
	m_postShader.loadShader(aie::eShaderStage::FRAGMENT, "../Shaders/post.frag");
	if (m_postShader.link() == false)
	{
		printf("Post Shader Error: %s\n", m_postShader.getLastError());
		return false;
	}

	m_outlineShader.loadShader(aie::eShaderStage::VERTEX, "../Shaders/outline.vert");
	m_outlineShader.loadShader(aie::eShaderStage::FRAGMENT, "../Shaders/outline.frag");
	if (m_outlineShader.link() == false)
	{
		printf("Outline Shader Error: %s\n", m_outlineShader.getLastError());
		return false;
	}

	m_fullScreenQuad.InitialiseFullscreenQuad();

	Mesh::Vertex vertices[4];
	vertices[0].position = { -0.5f, 0, 0.5f, 1 };
	vertices[1].position = { 0.5f, 0, 0.5f, 1 };
	vertices[2].position = { -0.5f, 0, -0.5f, 1 };
	vertices[3].position = { 0.5f, 0, -0.5f, 1 };

	unsigned int indices[6] = { 0, 1, 2, 2, 1, 3 };

	m_quadMesh.InitialiseQuad();

	m_boxMesh.InitialiseBox(glm::vec3(3,3,3));
	m_boxTransform =
	{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};

	if (m_spearMesh.load("../Data/soulspear/soulspear.obj", true, true) == false)
	{
		printf("Spear Mesh Error!\n");
		return false;
	}
	m_spearTransform =
	{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		3,0,0,1
	};
	m_toonSpearTransform =
	{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		-3,0,0,1
	};
	m_outlineSpearTransform =
	{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};

	if (m_fenceMesh.load("../Data/woodenfence/woodenfence.obj", true, true) == false)
	{
		printf("Fence mesh error!\n");
		return false;
	}
	m_fenceTransform =
	{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,3,1
	};

	if (m_chestMesh.load("../Data/chest/Model.obj", true, true) == false)
	{
		printf("Chest Mesh Error!\n");
		return false;
	}
	m_chestTransform =
	{
		10,0,0,0,
		0,10,0,0,
		0,0,10,0,
		0,0,-3,1
	};

	int width, height;
	glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);
	if (m_renderTarget.initialise(1, width, height) == false)
	{
		printf("Render Target Error!\n");
		return false;
	}
	/*if (m_carMesh.load("../Data/bugatti/bugatti.obj", false) == false)
	{
		printf("Car Mesh Error!\n");
		return false;
	}*/

	//unsigned char texelData[4] = { 0, 255, 255, 0 };
	//m_texture.create(2, 2, aie::Texture::RED, texelData);
	if (m_texture.load("../Data/numbered_grid.tga") == false)
	{
		printf("Failed to load texture!\n");
		return false;
	}

	m_light.diffuse = { 1,1,1 };
	m_light.specular = { 1,1,1 };
	m_light2.diffuse = { 1,1,1 };
	m_light2.specular = { 1,1,1 };
	//m_ambientLight = { 0.25f,0.25f,0.25f };
	m_ambientLight = { 1,1,1 };

	return true;
}

bool ApplicationTest::Update()
{
	aie::Input* input = aie::Input::getInstance();
	if (input->wasKeyPressed(aie::INPUT_KEY_RIGHT))
	{
		m_cameraRotation += 0.1;
	}
	if (input->wasKeyPressed(aie::INPUT_KEY_LEFT))
	{
		m_cameraRotation -= 0.1;
	}

	m_camera->Update();
	m_light.direction = glm::normalize(glm::vec3(glm::cos(glfwGetTime()), glm::sin(glfwGetTime()), 0));
	//m_light.direction = glm::normalize(glm::vec3(0, -1, 0));
	m_light2.direction = -glm::normalize(glm::vec3(glm::cos(glfwGetTime()), 0, glm::sin(glfwGetTime())));
	//m_light2.direction = glm::normalize(glm::vec3(0, 1, 0));

	return true;
}

void ApplicationTest::Draw()
{
	m_renderTarget.bind();

	ClearScreen();

	glm::mat3 rotationMatrix;
	float cs = cosf(m_cameraRotation);
	float sn = sinf(m_cameraRotation);

	rotationMatrix[0] = glm::vec3(cs, 0, sn);
	rotationMatrix[1] = glm::vec3(0, 1, 0);
	rotationMatrix[2] = glm::vec3(-sn, 0, cs);
	vec3 cameraPosition = rotationMatrix * vec4(-25, -25, -25 , 0);
	mat4 view = glm::lookAt(-cameraPosition, cameraPosition, vec3(0, 1, 0));
	mat4 projection = glm::perspective(glm::pi<float>() * 0.25f, 16.f / 9.f, 0.1f, 1000.f);

	aie::Gizmos::clear();

	aie::Gizmos::addTransform(mat4(1));

	vec4 white(1);
	vec4 black(0, 0, 0, 1);

	for (int i = 0; i < 21; i++)
	{
		aie::Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10), (i == 10 ? white : black));
		aie::Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i), (i == 10 ? white : black));
	}

	/*float s = glm::cos(glfwGetTime()) * 0.5f + 0.5f;
	glm::vec3 p = (1.0f - s) * m_positions[0] + s * m_positions[1];
	glm::quat r = glm::slerp(m_rotations[0], m_rotations[1], s);
	glm::mat4 m = glm::translate(p) * glm::toMat4(r);
	aie::Gizmos::addAABBFilled(p, glm::vec3(0.5f), glm::vec4(1, 0, 0, 1), &m);*/

	//aie::Gizmos::addSphere(vec3(0), 1, 50, 50, vec4(1, 1, 1, 1));

	aie::Gizmos::addSphere(-m_light.direction * 10, 1, 10, 10, glm::vec4(1, 1, 1, 1)); // light
	aie::Gizmos::addSphere(-m_light2.direction * 10, 1, 10, 10, glm::vec4(1, 1, 1, 1)); // light

	aie::Gizmos::draw(m_camera->GetProjectionView());

	// bind the BRDF shader
	m_BRDFShader.bind();

	// bind lights
	m_BRDFShader.bindUniform("Ia", m_ambientLight);

	m_BRDFShader.bindUniform("lights[0].diffuse", m_light.diffuse);
	m_BRDFShader.bindUniform("lights[0].specular", m_light.specular);
	m_BRDFShader.bindUniform("lights[0].direction", m_light.direction);

	m_BRDFShader.bindUniform("lights[1].diffuse", m_light2.diffuse);
	m_BRDFShader.bindUniform("lights[1].specular", m_light2.specular);
	m_BRDFShader.bindUniform("lights[1].direction", m_light2.direction);
	//

	m_BRDFShader.bindUniform("CameraPosition", m_camera->GetPosition());

	m_BRDFShader.bindUniform("Roughness", 1.f);
	m_BRDFShader.bindUniform("ReflectionCoefficient", 10.f);

	// bind matrices
	m_BRDFShader.bindUniform("ProjectionViewModel", m_camera->GetProjectionView() * m_spearTransform);
	m_BRDFShader.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_spearTransform)));
	m_BRDFShader.bindUniform("ModelMatrix", m_spearTransform);

	//m_BRDFShader.bindUniform("time", (float)glfwGetTime());
	m_spearMesh.draw();

	// bind the toon shader
	m_toonShader.bind();

	// bind lights
	m_toonShader.bindUniform("lights[0].diffuse", m_light.diffuse);
	m_toonShader.bindUniform("lights[0].specular", m_light.specular);
	m_toonShader.bindUniform("lights[0].direction", m_light.direction);

	m_toonShader.bindUniform("lights[1].diffuse", m_light2.diffuse);
	m_toonShader.bindUniform("lights[1].specular", m_light2.specular);
	m_toonShader.bindUniform("lights[1].direction", m_light2.direction);

	// bind spear matrices
	m_toonShader.bindUniform("ProjectionViewModel", m_camera->GetProjectionView() * m_toonSpearTransform);
	m_toonShader.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_toonSpearTransform)));
	m_toonShader.bindUniform("ModelMatrix", m_toonSpearTransform);

	m_spearMesh.draw();

	m_toonShader.bindUniform("ProjectionViewModel", m_camera->GetProjectionView() * m_fenceTransform);
	m_toonShader.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_fenceTransform)));
	m_toonShader.bindUniform("ModelMatrix", m_fenceTransform);

	m_fenceMesh.draw();

	// bind matrices
	m_toonShader.bindUniform("ProjectionViewModel", m_camera->GetProjectionView() * m_chestTransform);
	m_toonShader.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_chestTransform)));
	m_toonShader.bindUniform("ModelMatrix", m_chestTransform);

	m_chestMesh.draw();
	//

	// bind spear matrices
	/*m_fenceToonShader.bindUniform("ProjectionViewModel", m_camera->GetProjectionView() * m_fenceTransform);
	m_fenceToonShader.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_fenceTransform)));
	m_fenceToonShader.bindUniform("ModelMatrix", m_fenceTransform);

	m_fenceMesh.draw();*/
	//

	// bind the outline shader
	m_outlineShader.bind();

	// bind matrices
	m_outlineShader.bindUniform("ProjectionViewModel", m_camera->GetProjectionView() * m_outlineSpearTransform);
	m_outlineShader.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_outlineSpearTransform)));
	m_outlineShader.bindUniform("ModelMatrix", m_outlineSpearTransform);

	// bind lights
	m_outlineShader.bindUniform("lights[0].diffuse", m_light.diffuse);
	m_outlineShader.bindUniform("lights[0].specular", m_light.specular);
	m_outlineShader.bindUniform("lights[0].direction", m_light.direction);

	m_outlineShader.bindUniform("lights[1].diffuse", m_light2.diffuse);
	m_outlineShader.bindUniform("lights[1].specular", m_light2.specular);
	m_outlineShader.bindUniform("lights[1].direction", m_light2.direction);

	m_outlineShader.bindUniform("outlineColour", vec4(1, 0, 0, 1));
	m_outlineShader.bindUniform("cameraPosition", m_camera->GetPosition());

	m_spearMesh.draw();

	m_renderTarget.unbind();

	ClearScreen();

	m_postShader.bind();
	m_postShader.bindUniform("colourTarget", 0);
	m_renderTarget.getTarget(0).bind(0);

	m_fullScreenQuad.Draw();

	//m_texture.bind(0);

	// bind texture shader
	/*m_texturedShader.bind();

	m_texturedShader.bindUniform("ProjectionViewModel", m_camera->GetProjectionView() * m_toonSpearTransform);
	m_texturedShader.bindUniform("Kd", 0);
	m_renderTarget.getTarget(0).bind(0);*/

	//m_quadMesh.Draw();

	//m_boxMesh.Draw();

	//m_carMesh.draw();

	//aie::Gizmos::draw(projection * view);
	
}

void ApplicationTest::Shutdown()
{
	delete m_camera;
}
