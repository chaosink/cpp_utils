#pragma once

#include <vector>

namespace cpp_utils {

class ChangeMonitorBase {
public:
	virtual bool Changed() = 0;
	virtual ~ChangeMonitorBase() = default;
};

template<typename T>
class ChangeMonitorWrap : public ChangeMonitorBase {
	T target, backup;

public:
	ChangeMonitorWrap(const T &value)
	: target(value), backup(value) {}

	void Set(const T &value) {
		target = value;
	}

	T& Get() {
		return target;
	}

	virtual bool Changed() override {
		if(target != backup) {
			backup = target;
			return true;
		}
		return false;
	}
};

template<typename T>
class ChangeMonitorLink : public ChangeMonitorBase {
	const T &target;
	T backup;

public:
	ChangeMonitorLink(const T &target)
	: target(target), backup(target) {}

	virtual bool Changed() override {
		if(target != backup) {
			backup = target;
			return true;
		}
		return false;
	}
};

class ChangeMonitorSummary {
	std::vector<ChangeMonitorBase*> variables;

public:
	void Add(ChangeMonitorBase &variable) {
		variables.push_back(&variable);
	}

	void Add(const ChangeMonitorSummary &cms) {
		variables.reserve(variables.size() + cms.variables.size());
		variables.insert(variables.end(), cms.variables.begin(), cms.variables.end());
	}

	bool Changed() {
		bool changed = false;
		for(auto &v: variables) {
			if(v->Changed())
				changed = true;
		}
		return changed;
	}
};

}
