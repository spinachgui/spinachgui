
#include <shared/panic.hpp>
#define BOOST_ENABLE_ASSERT_HANDLER
#include <boost/assert.hpp>
#include <iostream>

#ifdef __linux__
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#endif


using namespace std;
using namespace boost;

typedef void (*AssertHandler_t)(char const * expr, char const * function, char const * file, long line);

bool (*gPanicHandler)(const string& s) = NULL;

void SetPanicHandler(bool (*handler)(const string& s)) {
    gPanicHandler = handler;
}

void PANIC_(const std::string& file,
            const std::string& function,
            int line,
            const std::string& s) {
    bool abandon = false;
    if(gPanicHandler != NULL) {
        abandon = gPanicHandler(s);
    }
    BOOST_ASSERT(false);
}

void NaNPANIC_(double n,
               const std::string& file,
               const std::string& function,
               int line,
               const std::string& s) {
    if(n != n) {
        PANIC_(file,function,line,s);
    }
}

void NaNPANIC_(const Eigen::Vector3d& v,
               const std::string& file,
               const std::string& function,
               int line,
               const std::string& s) {
    NaNPANIC_(v.x(),file,function,line,s);
    NaNPANIC_(v.y(),file,function,line,s);
    NaNPANIC_(v.z(),file,function,line,s);
}
void NaNPANIC_(const Eigen::Matrix3d& m,
               const std::string& file,
               const std::string& function,
               int line,
               const std::string& s) {
    NaNPANIC_(m(0,0),file,function,line,s);
    NaNPANIC_(m(0,1),file,function,line,s);
    NaNPANIC_(m(0,2),file,function,line,s);

    NaNPANIC_(m(1,0),file,function,line,s);
    NaNPANIC_(m(1,1),file,function,line,s);
    NaNPANIC_(m(1,2),file,function,line,s);

    NaNPANIC_(m(2,0),file,function,line,s);
    NaNPANIC_(m(2,1),file,function,line,s);
    NaNPANIC_(m(2,2),file,function,line,s);
}




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
