
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


IMPLEMENT_APP(SpinachApp);

SpinachApp::~SpinachApp() {
    for(unsigned long i=0;i<mIOFilters.size();i++) {
        delete mIOFilters[i];
    }
    delete mSelectionManager;
}

bool SpinachApp::OnInit() {
    //Create the global selection manager
    mSelectionManager = new SelectionManager;

    //Load the I/O Filters

    mIOFilters.push_back(new XYZLoader);
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

    return false;
}


//============================================================//
// Selection Manager
SelectionManager* SelectionManager::static_this=NULL;

SelectionManager::SelectionManager() {
    //There can be only one
    assert(NULL==static_this);
    static_this=this;
}

SelectionManager::~SelectionManager() {
    assert(this==static_this);
    static_this=NULL;
}

SelectionManager* SelectionManager::Instance() {
    assert(static_this != NULL);
    return static_this;
}


void SelectionManager::SetHover(SpinXML::Spin* spin) {
    mHover=spin;
    sigHover(spin);
}

void SelectionManager::SetSelection(const std::vector<SpinXML::Spin*>& selection) {
    mSelection=selection;
    sigSelect(mSelection);
}




