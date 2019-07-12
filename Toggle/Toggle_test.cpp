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

	// Key: SPACE
	Toggle toggle_space = Toggle(
		[&window]() {
			return glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
		},
		true
	);

	// Key: F
	Toggle toggle_f = Toggle(
		[&window]() {
			return glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS;
		},
		true
	);

	// Key combination: Ctrl+Shift+C
	Toggle toggle_ctrl_shift_c = Toggle(
		[&window]() {
			return glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS
				&& glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS
				&& glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS;
		},
		true
	);

	bool state_space = toggle_space.state();
	while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window)) {
		// No action to be taken.
		toggle_space.Update();
		if(state_space != toggle_space.state()) {
			state_space = toggle_space.state();
			cout << "Key SPACE's state changes to " << state_space << endl;
		}
		// Same action to be taken when the state changes from ON to OFF and from OFF to ON.
		toggle_f.Update([&toggle_space](bool state) {
			cout << "Key F pressed. State changes to " << state << endl;
		});
		// Different actions to be taken when the state changes from ON to OFF and from OFF to ON.
		toggle_ctrl_shift_c.Update(
			[](bool state) {
				cout << "Off2On(): Key Ctrl+Shift+C pressed. State changes to " << state << endl;
			},
			[](bool state) {
				cout << "On2Off(): Key Ctrl+Shift+C pressed. State changes to " << state << endl;
			}
		);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	TerminateGLFW(window);



// Tests on std::chrono timer.
	auto start = std::chrono::steady_clock::now();
	int second = 0;

	// Alarm every one second.
	Toggle toggle_alarm_1s = Toggle(
		[&start, &second]() {
			std::chrono::duration<double> delta = std::chrono::steady_clock::now() - start;
			if(delta.count() > second) {
				second++;
				return true;
			} else {
				return false;
			}
		},
		false,
		Toggle::None // For std::chrono timer, anti-jitter is needless.
	);
	while(second < 10) {
		toggle_alarm_1s.Update(
			[&second](bool state) {
				cout << second << " second(s). State changes to " << state << endl;
			},
			nullptr // Set on2off as nullptr to make it same as off2on.
		);
	}


	start = std::chrono::steady_clock::now();
	second = 0;

	// Alarm every two seconds.
	Toggle toggle_alarm_2s = Toggle(
		[&start, &second]() {
			std::chrono::duration<double> delta = std::chrono::steady_clock::now() - start;
			if(delta.count() > second) {
				second++;
				return true;
			} else {
				return false;
			}
		},
		false,
		Toggle::None // For std::chrono timer, anti-jitter is needless.
	);
	while(second < 10) {
		toggle_alarm_2s.Update(
			[&second](bool state) {
				cout << second << " second(s). State changes to " << state << endl;
			},
			[](bool) {} // Do nothing when the state changes from ON to OFF.
		);
	}
}
