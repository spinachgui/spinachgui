
#include <shared/assert.hpp>
#include <iostream>

using namespace std;
using namespace boost;

void defaultHandler(char const * expr, char const * function, char const * file, long line) {
	cout << file << "(" << line << ")::" << function << " Assert Failed in " << expr << endl;

	//TODO: Look put how to make a process send sigabrt to itself
    int* x = NULL;
    x++;
    x--;
    //Make use we use x with a side effect
    cout << (*x) << endl;
}

AssertHandler_t gAssertFailed = &defaultHandler;

void boost::assertion_failed(char const * expr, char const * function, char const * file, long line) {
	gAssertFailed(expr,function,file,line);
}
