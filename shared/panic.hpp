
#ifndef __PANIC__H__
#define __PANIC__H__

#include <string>
#include <Eigen/Dense>

///When a panic situation is detected this handler will be called.
void SetPanicHandler(bool (*handler)(const std::string& s));

void PANIC(const std::string& s);
void NaNPANIC(double n, const std::string& s);
void NaNPANIC(const Eigen::Vector3d& v, const std::string& s);
void NaNPANIC(const Eigen::Matrix3d& m, const std::string& s);


#endif
