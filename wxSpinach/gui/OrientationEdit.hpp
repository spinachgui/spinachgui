
#ifndef ORIENTEDIT_H
#define ORIENTEDIT_H

#include <res/SpinachGUI.h>
#include <shared/spinsys.hpp>
#include <gui/DialogCombo.hpp>



class OrientEditPanel : public OrientEditPanelBase {
public:
  OrientEditPanel(wxWindow* parent,
		  wxWindowID id = wxID_ANY,
		  const wxPoint& pos = wxDefaultPosition,
		  const wxSize& size = wxDefaultSize,
		  long style = wxTAB_TRAVERSAL);

  void OnTextChange(wxCommandEvent& e);

protected:
  DECLARE_EVENT_TABLE();
private:

};

class OrientEditDialog : public OrientDialogBase {
public:
  OrientEditDialog(wxWindow* parent,wxWindowID id=-1);
private:
  OrientEditPanel* mPanel;
};

class OrientDialogCombo : public DialogCombo<OrientEditDialog> {
public:
  OrientDialogCombo(wxWindow* parent,wxWindowID id=-1);
protected:
  virtual OrientEditDialog* CreateDialog();
  virtual wxString GetStringFromDialog(OrientEditDialog* dlg);
private:
};

#endif
