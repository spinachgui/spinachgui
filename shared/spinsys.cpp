
#include <shared/spinsys.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <shared/nuclear_data.hpp>
#include <boost/variant/static_visitor.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <shared/basic_math.hpp>
#include <shared/foreach.hpp>

using namespace std;
using namespace SpinXML;
using namespace sigc;
using namespace boost;

//============================================================//
// SpinSystemView

std::vector<SpinView> SpinSystemView::GetNearbySpins(Vector3d pos,double distance,SpinView Ignore) {
	Vector4d v=Vector4d(pos.x(),pos.y(),pos.z(),1);
	v=mFrame->getTransformToLab()*v;
	return MapVectorToViewVector<SpinView>(mData->GetNearbySpins(Vector3d(v.x(),v.y(),v.z()),distance * mUnitSystem->lengthUnit,Ignore.Get()));
}


//==============================================================================//
// SpinSystem

SpinSystem::SpinSystem()
	: mRootFrame(new Frame(Vector3d(0,0,0),Orientation(Quaterniond(1,0,0,0)),new UnitSystem)),mIgnoreSpinKill(NULL) {
}


SpinSystem::SpinSystem(const SpinSystem& system)
	: mRootFrame(new Frame(Vector3d(0,0,0),Orientation(Quaterniond(1,0,0,0)),new UnitSystem)),mIgnoreSpinKill(NULL) {
    long spinCount=system.mSpins.size();

    mSpins.resize(spinCount);

    for(long i=0;i<spinCount;i++) {
        mSpins[i]=new Spin(*system.mSpins[i]);
    }
}


SpinSystem::~SpinSystem() {
    Clear();
	delete mRootFrame;
}

void SpinSystem::Clear() {
    for(unsigned long i=0;i<mSpins.size();i++) {
        mIgnoreSpinKill=mSpins[i];
        delete mSpins[i];
    }
    mSpins.resize(0);

	delete mRootFrame;
	mRootFrame = new Frame(Vector3d(0,0,0),Orientation(Quaterniond(1,0,0,0)),new UnitSystem);

    sigReloaded();
}

void SpinSystem::Dump() const {
    cout << "Dumping SpinSystem:" << endl;
    cout << " Spin count=" << mSpins.size() << endl;
    cout << endl;
}

long SpinSystem::GetSpinCount() const {
    return mSpins.size();
}

Spin* SpinSystem::GetSpin(unsigned long n) const {
    if(n+1 > mSpins.size()) {
	//throw runtime_error("In SpinSystem::GetSpin Invalid spin number");
    }
    return mSpins[n];
}

long SpinSystem::GetSpinNumber(Spin* spin) const {
    for(unsigned long i=0;i<mSpins.size();i++) {
        if(mSpins[i]==spin) {
            return i;
        }
    }
    return -1;
}

vector<Spin*> SpinSystem::GetNearbySpins(Vector3d pos,length distance,Spin* Ignore) {
    std::vector<Spin*> result;
    length2 dist2=distance*distance;
    length x1=pos.x();
    length y1=pos.y();
    length z1=pos.z();

    long spinCount=mSpins.size();

    long skipped;
    for(skipped=0;mSpins[skipped]!=Ignore && skipped<spinCount;skipped++){}


    for(long i=skipped+1;i<spinCount;i++) {
        length x2,y2,z2;
        mSpins[i]->GetCoordinates(&x2,&y2,&z2);
        length deltaX=(x1-x2);
        length deltaY=(y1-y2);
        length deltaZ=(z1-z2);
		length2 deltaR=deltaX*deltaX+deltaY*deltaY+deltaZ*deltaZ;
        if(deltaR < dist2) {
            result.push_back(mSpins[i]);
        }
    }
    return result;
}
const vector<Spin*>& SpinSystem::GetSpins() const {
    return mSpins;
}

void SpinSystem::InsertSpin(Spin* _Spin,long Position) {

    if(Position==END) {
        mSpins.push_back(_Spin);
        sigNewSpin(_Spin,mSpins.size()-1);
    } else {
        mSpins.insert(mSpins.begin()+Position,_Spin);
        sigNewSpin(_Spin,Position);
    }

    _Spin->sigDying.connect(mem_fun(this,&SpinSystem::OnSpinDeleted));
}


Spin* SpinSystem::RemoveSpin(long Position) {
    Spin* spin = mSpins[Position];
    mSpins.erase(mSpins.begin()+Position);
    return spin;
}

Spin* SpinSystem::RemoveSpin(Spin* _Spin) {
    if(_Spin==mIgnoreSpinKill) {
        mIgnoreSpinKill=NULL;
        return NULL;
    }
    for(unsigned long i=0;i<mSpins.size();i++) {
        if(mSpins[i]==_Spin) {
            mSpins.erase(mSpins.begin()+i);
            return _Spin;
        }
    }
    return NULL;
}


void SpinSystem::LoadFromFile(const char* filename,ISpinSystemLoader* loader) {
    try {
        loader->LoadFile(this,filename);
    } catch(const runtime_error& e) {
        sigReloaded();
        throw e;
    }
    sigReloaded();
    return;
}

void SpinSystem::SaveToFile(const char* filename,ISpinSystemLoader* saver) const 
{
    saver->SaveFile(this,filename);
    return;
}

void SpinSystem::CalcNuclearDipoleDipole() {
    for(unsigned long i=0;i<mSpins.size();i++) {
        Spin* spin1=mSpins[i];
        long element1=spin1->GetElement();
        long isotope1=spin1->GetIsotope();
        double g1=getGyromagneticRatio(element1,isotope1);
        if(abs(g1)<1e-10) continue;

        for(unsigned long j=i+1;j<mSpins.size();j++) {
            Spin* spin2=mSpins[j]; 

            //A unit vector pointing from spin1 tp spin2
            length rx,ry,rz;
            Vector3d r_1_2=spin2->GetPosition()-spin1->GetPosition();
            rx=r_1_2.x() * metres;
            ry=r_1_2.y() * metres;
            rz=r_1_2.z() * metres;

            long element2=spin2->GetElement();
            long isotope2=spin2->GetIsotope();
            double g2=getGyromagneticRatio(element2,isotope2);
            if(abs(g2)<1e-10) continue;
            
            //cout << "isotope1=" << isotope1 << "  isotope2="<< isotope2 << endl;
            //cout << "g1="<< g1 << "  g2="<< g2 << endl;

            double r=r_1_2.norm();
            double r2=r*r;
            double r3=r2*r;
            double r5=r2*r3;

            Matrix3d dipole=MakeMatrix3d(r2-3.0*rx*rx, 3.0*rx*ry,      3.0*rx*rz,
                                         3.0*rx*ry,       r2-3.0*ry*ry,3.0*ry*rz,
                                         3.0*rx*rz,       3.0*ry*rz,   r2-3.0*rz*rz);
            static const double four_pi=12.5663706;
            double coeff=(mu0*hbar*g1*g2/(r5*four_pi));

            Matrix3d dipole_inter=coeff*dipole;
            Interaction* inter=new Interaction(dipole_inter,Interaction::DIPOLAR,spin1,spin2);
            this->InsertInteraction(inter);
        }
    }
}

std::vector<Interaction*> SpinSystem::GetInteractionsBySpin(const Spin* spin,Interaction::Type t) const {
    vector<Interaction*> retVal;
    for(unsigned long i=0;i<mInteractions.size();i++) {
		if(mInteractions[i]->IsType(t) && mInteractions[i]->GetHasSpin(spin)) { 
			retVal.push_back(mInteractions[i]);
		}
    }
    return retVal;
}

std::vector<Interaction*> SpinSystem::GetInteractionsBySpin(const Spin* spin1, Spin* spin2,Interaction::Type t) const {
    vector<Interaction*> retVal;
    for(unsigned long i=0;i<mInteractions.size();i++) {
		if(mInteractions[i]->IsType(t) && mInteractions[i]->GetHasSpin(spin1) && mInteractions[i]->GetHasSpin(spin2)) { 
			retVal.push_back(mInteractions[i]);
		}
    }
    return retVal;
}

std::vector<Interaction*> SpinSystem::GetInteractionsBySpinOnce(const Spin* spin,Interaction::Type t) const {
    vector<Interaction*> retVal;
    for(unsigned long i=0;i<mInteractions.size();i++) {
		if(mInteractions[i]->IsType(t) && mInteractions[i]->GetHasSpin(spin)) { 
			if(mInteractions[i]->GetOtherSpin(spin) < spin) {
				retVal.push_back(mInteractions[i]);
			}
		}
    }
    return retVal;
}

Interaction* SpinSystem::RemoveInteraction(Interaction* inter) {
    for(unsigned long i=0;i<mInteractions.size();i++) {
        if(mInteractions[i]==inter) {
            mInteractions.erase(mInteractions.begin()+i);
            return inter;
        }
    }
    return NULL;
}



void SpinSystem::InsertInteraction(Interaction* inter) {
	mInteractions.push_back(inter);
	sigNewInter.emit(inter);
}


void SpinSystem::OnSpinDeleted(Spin* spin) {
	RemoveSpin(spin);
}

void SpinSystem::CompressDuplicateInteractions() {
	//Do the monospin interactions
	foreach(Spin* spin,mSpins) {
		foreach(Interaction::Type t,MonoTypes) {
			vector<Interaction*> toCrush = GetInteractionsBySpin(spin,t);
			if(toCrush.size() > 1) {
				Matrix3d total;
				total << 
					0, 0, 0,
					0, 0, 0,
					0, 0, 0;
				foreach(Interaction* inter,toCrush) {
					total += inter->AsMatrix();
					delete RemoveInteraction(inter);
				}
				this->InsertInteraction(new Interaction(total,t,spin));
			}
		}
	}
	//Now do the binary spin interactions
	for(vector<Spin*>::iterator i = mSpins.begin();i != mSpins.end(); ++i) {
		for(vector<Spin*>::iterator j = i + 1; j != mSpins.end(); ++j) {
			foreach(Interaction::Type t,BinaryTypes) {
				vector<Interaction*> toCrush = GetInteractionsBySpin(*i,*j,t);
				if(toCrush.size() > 1) {
					Matrix3d total;
					total << 
						0, 0, 0,
						0, 0, 0,
						0, 0, 0;
					foreach(Interaction* inter,toCrush) {
						total += inter->AsMatrix();
						delete RemoveInteraction(inter);
					}
					this->InsertInteraction(new Interaction(total,t,*i,*j));
				}
			}
		}
	}
}
