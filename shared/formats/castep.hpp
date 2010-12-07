
#ifndef _CASTEP_H_
#define _CASTEP_H_

#include <shared/spinsys.hpp>

namespace SpinXML {

    class CASTEPLoader : public ISpinSystemLoader {
    public:
        virtual FilterType GetFilterType() const {return LOAD;}
        virtual const char* GetFilter() const {return "magres";}
        virtual const char* GetFormatName() const {return "CASTEP magres file (*.magres)";}
        virtual void SaveFile(const SpinSystem* ss,const char* filename) const {};
        virtual void LoadFile(SpinSystem* ss,      const char* filename) const;
    };
}

#endif
