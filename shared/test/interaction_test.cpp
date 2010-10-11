#define BOOST_TEST_MODULE raw_interactions
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <shared/test/common.hpp>
#include <shared/interaction.hpp>

using namespace SpinXML;

struct Setup {
    Setup() :
        o(Quaterniond(1/sqrt(2.0),1/sqrt(2.0),0,0)),

        scalar(20.0*MHz),
		
        m(MakeMatrix3d(1.0,0.0,0.0,
                       0.0,1.0,0.0,
                       0.0,0.0,1.0)),

        xx(10.0*MHz),
        yy(8.0*MHz),
        zz(4.0*MHz),
        ev(xx,yy,zz,o),

        ar_iso(20.0*MHz),
        ax(20.0*MHz),
        rh(15.0*MHz),
        ar(ar_iso,ax,rh,o),

        ss_iso(20.0*MHz),
        span(10.0*MHz),
        skew(0.5),
        ss(ss_iso,span,skew,o),
        
        spin1(new Spin(Vector3d(0,0,0),"Spin 1",1)),
        spin2(new Spin(Vector3d(1,1,1),"Spin 2",1)),

        storage_scalar(new Interaction(scalar,Interaction::DIPOLAR,    spin1,spin2)),
        storage_matrix(new Interaction(m     ,Interaction::SHIELDING  ,spin1)),
        storage_ev(    new Interaction(ev    ,Interaction::QUADRUPOLAR,spin1)),
        storage_ar(    new Interaction(ar,    Interaction::QUADRUPOLAR,spin1)),
        storage_ss(    new Interaction(ss,    Interaction::QUADRUPOLAR,spin1)) {
    }
    ~Setup() {
        delete spin1;
        delete spin2;

        delete storage_scalar;
        delete storage_matrix;
        delete storage_ev;
        delete storage_ar;
        delete storage_ss;
    }
    Orientation o;

    energy scalar;

    Matrix3d m;

    energy xx,yy,zz;
    Eigenvalues ev;

    energy ar_iso,ax,rh;
    AxRhom ar;

    energy ss_iso,span;
    double skew;
    SpanSkew ss;

    Spin* spin1;
    Spin* spin2;

    Interaction* storage_scalar;
    Interaction* storage_matrix;
    Interaction* storage_ev;
    Interaction* storage_ar;
    Interaction* storage_ss;
};

BOOST_FIXTURE_TEST_CASE( InteractionCtor, Setup) {
    BOOST_CHECK_EQUAL(storage_scalar->GetStorage(),Interaction::STORAGE_SCALAR);
    BOOST_CHECK_EQUAL(storage_matrix->GetStorage(),Interaction::MATRIX);
    BOOST_CHECK_EQUAL(storage_ev->GetStorage()    ,Interaction::EIGENVALUES);
    BOOST_CHECK_EQUAL(storage_ar->GetStorage()    ,Interaction::AXRHOM);
    BOOST_CHECK_EQUAL(storage_ss->GetStorage()   ,Interaction::SPANSKEW);
}


