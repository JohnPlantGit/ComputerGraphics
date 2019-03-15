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

	return true;
}

void ApplicationTest::Draw()
{
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

	float s = glm::cos(glfwGetTime()) * 0.5f + 0.5f;
	glm::vec3 p = (1.0f - s) * m_positions[0] + s * m_positions[1];
	glm::quat r = glm::slerp(m_rotations[0], m_rotations[1], s);
	glm::mat4 m = glm::translate(p) * glm::toMat4(r);
	aie::Gizmos::addAABBFilled(p, glm::vec3(0.5f), glm::vec4(1, 0, 0, 1), &m);

	aie::Gizmos::addSphere(vec3(0), 1, 50, 50, vec4(1, 1, 1, 1));

	//aie::Gizmos::draw(projection * view);
	aie::Gizmos::draw(m_camera->GetProjectionView());
}

void ApplicationTest::Shutdown()
{
	delete m_camera;
}
