%module spinsys
%{
/* Put header files here or function declarations like below */
#include <shared/spinsys.hpp>
%}

%include "std_vector.i"
// STD Vector handling
namespace std {
   %template(SpinVector) vector<Spin*>;
   %template(InteractionVector) vector<Interaction*>;
}

/* Set the input argument to point to a temporary variable */

%typemap(in,numinputs=0) Matrix3 *OutMatrix(Matrix3 *OutMatrix) {
  //SWIG_ConvertPtr($input, (void **) &$1, SWIGTYPE_p_Matrix3, SWIG_POINTER_EXCEPTION);
  $1=new Matrix3();
}
%typemap(argout) Matrix3 *OutMatrix {
  $result = SWIG_NewPointerObj($1, SWIGTYPE_p_Matrix3, SWIG_POINTER_OWN);
} 


%typemap(in,numinputs=0) Orientation* OrientOut(Orientation* OrientOut) {
  //SWIG_ConvertPtr($input, (void **) &$1, SWIGTYPE_p_Matrix3, SWIG_POINTER_EXCEPTION);
  $1=new Orientation();
}
%typemap(argout) Orientation* OrientOut {
  $result = SWIG_NewPointerObj($1, SWIGTYPE_p_Orientation, SWIG_POINTER_OWN);
} 



%include "typemaps.i"


//Assume all double* arguments are outputs
%apply double *OUTPUT {double *};


//%ignore GetMatrix;

%include "shared/spinsys.hpp"
