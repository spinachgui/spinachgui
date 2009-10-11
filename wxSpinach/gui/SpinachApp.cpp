
#include <gui/SpinachApp.hpp>

#include <gui/SpinDialog.hpp>

#include <shared/nuclear_data.hpp>
#include <gui/RootFrame.hpp>
#include <wx/log.h>
#include <boost/shared_ptr.hpp>
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


  shared_ptr<SpinSystem> SS = shared_ptr<SpinSystem>(new SpinSystem);

  mSSMgr = new SpinSysManager(SS);

  const SpinSysPtr* head = mSSMgr->Get();

  mSSMgr->Checkpoint(wxT("Load File"));
  (*head)->LoadFromG03File("data/tryosine.log");
  (*head)->SaveToXMLFile("tryosine.xml");

  RootFrame* frame = new RootFrame(NULL);

  //SpinDialog* dialog = new SpinDialog(frame,mSS->GetSpin(0));
  //dialog->ShowModal();

  frame->Show();
  return true;
}

