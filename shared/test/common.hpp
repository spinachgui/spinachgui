#ifndef TEST_COMMON_H_
#define TEST_COMMON_H_

#include <boost/test/unit_test.hpp>
#include <iostream>
#include <cmath>
using namespace std;

namespace SpinXML {


	boost::test_tools::predicate_result
	boundedCompaire(double n1,double n2,double range,double tolarance)
	{
		if(fabs((n1-n2)/range) > (tolarance/100)) {
			boost::test_tools::predicate_result res(false);
			res.message() << n1 << " !~= " << n2;
			return res;
		}
		return true;
	}


#define CHECK_MATRIX_CLOSE(obj)								\
	BOOST_CHECK(boundedCompaire(m(0,0),(obj)(0,0),2,0.01));	\
	BOOST_CHECK(boundedCompaire(m(0,1),(obj)(0,1),2,0.01));	\
	BOOST_CHECK(boundedCompaire(m(0,2),(obj)(0,2),2,0.01));	\
	BOOST_CHECK(boundedCompaire(m(1,0),(obj)(1,0),2,0.01));	\
	BOOST_CHECK(boundedCompaire(m(1,1),(obj)(1,1),2,0.01));	\
	BOOST_CHECK(boundedCompaire(m(1,2),(obj)(1,2),2,0.01));	\
	BOOST_CHECK(boundedCompaire(m(2,0),(obj)(2,0),2,0.01));	\
	BOOST_CHECK(boundedCompaire(m(2,1),(obj)(2,1),2,0.01));	\
	BOOST_CHECK(boundedCompaire(m(2,2),(obj)(2,2),2,0.01));			

}
#endif
