#include <gui/OrientationEdit.hpp>

OrientEditPanel::OrientEditPanel(wxWindow* parent,
		  wxWindowID id,
		  const wxPoint& pos,
		  const wxSize& size,
		  long style)
  : OrientEditPanelBase(parent,id,pos,size,style) {

}

BEGIN_EVENT_TABLE(OrientEditPanel,wxPanel)


END_EVENT_TABLE()


//============================================================//

OrientEditDialog::OrientEditDialog(wxWindow* parent,wxWindowID id) 
: OrientDialogBase(parent,id,wxT("Orientation")) {
  /*mPanel = new OrientEditPanel(this);
  wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
  sizer->Add(mPanel,1,wxEXPAND);
  SetSizer(sizer);*/
}

//==========================================================//

OrientDialogCombo::OrientDialogCombo(wxWindow* parent,wxWindowID id) 
  : DialogCombo<OrientEditDialog>(parent,id) {

}

OrientEditDialog* OrientDialogCombo::CreateDialog() {
  return new OrientEditDialog(this);
}
wxString OrientDialogCombo::GetStringFromDialog(OrientEditDialog* dlg) {
  return wxString(wxT("Orientation"));
}
