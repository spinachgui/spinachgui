
#include <gui/SpinachApp.hpp>

#include <gui/SpinDialog.hpp>

#include <shared/nuclear_data.hpp>
#include <wx/log.h>

IMPLEMENT_APP(SpinachApp);

bool SpinachApp::OnInit() {

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


  mSS = shared_ptr<SpinSystem>(new SpinSystem);

  mSS->LoadFromG03File("data/tryosine.log");
  mSS->SaveToXMLFile("data/tryosine.xml");


  mSSMgr = new SpinSysManager(mSS);
  const SpinSysPtr* head = mSSMgr->Get();

  mSSMgr->Checkpoint();
  (*head)->LoadFromG03File("install/data/tryosine.log");
  (*head)->SaveToXMLFile("install/data/tryosine.xml");

  RootFrame* frame = new RootFrame(NULL);

  //SpinDialog* dialog = new SpinDialog(frame,mSS->GetSpin(0));
  //dialog->ShowModal();

  frame->Show();
  return true;
}

//============================================================//
// RootFrame

void RootFrame::OnUndo(wxCommandEvent& e) {
  wxGetApp().GetSpinSysManager()->Undo();
}

void RootFrame::OnRedo(wxCommandEvent& e) {
  wxGetApp().GetSpinSysManager()->Redo();
}

BEGIN_EVENT_TABLE(RootFrame,wxFrame)

EVT_MENU(ID_UNDO,RootFrame::OnUndo)
EVT_MENU(ID_REDO,RootFrame::OnRedo)

END_EVENT_TABLE()




