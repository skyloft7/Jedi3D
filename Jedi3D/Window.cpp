#include "Window.h"
#include <iostream>


Window::Window(int width, int height, const char* title) {
	m_width = width;
	m_height = height;

	m_handle = glfwCreateWindow(m_width, m_height, title, nullptr, nullptr);
	glfwMakeContextCurrent(m_handle);
}

Window::~Window() {
	std::cout << "Hot dog" << std::endl;
}

bool Window::shouldClose() {

	return glfwWindowShouldClose(m_handle);
}

void Window::update() {
	glfwSwapBuffers(m_handle);
	glfwPollEvents();
}

void Window::close() {
	glfwDestroyWindow(m_handle);
}
