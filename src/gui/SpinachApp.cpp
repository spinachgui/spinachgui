
#include <gui/SpinachApp.hpp>

#include <gui/SpinDialog.hpp>

#include <shared/nuclear_data.hpp>
#include <gui/RootFrame.hpp>
#include <wx/log.h>

#include <shared/formats/xyz.hpp>
#include <shared/formats/g03.hpp>
#include <shared/formats/xml.hpp>

#include <wx/filename.h>



IMPLEMENT_APP(SpinachApp);

SpinachApp::~SpinachApp() {
  for(long i=0;i<mIOFilters.size();i++) {
    delete mIOFilters[i];
  }
}

bool SpinachApp::OnInit() {
  //Load the I/O Filters

  mIOFilters.push_back(new XYZLoader);
  mIOFilters.push_back(new G03Loader);

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
  //SS->LoadFromG03File("data/tryosine.log");
  G03Loader* loader= new G03Loader();
  //mSS->LoadFromFile("../../testing_kit/Gaussian/ESR spectroscopy/cpdyad_cation.log",loader);
  mSS->LoadFromFile("data/tryosine.log",loader);
  delete loader;

  RootFrame* frame = new RootFrame(NULL);
  frame->Show();

  return true;
}



