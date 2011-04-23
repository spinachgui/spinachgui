
#include <shared/unit.hpp>

using namespace Eigen;

double operator*(double number,const unit& u) {
    return u.toSI(number);
}

double operator/(double number,const unit& u) {
    return u.fromSI(number);
}

Vector3d operator*(const Vector3d& v,const unit& u) {
    return v*u.get_to_si();
}
Vector3d operator/(const Vector3d& v,const unit& u) {
    return v*u.get_from_si();
}

Matrix3d operator*(const Matrix3d& m,const unit& u) {
    return m*u.get_to_si();
}
Matrix3d operator/(const Matrix3d& m,const unit& u) {
    return m*u.get_from_si();
}
