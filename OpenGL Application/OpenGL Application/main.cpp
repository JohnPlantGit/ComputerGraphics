#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include "Gizmos.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "ApplicationTest.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;
int main() 
{
	/*if (glfwInit() == false)
		return -1;

	GLFWwindow* window = glfwCreateWindow(1280, 720, "Computer Graphics", nullptr, nullptr);

	if (window == nullptr)
	{
		glfwTerminate();
		return -2;
	}

	glfwMakeContextCurrent(window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(window);
		glfwTerminate();
		return -3;
	}

	aie::Gizmos::create(10000, 10000, 10000, 10000);

	mat4 view = glm::lookAt(vec3(10, 10, 10), vec3(-10,-10,-10), vec3(0, 1, 0));
	mat4 projection = glm::perspective(glm::pi<float>() * 0.25f, 16.f / 9.f, 0.1f, 1000.f);

	glClearColor(0.25f, 0.25f, 0.25f, 1);
	glEnable(GL_DEPTH_TEST);
	while (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		aie::Gizmos::clear();

		aie::Gizmos::addTransform(mat4(1));

		vec4 white(1);
		vec4 black(0, 0, 0, 1);

		for (int i = 0; i < 21; i++)
		{
			aie::Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10), (i == 10 ? white : black));
			aie::Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i), (i == 10 ? white : black));
		}

		aie::Gizmos::addSphere(vec3(0), 1, 50, 50, vec4(1, 1, 1, 1));

		aie::Gizmos::draw(projection * view);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	aie::Gizmos::destroy();

	glfwDestroyWindow(window);
	glfwTerminate();*/

	ApplicationTest* app = new ApplicationTest();
	app->Run(1280, 720, "OpenGL App");

	delete app;
	return 0;
}