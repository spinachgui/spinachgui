
#include <shared/unit.hpp>

double operator*(double number,const unit& u) {
	return u.toSI(number);
}

double operator/(double number,const unit& u) {
	return u.fromSI(number);
}
