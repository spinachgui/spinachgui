#define BOOST_TEST_MODULE raw_interactions
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <shared/orientation.hpp>

using namespace SpinXML;

struct Setup {
    Setup() :
		alpha(0.2),
		beta (0.4),
		gamma(0.1),
		ea(alpha,beta,gamma),
		euler(ea),

		angle(PI/2),
		axis(Vector3d(1,0,0)),
		aa(angle,axis),
		angle_axis(aa),

		real(sqrt(2.0)),
		i(sqrt(2.0)),
		j(0),
		k(0),
		q(real,i,j,k),
		quat(q),

		m(MakeMatrix3d(1,0,0,
					   0,0,-1,
					   0,1,0)),
		matrix(m) { 
		BOOST_TEST_MESSAGE( "setup fixture" );
	}
    ~Setup() {
		BOOST_TEST_MESSAGE( "teardown fixture" );
	}
	double alpha, beta, gamma;
	EulerAngles ea;
	Orientation euler;

	double angle;
	Vector3d axis;
	AngleAxisd aa;
    Orientation angle_axis;

	double real,i,j,k;
	Quaterniond q;
    Orientation quat;

	Matrix3d m;
    Orientation matrix;
};



BOOST_AUTO_TEST_CASE( MakeMatrix3dTest ) {
	Matrix3d m = MakeMatrix3d(1,2,3,
							  4,5,6,
							  7,8,9);
	BOOST_CHECK_EQUAL(m(0,0),1);
	BOOST_CHECK_EQUAL(m(0,1),2);
	BOOST_CHECK_EQUAL(m(0,2),3);

	BOOST_CHECK_EQUAL(m(1,0),4);
	BOOST_CHECK_EQUAL(m(1,1),5);
	BOOST_CHECK_EQUAL(m(1,2),6);

	BOOST_CHECK_EQUAL(m(2,0),7);
	BOOST_CHECK_EQUAL(m(2,1),8);
	BOOST_CHECK_EQUAL(m(2,2),9);
}


BOOST_FIXTURE_TEST_CASE( EulerAnglesConstructor, Setup ) {
	//Check reading and writing to EulearAngle
	BOOST_CHECK_EQUAL(alpha,ea.alpha);
	BOOST_CHECK_EQUAL(beta, ea.beta);
	BOOST_CHECK_EQUAL(gamma,ea.gamma);
}

BOOST_FIXTURE_TEST_CASE( OrientationConstructorEuler, Setup ) {
	BOOST_CHECK_EQUAL(euler.GetType(),Orientation::EULER);
	double alpha2,beta2,gamma2;
	euler.GetEuler(&alpha2,&beta2,&gamma2);
	BOOST_CHECK_EQUAL(alpha,alpha2);
	BOOST_CHECK_EQUAL(beta, beta2);
	BOOST_CHECK_EQUAL(gamma,gamma2);
}


BOOST_FIXTURE_TEST_CASE( OrientationConstructorQuat, Setup ) {
	BOOST_CHECK_EQUAL(quat.GetType(),Orientation::QUATERNION);
	double real2,i2,j2,k2;
	quat.GetQuaternion(&real2,&i2,&j2,&k2);
	BOOST_CHECK_EQUAL(real,real2);
	BOOST_CHECK_EQUAL(i,   i2);
	BOOST_CHECK_EQUAL(j,   j2);
	BOOST_CHECK_EQUAL(k,   k2);
}

BOOST_FIXTURE_TEST_CASE( OrientationConstructorAngleAxis, Setup ) {
	BOOST_CHECK_EQUAL(quat.GetType(),Orientation::QUATERNION);
	double angle2;
	Vector3d axis2;
	angle_axis.GetAngleAxis(&angle2,&axis2);
	BOOST_CHECK_EQUAL(angle,angle2);
	BOOST_CHECK_EQUAL(axis ,axis2);
}

BOOST_FIXTURE_TEST_CASE( OrientationConstructorMatrix, Setup ) {
	BOOST_CHECK_EQUAL(matrix.GetType(),Orientation::DCM);
	Matrix3d m2;
	matrix.GetDCM(&m2);
	BOOST_CHECK_EQUAL(m,m2);	
}

#define CHECK_EULER_CLOSE(obj)					\
	BOOST_CHECK_CLOSE(alpha,(obj).alpha ,0.01);	\
	BOOST_CHECK_CLOSE(beta, (obj).beta  ,0.01);	\
	BOOST_CHECK_CLOSE(gamma,(obj).gamma ,0.01);	

#define CHECK_QUAT_CLOSE(obj)					\
	BOOST_CHECK_CLOSE(real,(obj).w(),0.01);		\
	BOOST_CHECK_CLOSE(i,   (obj).x(),0.01);	\
	BOOST_CHECK_CLOSE(j,   (obj).y(),0.01);	\
	BOOST_CHECK_CLOSE(k,   (obj).z(),0.01);	

#define CHECK_ANGLE_AXIS_CLOSE(obj)						\
	BOOST_CHECK_CLOSE(angle   ,(obj).angle()    ,0.01);	\
	BOOST_CHECK_CLOSE(axis.x(),(obj).axis().x() ,0.01);	\
	BOOST_CHECK_CLOSE(axis.y(),(obj).axis().y() ,0.01);	\
	BOOST_CHECK_CLOSE(axis.z(),(obj).axis().z() ,0.01);	

#define CHECK_MATRIX_CLOSE(obj)							\
	BOOST_CHECK_CLOSE(m(0,0),(obj)(0,0),0.01);			\
	BOOST_CHECK_CLOSE(m(0,1),(obj)(0,1),0.01);			\
	BOOST_CHECK_CLOSE(m(0,2),(obj)(0,2),0.01);			\
	BOOST_CHECK_CLOSE(m(1,0),(obj)(1,0),0.01);			\
	BOOST_CHECK_CLOSE(m(1,1),(obj)(1,1),0.01);			\
	BOOST_CHECK_CLOSE(m(1,2),(obj)(1,2),0.01);			\
	BOOST_CHECK_CLOSE(m(2,0),(obj)(2,0),0.01);			\
	BOOST_CHECK_CLOSE(m(2,1),(obj)(2,1),0.01);			\
	BOOST_CHECK_CLOSE(m(2,2),(obj)(2,2),0.01);			

#define CHECK_CONVERSION(name, seed, seedType, otherType, fromSeedConv, toSeedConv,checkMacro) \
	BOOST_FIXTURE_TEST_CASE( name, Setup ) {							\
	otherType tmp1 = fromSeedConv(seed);								\
	seedType  tmp2 = toSeedConv(tmp1);									\
																		\
	for(int itor=0;itor<30;itor++) {									\
		tmp1 = fromSeedConv(NormalizeRotation(tmp2));					\
	    tmp2 = toSeedConv  (NormalizeRotation(tmp1));					\
	}																	\
	checkMacro(tmp2);													\
}

#define CONVERSION_CHECK_ROUND(prefix,seed,seedType,toSeedConv,checkMacro) \
	CHECK_CONVERSION(prefix##EulerStability     ,seed,seedType,EulerAngles,ConvertToEuler     ,toSeedConv,checkMacro); \
	CHECK_CONVERSION(prefix##QuatStability      ,seed,seedType,Quaterniond,ConvertToQuaternion,toSeedConv,checkMacro); \
	CHECK_CONVERSION(prefix##AngleAxisStability ,seed,seedType,AngleAxisd ,ConvertToAngleAxis ,toSeedConv,checkMacro); \
	CHECK_CONVERSION(prefix##DCMStability       ,seed,seedType,Matrix3d   ,ConvertToDCM       ,toSeedConv,checkMacro); \

CONVERSION_CHECK_ROUND(Euler    ,ea,EulerAngles,ConvertToEuler     ,CHECK_EULER_CLOSE);
CONVERSION_CHECK_ROUND(Quat     ,q ,Quaterniond,ConvertToQuaternion,CHECK_QUAT_CLOSE);
CONVERSION_CHECK_ROUND(AngleAxis,aa,AngleAxisd ,ConvertToAngleAxis ,CHECK_ANGLE_AXIS_CLOSE);
CONVERSION_CHECK_ROUND(DCM      ,m, Matrix3d   ,ConvertToDCM       ,CHECK_MATRIX_CLOSE);
