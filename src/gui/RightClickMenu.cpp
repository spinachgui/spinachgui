
#include <gui/RightClickMenu.hpp>
#include <gui/SpinDialog.hpp>
#include <gui/SpinachApp.hpp>
#include <gui/StdEvents.hpp>

//============================================================//
// PopupMenu

enum {
  MENU_SPIN_PROP,
  MENU_SPIN_DELETE
};

RightClickMenu::RightClickMenu(wxWindow* parent) : wxMenu(),
						   mParent(parent),
						   mOptionDelete(NULL),
						   mOptionSpinProperties(NULL) {

}

void RightClickMenu::Build() {
  if(mOptionDelete!=NULL) {
    Append(MENU_SPIN_DELETE, wxT("Delete Spins..."));    
  }
  if (mOptionSpinProperties!=NULL) {
    Append(MENU_SPIN_PROP, wxT("Spin Properties..."));
  }
}

void RightClickMenu::OnShowSpinProperties(wxCommandEvent& e) {
  SpinDialog* dialog=new SpinDialog(mParent,mOptionSpinProperties);
  dialog->ShowModal(); 
}

void RightClickMenu::OnDeleteSpin(wxCommandEvent& e) {
  Chkpoint(wxT("Delete Spin"));
  GetSS()->RemoveSpin(mOptionDelete);
}


BEGIN_EVENT_TABLE(RightClickMenu,wxMenu)

EVT_MENU  (  MENU_SPIN_PROP,          RightClickMenu::OnShowSpinProperties)
EVT_MENU  (  MENU_SPIN_DELETE,        RightClickMenu::OnDeleteSpin)

END_EVENT_TABLE()
