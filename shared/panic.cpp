
#include <shared/panic.hpp>

#include <iostream>

using namespace std;

void PANIC(const string& s) {
	cout << "Panicking, " << s << endl;
	int* x = NULL;
	x++;
	x--;
	//Make use we use x with a side effect
	cout << (*x) << endl;
}

void NaNPANIC(double n, const std::string& s) {
	if(n != n) {
		PANIC(s);
	}
}
