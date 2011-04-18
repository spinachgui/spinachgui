
#ifndef BASE_HPP
#define BASE_HPP

#include <shared/frame.hpp>

namespace SpinXML {

	template<typename Derived,typename DerivedView>
	class SpinXMLBase  : public sigc::trackable {
	public:
		SpinXMLBase() : mFrame(NULL) {
			sigChange.connect(mem_fun(this,&SpinXMLBase::relayToAnyChange));
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

		static sigc::signal<void> sigAnyChange;
	private:
		void relayToAnyChange() {
			sigAnyChange();
		}
		Frame* mFrame;
	};

	template<typename Derived,typename DerivedView>
	sigc::signal<void> SpinXMLBase<Derived,DerivedView>::sigAnyChange;
};

#endif
