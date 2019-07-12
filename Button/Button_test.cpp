#include "Button.hpp"
using namespace cpp_utils;

#include "GLFW.hpp"

#include <iostream>
#include <chrono>
using namespace std;

int main() {
// Tests on GLFW key events.
	auto window = InitGLFW("Button_test", 512, 384);

	// Key: SPACE
	Button Button_space = Button([&window]() {
		return glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
	});

	// Key: F
	Button Button_f = Button([&window]() {
		return glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS;
	});

	// Key combination: Ctrl+Shift+C
	Button Button_ctrl_shift_c = Button([&window]() {
		return glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS
			&& glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS
			&& glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS;
	});

	bool space_pressing = Button_space.pressing();
	while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window)) {
		// No action to be taken.
		Button_space.Update();
		if(space_pressing != Button_space.pressing()) {
			space_pressing = Button_space.pressing();
			if(space_pressing)
				cout << "Key SPACE's pressed." << endl;
			else
				cout << "Key SPACE's released." << endl;
		}
		// An action to be taken when the button is pressed.
		Button_f.Update([&Button_space]() {
			cout << "OnPress(): Key F pressed." << endl;
		});
		// Different actions to be taken when the button is pressed and released.
		Button_ctrl_shift_c.Update(
			[]() {
				cout << "OnPress(): Key Ctrl+Shift+C pressed." << endl;
			},
			[]() {
				cout << "OnRelease(): Key Ctrl+Shift+C released." << endl;
			}
		);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	TerminateGLFW(window);



// Tests on `std::chrono` timer.
	auto start = std::chrono::steady_clock::now();
	int second = 0;

	// Alarm every one second.
	Button Button_alarm_1s = Button(
		[&start, &second]() {
			std::chrono::duration<double> delta = std::chrono::steady_clock::now() - start;
			if(delta.count() > second) {
				second++;
				return true;
			} else {
				return false;
			}
		},
		Button::None // For `std::chrono` timer, anti-jitter is needless.
	);
	while(second < 10) {
		Button_alarm_1s.Update(
			[&second]() {
				cout << "OnPress(): " << second << " second(s)." << endl;
			},
			nullptr // Set `OnRelease()` as `nullptr` or `[]() {}`
					// to do nothing when the button is released.
		);
	}
}
