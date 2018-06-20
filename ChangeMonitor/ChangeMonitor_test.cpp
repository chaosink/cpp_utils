#include "ChangeMonitor.hpp"
using namespace cpp_utils;

#include <iostream>
using namespace std;

int main() {
	// ChangeMonitorLink
	int i = 1;
	float f = 1.1f;

	ChangeMonitorLink<int> i_cml(i);
	ChangeMonitorLink<float> f_cml(f);

	ChangeMonitorSummary cms_link;
	cms_link.Add(i_cml);
	cms_link.Add(f_cml);

	std::cout << cms_link.Changed() << std::endl;
	i = 2;
	std::cout << cms_link.Changed() << std::endl;

	std::cout << cms_link.Changed() << std::endl;
	f = 2.2f;
	std::cout << cms_link.Changed() << std::endl;



	std::cout << std::endl;

	// ChangeMonitorWrap
	bool b = false;
	char c = 'a';

	ChangeMonitorWrap<bool> b_cmw(b); // same as b_cmw(false);
	ChangeMonitorWrap<char> c_cmw(c); // same as c_cmw('a');

	ChangeMonitorSummary cms_wrap;
	cms_wrap.Add(b_cmw);
	cms_wrap.Add(c_cmw);

	std::cout << cms_wrap.Changed() << std::endl;
	b_cmw.Set(true); // same as b_cmw.Get() = true;
	std::cout << cms_wrap.Changed() << std::endl;

	std::cout << cms_wrap.Changed() << std::endl;
	c_cmw.Set('b'); // same as c_cmw.Get() = 'b';
	std::cout << cms_wrap.Changed() << std::endl;



	std::cout << std::endl;

	// ChangeMonitorSummary, merge
	ChangeMonitorSummary cms(cms_link);
	cms.Add(cms_wrap);

	std::cout << cms.Changed() << std::endl;
	f = 3.3f;
	std::cout << cms.Changed() << std::endl;

	std::cout << cms.Changed() << std::endl;
	c_cmw.Set('c'); // same as c_cmw.Get() = 'c';
	std::cout << cms.Changed() << std::endl;



	// ChangeMonitorWrap, type conversion and assignment
	bool b_value = b_cmw;
	bool &b_reference = b_cmw;
	int i_value = b_cmw;
	const int &i_reference_const = b_cmw;

	b_cmw = true;
	b_cmw = 1;
}
