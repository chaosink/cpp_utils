#include "ChangeMonitor.hpp"
using namespace cpp_utils;

#include <iostream>
using namespace std;

int main() {
// Wrapper
	{
		string s = "hello";
		Wrapper<string> s_w; // Constructor without wrapping s.
		// s_w = "world"; // Error: assignment before wrapping.
		// cout << string(s_w) << endl; // Error: access before wrapping.
		s_w.Wrap(s);
		cout << string(s_w) << endl; // hello
		s_w = "world";
		cout << string(s_w) << endl; // world
	}

	{
		string s0 = "hello";
		Wrapper<string> s_w(s0); // Constructor with binding to s.
		cout << string(s_w) << endl; // hello
		s_w = "world";
		cout << string(s_w) << endl; // world

		string s1 = "foo";
		s_w.Wrap(s1); // Change wrapping target.
		cout << string(s_w) << endl; // foo
		s_w = "bar";
		cout << string(s_w) << endl; // bar
	}

/*--------------------------------------------------*/

// ChangeMonitorReferenceIndividual
	int i = 1;
	float f = 1.1f;

	ChangeMonitorReferenceIndividual<int> i_cmri(i); // A monitor on i.
	ChangeMonitorReferenceIndividual<float> f_cmri(f); // A monitor on f.
									//             \_
	ChangeMonitorSummary cms_cmri;	//              \_
	cms_cmri.Insert(i_cmri);		//               \_
	cms_cmri.Insert(f_cmri);		//                \_
			 						//                 \_
	// Another monitor on f, and a different state from f_cmri's.
	ChangeMonitorReferenceIndividual<float> f_cmri_another(f);

	cout << cms_cmri.Changed(); // 0
	i = 2;
	cout << cms_cmri.Changed(); // 1
	cout << cms_cmri.Changed(); // 0
	f = 2.2f;
	// f_cmri's state is "changed".
	cout << cms_cmri.Changed(); // 1
	// f_cmri's state is "unchanged".
	cout << cms_cmri.Changed(); // 0
	// f_cmri_another's state is still "changed",
	// because its state is independent from f_cmri's.
	cout << f_cmri_another.Changed(); // 1

	cout << endl;

/*--------------------------------------------------*/

// ChangeMonitorReferenceShared
	long l = 1;
	double d = 1.1;

	ChangeMonitorReferenceShared<long> l_cmrs(l); // A monitor on l.
	ChangeMonitorReferenceShared<double> d_cmrs(d); // A monitor on d.
									//          \_
	ChangeMonitorSummary cms_cmrs;	//           \_
	cms_cmrs.Insert(l_cmrs);		//            \_
	cms_cmrs.Insert(d_cmrs);		//             \_
									//              \_
	// Another monitor on d, but the same state with d_cmrs's.
	ChangeMonitorReferenceShared<double> d_cmrs_another(d);

	cout << cms_cmrs.Changed(); // 0
	l = 2;
	cout << cms_cmrs.Changed(); // 1
	cout << cms_cmrs.Changed(); // 0
	d = 2.2;
	// d_cmrs's state is changed.
	cout << cms_cmrs.Changed(); // 1
	// d_cmrs's state is unchanged.
	cout << cms_cmrs.Changed(); // 0
	// d_cmrs_another's state is also unchanged,
	// because d_cmrs's state (the shared state) has already been queried.
	cout << d_cmrs_another.Changed(); // 0
	cout << endl;

/*--------------------------------------------------*/

// ChangeMonitorInstance
	[[maybe_unused]] char c = 'a';
	[[maybe_unused]] string s = "hello";
	[[maybe_unused]] vector<int> vi = {1, 2, 3};

	ChangeMonitorInstance<char> c_cmi('a');   // Same as c_cmi(c).
	ChangeMonitorInstance<string> s_cmi("hello"); // Same as s_cmi(s).
	ChangeMonitorInstance<vector<int>> vi_cmi({1, 2, 3}); // Same as vi_cmi(vi).

	ChangeMonitorSummary cms_cmi;
	cms_cmi.Insert(c_cmi);
	cms_cmi.Insert(s_cmi);
	cms_cmi.Insert(vi_cmi);

	cout << cms_cmi.Changed(); // 0
	c_cmi = 'b'; // "operator=" overloaded to modify the value.
	cout << cms_cmi.Changed(); // 1
	cout << cms_cmi.Changed(); // 0
	s_cmi = "world";
	cout << cms_cmi.Changed(); // 1
	cout << cms_cmi.Changed(); // 0
	vi_cmi().push_back(4);
	cout << cms_cmi.Changed(); // 1
	cout << cms_cmi.Changed(); // 0

	cout << endl;

/*--------------------------------------------------*/

// ChangeMonitorSummary
	ChangeMonitorSummary cms(cms_cmi); // copy ctor
	cms.Merge(cms_cmri); // Merge previous ChangeMonitorSummarys in.
	cms.Merge(cms_cmrs);

	struct A {
		short s;
		void *p;

		A() = delete; // Default ctor is deleted.
		A(short s, void *p): s(s), p(p) {}
		A(const A &a): s(a.s), p(a.p) {}

		bool operator!=(const A &a) { // Operator "!=" is needed.
			return s != a.s || p != a.p;
		}
	};

	ChangeMonitorInstance<A> a_cmi(A(2, nullptr));
	cms.Insert(a_cmi); // Add a new ChangeMonitor.

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
	a_cmi().s = 3; // Use operator() to get the reference,
				   // for the lack of the dot operator to access members.
	cout << cms.Changed(); // 1
	cout << cms.Changed(); // 0

	cout << endl;
}
