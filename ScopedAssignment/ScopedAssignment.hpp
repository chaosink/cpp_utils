#pragma once

#include <utility>

namespace cpp_utils {

template<typename T>
class ScopedAssignment {
	T backup_, *target_;

public:
	ScopedAssignment(): target_(nullptr) {}
	// backup old values and assign new values
	ScopedAssignment(T &target, T value)
	: backup_(target), target_(&target) {
		*target_ = value;
	}

	// recover to the old values
	~ScopedAssignment() {
		if(target_) *target_ = backup_;
	}

	// movable but non-copyable
	ScopedAssignment(const ScopedAssignment &) = delete;
	ScopedAssignment(ScopedAssignment &&other) {
		*this = std::move(other);
	}
	ScopedAssignment &operator=(const ScopedAssignment &) = delete;
	ScopedAssignment &operator=(ScopedAssignment &&other) {
		if(target_) *target_ = backup_;
		target_ = other.target_;
		backup_ = other.backup_;
		other.target_ = nullptr;
		return *this;
	}
};

}
