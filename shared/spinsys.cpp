
#include <shared/spinsys.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <shared/nuclear_data.hpp>
#include <boost/variant/static_visitor.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <shared/basic_math.hpp>

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

SpinSystem::SpinSystem() : mIgnoreSpinKill(NULL) {
}


SpinSystem::SpinSystem(const SpinSystem& system)  {
    long spinCount=system.mSpins.size();

    mSpins.resize(spinCount);

    for(long i=0;i<spinCount;i++) {
        mSpins[i]=new Spin(*system.mSpins[i]);
    }
}


SpinSystem::~SpinSystem() {
    Clear();
    sigDying(this);
}

void SpinSystem::Clear() {
    //Currently we need to delete the interactions Before the spins as
    //the spins have interactions as their children. 

    for(unsigned long i=0;i<mSpins.size();i++) {
        mIgnoreSpinKill=mSpins[i];
        delete mSpins[i];
    }
    mSpins.resize(0);

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

Spin* SpinSystem::GetSpin(long n) const {
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
    length x1=pos.x() * metres;
    length y1=pos.x() * metres;
    length z1=pos.x() * metres;

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

            length r=r_1_2.norm()*metres;
            length2 r2=r*r;
            length3 r3=r2*r;
            length5 r5=r2*r3;

            Matrix3d dipole=MakeMatrix3d(r2.si-3.0*rx.si*rx.si, 3.0*rx.si*ry.si,      3.0*rx.si*rz.si,
                                         3.0*rx.si*ry.si,       r2.si-3.0*ry.si*ry.si,3.0*ry.si*rz.si,
                                         3.0*rx.si*rz.si,       3.0*ry.si*rz.si,      r2.si-3.0*rz.si*rz.si);
            static const double four_pi=12.5663706;
            double coeff=(mu0*hbar*g1*g2/(r5.si*four_pi));

            dreal<double,_mass_per_time2> dcoeff(coeff);
            Matrix3d dipole_inter=dcoeff.si*dipole;
            Interaction* inter=new Interaction(dipole_inter,Interaction::DIPOLAR,spin1,spin2);
            this->InsertInteraction(inter);
        }
    }
}

void SpinSystem::InsertInteraction(Interaction* inter) {
	mInteractions.push_back(inter);
	sigNewInter.emit(inter);
}


void SpinSystem::OnSpinDeleted(Spin* spin) {
	RemoveSpin(spin);
}
