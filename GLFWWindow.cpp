#include "stdafx.h"
#include "GLFWWindow.h"
#include "ActionManager.h"

GLFWWindow::GLFWWindow(GLuint width, GLuint height)
{
	WIDTH = width;
	HEIGHT = height;

	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);

	// to use only new features we setting profile as "core profile"
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Simple example", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	mouseState.windowWidth = WIDTH;
	mouseState.windowHeight = HEIGHT;

	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferWidth);
	glViewport(0, 0, frameBufferWidth, frameBufferWidth);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetWindowSizeCallback(window, window_size_callback);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	// to use in a modern way
	glewExperimental = GLFW_TRUE;

	if (GLEW_OK != glewInit())
	{
		std::cerr << "Failed to initialise glew" << std::endl;
		return;
	}
}

GLFWWindow::~GLFWWindow()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

GLFWwindow * GLFWWindow::getGLFWwindow()
{
	return window;
}

void GLFWWindow::error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

void GLFWWindow::pollEvents()
{
	glfwPollEvents();
}

bool GLFWWindow::shouldClose()
{
	return glfwWindowShouldClose(window);
}

void GLFWWindow::getWindowSize(int & width, int & height)
{
	width = WIDTH;
	height = HEIGHT;
}

void GLFWWindow::getFrameBufferSize(int & width, int & height)
{
	width = frameBufferWidth;
	height = frameBufferHeight;
}

void GLFWWindow::swapBuffers()
{
	glfwSwapBuffers(window);
}

void GLFWWindow::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	if (key == GLFW_KEY_W && action == GLFW_PRESS)
		keyboardState.W = DOWN;
	if (key == GLFW_KEY_W && action == GLFW_RELEASE)
		keyboardState.W = UP;

	if (key == GLFW_KEY_A && action == GLFW_PRESS)
		keyboardState.A = DOWN;
	if (key == GLFW_KEY_A && action == GLFW_RELEASE)
		keyboardState.A = UP;

	if (key == GLFW_KEY_S && action == GLFW_PRESS)
		keyboardState.S = DOWN;
	if (key == GLFW_KEY_S && action == GLFW_RELEASE)
		keyboardState.S = UP;

	if (key == GLFW_KEY_D && action == GLFW_PRESS)
		keyboardState.D = DOWN;
	if (key == GLFW_KEY_D && action == GLFW_RELEASE)
		keyboardState.D = UP;

	if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
		keyboardState.Shift = DOWN;
	if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
		keyboardState.Shift = UP;


}

void GLFWWindow::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	mouseState.shiftX = xpos - mouseState.x;
	mouseState.shiftY = ypos - mouseState.y;
	mouseState.x = xpos;
	mouseState.y = ypos;
}

void GLFWWindow::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		mouseState.left_button = DOWN;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		mouseState.left_button = UP;
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		mouseState.right_button = DOWN;
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
		mouseState.right_button = UP;
	}
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
		mouseState.midle_button = DOWN;
	}
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE) {
		mouseState.midle_button = UP;
	}

	glfwGetCursorPos(window, &mouseState.clickPositionX, &mouseState.clickPositionY);
}

void GLFWWindow::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void GLFWWindow::window_size_callback(GLFWwindow* window, int width, int height)
{

	mouseState.windowWidth = width;
	mouseState.windowHeight = height;
}