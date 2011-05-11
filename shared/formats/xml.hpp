
#ifndef _SPINXML_H_
#define _SPINXML_H_

#include <shared/spinsys.hpp>

namespace SpinXML {

  class XMLLoader : public ISpinSystemLoader {
  public:
    virtual FilterType GetFilterType() const {return LOADSAVE;}
    virtual const char* GetFilter() const {return "xml";}
    virtual const char* GetFormatName() const {return "SpinXML file";}
    virtual void SaveFile(const SpinSystem* ss,const char* filename) const;
    virtual void LoadFile(SpinSystem* ss,      const char* filename) const;
  };
}

#endif

