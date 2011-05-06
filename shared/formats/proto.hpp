
#include <boost/optional.hpp>
#include <vector>
#include <string>
#include <shared/orientation.hpp>
#include <shared/spinsys.hpp>

//================================================================================//
//                                proto-structures                                //
//================================================================================//

namespace SpinXML {

	struct ProtoSpin {
		int number;

		double x;
		double y;
		double z;

		int element;
		int isotope;
		std::string label;

		int frame;
		void selfCheck() const;
	};

	struct ProtoInteraction {
		ProtoInteraction() : payload(0.0) {}
		int spin1;
		boost::optional<int> spin2;
    
		Interaction::Type type;
		InteractionPayload payload;

		int frame;
		std::string label;

		void selfCheck() const;
	};

	struct ProtoFrame {
		int number;
		int parent;
		std::string label;

		double x;
		double y;
		double z;

		Orientation o;

		void selfCheck() const;
	};
	void Assemble(SpinSystem* ss,
				  const std::vector<ProtoSpin>& spins,
				  const std::vector<ProtoInteraction>& interactions,
				  const std::vector<ProtoFrame>& frames);

};
