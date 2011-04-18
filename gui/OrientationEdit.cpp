#include <gui/OrientationEdit.hpp>

#include <iostream>
#include <wx/log.h>
#include <shared/basic_math.hpp>

using namespace std;
using namespace SpinXML;

DEFINE_EVENT_TYPE(EVT_ORIENT_EDIT)

Orientation::Type TypeOrder[]={
    Orientation::EULER,
    Orientation::ANGLE_AXIS,
    Orientation::QUATERNION,
    Orientation::DCM
};

OrientEditPanel::OrientEditPanel(wxWindow* parent,
                                 wxWindowID id,
                                 const wxPoint& pos,
                                 const wxSize& size,
                                 long style)
    : OrientEditPanelBase(parent,id,pos,size,style),mLoading(false),mDirty(false) {
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
    case Orientation::EULER:
        mOrientTypeChoiceBook->SetSelection(0);
        break;
    case Orientation::ANGLE_AXIS:
        mOrientTypeChoiceBook->SetSelection(1);
        break;
    case Orientation::QUATERNION:
        mOrientTypeChoiceBook->SetSelection(2);
        break;
    case Orientation::DCM:
        mOrientTypeChoiceBook->SetSelection(3);
        break;
    }
    switch(mOrient.GetType()) {
    case Orientation::EULER: {
        double alpha,beta,gamma;
        mOrient.GetEuler(&alpha,&beta,&gamma);
        mAlphaCtrl->SetValue(wxString() << alpha);
        mBetaCtrl-> SetValue(wxString() << beta);
        mGammaCtrl->SetValue(wxString() << gamma);
        break;
    }
    case Orientation::ANGLE_AXIS: {
        double angle;
        Vector3d axis;
        mOrient.GetAngleAxis(&angle,&axis);
        mAngleCtrl->SetValue(wxString() << angle);
        mXCtrl->SetValue(wxString() << axis.x());
        mYCtrl->SetValue(wxString() << axis.y());
        mZCtrl->SetValue(wxString() << axis.z());
        break;
    }
    case Orientation::QUATERNION: {
        double re,i,j,k;
        mOrient.GetQuaternion(&re,&i,&j,&k);
        mReCtrl->SetValue(wxString() << re);
        mICtrl-> SetValue(wxString() << i);
        mJCtrl-> SetValue(wxString() << j);
        mKCtrl-> SetValue(wxString() << k);
        break;
    }
    case Orientation::DCM: {
        Matrix3d mat;
        mOrient.GetDCM(&mat);

        mXXCtrl->SetValue(wxString() << mat(0,0));
        mXYCtrl->SetValue(wxString() << mat(0,1));
        mXZCtrl->SetValue(wxString() << mat(0,2));
             	     
        mYXCtrl->SetValue(wxString() << mat(1,0));
        mYYCtrl->SetValue(wxString() << mat(1,1));
        mYZCtrl->SetValue(wxString() << mat(1,2));
             	     
        mZXCtrl->SetValue(wxString() << mat(2,0));
        mZYCtrl->SetValue(wxString() << mat(2,1));
        mZZCtrl->SetValue(wxString() << mat(2,2));
        break;
    }
    }
    mDirty=false;
    mLoading=false;
}

void OrientEditPanel::SaveToOrient() {
    mDirty=false;
    cout << "Saving" << endl;
    switch(mOrientTypeChoiceBook->GetSelection()) {
    case 0: {
        //Euler Angles
        double alpha,beta,gamma;
        mAlphaCtrl->GetValue().ToDouble(&alpha);
        mBetaCtrl ->GetValue().ToDouble(&beta);
        mGammaCtrl->GetValue().ToDouble(&gamma);

        mOrient = EulerAngles(alpha,beta,gamma);
        break;
    }
    case 1: {
        //Angle Axis
        double angle,x,y,z;
        mAngleCtrl->GetValue().ToDouble(&angle);
        mXCtrl     ->GetValue().ToDouble(&x);
        mYCtrl    ->GetValue().ToDouble(&y);
        mZCtrl    ->GetValue().ToDouble(&z);

        mOrient = AngleAxisd(angle,Vector3d(x,y,z));
        break;
    }
    case 2: {
        //Quaternion
        double re,i,j,k;
        mReCtrl->GetValue().ToDouble(&re);
        mICtrl  ->GetValue().ToDouble(&i);
        mJCtrl ->GetValue().ToDouble(&j);
        mKCtrl ->GetValue().ToDouble(&k);

        mOrient = Quaterniond(re,i,j,k);
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

        mOrient = MakeMatrix3d(xx,xy,xz,
							   yx,yy,yz,
							   zx,zy,zz);
        break;
        default:
            //Uh-oh, we have an undefined orientation for some reason
            cerr << "Trying to save back to the orientation, but could not determing the orientation's type" << endl;
    }
    }
    mOrient.Normalize();
}

void OrientEditPanel::OnTextChange(wxCommandEvent& e) {
    mDirty=true;
}

void OrientEditPanel::OnPageChange(wxChoicebookEvent& e) {
    if(mLoading) {
        return;
    }
    //First we need to save any changes the user made to the orientation
    if(mDirty) SaveToOrient();  //Only save changes if the user has
                                //made changes, otherwise we are
                                //throwing away precision.
    switch(TypeOrder[e.GetSelection()]) {
    case Orientation::EULER:{
        mOrient=mOrient.GetAsEuler();
        break;
    }
    case Orientation::ANGLE_AXIS: {
        mOrient=mOrient.GetAsAngleAxis();
        break;
    }  
    case Orientation::QUATERNION: {
        mOrient=mOrient.GetAsQuaternion();
        break;
    }
    case Orientation::DCM: {
        mOrient=mOrient.GetAsDCM();
        break;
    }
    }
    LoadFromOrient();
}

BEGIN_EVENT_TABLE(OrientEditPanel,wxPanel)

EVT_CHOICEBOOK_PAGE_CHANGING(wxID_ANY,OrientEditPanel::OnPageChange)

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
: DialogCombo<OrientEditDialog>(parent,id),mOrient(Quaterniond(1,0,0,0)) {
  
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
