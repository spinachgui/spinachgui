#define BOOST_TEST_MODULE raw_interactions
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <shared/orientation.hpp>
#include <iostream>

using namespace SpinXML;
using namespace std;

boost::test_tools::predicate_result
boundedCompaire(double n1,double n2,double range,double tolarance)
{
	if(abs((n1-n2)/range) > (tolarance/100)) {
		boost::test_tools::predicate_result res(false);
        res.message() << n1 << " !~= " << n2;
        return res;
	}
	return true;
}

//Quaternions have the same physical meaning if inverted (q1 == -q2) so
//we need to check both possibilities
boost::test_tools::predicate_result
quaternionCompaire(Quaterniond q1,Quaterniond q2,double tolarance)
{
	if(!( //Watch out for this not operation
		 (abs(q1.w()-q2.w())/2 < tolarance/100 &&   // is q1 == q2 ?
		  abs(q1.x()-q2.x())/2 < tolarance/100 &&
		  abs(q1.y()-q2.y())/2 < tolarance/100 &&
		  abs(q1.z()-q2.z())/2 < tolarance/100)
		 ||
		 (abs(q1.w()+q2.w())/2 < tolarance/100 &&   // is q1 == -q2 ?
		  abs(q1.x()+q2.x())/2 < tolarance/100 &&
		  abs(q1.y()+q2.y())/2 < tolarance/100 &&
		  abs(q1.z()+q2.z())/2 < tolarance/100)
		 )) {
		boost::test_tools::predicate_result res(false);
        res.message() << "[" << q1.w() << "," << q1.x() << "," << q1.y() << "," << q1.z() << "]" << " !~= "
					  << "[" << q2.w() << "," << q2.x() << "," << q2.y() << "," << q2.z() << "]"; 
        return res;
	}
	return true;
}

//Two angle axis rotations have the same meaning if they are the same
//or if you can flip the vector and set the new angle to 2*pi minus to
//old angle.
boost::test_tools::predicate_result
quaternionCompaire(AngleAxisd a1,AngleAxisd a2,double tolarance)
{
	if(!( //Watch out for this not operation
		 (abs(a1.angle()-a2.angle())/(2*PI) < tolarance/100 &&   // is a1 == a2 ?
		  abs(a1.axis().x()-a2.axis().x())/2 < tolarance/100 &&
		  abs(a1.axis().y()-a2.axis().y())/2 < tolarance/100 &&
		  abs(a1.axis().z()-a2.axis().z())/2 < tolarance/100)
		 ||
		 (abs(a1.angle()-(2*PI-a2.angle()))/(2*PI) < tolarance/100 &&   // is a1 == inverted a2 ?
		  abs(a1.axis().x()+a2.axis().x())/2 < tolarance/100 &&
		  abs(a1.axis().y()+a2.axis().y())/2 < tolarance/100 &&
		  abs(a1.axis().z()+a2.axis().z())/2 < tolarance/100)
		 )) {
		boost::test_tools::predicate_result res(false);
        res.message() <<   a1.angle() << ",[" << a1.axis().x() << "," << a1.axis().y() << "," << a1.axis().z() << "]" << " !~= "
					  <<   a2.angle() << ",[" << a2.axis().x() << "," << a2.axis().y() << "," << a2.axis().z() << "]"; 
        return res;
	}
	return true;
}



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

		real(1.0/sqrt(2.0)),
		i(1.0/sqrt(2.0)),
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

#define CHECK_EULER_CLOSE(obj)									\
	BOOST_CHECK(boundedCompaire(alpha,(obj).alpha ,2*PI,0.01));	\
	BOOST_CHECK(boundedCompaire(beta, (obj).beta  ,PI,  0.01));	\
	BOOST_CHECK(boundedCompaire(gamma,(obj).gamma ,2*PI,0.01));	

#define CHECK_QUAT_CLOSE(obj)					\
	BOOST_CHECK(quaternionCompaire(q,(obj),0.01));

#define CHECK_ANGLE_AXIS_CLOSE(obj)						\
	quaternionCompaire(aa,obj,0.01)

#define CHECK_MATRIX_CLOSE(obj)							\
	BOOST_CHECK(boundedCompaire(m(0,0),(obj)(0,0),2,0.01));			\
	BOOST_CHECK(boundedCompaire(m(0,1),(obj)(0,1),2,0.01));			\
	BOOST_CHECK(boundedCompaire(m(0,2),(obj)(0,2),2,0.01));			\
	BOOST_CHECK(boundedCompaire(m(1,0),(obj)(1,0),2,0.01));			\
	BOOST_CHECK(boundedCompaire(m(1,1),(obj)(1,1),2,0.01));			\
	BOOST_CHECK(boundedCompaire(m(1,2),(obj)(1,2),2,0.01));			\
	BOOST_CHECK(boundedCompaire(m(2,0),(obj)(2,0),2,0.01));			\
	BOOST_CHECK(boundedCompaire(m(2,1),(obj)(2,1),2,0.01));			\
	BOOST_CHECK(boundedCompaire(m(2,2),(obj)(2,2),2,0.01));			

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
	CHECK_CONVERSION(prefix##EulerStability     ,seed,seedType,EulerAngles,ConvertToEuler     ,toSeedConv,checkMacro); \
	CHECK_CONVERSION(prefix##QuatStability      ,seed,seedType,Quaterniond,ConvertToQuaternion,toSeedConv,checkMacro); \
	CHECK_CONVERSION(prefix##AngleAxisStability ,seed,seedType,AngleAxisd ,ConvertToAngleAxis ,toSeedConv,checkMacro); \
	CHECK_CONVERSION(prefix##DCMStability       ,seed,seedType,Matrix3d   ,ConvertToDCM       ,toSeedConv,checkMacro); \

CONVERSION_CHECK_ROUND(Euler    ,ea,EulerAngles,ConvertToEuler     ,CHECK_EULER_CLOSE);
CONVERSION_CHECK_ROUND(Quat     ,q ,Quaterniond,ConvertToQuaternion,CHECK_QUAT_CLOSE);
CONVERSION_CHECK_ROUND(AngleAxis,aa,AngleAxisd ,ConvertToAngleAxis ,CHECK_ANGLE_AXIS_CLOSE);
CONVERSION_CHECK_ROUND(DCM      ,m, Matrix3d   ,ConvertToDCM       ,CHECK_MATRIX_CLOSE);
