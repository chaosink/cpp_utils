#include "Toggle.hpp"
using namespace cpp_utils;

#include <iostream>
#include <chrono>
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


int main() {
// Tests on GLFW key events.
	auto window = InitGLFW("Toggle_test", 512, 384);

	// Key: F
	Toggle toggle_f = Toggle(true, [&window]() {
		return glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS;
	}, [](const bool &state) {
		cout << "Key F pressed. State changes to " << state << endl;
	});

	// Key combination: Ctrl+Shift+C
	Toggle toggle_ctrl_shift_c = Toggle(true, [&window]() {
		return glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS
			&& glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS
			&& glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS;
	}, [](const bool &state) {
		cout << "Off2On(): Key Ctrl+Shift+C pressed. State changes to " << state << endl;
	}, [](const bool &state) {
		cout << "On2Off(): Key Ctrl+Shift+C pressed. State changes to " << state << endl;
	});

	while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window)) {
		toggle_f.Update();
		toggle_ctrl_shift_c.Update();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	TerminateGLFW(window);

// Tests on std::chrono timer.
	auto start = std::chrono::steady_clock::now();
	int second = 0;
	Toggle toggle_alarm = Toggle(true, [&start, &second]() {
		std::chrono::duration<double> diff = std::chrono::steady_clock::now() - start;
		return diff.count() > second;
	}, [&second](const bool &state) {
		cout << second << " second(s). State changes to " << state << endl;
		second++;
	});
	while(second < 10) {
		toggle_alarm.Update();
	}
}
