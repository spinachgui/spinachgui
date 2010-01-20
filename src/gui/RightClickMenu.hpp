#ifndef RIGHT_CLICK_MENU_H
#define RIGHT_CLICK_MENU_H

#include <wx/menu.h>
#include <shared/spinsys.hpp>

///Class for easily building the right click menu. Just construct it,
///and the required options, call build and show it. The menu should
///take care of ensuing the required actions are carried out.
class RightClickMenu : public wxMenu {
public:
  RightClickMenu(wxWindow* parent);

  void OptionDeleteSpin(SpinXML::Spin* spin) {mOptionDelete=spin;}
  void OptionShowSpinProperties(SpinXML::Spin* spin) {mOptionSpinProperties=spin;}

  //Event Handlers
  void OnShowSpinProperties(wxCommandEvent& e);
  void OnDeleteSpin(wxCommandEvent& e);

  ///Call to build the menu. inserts the menu items in a sensible
  ///order.
  void Build();
protected:
  DECLARE_EVENT_TABLE();
  wxWindow* mParent;

  SpinXML::Spin* mOptionDelete;
  SpinXML::Spin* mOptionSpinProperties;


};


#endif RIGHT_CLICK_MENU_H
