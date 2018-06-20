#pragma once

namespace cpp_utils {

template<typename T>
class ScopedAssignment {
	T &target, backup;

public:
	ScopedAssignment(T &target, T value)
	: target(target) {
		backup = target;
		target = value;
	}
	~ScopedAssignment() {
		target = backup;
	}
	ScopedAssignment(const ScopedAssignment &) = delete;
	ScopedAssignment &operator=(const ScopedAssignment &) = delete;
	ScopedAssignment &operator=(ScopedAssignment &&other) {
		target = other.target;
		backup = other.backup;
		return *this;
	}
};

}
