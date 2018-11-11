#pragma once

#include <functional>

namespace cpp_utils {

// A toggle simulator.
// Toggle state is on(true) or off(false).
// Toggle can be switched from one state to another.
// Actions can be set that will be taken when state changes.
class Toggle {
	std::function<bool()> pressed_;
	std::function<void(const bool &)> off2on_, on2off_;
	bool state_;
	bool pressing_ = false;
	int jitter_;
	int count_ = -1;

public:
	enum JitterType: int {
		None = 0,
		Keyboard = 4, // 4 is suitable for keyboard.
	};

	// on2off_ will be same as off2of_ if it's nullptr: call the same function when state_
	// changes from on to off and from off to on.
	// Both of them could be nullptr: no action taken when state_ changes.
	Toggle(bool state, std::function<bool()> pressed,
		std::function<void(const bool &)> off2on = nullptr,
		std::function<void(const bool &)> on2off = nullptr, int jitter = Keyboard):
	state_(state), pressed_(std::move(pressed)),
	off2on_(std::move(off2on)),
	on2off_(on2off ? std::move(on2off) : off2on_),
	jitter_(jitter) {}

	// Actions taken when state_ changes from off to on and from on to off.
	bool Update() {
		if(count_-- > 0) return state_;
		if(pressed_()) {
			if(!pressing_) {
				state_ = !state_;
				if(state_)
					// Call off2on_() when state_ changes from off(false) to on(true).
					off2on_(state_);
				else
					// Call on2off_() when state_ changes from on(true) to off(false).
					on2off_(state_);
				pressing_ = true;
				count_ = jitter_;
			}
		} else {
			if(pressing_) {
				pressing_ = false;
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
