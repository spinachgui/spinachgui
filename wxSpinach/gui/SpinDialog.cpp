
#include <gui/SpinDialog.hpp>
#include <shared/nuclear_data.hpp>

#include <string>
#include <wx/log.h>

using namespace SpinXML;

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

  LoadFromSpin();
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

  //mInterEdit->SaveToSpin();
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

  //mInterEdit->LoadFromSpin();
}



//self.Bind(wx.EVT_BUTTON,self.onApply,id=wx.ID_APPLY);


