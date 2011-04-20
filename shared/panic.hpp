
#ifndef __PANIC__H__
#define __PANIC__H__

#include <string>
#include <Eigen/Dense>

void PANIC(const std::string& s);
void NaNPANIC(double n, const std::string& s);
void NaNPANIC(const Eigen::Vector3d& v, const std::string& s);
void NaNPANIC(const Eigen::Matrix3d& m, const std::string& s);


#endif
