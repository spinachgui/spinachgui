
#include <shared/spinsys_spec.hpp>

using namespace std;

class Spinsys  {
public:
  ///Default contructor. Creates an empty object
  Spinsys();
  ///Create an empty spin system
  void createNew();
  ///Load a spin system from an XML file located at filename
  void loadFromFile(const char* filename);
  ///Save the spin system to an XML file at filename
  void saveToFile(const char* filename);
  ///Output the spin system in a human readable format to the standard
  ///output for debugging purposes.
  void dump();
  ///Get a reference to a spin
  void getSpin();

  ///Attach a spin
  void addSpin();
protected:
  auto_ptr<Spin_system> mXMLSpinSys;
};
