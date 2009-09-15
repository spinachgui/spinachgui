#include <gui/OrientationEdit.hpp>

using namespace SpinXML;

OrientEditPanel::OrientEditPanel(wxWindow* parent,
		  wxWindowID id,
		  const wxPoint& pos,
		  const wxSize& size,
		  long style)
  : OrientEditPanelBase(parent,id,pos,size,style),mOrient(NULL) {
  Enable(false);
}

void OrientEditPanel::SetOrient(SpinXML::Orientation* orient) {
  Enable(orient != NULL);
  mOrient=orient;
  if(orient != NULL) { 
    LoadFromOrient();
  }
}

void OrientEditPanel::LoadFromOrient() {
  switch(mOrient->GetType()) {
  case Orientation::EULER: {
    double alpha,beta,gamma;
    mOrient->GetEuler(&alpha,&beta,&gamma);
    mAlphaCtrl->SetValue(wxString() << alpha);
    mBetaCtrl-> SetValue(wxString() << beta);
    mGammaCtrl->SetValue(wxString() << gamma);

    mOrientTypeChoiceBook->SetSelection(0);
    break;
  }
  case Orientation::ANGLE_AXIS: {
    double angle;
    Vector3 axis;
    mOrient->GetAngleAxis(&angle,&axis);
    mAngleCtrl->SetValue(wxString() << angle);
    mXCtrl->SetValue(wxString() << axis.GetX());
    mYCtrl->SetValue(wxString() << axis.GetY());
    mZCtrl->SetValue(wxString() << axis.GetZ());

    mOrientTypeChoiceBook->SetSelection(1);
    break;
  }
  case Orientation::QUATERNION: {
    double re,i,j,k;
    mOrient->GetQuaternion(&re,&i,&j,&k);
    mReCtrl->SetValue(wxString() << re);
    mICtrl-> SetValue(wxString() << i);
    mJCtrl-> SetValue(wxString() << j);
    mKCtrl-> SetValue(wxString() << k);

    mOrientTypeChoiceBook->SetSelection(2);
    break;
  }
  case Orientation::EIGENSYSTEM: {
    Vector3 x,y,z;
    mOrient->GetEigenSystem(&x,&y,&z);

    mXXCtrl->SetValue(wxString() << x.GetX());
    mXYCtrl->SetValue(wxString() << x.GetY());
    mXZCtrl->SetValue(wxString() << x.GetZ());

    mYXCtrl->SetValue(wxString() << y.GetX());
    mYYCtrl->SetValue(wxString() << y.GetY());
    mYZCtrl->SetValue(wxString() << y.GetZ());

    mZXCtrl->SetValue(wxString() << z.GetX());
    mZYCtrl->SetValue(wxString() << z.GetY());
    mZZCtrl->SetValue(wxString() << z.GetZ());

    mOrientTypeChoiceBook->SetSelection(3);
    break;
  }
  }
}

void OrientEditPanel::SaveToOrient() {
  switch(mOrientTypeChoiceBook->GetSelection()) {
  case 0:
    //Euler Angles
    break;
  case 1:
    //Angle Axis
    break;
  case 2:
    //Quaternion
    break;
  case 3:
    //Eigensystem
    break;
  }
}

void OrientEditPanel::OnTextChange(wxCommandEvent& e) {
  
}

void OrientEditPanel::OnPageChange(wxChoicebookEvent& e) {

}

BEGIN_EVENT_TABLE(OrientEditPanel,wxPanel)


END_EVENT_TABLE()


//============================================================//

OrientEditDialog::OrientEditDialog(wxWindow* parent,wxWindowID id) 
: OrientDialogBase(parent,id,wxT("Orientation")) {

}

void OrientEditDialog::SetOrient(Orientation* orient) {
  mOrientEditPanel->SetOrient(orient);
} 


//==========================================================//

OrientDialogCombo::OrientDialogCombo(wxWindow* parent,wxWindowID id) 
  : DialogCombo<OrientEditDialog>(parent,id),mOrient(NULL) {
  
}

OrientEditDialog* OrientDialogCombo::CreateDialog() {
  OrientEditDialog* dlg=new OrientEditDialog(this);
  dlg->SetOrient(mOrient);
  return dlg;
}
wxString OrientDialogCombo::GetStringFromDialog(OrientEditDialog* dlg) {
  return wxString(wxT("Orientation"));
}
