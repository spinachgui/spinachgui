
#include <gui/RootFrame.hpp>
#include <gui/SpinachApp.hpp>

const CEventType EVT_CHECKPOINT("checkpoint");
const CEventType EVT_UNDO("undo");
const CEventType EVT_REDO("redo");


//============================================================//
// RootFrame

void RootFrame::InitFrame() {
  CEventManager::Instance()->addListener(EventListenerPtr(this),EVT_CHECKPOINT);
  CEventManager::Instance()->addListener(EventListenerPtr(this),EVT_UNDO);
  CEventManager::Instance()->addListener(EventListenerPtr(this),EVT_REDO);

  mNotebook=new wxAuiNotebook(mAuiPanel);

  mSpinGridPanel=new SpinGridPanel(mNotebook);

  mGLDisplay=new glDisplay(mNotebook);

  // add the panes to the manager
  mNotebook->AddPage(mGLDisplay, wxT("3D View"));
  mNotebook->AddPage(mSpinGridPanel, wxT("Grid View"));

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
    mMenuItemUndo->SetText(wxString(wxT("Undo ")) << GetSSMgr().GetUndoMessage());
    mMenuItemRedo->SetText(wxString(wxT("Redo ")) << GetSSMgr().GetRedoMessage());

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


