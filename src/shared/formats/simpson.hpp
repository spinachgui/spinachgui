
#ifndef _SIMPSON_H_
#define _SIMPSON_H_

#include <shared/spinsys.hpp>

namespace SpinXML {

  class SIMPSONLoader : public ISpinSystemLoader {
  public:
    SIMPSONLoader() {};
    virtual FilterType GetFilterType() const {return SAVE;}
    virtual const char* GetFilter() const {return "in";}
    virtual const char* GetFormatName() const {return "SIMPSON input file";}
    virtual void SaveFile(const SpinSystem* ss,const char* filename) const;
      virtual void LoadFile(SpinSystem* ss,      const char* filename) const {};
  };
}

#endif

