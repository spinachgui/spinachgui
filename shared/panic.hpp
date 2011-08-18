
#ifndef __PANIC__H__
#define __PANIC__H__

#include <string>
#include <Eigen/Dense>

#define PANIC()            (PANIC_(__FILE__,__FUNCTION__,__LINE__))
#define MAYBEPANIC(b)      if(b){PANIC_(__FILE__,__FUNCTION__,__LINE__);}
#define PANICMSG(msg)      (PANIC_(__FILE__,__FUNCTION__,__LINE__,(msg)))
#define NaNPANIC(n)        (NaNPANIC_(n,__FILE__,__FUNCTION__,__LINE__))
#define NaNPANICMSG(n,msg) (NaNPANIC_(n,__FILE__,__FUNCTION__,__LINE__,(msg)))

///When a panic situation is detected this handler will be called.
void SetPanicHandler(bool (*handler)(const std::string& s));

void PANIC_(const std::string& file,
            const std::string& function,
            int line,
            const std::string& s="");
void NaNPANIC_(double n,
               const std::string& file,
               const std::string& function,
               int line,
               const std::string& s="");
void NaNPANIC_(const Eigen::Vector3d& v,
               const std::string& file,
               const std::string& function,
               int line,
               const std::string& s);
void NaNPANIC_(const Eigen::Matrix3d& m,
               const std::string& file,
               const std::string& function,
               int line,
               const std::string& s);


#endif
