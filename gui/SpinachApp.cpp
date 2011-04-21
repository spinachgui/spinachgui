

#include<algorithm>


#include <shared/nuclear_data.hpp>
#include <gui/RootFrame.hpp>
#include <wx/log.h>

#include <shared/formats/xyz.hpp>
#include <shared/formats/g03.hpp>
#include <shared/formats/xml.hpp>
#include <shared/formats/castep.hpp>
#include <shared/formats/simpson.hpp>
#include <shared/formats/easyspin.hpp>
#include <shared/panic.hpp>

#include <wx/filename.h>

#include <shared/unit.hpp>

#include <gui/SpinachApp.hpp>

#include <cstdlib>

#ifndef __LINUX__
#include <windows.h>
#endif

using namespace SpinXML;
using namespace std;
using namespace Eigen;

extern char spinxmlSchema[];
extern unsigned int spinxmlSchemaSize;


template<typename T>
  class InvariantChecker {
  public:
    InvariantChecker(const T* x) : m(x) {
      m->CheckInvariant();
    }
    ~InvariantChecker() {
      m->CheckInvariant();
    }
  private:
    const T* m;
  };

//------------------------------------------------------------//
// Unit Systems

UnitSystem gUnitSystem;
sigc::signal<void> sigUnitSystemChange;
sigc::signal<void,PhysDimension,unit> sigUnitChange;

void SetUnit(PhysDimension d,unit u) {
	switch(d) {
	case DIM_LENGTH: gUnitSystem.lengthUnit = u; break;
	case DIM_ENERGY: gUnitSystem.energyUnit = u; break;
	}
	sigUnitChange(d,u);
	sigUnitSystemChange();
}

unit GetUnit(PhysDimension d) {
	switch(d) {
	case DIM_LENGTH:
		return gUnitSystem.lengthUnit;
	case DIM_ENERGY:
		return gUnitSystem.energyUnit;
	}
}

const UnitSystem* GetUnitSystem() {
	return &gUnitSystem;
}

void SetUnitSystem(const UnitSystem& us) {
	gUnitSystem = us;
	sigUnitSystemChange();
}

void CheesyUnitSystemChangerHandler() {
	//Quick hack for making sure everything gets updated, emit a
	//sigReloaded signal
	GetRawSS()->sigReloaded();
}

//================================================================================//
// Set and get gobal reference frame

Frame* gFrame = NULL;

void SetFrame(SpinXML::Frame* frame) {
	gFrame = frame;
	sigFrameChange(frame);
}

SpinXML::Frame* GetFrame() {
	return gFrame;
}

void CheesyFrameChangerHandler(Frame* frame) {
	//Quick hack for making sure everything gets updated, emit a
	//sigReloaded signal
	GetRawSS()->sigReloaded();
}


sigc::signal<void,SpinXML::Frame*> sigFrameChange;

void leakReport();

//--------------------------------------------------------------------------------//
// The Application Object

SpinachApp* gApp;

SpinachApp& wxGetApp() {
  return *gApp;
}

#ifdef __LINUX__
int main(int argc,char** argv) {
#else
	int WINAPI WinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPSTR lpCmdLine,
					   int nShowCmd) {

#endif
		atexit(&leakReport);
		try {
			gApp = new SpinachApp;
			wxApp::SetInstance(gApp);
#ifdef __LINUX__
			wxEntry(argc,argv);
#else
			wxEntry(hInstance,hPrevInstance,NULL,nShowCmd);
#endif
		} catch (logic_error& e) {
			cerr << "Uncaught logic_error what()=" << e.what() << endl;
		} catch (runtime_error& e) {
			cerr << "Uncaught runtime_error what()=" << e.what() << endl;
		} catch (...) {
			cerr << "Uncaught unknown exception." << endl;
		}


		return 0;
	}

	void leakReport() {
		//Print out a memory leak report
		cout << "========================================" << endl;
		cout << "             Leak Report" << endl;
		cout << "========================================" << endl;
		cout << "Spin:" << Spin::objCount() << endl;
		cout << "Interaction:" << Interaction::objCount() << endl;
		cout << "Orientation:" << Orientation::objCount() << endl;
		cout << "UnitSystem:" << UnitSystem::objCount() << endl;
		cout << "========================================" << endl;
	}

	SpinachApp::~SpinachApp() {
		for(unsigned long i=0;i<mIOFilters.size();i++) {
			delete mIOFilters[i];
		}
		delete mSS;
	}

	bool SpinachApp::OnInit() {
		printf("%p %u\n",spinxmlSchema, spinxmlSchemaSize);
		//Load the I/O Filters

		ISpinSystemLoader* g03 = new G03Loader;

		mIOFilters.push_back(g03);
		mIOFilters.push_back(new SIMPSONLoader);
		mIOFilters.push_back(new CASTEPLoader);
		mIOFilters.push_back(new EasySpinLoader);
		mIOFilters.push_back(new XMLLoader(spinxmlSchema));

		//Connect up the selection manager so that when a spin is deleted
		//it also gets unselected
		sigAnySpinDying.connect(sigc::ptr_fun(RemoveSelection));

		//Setup a sensible system of units
		gUnitSystem.energyUnit = MHz;
		gUnitSystem.lengthUnit = Angstroms;

		sigUnitSystemChange.connect(sigc::ptr_fun(&CheesyUnitSystemChangerHandler));
		sigFrameChange.connect(sigc::ptr_fun(&CheesyFrameChangerHandler));

		//Load the isotopes

		try {
			LoadIsotopes();
		} catch(runtime_error e) {
			cout << "Isotopes not loaded" << endl;
			wxLogError(wxString() <<
					   wxT("Error loading data/isotopes.dat. Is the file present and not corrupt?\n") <<
					   wxT("Message was:") <<
					   wxString(e.what(),wxConvUTF8));
			return false;
		}


		mSS = new SpinSystem;
		//Set the active frame as the lab frame
		SetFrame(mSS->GetLabFrame());


		/*
		  Spin* spin1 = new Spin(Vector3d(1e-10,0,0),"Test Spin A",8,16);
		  mSS->InsertSpin(spin1);

		  Spin* spin2 = new Spin(Vector3d(0,1e-10,0),"Test Spin B",1,1);
		  mSS->InsertSpin(spin2);

		  Spin* spin3 = new Spin(Vector3d(0,0,0)    ,"Test Spin O",1,1);
		  mSS->InsertSpin(spin3);

		  mSS->CalcNuclearDipoleDipole();

		  mSS->InsertInteraction(new Interaction(10*MHz,Interaction::SHIELDING,spin1));

		  mSS->InsertInteraction(new Interaction(Eigenvalues(10*MHz,20*MHz,50*MHz,Orientation()),Interaction::SHIELDING,spin2));


		  mSS->GetLabFrame()->AddChild(new Frame(Vector3d(1,0 ,0),Orientation(EulerAngles(1,1,0)),GetUnitSystem()));
		  Frame* f1 = new Frame(Vector3d(3,-2,0),Orientation(EulerAngles(1,2,0)),GetUnitSystem());

		  mSS->GetLabFrame()->AddChild(f1);

		  f1->AddChild(new Frame(Vector3d(1,0 ,0),Orientation(EulerAngles(1,1,0)),GetUnitSystem()));
		*/
		RootFrame* frame = new RootFrame(NULL);
		frame->Show();

		return true;
	}

	//============================================================//
	// Selection Manager

	sigc::signal<void,SpinXML::Spin*>            sigHover;
	sigc::signal<void,std::set<SpinXML::Spin*> > sigSelectChange;


	Spin* gHover;
	set<Spin*> gSelection;

	typedef set<Spin*>::iterator itor;

	//Selection Manager Invariants

	void AssertSelectionExists() {
		std::vector<Spin*> spins = GetRawSS()->GetSpins();
		for(itor i = gSelection.begin();i!=gSelection.end();++i) {
			if(find(spins.begin(),spins.end(),*i) == spins.end()) {
				PANIC("A spin in the selection manager was not present in the spin system");
			}
		}
	}

	//Selection Readers

	bool IsSelected(SpinXML::Spin* spin)  {
		AssertSelectionExists();
		return gSelection.find(spin) != gSelection.end();
	}

	unsigned int GetSelectedCount(){
		AssertSelectionExists();
		return gSelection.size();
	}

	const set<Spin*>& GetSelection() {
		AssertSelectionExists();
		return gSelection;
	}

	Spin* GetHover() {
		return gHover;
	}

	//Selection Writers

	void ClearSelection() {
		AssertSelectionExists();
		gSelection.clear();
		sigSelectChange(gSelection);
	}

	void DeleteSelectedSpins(){
		AssertSelectionExists();
		for(set<Spin*>::iterator i=gSelection.begin();i!=gSelection.end();) {
			//i is about to be invalidated, so save it and incriment before erasing
			set<Spin*>::iterator j = i;
			++j;
			cout << "Calling the destructor of spin" << *i << endl;
			delete (*i);
			i = j;
		}
		AssertSelectionExists();
	};

	void SetHover(SpinXML::Spin* spin) {
		gHover=spin;
		sigHover(spin);
	}


	void SetSelection(Spin* spin) {
		AssertSelectionExists();
		ClearSelection();

		gSelection.insert(spin);
		sigSelectChange(gSelection);
		AssertSelectionExists();
	}


	void SetSelection(set<SpinXML::Spin*>& selection) {
		AssertSelectionExists();
		ClearSelection();
		gSelection = selection;
		sigSelectChange(gSelection);
		AssertSelectionExists();
	}

	void AddSelection(SpinXML::Spin* spinToAdd) {
		if(spinToAdd == NULL) return;
		AssertSelectionExists();
		gSelection.insert(spinToAdd);
		sigSelectChange(gSelection);
		AssertSelectionExists();
	}

	void RemoveSelection(SpinXML::Spin* spin) {
		set<Spin*>::iterator i = gSelection.find(spin);
		if(i != gSelection.end()) {
			gSelection.erase(i);
			sigSelectChange(gSelection);
		}
		AssertSelectionExists();
	}



