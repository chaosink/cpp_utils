#pragma once

#include <functional>
#include <cassert>

namespace cpp_utils {

// A toggle simulator.
// Toggle state is ON(true) or OFF(false).
// Toggle can be switched from one state to another.
// Actions can be set to be taken when the toggle is switched (just after state changes).
class Toggle {
	const std::function<bool()> switched_;
	bool state_;
	bool switching_ = false;
	const int jitter_; // For anti-jitter.
	int count_ = -1;

public:
	enum JitterType: int {
		None = 0,
		Keyboard = 4, // 4 is suitable for keyboard.
	};

	Toggle(const std::function<bool()> &pressed, bool state = false, int jitter = Keyboard)
	: switched_(pressed), state_(state), jitter_(jitter) {
		assert(pressed != nullptr);
	}

	// Actions (`off2on()` and `on2off()`) will be taken
	// when the toggle is switched (just after state changes from OFF to ON and from ON to OFF.
	// `on2off()` will be same as `off2of()` if it's `nullptr`: the same action will be taken.
	// Both of them could be `nullptr`: no action taken when the toggle is switched.
	bool Update(
		const std::function<void(bool)> &off2on = nullptr,
		const std::function<void(bool)> &on2off = nullptr
	) {
		if(count_-- > 0) return state_;
		if(switched_()) {
			if(!switching_) {
				// `state_` changes just before actions taken.
				state_ = !state_;
				if(state_) {
					// Call `off2on()` when `state_` changes from off(false) to on(true).
					if(off2on) off2on(state_);
				} else {
					// Call `on2off()` when `state_` changes from on(true) to off(false).
					// Or, if it is `nullptr`, take the same action as `off2on()`.
					if(on2off)
						on2off(state_);
					else if(off2on)
						off2on(state_);
				}
				switching_ = true;
				count_ = jitter_;
			}
		} else {
			if(switching_) {
				switching_ = false;
				count_ = jitter_;
			}
		}
		return state_;
	}

	bool state() {
		return state_;
	}
};

}
