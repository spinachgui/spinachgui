

%module spinsys
%{
/* Put header files here or function declarations like below */
#include <shared/spinsys.hpp>
%}



%include "std_vector.i"
%include "stl.i"


// STD Vector handling
namespace std {
  %template(SpinVector) vector<SpinXML::Spin*>;
  %template(InteractionVector) vector<SpinXML::Interaction*>;
}


/* Set the input argument to point to a temporary variable */

%typemap(in,numinputs=0) SpinXML::Matrix3 *OutMatrix(SpinXML::Matrix3 *OutMatrix) {
  //SWIG_ConvertPtr($input, (void **) &$1, SWIGTYPE_p_Matrix3, SWIG_POINTER_EXCEPTION);
  $1=new SpinXML::Matrix3();
}
%typemap(argout) Matrix3 *OutMatrix {
  $result = SWIG_NewPointerObj($1, $1_descriptor, SWIG_POINTER_OWN);
} 



%typemap(in,numinputs=0) SpinXML::Orientation** OrientOut(SpinXML::Orientation** OrientOut) {
  $1=new SpinXML::Orientation*;
}
%typemap(argout) SpinXML::Orientation** OrientOut {
  PyObject* orient=SWIG_NewPointerObj(*$1, SWIGTYPE_p_SpinXML__Orientation, 0);
  delete $1;
  PyObject* orientList = PyList_New(1);
  PyList_SetItem(orientList,0,orient);
  PySequence_InPlaceConcat($result,orientList);
} 



%include "typemaps.i"


//Assume all double* arguments are outputs
%apply double *OUTPUT {double *};


//%ignore GetMatrix;

%include "shared/spinsys.hpp"

