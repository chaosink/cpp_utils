#pragma once

#include <utility>

namespace cpp_utils {

template<typename T>
class ScopedAssignment {
	T backup_, *target_;

public:
	ScopedAssignment(): target_(nullptr) {}
	// Backup old values and assign new values.
	ScopedAssignment(T &target, T value)
	: backup_(target), target_(&target) {
		*target_ = value;
	}

	// Recover to the old values.
	~ScopedAssignment() {
		if(target_) *target_ = backup_;
	}

	// Movable but non-copyable.
	ScopedAssignment(const ScopedAssignment &) = delete;
	ScopedAssignment(ScopedAssignment &&other) {
		*this = std::move(other);
	}
	ScopedAssignment &operator=(const ScopedAssignment &) = delete;
	ScopedAssignment &operator=(ScopedAssignment &&other) {
		// Recover the old instance before monitoring the new one.
		if(target_) *target_ = backup_;
		target_ = other.target_;
		backup_ = other.backup_;
		other.target_ = nullptr; // other has no effect any more.
		return *this;
	}
};

}
