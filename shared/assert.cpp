
#include <shared/assert.hpp>
#include <iostream>

#ifdef __linux__
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#endif

using namespace std;
using namespace boost;

void defaultHandler(char const * expr, char const * function, char const * file, long line) {
	cout << file << "(" << line << ")::" << function << " Assert Failed in " << expr << endl;

#ifdef __linux__
	cout << "Backtrace:" << endl;
	void* tracePtrs[100];
	int count = backtrace( tracePtrs, 100 );

	char** funcNames = backtrace_symbols( tracePtrs, count );

	// Print the stack trace
	for( int ii = 0; ii < count; ii++ )
		cout << funcNames[ii] << endl;;

	// Free the string pointers
	free( funcNames );
	abort();
#else
    int* x = NULL;
    x++;
    x--;
    //Make use we use x with a side effect
    cout << (*x) << endl;
#endif
}

AssertHandler_t gAssertFailed = &defaultHandler;

void boost::assertion_failed(char const * expr, char const * function, char const * file, long line) {
	gAssertFailed(expr,function,file,line);
}
