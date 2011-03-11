

#include<algorithm>

#include <gui/SpinachApp.hpp>

#include <gui/SpinDialog.hpp>

#include <shared/nuclear_data.hpp>
#include <gui/RootFrame.hpp>
#include <wx/log.h>

#include <shared/formats/xyz.hpp>
#include <shared/formats/g03.hpp>
#include <shared/formats/xml.hpp>
#include <shared/formats/castep.hpp>
#include <shared/formats/simpson.hpp>

#include <wx/filename.h>

#include <shared/unit.hpp>

using namespace SpinXML;

SpinachApp* gApp;

SpinachApp& wxGetApp() {
  return *gApp;
}

int main(int argc,char** argv) {
  try {
    gApp = new SpinachApp;
    wxApp::SetInstance(gApp);
    wxEntry(argc,argv);
  } catch (logic_error& e) {
    cerr << "Uncaught logic_error what()=" << e.what() << endl;
  } catch (runtime_error& e) {
    cerr << "Uncaught runtime_error what()=" << e.what() << endl;
  } catch (...) {
    cerr << "Uncaught unknown exception." << endl;
    throw;
  }
  return 0;
}

SpinachApp::~SpinachApp() {
    for(unsigned long i=0;i<mIOFilters.size();i++) {
        delete mIOFilters[i];
    }
}

bool SpinachApp::OnInit() {
    //Load the I/O Filters

    mIOFilters.push_back(new G03Loader);
    mIOFilters.push_back(new SIMPSONLoader);
    mIOFilters.push_back(new CASTEPLoader);

    //Load the xml schema, it's more complicated, of course
    wxFileName fn(argv[0]);
    fn.Normalize();
    fn=fn.GetPath();
    fn.AppendDir(wxT("data"));
    fn.SetFullName(wxT("spinxml_schema.xsd"));
    wxString url(wxString(wxT("file://")) << fn.GetFullPath());
    mIOFilters.push_back(new XMLLoader(url.char_str()));

	//Connect up the selection manager so that when a spin is deleted
	//it also gets unselected
	sigAnySpinDying.connect(sigc::ptr_fun(RemoveSelection));
  
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

//Selection Readers

bool IsSelected(SpinXML::Spin* spin)  {
	return gSelection.find(spin) != gSelection.end();
}

unsigned int GetSelectedCount(){
	return gSelection.size();
}

set<Spin*> GetSelection() {
	return gSelection;
}

//Selection Writers

void ClearSelection() {
	gSelection.clear();
}

void DeleteSelectedSpins(){
	for(set<Spin*>::iterator i=gSelection.begin();i!=gSelection.end();) {
		//i is about to be invalidated, so save it and incriment before erasing
		set<Spin*>::iterator j = i;
		++j;
		cout << "Calling the destructor of spin" << *i << endl;
		delete (*i);
		i = j;
	}
};

void SetHover(SpinXML::Spin* spin) {
    gHover=spin;
    sigHover(spin);
}


void SetSelection(Spin* spin) {
	ClearSelection();
	gSelection.insert(spin);
    sigSelectChange(gSelection);
}


void SetSelection(set<SpinXML::Spin*>& selection) {
	ClearSelection();
	gSelection = selection;
    sigSelectChange(gSelection);
}

void AddSelection(SpinXML::Spin* spinToAdd) {
	gSelection.insert(spinToAdd);
	sigSelectChange(gSelection);
}

void RemoveSelection(SpinXML::Spin* spin) {
	set<Spin*>::iterator i = gSelection.find(spin);
	if(i != gSelection.end()) {
		cout << "Removing spin " << spin << endl;
		gSelection.erase(i);
		sigSelectChange(gSelection);
	}
}












