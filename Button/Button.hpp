#pragma once

#include <functional>
#include <cassert>

namespace cpp_utils {

// A button simulator.
// Button state is pressing or not pressing.
// Button can be pressed and released.
// Actions can be set to be taken when the button is pressed and released (just after state changes).
class Button {
	const std::function<bool()> pressed_;
	bool pressing_ = false;
	const int jitter_; // For anti-jitter.
	int count_ = -1;

public:
	enum JitterType: int {
		None = 0,
		Keyboard = 4, // 4 is suitable for keyboard.
	};

	Button(const std::function<bool()> &pressed, int jitter = Keyboard)
	: pressed_(pressed), jitter_(jitter) {
		assert(pressed_ != nullptr);
	}

	// Actions (`on_press()` and `on_release()`) will be taken
	// when the button is pressed and released (just after the state `pressing_` changes).
	// Both of them could be `nullptr`: no action taken when the button is pressed or released.
	bool Update(
		const std::function<void()> &on_press = nullptr,
		const std::function<void()> &on_release = nullptr
	) {
		if(count_-- > 0) return pressing_;
		if(pressed_()) {
			if(!pressing_) {
				// State `pressing_` changes just before the action `on_press()` taken.
				pressing_ = true;
				// Call `on_press()` when the button is pressed.
				if(on_press) on_press();
				count_ = jitter_;
			}
		} else {
			if(pressing_) {
				// State `pressing_` changes just before the action `on_release()` taken.
				pressing_ = false;
				// Call `on_release()` when the button is released.
				if(on_release) on_release();
				count_ = jitter_;
			}
		}
		return pressing_;
	}
	bool pressing() {
		return pressing_;
	}
};

}
