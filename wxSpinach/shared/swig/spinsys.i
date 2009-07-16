 %module spinsys
 %{
 /* Put header files here or function declarations like below */
#include <shared/spinsys.hpp>
 %}

%typemap(out) Spin_system::SpinSequence&
{
  int j=0;
  $result = PyList_New($1->size());
  for (Spin_system::SpinConstIterator i=$1->begin();i != $1->end(); ++i) {
    PyObject* label;
    if(i->getLabel().present()) {
      label = PyString_FromString(i->getLabel().get().c_str());
    } else {
      label = PyString_FromString("");
    }
    PyObject* isotope=PyString_FromString(i->getIsotope().c_str());
    PyObject* number =PyLong_FromLong(i->getNumber());
    PyObject* spin=PyTuple_Pack(3,number,isotope,label);
    PyList_SetItem($result,j,spin);
    j++;
  }
}

class Spinsys {
public:
  Spinsys();
  ///Create an empty spin system
  void createNew();
  ///Load a spin system from an XML file located at filename
  void loadFromFile(const char* filename);
  ///Save the spin system to an XML file at filename
  void saveToFile(const char* filename);
  ///Output the spin system in a human readable format to the standard
  ///output for debugging purposes.
  void dump() const;
  ///Get a reference to a spin
  void addSpin();
  ///Get a list of references to spins
  Spin_system::SpinSequence& getSpins();
};
