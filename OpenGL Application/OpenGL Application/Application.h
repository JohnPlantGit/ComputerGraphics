#pragma once

struct GLFWwindow;

class Application
{
public:
	Application();
	~Application();
	
	// Start the application
	// Params:
	//		width: width of the window
	//		height: height of the window
	//		title: the name of the window
	void Run(int width, int height, const char* title);

	// Used for initialization
	// returns if sucessfully completed the start-up 
	virtual bool Startup() = 0;
	// Main update loop
	// returns if the application shouldn't close
	virtual bool Update() = 0;
	// main draw loop
	virtual void Draw() = 0;
	// Used for de-allocation
	virtual void Shutdown() = 0;

protected:
	// Create a window with glfw
	// Params:
	//		width: width of the window
	//		height: height of the window
	//		title: the name of the window
	// Returns if sucessfully created window
	virtual bool CreateWindow_(int width, int height, const char* title);
	// Destroy the application window
	virtual void DestroyWindow_();
	// Clear the screen
	void ClearScreen();

	GLFWwindow* m_window;
	bool m_gameOver;
};

