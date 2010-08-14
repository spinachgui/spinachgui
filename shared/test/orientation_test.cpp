#define BOOST_TEST_MODULE raw_interactions
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <shared/mathtypes.hpp>

using namespace SpinXML;

BOOST_AUTO_TEST_CASE( TesterTest ) {
	double alpha = 0.2;
	double beta = 0.4;
	double gamma = 0.1;
	EulerAngles ea(alpha,beta,gamma);

	//Check reading and writing to EulearAngle
	BOOST_CHECK_EQUAL(alpha,ea.alpha);
	BOOST_CHECK_EQUAL(beta,ea.beta);
	BOOST_CHECK_EQUAL(gamma,ea.gamma);

	//Check creation of an Orientation with Euler Angles
    Orientation euler(ea);
	BOOST_CHECK_EQUAL(euler.GetType(),Orientation::EULER);
	double alpha2,beta2,gamma2;
	euler.GetEuler(&alpha2,&beta2,&gamma2);
	BOOST_CHECK_EQUAL(alpha,alpha2);
	BOOST_CHECK_EQUAL(beta, beta2);
	BOOST_CHECK_EQUAL(gamma,gamma2);

    Orientation angle_axis(AngleAxisd(0.0,Vector3d(1.0,0.0,0.0)));
    Orientation quat(Quaterniond(1.0,0.0,0.0,0.0));
    Orientation matrix(Matrix3d::Identity());

	

	//Check reads and writes
    BOOST_CHECK(true);

	//Check Euler to Angle Axis

	//Check Euler to Quaternion

	//Check Euler to Matrix

	//Check Angle Axis to Quaternion

	//Check Angle Axis to Matrix

	//Check Quaternion to Matrix

}


