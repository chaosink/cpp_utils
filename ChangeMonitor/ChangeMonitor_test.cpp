#include "ChangeMonitor.hpp"
using namespace cpp_utils;

#include <iostream>
using namespace std;

struct A {
	short s;
	void *p;

	A() = delete; // default ctor is deleted
	A(short s, void *p): s(s), p(p) {}
	A(const A &a): s(a.s), p(a.p) {}

	bool operator!=(const A &a) { // operator "!=" is needed
		return s != a.s || p != a.p;
	}
};

int main() {
	// ChangeMonitorReferenceIndividual
	int i = 1;
	float f = 1.1f;

	ChangeMonitorReferenceIndividual<int> i_cmri(i);
	ChangeMonitorReferenceIndividual<float> f_cmri(f);

	ChangeMonitorSummary cms_cmri;
	cms_cmri.Insert(i_cmri);
	cms_cmri.Insert(f_cmri);

	ChangeMonitorReferenceIndividual<float> f_cmri_another(f);

	cout << cms_cmri.Changed(); // 0
	i = 2;
	cout << cms_cmri.Changed(); // 1
	cout << cms_cmri.Changed(); // 0
	f = 2.2f;
	cout << cms_cmri.Changed(); // 1
	cout << cms_cmri.Changed(); // 0
	cout << f_cmri_another.Changed(); // 1

	cout << endl;



	// ChangeMonitorReferenceShared
	long l = 1;
	double d = 1.1;

	ChangeMonitorReferenceShared<long> l_cmrs(l);
	ChangeMonitorReferenceShared<double> d_cmrs(d);

	ChangeMonitorSummary cms_cmrs;
	cms_cmrs.Insert(l_cmrs);
	cms_cmrs.Insert(d_cmrs);

	ChangeMonitorReferenceShared<double> d_cmrs_another(d);

	cout << cms_cmrs.Changed(); // 0
	l = 2;
	cout << cms_cmrs.Changed(); // 1
	cout << cms_cmrs.Changed(); // 0
	d = 2.2;
	cout << cms_cmrs.Changed(); // 1
	cout << cms_cmrs.Changed(); // 0
	cout << d_cmrs_another.Changed(); // 0

	cout << endl;



	// ChangeMonitorInstance
	bool b = false;
	char c = 'a';

	ChangeMonitorInstance<bool> b_cmi(false); // same as b_cmi(b);
	ChangeMonitorInstance<char> c_cmi('a');   // same as c_cmi(c);

	ChangeMonitorSummary cms_cmi;
	cms_cmi.Insert(b_cmi);
	cms_cmi.Insert(c_cmi);

	cout << cms_cmi.Changed(); // 0
	b_cmi = true;
	cout << cms_cmi.Changed(); // 1
	cout << cms_cmi.Changed(); // 0
	c_cmi = 'b';
	cout << cms_cmi.Changed(); // 1
	cout << cms_cmi.Changed(); // 0

	cout << endl;



	// ChangeMonitorSummary
	ChangeMonitorSummary cms(cms_cmi); // copy ctor
	ChangeMonitorInstance<A> a_cmi(A(2, nullptr));
	cms.Insert(a_cmi);
	cms.Merge(cms_cmri);
	cms.Merge(cms_cmrs);

	cout << cms.Changed(); // 0
	float &ff = f; // type conversion
	ff = 3.3f;
	cout << cms.Changed(); // 1
	cout << cms.Changed(); // 0
	double &dd = d; // type conversion
	dd = 3.3;
	cout << cms.Changed(); // 1
	cout << cms.Changed(); // 0
	char &cc = c_cmi; // type conversion
	cc = 'c';
	cout << cms.Changed(); // 1
	cout << cms.Changed(); // 0
	a_cmi().s = 3; // use operator() to get the reference,
	// for the lack of the dot operator to access members
	cout << cms.Changed(); // 1
	cout << cms.Changed(); // 0

	cout << endl;
}
