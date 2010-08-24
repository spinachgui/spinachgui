#define BOOST_TEST_MODULE raw_interactions
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <shared/raw_interactions.hpp>

using namespace SpinXML;

struct Setup {
	Setup() :
		o(Quaterniond(sqrt(2.0),sqrt(2.0),0,0)),

		scalar(20.0*MHz),
		
		mat(MakeMatrix3d(1.0,0.0,0.0,
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
		ss(ss_iso,span,skew,o)	{
	}
	Orientation o;

	energy scalar;

	Matrix3d mat;

	energy xx,yy,zz;
	Eigenvalues ev;

	energy ar_iso,ax,rh;
	AxRhom ar;

	energy ss_iso,span;
	double skew;
	SpanSkew ss;
};

BOOST_FIXTURE_TEST_CASE( EigenvaluesConstructor, Setup) {
	BOOST_CHECK_EQUAL(xx.si,ev.xx.si);
	BOOST_CHECK_EQUAL(yy.si,ev.yy.si);
	BOOST_CHECK_EQUAL(zz.si,ev.zz.si);
	BOOST_CHECK(o==ev.mOrient);
}

BOOST_FIXTURE_TEST_CASE( AxRhomConstructor, Setup ) {
	BOOST_CHECK_EQUAL(ar_iso.si,ar.iso.si);
	BOOST_CHECK_EQUAL(ax.si, 	 ar.ax.si);
	BOOST_CHECK_EQUAL(rh.si, 	 ar.rh.si);
	BOOST_CHECK(o==ar.mOrient);
}

BOOST_FIXTURE_TEST_CASE( SpanSkewConstructor, Setup ) {
	BOOST_CHECK_EQUAL(ss_iso.si,ss.iso.si);
	BOOST_CHECK_EQUAL(span.si,	 ss.span.si);
	BOOST_CHECK_EQUAL(skew,  	 ss.skew);
	BOOST_CHECK(o==ss.mOrient);
}



