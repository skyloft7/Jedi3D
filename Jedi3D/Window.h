#pragma once
#include <GLFW/glfw3.h>


class Window {

public:
	Window(int width, int height, const char* title);
	~Window();
	bool shouldClose();
	int m_width, m_height;
	GLFWwindow* m_handle;
	void close();
	void update();


};