
#include <shared/formats/proto.hpp>
#include <shared/foreach.hpp>
#include <set>
#include <map>
#include <Eigen/Dense>

using namespace SpinXML;
using namespace std;
using namespace Eigen;



void ProtoSpin::selfCheck() const {
	//Check for NAN
	if(x != x) {
		throw runtime_error("x was NaN");
	}
	if(y != y) {
		throw runtime_error("y was NaN");
	}
	if(z != z) {
		throw runtime_error("z was NaN");
	}
        
	if(element < 0) {
		throw runtime_error("Spin cannot have a negative number of protons");
	}
	if(isotope < element) {
		stringstream ss;
		ss << "Spin cannot have a negative number of neutrons (spin had mass number " << isotope
		   << " and atomic number " << element;
		throw runtime_error(ss.str());
	}
}


void ProtoInteraction::selfCheck() const {
	switch(type) {
	case Interaction::HFC:
	case Interaction::EXCHANGE:
	case Interaction::CUSTOM_BILINEAR:
	case Interaction::DIPOLAR:
	case Interaction::SCALAR:
		if(!spin2) {
			throw runtime_error("Only one spin specified for a two spin interaction");
		}
		break;
	case Interaction::CUSTOM_LINEAR:
	case Interaction::CUSTOM_QUADRATIC:
	case Interaction::QUADRUPOLAR:
	case Interaction::SHIELDING:
	case Interaction::ZFS:
	case Interaction::G_TENSER:
		if(spin2) {
			throw runtime_error("Two spins specified for a one spin interaction");
		}
		break;
	default:
		//This should never happen because we should have already
		//caught this. This is just some defense in depth.
		throw logic_error("Internal error, unknown Interaction::Type encountered in ProtoInteraction::selfCheck");
	}
}


void ProtoFrame::selfCheck() const {
	if(x != x) {
		throw runtime_error("x was NaN");
	}
	if(y != y) {
		throw runtime_error("y was NaN");
	}
	if(z != z) {
		throw runtime_error("z was NaN");
	}
	if(number == 0) {
		throw runtime_error("reference_frame 0 is the implicit molecular frame but was explicitly specifed");
	}
}


void SpinXML::Assemble(SpinSystem* ss,
              const vector<ProtoSpin>& spins,
              const vector<ProtoInteraction>& interactions,
              const vector<ProtoFrame>& frames) {

    //Assemble a spin system from the "parts" provided by the
    //context free step

    //Step 1, every proto-object should run it's own self checks
    foreach(ProtoSpin spin,spins) {
        spin.selfCheck();
    }
    foreach(ProtoInteraction inter,interactions) {
        inter.selfCheck();
    }
    foreach(ProtoFrame frame,frames) {
        frame.selfCheck();
    }


    //Step 2, check that the number attribute in every protoSpin
    //and protoFrame is unique
    set<int> spinNumbers;
    foreach(ProtoSpin spin,spins) {
        if(spinNumbers.find(spin.number) != spinNumbers.end()) {
            throw runtime_error("Two spins contained duplicate numbers");
        }
        cout << "Inserted " << spin.number << endl;
        spinNumbers.insert(spin.number);
    }
    set<int> frameNumbers;
    foreach(ProtoFrame frame,frames) {
        if(frameNumbers.find(frame.number) != frameNumbers.end()) {
            throw runtime_error("Two reference frames contained duplicate numbers");
        }
        frameNumbers.insert(frame.number);
    }

    //Step 3, The frame number in each spin should point to a
    //valid frame

    foreach(ProtoSpin spin,spins) {
        if(spin.frame != 0 && frameNumbers.find(spin.frame) == frameNumbers.end()) {
            throw runtime_error("Spin refers to a missing reference frame");
        }
    }

    //Step 4, Dito the interaction

    foreach(ProtoInteraction interaction,interactions) {
        if(interaction.frame !=0 && frameNumbers.find(interaction.frame) == frameNumbers.end()) {
            throw runtime_error("Interaction refers to a missing reference frame");
        }
    }

    //Step 5, Dito the parent frame of every frame
    foreach(ProtoFrame frame,frames) {
        if(frame.parent != 0 && frameNumbers.find(frame.parent) == frameNumbers.end()) {
            //Because of the way frames appear in the XML file this
            //shouldn't happen. This is defense in depth.
            throw logic_error("Internal error, frame refers to a non-existant parent frame");
        }
    }

    //Step 6, spin1 and spin2 should point to a valid spin, at
    //least, if they are present. After doing this test we can be
    //sure that if they are present they are valid

    foreach(ProtoInteraction interaction,interactions) {
        if(spinNumbers.find(interaction.spin1) == spinNumbers.end()) {
            cout << "interaction.spin1 = " << interaction.spin1 << endl;
            throw runtime_error("Interaction spin_1 attribute refers to a missing spin");                   
        }
        if(interaction.spin2) {
            if(spinNumbers.find(interaction.spin2.get()) == spinNumbers.end()) {
                throw runtime_error("Interaction spin_2 attribute refers to a missing spin");                   
            }
        }
    }

    //By this point all errors should be accounted for. We can
    //start assembling the spin system and allocating memory. Code
    //beyond this point should not throw
    map<long,Spin*> number2spin;
    //Map a index to a (parent index,frame) pair
    typedef pair<long,pair<long,Frame*> > number2Frame_t;
    map<long,pair<long,Frame*> > number2Frame;

    //As we aren't assuming anything about the order we are reciving
    //the frames here, instanciate the frames then link them up.
    foreach(ProtoFrame protoFrame,frames) {
        Frame* frame = new Frame(Vector3d(protoFrame.x,protoFrame.y,protoFrame.z),protoFrame.o);
        number2Frame[protoFrame.number] = pair<long,Frame*>(protoFrame.parent,frame);
    }
    Frame* lab = ss->GetLabFrame();

    foreach(number2Frame_t p,number2Frame) {
        long parentN = p.second.first;
        Frame* frame = p.second.second;
        Frame* parent = parentN == 0 ? lab : number2Frame[parentN].second;
        parent->AddChild(frame);
    }

    //Instanciate the spins
    foreach(ProtoSpin protoSpin,spins) {
        Vector3d position(protoSpin.x,protoSpin.y,protoSpin.z);

        Frame* frame = protoSpin.frame == 0 ? lab : number2Frame[protoSpin.frame].second;
        position = ToLabVec3d(frame,position);

        Spin* spin = new Spin(position,protoSpin.label,protoSpin.element,protoSpin.isotope-protoSpin.element);
        number2spin[protoSpin.number] = spin;
        spin->SetPreferedFrame(frame);
        ss->InsertSpin(spin);
    }

    //Instanciate the interactions
    foreach(ProtoInteraction protoInteraction,interactions) {
        //NB: Eventually we could put protoSpin.frame == 0 in an if
        //statement and skip the transform altogether. But I want the
        //transform code path stressed as much as possible
        Frame* frame = protoInteraction.frame == 0 ? lab : number2Frame[protoInteraction.frame].second;

        protoInteraction.payload = protoInteraction.payload.ToLabFrame(frame);
        
        Spin* spin1 = number2spin[protoInteraction.spin1];
        Spin* spin2 = protoInteraction.spin2 ? number2spin[protoInteraction.spin2.get()] : NULL;
        Interaction* interaction = new Interaction(protoInteraction.payload,protoInteraction.type,spin1,spin2);
        interaction->SetPreferedFrame(frame);
        ss->InsertInteraction(interaction);
    }


}

