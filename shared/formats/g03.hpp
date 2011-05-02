
#ifndef _G03_H_
#define _G03_H_

#include <shared/spinsys.hpp>

namespace SpinXML {

  class G03Loader : public ISpinSystemLoader {
  public:
    virtual FilterType GetFilterType() const {return LOAD;}
    virtual const char* GetFilter() const {return "log";}
    virtual const char* GetFormatName() const {return "Gaussian 03 file";}
    virtual void SaveFile(const SpinSystem* ss,const char* filename) const {};
    virtual void LoadFile(SpinSystem* ss,      const char* filename) const;
  };

}

//Private

class __G03Init  {
public:
    __G03Init();
    ~__G03Init();
};

#endif

