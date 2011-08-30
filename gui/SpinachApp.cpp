

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
#include <shared/log.hpp>

#include <shared/foreach.hpp>

#include <wx/filename.h>

#include <shared/unit.hpp>
#include <fstream>

#include <gui/SpinachApp.hpp>

#include <cstdlib>
#include <wx/msgdlg.h>
#ifndef __LINUX__
#include <windows.h>
#endif

using namespace SpinXML;
using namespace std;
using namespace Eigen;


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
// Panic handler

bool panicHandler(const string& s) {
    wxString str(s.c_str(),wxConvUTF8);
    wxString message;
    message << wxT("An error occured, try to continue? The error was:\n") << str 
            << wxT("\n\n If you continue, you should save your work as soon as possible,")
            << wxT("preferably as a new file");
    int result = wxMessageBox(message,wxT("Fatal Error"),wxYES_NO);
    return result != wxYES;
}

ofstream logout;

void logHandler(const string& s) {
    logout << s;
    logout.flush();
}

void initLogging() {
    logout.open("log.txt");
    if(!logout.is_open()) {
        wxMessageBox(wxT("Could not open the log file"));
    }
    setLogHandler(&logHandler);
}

//------------------------------------------------------------//
// Unit Systems

unit gLengthUnit = Angstroms;
map<Interaction::Type,unit> gInterUnits;

sigc::signal<void,unit> sigLengthUnitChange;
sigc::signal<void,Interaction::Type,unit> sigInterUnitChange;

void SetLengthUnit(unit u) {
    gLengthUnit = u;
    sigLengthUnitChange(u);
}
unit GetLengthUnit() {
    return gLengthUnit;
}

void SetInterUnit(Interaction::Type t,unit u) {
    gInterUnits.find(t)->second=u;
    sigInterUnitChange(t,u);
}
unit GetInterUnit(Interaction::Type t) {
    return gInterUnits.find(t)->second;
}

void CheesyLengthUnitChangerHandler(unit) {
    //Quick hack for making sure everything gets updated, emit a
    //sigReloaded signal
    GetRawSS()->sigReloaded();
}
void CheesyInterUnitChangerHandler(Interaction::Type,unit) {
    //Quick hack for making sure everything gets updated, emit a
    //sigReloaded signal
    GetRawSS()->sigReloaded();
}


//================================================================================//
// 

vector<ISpinSystemLoader*> gIOFilters;

const vector<ISpinSystemLoader*>& GetIOFilters()  {
	return gIOFilters;
}

ISpinSystemLoader* GetLoaderFromExtension(const string& ext) {
	foreach(ISpinSystemLoader* loader,gIOFilters) {
		if(string(loader->GetFilter()) == ext) {
			return loader;
		}
	}
	return NULL;
}

G03Loader      gG03Loader;	   
SIMPSONLoader  gSIMPSONLoader; 
CASTEPLoader   gCASTEPLoader;  
EasySpinLoader gEasySpinLoader;
XMLLoader      gXMLLoader;
XYZLoader      gXYZLoader;

//================================================================================//
// Set and get gobal reference frame

Frame* gFrame = NULL;

void SetFrame(SpinXML::Frame* frame) {
    TRACE("Setting the active frame, was " << gFrame << " now " << frame);
    TRACE("frame->getTransformFromLab() = " << frame->getTransformFromLab());
    TRACE("frame->getTransformToLab()   = " << frame->getTransformToLab());
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

SpinachApp::SpinachApp()
  : mSS(NULL) {
}


SpinachApp& wxGetApp() {
    return *gApp;
}

#ifdef __LINUX__
int main(int argc,char** argv)
#else
    int WINAPI WinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPSTR lpCmdLine,
                       int nShowCmd)

#endif
{
    atexit(&leakReport);
    initLogging();
    TRACE("logger started");
    SetPanicHandler(&panicHandler);
    try {
        TRACE("Creating the app object");
        gApp = new SpinachApp;
        TRACE("Setting instance, gApp = " << gApp);
        MAYBEPANIC(gApp != NULL);
        wxApp::SetInstance(gApp);
#ifdef __LINUX__
        TRACE("Starting wxWidgets by the linux wxEntry");
        wxEntry(argc,argv);
#else
        TRACE("Starting wxWidgets by the windows wxEntry");
        wxEntry(hInstance,hPrevInstance,NULL,nShowCmd);
#endif
    } catch (logic_error& e) {
        cerr << "Uncaught logic_error what()=" << e.what() << endl;
        _ERROR_("Uncaught logic_error what()=" << e.what());
    } catch (runtime_error& e) {
        cerr << "Uncaught runtime_error what()=" << e.what() << endl;
        _ERROR_("Uncaught runtime_error what()=" << e.what())
    } catch (...) {
        cerr << "Uncaught unknown exception." << endl;
        _ERROR_("Uncaught unknown exception.")
    }
    TRACE("exiting via the main function");
    return 0;
}


void leakReport() {
    //Print out a memory leak report
    TRACE("========================================");
    TRACE("             Leak Report");
    TRACE("========================================");
    TRACE("Spin:" << Spin::objCount());
    TRACE("Interaction:" << Interaction::objCount());
    TRACE("Orientation:" << Orientation::objCount());
    TRACE("========================================");
}

SpinachApp::~SpinachApp() {
	if(mSS) {
        delete mSS;
	}
}

bool SpinachApp::OnInit() {
    TRACE("OnInit");
    wxInitAllImageHandlers();

    //Connect up the selection manager so that when a spin is deleted
    //it also gets unselected
    TRACE("Connecting Signals");
    sigAnySpinDying.connect(sigc::ptr_fun(RemoveSelection));
    sigLengthUnitChange.connect(sigc::ptr_fun(CheesyLengthUnitChangerHandler));
    sigInterUnitChange.connect(sigc::ptr_fun(CheesyInterUnitChangerHandler));

    sigFrameChange.connect(sigc::ptr_fun(&CheesyFrameChangerHandler));

    //Load the isotopes

    try {
        TRACE("Loading Elements");
        LoadElements();
        TRACE("Loading Isotopes");
        LoadIsotopes();
    } catch(runtime_error e) {
        ERROR("Failed to loaded isotopes");
        cout << "Isotopes not loaded" << endl;
        wxLogError(wxString() <<
                   wxT("Error loading data/isotopes.dat. Is the file present and not corrupt?\n") <<
                   wxT("Message was:") <<
                   wxString(e.what(),wxConvUTF8));
        return false;
    }

    TRACE("Created the spin system");
    mSS = new SpinSystem;
    //Set the active frame as the lab frame
    TRACE("Setting the active frame to the molecular frame");
    SetFrame(mSS->GetLabFrame());

    TRACE("About to show the main frame");
    RootFrame* frame = new RootFrame(NULL);
    frame->Show();

    TRACE("Application initalised normally");
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
            PANICMSG("A spin in the selection manager was not present in the spin system");
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
    TRACE("Clearing the selection");
    AssertSelectionExists();
    gSelection.clear();
    sigSelectChange(gSelection);
}

void DeleteSelectedSpins(){
    TRACE("Deleting all spins in the current selection, size=" << gSelection.size());
    AssertSelectionExists();
    for(set<Spin*>::iterator i=gSelection.begin();i!=gSelection.end();) {
        //i is about to be invalidated, so save it and incriment before erasing
        set<Spin*>::iterator j = i;
        ++j;
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
    TRACE("Setting the selection to a single spin " << spin << " named " << spin->GetLabel());
    AssertSelectionExists();
    ClearSelection();

    gSelection.insert(spin);
    sigSelectChange(gSelection);
    AssertSelectionExists();
}


void SetSelection(set<SpinXML::Spin*>& selection) {
    TRACE("Setting the selection to a set of size " << selection.size());
    AssertSelectionExists();
    ClearSelection();
    gSelection = selection;
    sigSelectChange(gSelection);
    AssertSelectionExists();
}

void AddSelection(SpinXML::Spin* spinToAdd) {
    if(spinToAdd == NULL) return;
    TRACE("Adding spin " << spinToAdd << " named " << spinToAdd->GetLabel() << " to the selection");
    AssertSelectionExists();
    gSelection.insert(spinToAdd);
    sigSelectChange(gSelection);
    AssertSelectionExists();
}

void RemoveSelection(SpinXML::Spin* spin) {
    set<Spin*>::iterator i = gSelection.find(spin);
    if(i != gSelection.end()) {
        TRACE("Removing spin " << spin << " named " << spin->GetLabel() << " from the selection");
        gSelection.erase(i);
        sigSelectChange(gSelection);
    }
    AssertSelectionExists();
}



//================================================================================//
// Module Initalisation

 __GUI__Init::__GUI__Init() {
	 gIOFilters.push_back(&gXMLLoader);
	 gIOFilters.push_back(&gG03Loader);
	 //gIOFilters.push_back(&gSIMPSONLoader);  //<- Uncomment when ready 
	 //gIOFilters.push_back(&gCASTEPLoader);   //<- Uncomment when ready 
	 gIOFilters.push_back(&gEasySpinLoader);
	 gIOFilters.push_back(&gXYZLoader);

     gInterUnits.insert(pair<Interaction::Type,unit>(Interaction::HFC             , MHz));
     gInterUnits.insert(pair<Interaction::Type,unit>(Interaction::G_TENSER        , Unitless));
     gInterUnits.insert(pair<Interaction::Type,unit>(Interaction::ZFS             , MHz));
     gInterUnits.insert(pair<Interaction::Type,unit>(Interaction::EXCHANGE        , MHz));
     gInterUnits.insert(pair<Interaction::Type,unit>(Interaction::SHIELDING       , Unitless));
     gInterUnits.insert(pair<Interaction::Type,unit>(Interaction::SCALAR          , Hz));
     gInterUnits.insert(pair<Interaction::Type,unit>(Interaction::QUADRUPOLAR     , MHz));
     gInterUnits.insert(pair<Interaction::Type,unit>(Interaction::DIPOLAR         , MHz));
     gInterUnits.insert(pair<Interaction::Type,unit>(Interaction::CUSTOM_LINEAR   , MHz));
     gInterUnits.insert(pair<Interaction::Type,unit>(Interaction::CUSTOM_BILINEAR , MHz));
     gInterUnits.insert(pair<Interaction::Type,unit>(Interaction::CUSTOM_QUADRATIC, MHz));
 }

 __GUI__Init::~__GUI__Init() {

 }

 static __GUI__Init __init;
