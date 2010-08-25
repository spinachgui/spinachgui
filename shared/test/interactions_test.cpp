#define BOOST_TEST_MODULE raw_interactions
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <shared/test/common.hpp>
#include <shared/raw_interactions.hpp>

using namespace SpinXML;

struct Setup {
	Setup() :
		o(Quaterniond(sqrt(2.0),sqrt(2.0),0,0)),

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
		ss(ss_iso,span,skew,o)	{
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

#define CHECK_EIGENVALUES_CLOSE(obj)									\
	BOOST_CHECK_CLOSE(xx.si,(obj).xx.si ,0.01);							\
	BOOST_CHECK_CLOSE(yy.si,(obj).yy.si ,0.01);							\
	BOOST_CHECK_CLOSE(zz.si,(obj).zz.si ,0.01);						

#define CHECK_AXRHOM_CLOSE(obj)									\
	BOOST_CHECK_CLOSE(ar_iso.si,(obj).iso.si,0.01);					\
	BOOST_CHECK_CLOSE(ax.si,(obj).ax.si ,0.01);						\
	BOOST_CHECK_CLOSE(rh.si,(obj).rh.si ,0.01);						


#define CHECK_SPANSKEW_CLOSE(obj)										\
	BOOST_CHECK_CLOSE(ss_iso.si,(obj).iso.si ,0.01);					\
	BOOST_CHECK_CLOSE(span.si,(obj).span.si ,0.01);					\
	BOOST_CHECK_CLOSE(skew,(obj).skew ,0.01);						

#define CHECK_CONVERSION(name, seed, seedType, otherType, fromSeedConv, toSeedConv,checkMacro) \
	BOOST_FIXTURE_TEST_CASE( name, Setup ) {							\
	otherType tmp1 = fromSeedConv(seed);								\
	seedType  tmp2 = toSeedConv(tmp1);									\
																		\
	checkMacro(tmp2);													\
}
	/*for(int itor=0;itor<30;itor++) {									\
		tmp1 = fromSeedConv(NormalizeRotation(tmp2));					\
	    tmp2 = toSeedConv  (NormalizeRotation(tmp1));					\
		}																	\*/

#define CONVERSION_CHECK_ROUND(prefix,seed,seedType,toSeedConv,checkMacro) \
	CHECK_CONVERSION(prefix##MatrixStability     ,seed,seedType,Matrix3d   ,ConvertToMatrix     ,toSeedConv,checkMacro); \
	CHECK_CONVERSION(prefix##EigenStability      ,seed,seedType,Eigenvalues,ConvertToEigenvalues,toSeedConv,checkMacro); \
	CHECK_CONVERSION(prefix##AxRhomStability     ,seed,seedType,AxRhom     ,ConvertToAxRhom     ,toSeedConv,checkMacro); \
	CHECK_CONVERSION(prefix##SpanSkewStability   ,seed,seedType,SpanSkew   ,ConvertToSpanSkew   ,toSeedConv,checkMacro); \

CONVERSION_CHECK_ROUND(Matrix    ,m  ,Matrix3d   ,ConvertToMatrix     ,CHECK_MATRIX_CLOSE     );
CONVERSION_CHECK_ROUND(Eigen     ,ev ,Eigenvalues,ConvertToEigenvalues,CHECK_EIGENVALUES_CLOSE);
CONVERSION_CHECK_ROUND(AxRhom    ,ar ,AxRhom     ,ConvertToAxRhom     ,CHECK_AXRHOM_CLOSE     );
CONVERSION_CHECK_ROUND(SpanSkew  ,ss ,SpanSkew   ,ConvertToSpanSkew   ,CHECK_SPANSKEW_CLOSE   );

