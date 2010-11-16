
#ifndef BASE_HPP
#define BASE_HPP

#include <shared/frame.hpp>

namespace SpinXML {

	template<typename Derived,typename DerivedView>
	class SpinXMLBase  : public sigc::trackable {
	public:

		~SpinXMLBase() {
			sigDying(static_cast<Derived*>(this));
		}

		DerivedView GetView(const Frame* frame, const UnitSystem* unitSystem) {
			return DerivedView(static_cast<Derived*>(this),frame,unitSystem);
		}

		sigc::signal<void,Derived*> sigDying;
        sigc::signal<void> sigChange;
	};

};
#endif
