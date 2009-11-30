
#ifndef _XYZ_H_
#define _XYZ_H_

#include <shared/spinsys.hpp>

namespace SpinXML {

  class XYZLoader : public ISpinSystemLoader {
  public:
    virtual FilterType GetFilterType() const {return LOADSAVE;}
    virtual const char* GetFilter() const {return "xyz";}
    virtual const char* GetFormatName() const {return "xyz file";}
    virtual void SaveFile(const SpinSystem* ss,const char* filename) const;
    virtual void LoadFile(SpinSystem* ss,      const char* filename) const;
  };

}

#endif

