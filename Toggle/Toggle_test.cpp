#include "Toggle.hpp"
using namespace cpp_utils;

#include <iostream>
using namespace std;

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

// Tests on GLFW key events.
int main() {
	auto window = InitGLFW("Toggle_test", 512, 384);

	Toggle toggle_s = Toggle(true, [&window]() {
		return glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
	}, [](const bool &state) {
		cout << "Key S pressed. State changes to " << state << endl;
	});

	Toggle toggle_f = Toggle(true, [&window]() {
		return glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS;
	}, [](const bool &state) {
		cout << "Off2On(): Key F pressed. State changes to " << state << endl;
	}, [](const bool &state) {
		cout << "On2Off(): Key F pressed. State changes to " << state << endl;
	});

	while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window)) {
		toggle_s.Update();
		toggle_f.Update();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	TerminateGLFW(window);
}
