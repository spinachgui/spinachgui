
#ifndef PDB__HPP
#define PDB__HPP

#include <shared/spinsys.hpp>

namespace SpinXML {

    class PDBLoader : public ISpinSystemLoader {
    public:
        virtual FilterType GetFilterType() const {return LOAD;}
        virtual const char* GetFilter() const {return "pdb";}
        virtual const char* GetFormatName() const {return "Protien Databank file (*.pdb)";}
        virtual void SaveFile(const SpinSystem* ss,const char* filename) const {};
        virtual void LoadFile(SpinSystem* ss,      const char* filename) const;
    };
}

#endif
