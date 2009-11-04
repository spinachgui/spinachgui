
#include <gui/SpinachApp.hpp>

#include <gui/SpinDialog.hpp>

#include <shared/nuclear_data.hpp>
#include <gui/RootFrame.hpp>
#include <wx/log.h>
#include <boost/shared_ptr.hpp>

#include <gui/StdEvents.hpp>

#include <wx/filename.h>

using boost::shared_ptr;
using boost::shared_ptr;



IMPLEMENT_APP(SpinachApp);


bool SpinachApp::OnInit() {
  //Start the event system up

  CEventManager* evSys=CEventManager::Instance();

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

  //Load the xml schema
  wxFileName fn(argv[0]);
  fn.Normalize();
  fn=fn.GetPath();
  fn.AppendDir(wxT("data"));
  fn.SetFullName(wxT("spinxml_schema.xsd"));
  wxString url(wxString(wxT("file://")) << fn.GetFullPath());
  SpinXML::SetSchemaLocation(url.char_str());
  
  cout << "Loaded " << fn.GetFullPath() << endl;

  shared_ptr<SpinSystem> SS = shared_ptr<SpinSystem>(new SpinSystem);
  SS->LoadFromXMLFile("tryo.xml");
  SS->LoadFromXMLFile("tryo.xml");
  mSSMgr = new SpinSysManager(SS);

  RootFrame* frame = new RootFrame(NULL);
  frame->Show();

  return true;
}

