#pragma once

struct GLFWwindow;

class Application
{
public:
	Application();
	~Application();
	
	void Run(int width, int height, const char* title);

	virtual bool Startup() = 0;
	virtual bool Update() = 0;
	virtual void Draw() = 0;
	virtual void Shutdown() = 0;

protected:
	virtual bool CreateWindow_(int width, int height, const char* title);
	virtual void DestroyWindow_();
	void ClearScreen();

	GLFWwindow* m_window;
	bool m_gameOver;
};

