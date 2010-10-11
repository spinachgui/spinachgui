#define BOOST_TEST_MODULE spin
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <shared/test/common.hpp>
#include <shared/spin.hpp>
#include <iostream>

using namespace std;
using namespace SpinXML;

struct Setup {
    Setup() :
        x(1.0 * Angstroms),
        y(2.0 * Angstroms),
        z(-3.0 * Angstroms),
		label("Hello world"),
		element(5),
        s(new Spin(Vector3d(x.si,y.si,z.si),label,element)) {
    }
    ~Setup() {
        if(s != NULL) {
            delete s;
        }
    }
    length x,y,z;
	string label;
	long element;
    Spin* s;
};


BOOST_FIXTURE_TEST_CASE( SpinConstructor, Setup ) {
    length tx,ty,tz;
    s->GetCoordinates(&tx,&ty,&tz);
    BOOST_CHECK_EQUAL(tx.si,x.si);
    BOOST_CHECK_EQUAL(ty.si,y.si);
    BOOST_CHECK_EQUAL(tz.si,z.si);
	BOOST_CHECK_EQUAL(s->GetElement(),element);
	BOOST_CHECK_EQUAL(s->GetLabel(),label);
}


long signals_count = 0;
void changeHandler() {
	signals_count++;
}
Spin* deleted_spin=NULL;
void dieHandler(Spin* spin) {
	deleted_spin=spin;
}
BOOST_FIXTURE_TEST_CASE( Signals, Setup ) {
	s->sigChange.connect(sigc::ptr_fun(&changeHandler));
	s->sigDying.connect(sigc::ptr_fun(&dieHandler));

	s->SetPosition(Vector3d(1,2,3));
	BOOST_CHECK_EQUAL(signals_count,1);

	s->SetCoordinates(1.0*Angstroms,2.0*Angstroms,3.0*Angstroms);
	BOOST_CHECK_EQUAL(signals_count,2);

	s->SetLabel("Changed label");
	BOOST_CHECK_EQUAL(signals_count,3);

	s->SetElement(1);
	BOOST_CHECK_EQUAL(signals_count,4);

	delete s;
	BOOST_CHECK_EQUAL(deleted_spin,s);
	s=NULL;
}

