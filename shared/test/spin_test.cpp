#define BOOST_TEST_MODULE spin
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <shared/spin.hpp>
#include <iostream>

using namespace SpinXML;

struct Setup {
    Setup() :
        x(1.0 * Angstroms),
        y(2.0 * Angstroms),
        z(-3.0 * Angstroms),
        s(new Spin(Vector3d(x.si,y.si,z.si),"Hello World",5)) {
    }
    ~Setup() {
        if(s != NULL) {
            delete s;
        }
    }
    length x,y,z;
    Spin* s;
};


BOOST_FIXTURE_TEST_CASE( SpinConstructor, Setup ) {
    length tx,ty,tz;
    s->GetCoordinates(&tx,&ty,&tz);
    BOOST_CHECK_EQUAL(tx.si,x.si);
    BOOST_CHECK_EQUAL(ty.si,y.si);
    BOOST_CHECK_EQUAL(tz.si,z.si);
}



