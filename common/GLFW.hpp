#include <GLFW/glfw3.h>

GLFWwindow* InitGLFW(const char *window_title, int window_w, int window_h) {
	if(!glfwInit()) exit(EXIT_FAILURE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

	GLFWwindow* window = glfwCreateWindow(window_w, window_h, window_title, NULL, NULL);
	if(!window) {
		glfwTerminate();
		fprintf(stderr, "Failed to create GLFW window\n");
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	return window;
}

void TerminateGLFW(GLFWwindow *window) {
	glfwDestroyWindow(window);
	glfwTerminate();
}
