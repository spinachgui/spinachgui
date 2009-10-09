
#include <gui/SpinDialog.hpp>
#include <gui/EventSystem.hpp>
#include <gui/SpinachApp.hpp>
#include <shared/nuclear_data.hpp>
#include <gui/SpinSysManager.hpp>

#include <string>
#include <wx/log.h>

using namespace SpinXML;

const CEventType EV_SSCHANGE("SSCHANGE");
const CEventType EV_SCHANGE("SCHANGE");

enum {
  ID_ELEMENT
};

SpinDialog::SpinDialog(wxWindow* parent,Spin* spin,wxWindowID id) : SpinDialogBase(parent,id),mSpin(spin) {
  mInterEdit=new SpinInterEditPanel(mSpinDialogPanel,spin);

  wxSizer* interSizer=mSpinDialogPanel->GetSizer()->GetItem(2)->GetSizer();
  interSizer->Add(mInterEdit,1,wxEXPAND | wxALL);

  GetSizer()->Fit(this);

  //Populate the element combo

  for(long i=0;i<getElementCount();i++) {
    mElementCombo->Append(wxString() <<
			  i <<
			  wxT(" ") <<
			  wxString(getElementSymbol(i),wxConvUTF8) <<
			  wxT(" - ") <<
			  wxString(getElementName(i),wxConvUTF8));
  }

  mElementCombo->SetId(ID_ELEMENT);
  LoadFromSpin();
}

void SpinDialog::UpdateIsotopeDropDown() {
  long p=mElementCombo->GetSelection();

  mIsotopeCombo->Clear();
  for(long i=0;i<getIsotopeCount(p);i++) {
    mIsotopeCombo->Append(wxString(getElementSymbol(p),wxConvUTF8) <<
			  wxT("(") <<
			  (getNeutrons(p,i) + p)<<
			  wxT(")"));
  }
  if(getIsotopeCount(p)>0) {
    mIsotopeCombo->SetSelection(0);
  }
}
 
void SpinDialog::OnApply(wxCommandEvent& e) {
  SaveToSpin();
}

int SpinDialog::ShowModal() {
  long result=wxDialog::ShowModal();
  if(result==wxID_OK) {
    SaveToSpin();
  }
  return result;
}

void SpinDialog::SaveToSpin() {
  double x,y,z;
  double parseError=false;
  parseError = parseError || !mXPosCtrl->GetValue().ToDouble(&x);
  parseError = parseError || !mYPosCtrl->GetValue().ToDouble(&y);
  parseError = parseError || !mZPosCtrl->GetValue().ToDouble(&z);
  //parseError=true if any of the above functions failed.
  if(parseError) {
    wxLogError(wxT("Failed to parse a floating point number"));
    return;
  }
    
  mSpin->SetCoordinates(x,y,z);

  wxString label=mSpinTitle->GetValue();
  mSpin->SetLabel(string(label.mb_str()));


  mSpin->SetElement(mElementCombo->GetSelection());
  CEventManager::Instance()->trigger(EvtSChange((*(wxGetApp().GetSpinSysManager()->Get()))->GetSpinNumber(mSpin)));
}


void SpinDialog::LoadFromSpin() {
  double x,y,z;
  mSpin->GetCoordinates(&x,&y,&z);
  wxString label=wxString(mSpin->GetLabel(),wxConvUTF8);

  mSpinTitle->SetValue(label);

  wxString strX,strY,strZ;
  strX << x;
  strY << y;
  strZ << z;

  mXPosCtrl->SetValue(strX);
  mYPosCtrl->SetValue(strY);
  mZPosCtrl->SetValue(strZ);

  mElementCombo->SetSelection(mSpin->GetElement());

  UpdateIsotopeDropDown();
  mIsotopeCombo->SetSelection(0);
}

void SpinDialog::OnElementChange(wxCommandEvent& e) {
  UpdateIsotopeDropDown();
}


BEGIN_EVENT_TABLE(SpinDialog,wxDialog) 

EVT_CHOICE(ID_ELEMENT,SpinDialog::OnElementChange)

END_EVENT_TABLE()



//self.Bind(wx.EVT_BUTTON,self.onApply,id=wx.ID_APPLY);


