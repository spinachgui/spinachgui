%module spinsys
%{
/* Put header files here or function declarations like below */
#include <shared/spinsys.hpp>
%}

%include "std_vector.i"
// STD Vector handling
namespace std {
   %template(SpinVector) vector<Spin*>;
}


%include "typemaps.i"

 //Assume all double* arguments are outputs
%apply double *OUTPUT {double *};


%include "shared/spinsys.hpp"
