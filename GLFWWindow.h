#ifndef GLWIND
#define GLWIND

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class GLFWWindow
{
private:
	GLFWwindow* window;

	// Window size
	GLuint WIDTH;
	GLuint HEIGHT;

	// Framebuffer size
	int frameBufferWidth;
	int frameBufferHeight;

public:
	GLFWWindow(GLuint width, GLuint height);
	~GLFWWindow();
	GLFWwindow* getGLFWwindow();

	// GLFW callback functions
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_button_callback(GLFWwindow * window, int button, int action, int mods);
	static void framebuffer_size_callback(GLFWwindow * window, int width, int height);
	static void window_size_callback(GLFWwindow * window, int width, int height);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void error_callback(int error, const char* description);

	void pollEvents();
	void swapBuffers();
	bool shouldClose();

	void getWindowSize(int& width, int& height);
	void getFrameBufferSize(int& width, int& height);

};

#endif