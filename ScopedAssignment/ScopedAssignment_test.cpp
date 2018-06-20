#include "ScopedAssignment.hpp"
using namespace cpp_utils;

#include <iostream>
using namespace std;

int main() {
	int i = 1;
	float f = 1.1f;

	{
		ScopedAssignment<int> i_sa(i, 2);
		ScopedAssignment<float> f_sa(f, 2.2f);

		cout << i << endl;
		cout << f << endl;
	}

	cout << endl;

	cout << i << endl;
	cout << f << endl;
}
