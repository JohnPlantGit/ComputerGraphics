#include "Application.h"
#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include "Input.h"

Application::Application()
{
	m_gameOver = false;
}

Application::~Application()
{
}

void Application::Run(int width, int height, const char* title)
{
	if (CreateWindow_(width, height, title) && Startup())
	{
		while (!m_gameOver)
		{
			aie::Input::getInstance()->clearStatus();
			glfwPollEvents();

			Update();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			Draw();

			glfwSwapBuffers(m_window);

			m_gameOver = m_gameOver || glfwWindowShouldClose(m_window) == GLFW_TRUE;
		}
	}

	Shutdown();
	DestroyWindow_();
}

bool Application::CreateWindow_(int width, int height, const char* title)
{
	if (glfwInit() == false)
		return false;

	m_window = glfwCreateWindow(width, height, "Computer Graphics", nullptr, nullptr);

	if (m_window == nullptr)
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
		return false;
	}

	glClearColor(0.25f, 0.25f, 0.25f, 1);
	glEnable(GL_DEPTH_TEST);

	aie::Input::create();

	return true;
}

void Application::DestroyWindow_()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}
