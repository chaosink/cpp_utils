#include "ScopedAssignment.hpp"
using namespace cpp_utils;

#include <iostream>
using namespace std;

int main() {
	int i = 1;
	float f = 1.1f;

	{
		ScopedAssignment<int> i_sa(i, 2); // Assign new values in ctor.
		ScopedAssignment<float> f_sa(f, 2.2f);

		cout << i << endl; // 2
		cout << f << endl; // 2.2
	} // Recover to the old values in detor.

	cout << endl;

	cout << i << endl; // 1
	cout << f << endl; // 1.1

	cout << endl;

	{ // move ctor
		ScopedAssignment<int> i_sa(i, 2);
		cout << i << endl; // 2
		{
			ScopedAssignment<int> i_sa_another(move(i_sa));
			cout << i << endl; // 2
		}
		cout << i << endl; // 1
	}

	cout << endl;

	{ // move assignment
		ScopedAssignment<int> i_sa(i, 2);
		cout << i << endl; // 2
		{
			ScopedAssignment<int> i_sa_another;
			i_sa_another = move(i_sa);
			cout << i << endl; // 2
		}
		cout << i << endl; // 1
	}
}
