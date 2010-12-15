
#ifndef BASE_HPP
#define BASE_HPP

#include <shared/frame.hpp>

namespace SpinXML {

	template<typename Derived,typename DerivedView>
	class SpinXMLBase  : public sigc::trackable {
	public:
		SpinXMLBase() : mFrame(NULL) {
		}
		~SpinXMLBase() {
			sigDying(static_cast<Derived*>(this));
		}

		DerivedView GetView(const Frame* frame, const UnitSystem* unitSystem) {
			return DerivedView(static_cast<Derived*>(this),frame,unitSystem);
		}

		Frame* GetPreferedFrame() {return mFrame;}
		void   SetPreferedFrame(Frame* frame) {mFrame = frame;}

		sigc::signal<void,Derived*> sigDying;
        sigc::signal<void> sigChange;
	private:
		Frame* mFrame;
	};

};
#endif
