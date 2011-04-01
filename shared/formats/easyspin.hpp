
#ifndef _EASYSPIN_H_
#define _EASYSPIN_H_

#include <shared/spinsys.hpp>

namespace SpinXML {

	class EasySpinLoader : public ISpinSystemLoader {
	public:
		virtual FilterType GetFilterType() const {return SAVE;}
		virtual const char* GetFilter() const {return "m";}
		virtual const char* GetFormatName() const {return "Easyspin MATLAB file";}
		virtual void SaveFile(const SpinSystem* ss,const char* filename) const;
		virtual void LoadFile(SpinSystem* ss,      const char* filename) const {};
	};

}

#endif

