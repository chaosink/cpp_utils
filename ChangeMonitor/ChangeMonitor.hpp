#pragma once

#include <vector>
#include <map>
#include <cassert>

namespace cpp_utils {

class ChangeMonitorInterface {
public:
	virtual bool Changed() = 0;
	virtual ~ChangeMonitorInterface() = default;
};

template<typename T>
class Wrapper {
	T *target_ = nullptr;

public:
	Wrapper() {}
	Wrapper(T &target): target_(&target) {}
	// Disable copy ctor, and hence the derived's.
	Wrapper(const Wrapper &) = delete;
	// Disable move ctor, and hence the derived's.
	Wrapper(Wrapper &&) = delete;

	Wrapper& operator=(const T &value) {
		assert(target_);
		*target_ = value;
		return *this;
	}
	// Disable copy assignment, and hence the derived's.
	Wrapper& operator=(const Wrapper &) = delete;
	// Disable move assignment, and hence the derived's.
	Wrapper& operator=(Wrapper &&) = delete;

	void Wrap(T &target) {
		target_ = &target;
	}

	operator T&() {
		assert(target_);
		return *target_;
	}

	T& operator()() { // For the lack of the dot operator to access members.
		assert(target_);
		return *target_;
	}
};

// Monitors on the same intance are independent.
// A monitor's query Changed() won't influence other monitors' query results.
template<typename T>
class ChangeMonitorReferenceIndividual
: public Wrapper<T>, public ChangeMonitorInterface {
	T backup_;

public:
	ChangeMonitorReferenceIndividual(T &target): Wrapper<T>(target), backup_(target) {}

	virtual bool Changed() override {
		if(backup_ != this->operator T&()) {
			backup_ = *this;
			return true;
		}
		return false;
	}

	using Wrapper<T>::operator=;
};

// Monitors on the same intance share the same state.
// Only one monitor's query Changed() will get true.
template<typename T>
class ChangeMonitorReferenceShared
: public Wrapper<T>, public ChangeMonitorInterface {
	static std::map<void*, T> backup_;

public:
	ChangeMonitorReferenceShared(T &target): Wrapper<T>(target) {
		backup_[&target] = target;
	}

	virtual bool Changed() override {
		if(backup_[&(this->operator T&())] != *this) {
			backup_[&(this->operator T&())] = *this;
			return true;
		}
		return false;
	}

	using Wrapper<T>::operator=;
};
template<typename T>
std::map<void*, T> ChangeMonitorReferenceShared<T>::backup_;

// Each monitor owns an instance, so it's independent.
template<typename T>
class ChangeMonitorInstance
: public Wrapper<T>, public ChangeMonitorInterface {
	T target_instance_;
	ChangeMonitorReferenceIndividual<T> monitor_;

public:
	ChangeMonitorInstance(const T &value)
	: target_instance_(value), monitor_(target_instance_) {
		Wrapper<T>::Wrap(target_instance_);
	}

	virtual bool Changed() override {
		return monitor_.Changed();
	}

	using Wrapper<T>::operator=;
};

// A Summary that can query the state of a set of instances.
// The query returns true if one of the instances has changed.
class ChangeMonitorSummary {
	std::vector<ChangeMonitorInterface*> variables;

public:
	void Insert(ChangeMonitorInterface &variable) {
		variables.push_back(&variable);
	}

	void Merge(const ChangeMonitorSummary &cms) {
		variables.reserve(variables.size() + cms.variables.size());
		variables.insert(variables.end(), cms.variables.begin(), cms.variables.end());
	}

	bool Changed() {
		bool changed = false;
		for(auto &v: variables)
			if(v->Changed()) {
				changed = true;
				// No `break;`, because `Changed()` will update the backup data.
			}
		return changed;
	}
};

}
