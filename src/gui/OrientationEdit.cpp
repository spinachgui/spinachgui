#include <gui/OrientationEdit.hpp>

#include <iostream>

using namespace std;
using namespace SpinXML;

DEFINE_EVENT_TYPE(EVT_ORIENT_EDIT)

Orientation::Type TypeOrder[]={
  Orientation::EULER,
  Orientation::ANGLE_AXIS,
  Orientation::QUATERNION,
  Orientation::EIGENSYSTEM
};

OrientEditPanel::OrientEditPanel(wxWindow* parent,
		  wxWindowID id,
		  const wxPoint& pos,
		  const wxSize& size,
		  long style)
  : OrientEditPanelBase(parent,id,pos,size,style),mOrient(),mLoading(false) {
  Enable(false);
}

void OrientEditPanel::SetOrient(const Orientation& orient) {
  Enable(true);
  mOrient=orient;
  LoadFromOrient();
}

void OrientEditPanel::LoadFromOrient() {
  mLoading=true;
  switch(mOrient.GetType()) {
  case Orientation::EULER: {
    double alpha,beta,gamma;
    mOrient.GetEuler(&alpha,&beta,&gamma);
    mAlphaCtrl->SetValue(wxString() << alpha);
    mBetaCtrl-> SetValue(wxString() << beta);
    mGammaCtrl->SetValue(wxString() << gamma);

    mOrientTypeChoiceBook->SetSelection(0);
    break;
  }
  case Orientation::ANGLE_AXIS: {
    double angle;
    Vector3 axis;
    mOrient.GetAngleAxis(&angle,&axis);
    mAngleCtrl->SetValue(wxString() << angle);
    mXCtrl->SetValue(wxString() << axis.GetX());
    mYCtrl->SetValue(wxString() << axis.GetY());
    mZCtrl->SetValue(wxString() << axis.GetZ());

    mOrientTypeChoiceBook->SetSelection(1);
    break;
  }
  case Orientation::QUATERNION: {
    double re,i,j,k;
    mOrient.GetQuaternion(&re,&i,&j,&k);
    mReCtrl->SetValue(wxString() << re);
    mICtrl-> SetValue(wxString() << i);
    mJCtrl-> SetValue(wxString() << j);
    mKCtrl-> SetValue(wxString() << k);

    mOrientTypeChoiceBook->SetSelection(2);
    break;
  }
  case Orientation::EIGENSYSTEM: {
    Vector3 x,y,z;
    mOrient.GetEigenSystem(&x,&y,&z);

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
  case Orientation::UNDEFINED: {
    mAngleCtrl->SetValue(wxT("0.0"));
    mXCtrl->SetValue(wxT("0.0"));
    mYCtrl->SetValue(wxT("0.0"));
    mZCtrl->SetValue(wxT("1.0"));

  }  
  }
  mLoading=false;
}

void OrientEditPanel::SaveToOrient() {
  cout << "Saving" << endl;
  switch(mOrientTypeChoiceBook->GetSelection()) {
  case 0: {
    //Euler Angles
    double alpha,beta,gamma;
    mAlphaCtrl->GetValue().ToDouble(&alpha);
    mBetaCtrl ->GetValue().ToDouble(&beta);
    mGammaCtrl->GetValue().ToDouble(&gamma);

    mOrient.SetEuler(alpha,beta,gamma);
    break;
  }
  case 1: {
    //Angle Axis
    double angle,x,y,z;
    mAngleCtrl->GetValue().ToDouble(&angle);
    mXCtrl     ->GetValue().ToDouble(&x);
    mYCtrl    ->GetValue().ToDouble(&y);
    mZCtrl    ->GetValue().ToDouble(&z);

    mOrient.SetAngleAxis(angle,Vector3(x,y,z));
    break;
  }
  case 2: {
    //Quaternion
    double re,i,j,k;
    mReCtrl->GetValue().ToDouble(&re);
    mICtrl  ->GetValue().ToDouble(&i);
    mJCtrl ->GetValue().ToDouble(&j);
    mKCtrl ->GetValue().ToDouble(&k);

    mOrient.SetQuaternion(re,i,j,k);
    break;
  }
  case 3:{
    //Eigensystem
    double xx,xy,xz;
    double yx,yy,yz;
    double zx,zy,zz;

    mXXCtrl->GetValue().ToDouble(&xx);
    mXYCtrl->GetValue().ToDouble(&xy);
    mXZCtrl->GetValue().ToDouble(&xz);
         	     
    mYXCtrl->GetValue().ToDouble(&yx);
    mYYCtrl->GetValue().ToDouble(&yy);
    mYZCtrl->GetValue().ToDouble(&yz);
         	     
    mZXCtrl->GetValue().ToDouble(&zx);
    mZYCtrl->GetValue().ToDouble(&zy);
    mZZCtrl->GetValue().ToDouble(&zz);

    mOrient.SetEigenSystem(Vector3(xx,xy,xz),Vector3(yx,yy,yz),Vector3(zx,zy,zz));
    break;
    default:
      //Uh-oh, we have an undefined orientation for some reason
      cerr << "Trying to save back to the orientation, but could not determing the orientation's type" << endl;
  }
  }
}

void OrientEditPanel::OnTextChange(wxCommandEvent& e) {
  
}

void OrientEditPanel::OnPageChange(wxChoicebookEvent& e) {
  if(mLoading) {
    return;
  }
  switch(TypeOrder[e.GetSelection()]) {
  case Orientation::EULER:{
    mAlphaCtrl->SetValue(wxT("0.0"));
    mBetaCtrl-> SetValue(wxT("0.0"));
    mGammaCtrl->SetValue(wxT("0.0"));
  }
  case Orientation::ANGLE_AXIS: {
    mAngleCtrl->SetValue(wxT("0.0"));
    mXCtrl->SetValue(wxT("0.0"));
    mYCtrl->SetValue(wxT("0.0"));
    mZCtrl->SetValue(wxT("1.0"));

  }  
  case Orientation::QUATERNION: {
    mReCtrl->SetValue(wxT("1.0"));
    mICtrl-> SetValue(wxT("0.0"));
    mJCtrl-> SetValue(wxT("0.0"));
    mKCtrl-> SetValue(wxT("0.0"));
  }
  case Orientation::EIGENSYSTEM: {
    mXXCtrl->SetValue(wxT("1.0"));
    mXYCtrl->SetValue(wxT("0.0"));
    mXZCtrl->SetValue(wxT("0.0"));

    mYXCtrl->SetValue(wxT("0.0"));
    mYYCtrl->SetValue(wxT("1.0"));
    mYZCtrl->SetValue(wxT("0.0"));

    mZXCtrl->SetValue(wxT("0.0"));
    mZYCtrl->SetValue(wxT("0.0"));
    mZZCtrl->SetValue(wxT("1.0"));
  }
  }
}

BEGIN_EVENT_TABLE(OrientEditPanel,wxPanel)

EVT_CHOICEBOOK_PAGE_CHANGED(wxID_ANY,OrientEditPanel::OnPageChange)

END_EVENT_TABLE()


//============================================================//

OrientEditDialog::OrientEditDialog(wxWindow* parent,wxWindowID id) 
: OrientDialogBase(parent,id,wxT("Orientation")) {
  mOrientEditPanel=new OrientEditPanel(this);
  GetSizer()->Insert(0,mOrientEditPanel,1,wxEXPAND);
  Fit();
}

int OrientEditDialog::ShowModal() {
  int result=OrientDialogBase::ShowModal();
  if(result==wxID_OK) {
    mOrientEditPanel->SaveToOrient();
  } 
  return result;
}


void OrientEditDialog::OnApply(wxCommandEvent& e) {
  mOrientEditPanel->SaveToOrient(); 
}

BEGIN_EVENT_TABLE(OrientEditDialog,wxDialog)

EVT_BUTTON(wxID_APPLY,OrientEditDialog::OnApply)

END_EVENT_TABLE()


  //==========================================================//

OrientDialogCombo::OrientDialogCombo(wxWindow* parent,wxWindowID id) 
  : DialogCombo<OrientEditDialog>(parent,id),mOrient() {
  
}

void OrientDialogCombo::SetOrient(const Orientation& orient) {
  mOrient=orient;
  SetText(wxString(mOrient.ToString().c_str(),wxConvUTF8));
}

OrientEditDialog* OrientDialogCombo::CreateDialog() {
  OrientEditDialog* dlg=new OrientEditDialog(this);
  dlg->SetOrient(mOrient);
  return dlg;
}

void OrientDialogCombo::ReadDialog(OrientEditDialog* dlg) {
  mOrient=dlg->GetOrient();
}

void OrientDialogCombo::SetStringValue(const wxString& s) {
  
  return;
}

wxString OrientDialogCombo::GetStringFromDialog(OrientEditDialog* dlg) {
  mOrient=dlg->GetOrient();

  wxCommandEvent event(EVT_ORIENT_EDIT,GetId());
  event.SetEventObject(this);
  GetEventHandler()->ProcessEvent(event);

  cout << "Getting the string " << mOrient.ToString() << endl;
  return wxString(mOrient.ToString().c_str(),wxConvUTF8);
}
