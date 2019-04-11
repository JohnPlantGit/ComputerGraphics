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

// Creates the window and handles the main game loop
void Application::Run(int width, int height, const char* title)
{
	if (CreateWindow_(width, height, title) && Startup())
	{
		while (!m_gameOver)
		{
			aie::Input::getInstance()->clearStatus();
			glfwPollEvents(); // update the input

			Update(); // run the update function

			//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			Draw(); // draw the application

			glfwSwapBuffers(m_window); // swap out the buffers to render the next frame

			m_gameOver = m_gameOver || glfwWindowShouldClose(m_window) == GLFW_TRUE; // check if the application should close
		}
	}

	// clean up
	Shutdown();
	DestroyWindow_();
}

// Create a window in glfw
// Params:
//		width: width of the window
//		height: height of the window
//		title: the name of the window
bool Application::CreateWindow_(int width, int height, const char* title)
{
	if (glfwInit() == false) // if glfw fails to initialize
		return false;

	m_window = glfwCreateWindow(width, height, "Computer Graphics", nullptr, nullptr); // create the window

	if (m_window == nullptr) // check the window created correctly
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_window); // highlight the window

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) // attempt to load open gl 
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
		return false;
	}

	glClearColor(0.25f, 0.25f, 0.25f, 1); // the color to clear the background to
	glEnable(GL_DEPTH_TEST); 

	aie::Input::create();

	return true;
}

// Destroy the window in glfw
void Application::DestroyWindow_()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

// Clear the screen to the clear color
void Application::ClearScreen()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
