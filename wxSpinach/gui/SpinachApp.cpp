
#include <gui/SpinachApp.hpp>

#include <gui/SpinDialog.hpp>

#include <shared/nuclear_data.hpp>
#include <gui/EventSystem.hpp>
#include <wx/log.h>

IMPLEMENT_APP(SpinachApp);

const CEventType EVT_CHECKPOINT("checkpoint");
const CEventType EVT_UNDO("undo");
const CEventType EVT_REDO("redo");

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

  mSSMgr->Checkpoint();
  (*head)->LoadFromG03File("data/tryosine.log");
  (*head)->SaveToXMLFile("tryosine.xml");

  RootFrame* frame = new RootFrame(NULL);

  //SpinDialog* dialog = new SpinDialog(frame,mSS->GetSpin(0));
  //dialog->ShowModal();

  frame->Show();
  return true;
}


//============================================================//
// RootFrame

void RootFrame::InitFrame() {
  CEventManager::Instance()->addListener(EventListenerPtr(this),EVT_CHECKPOINT);
  CEventManager::Instance()->addListener(EventListenerPtr(this),EVT_UNDO);
  CEventManager::Instance()->addListener(EventListenerPtr(this),EVT_REDO);

  mNotebook=new wxAuiNotebook(mAuiPanel);

  mSpinGrid=new SpinGrid(mNotebook);

  mGLDisplay=new glDisplay(mNotebook);

  // add the panes to the manager
  mNotebook->AddPage(mGLDisplay, wxT("3D View"));
  mNotebook->AddPage(mSpinGrid, wxT("Grid View"));

  mAuiPanel->GetSizer()->Add(mNotebook,1,wxEXPAND);

  //Grey the undo and redo menu ideams. They can be ungreyed when
  //there is an undo history

  mMenuItemUndo->Enable(false);
  mMenuItemRedo->Enable(false);
}

bool RootFrame::HandleEvent(CEvent const& event) {
  cout << "Root window got an event" << endl;
  if(event.getType() == EVT_CHECKPOINT ||
     event.getType() == EVT_UNDO || 
     event.getType() == EVT_REDO) {
    //In these circumstances, we may need to grey or ungrey the
    //edit->undo and edit->redo menus.
    mMenuItemUndo->Enable(GetSSMgr().CanUndo());
    mMenuItemRedo->Enable(GetSSMgr().CanRedo());
  }
  return true;
}

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




