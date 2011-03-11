

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

struct SelSpin {
	SelSpin(Spin* s,sigc::connection c) :spin(s),connect(c) {}
	bool operator < (const SelSpin& o) const {return spin < o.spin;}

	Spin* spin;
	sigc::connection connect;
};
struct SelSpinEq {
	SelSpinEq(Spin* spin) : s(spin){}
	bool operator()(SelSpin sel) {return sel.spin==s;}
	Spin* s;
};

Spin* gHover;
set<SelSpin> gSelection;

//Private

SelSpin packf(Spin* spin) {
	return SelSpin(spin,spin->sigDying.connect(sigc::ptr_fun<Spin*,void>(RemoveSelection)));
}
set<SelSpin> packSet(const set<Spin*>& s) {
	set<SelSpin> out;
	for(set<Spin*>::iterator i=s.begin();i!=s.end();++i) {
		out.insert(packf(*i));
	}
	return out;
}
set<Spin*> unpackSet(const set<SelSpin>& s) {
	set<Spin*> out;
	for(set<SelSpin>::iterator i=s.begin();i!=s.end();++i) {
		out.insert((*i).spin);
	}
	return out;
}

//Selection Readers

bool IsSelected(SpinXML::Spin* spin)  {
	return find_if(gSelection.begin(),gSelection.end(),SelSpinEq(spin)) != gSelection.end();
}

unsigned int GetSelectedCount(){
	return gSelection.size();
}

set<Spin*> GetSelection() {
	return unpackSet(gSelection);
}

//Selection Writers

void ClearSelection() {
	gSelection.clear();
}

void DeleteSelectedSpins(){
	for(set<SelSpin>::iterator i=gSelection.begin();i!=gSelection.end();++i) {
		delete (*i).spin;
	}
};

void SetHover(SpinXML::Spin* spin) {
    gHover=spin;
    sigHover(spin);
}


void SetSelection(Spin* spin) {
	ClearSelection();
	gSelection.insert(packf(spin));
    sigSelectChange(unpackSet(gSelection));
}


void SetSelection(set<SpinXML::Spin*>& selection) {
	ClearSelection();
	for(set<Spin*>::iterator i = selection.begin();i!=selection.end();++i) {
		gSelection.insert(packf(*i));
	}
    sigSelectChange(selection);
}

void AddSelection(SpinXML::Spin* spinToAdd) {
	gSelection.insert(packf(spinToAdd));
	sigSelectChange(unpackSet(gSelection));
}

void RemoveSelection(SpinXML::Spin* spin) {
	// Tried
	//	set<SelSpin>::iterator i = find_if(gSelection.begin(),gSelection.end(),SelSpinEq(spin));
	// 	(*i).connect.disconnect();
	// Got  error: passing ‘const sigc::connection’ as ‘this’ argument of ‘void sigc::connection::disconnect()’ discards qualifiers
	// And yet i is not a const iterator. WTF? Standard Template Library, you have failed me again

	for(set<SelSpin>::iterator i = gSelection.begin();i != gSelection.end() ; ++i) {
		if((*i).spin == spin) {
			gSelection.erase(i);
			sigSelectChange(unpackSet(gSelection));
			return;
		}
	}
}












