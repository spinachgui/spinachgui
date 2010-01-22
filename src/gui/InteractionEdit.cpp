
#include <boost/shared_ptr.hpp>

#include <gui/InteractionEdit.hpp>
#include <gui/SpinachApp.hpp>

#include <iostream>
#include <map>

#include <wx/log.h>

#include <stdexcept>

using namespace std;
using namespace SpinXML;
using namespace sigc;

//============================================================//
// InterEditPanel

//Static constructory stuff

enum {
  SUBTYPE_COMBO,
  SPIN2_COMBO
};

const Interaction::SubType NuclearST[] =
  {Interaction::ST_EXCHANGE,   
   Interaction::ST_SHIELDING,  
   Interaction::ST_HFC,        
   Interaction::ST_DIPOLAR,    
   Interaction::ST_QUADRUPOLAR,
   Interaction::ST_CUSTOM_LINEAR,     
   Interaction::ST_CUSTOM_BILINEAR,     
   Interaction::ST_CUSTOM_QUADRATIC};
const long NuclearSTLen = 8;

const Interaction::SubType ElectronST[] = 
  {Interaction::ST_EXCHANGE,   
   Interaction::ST_G_TENSER,   
   Interaction::ST_DIPOLAR,    
   Interaction::ST_QUADRUPOLAR,
   Interaction::ST_ZFS,        
   Interaction::ST_CUSTOM_LINEAR,     
   Interaction::ST_CUSTOM_BILINEAR,     
   Interaction::ST_CUSTOM_QUADRATIC};
const long ElectronSTLen = 8;


Interaction::Type TypeOrders[]={
  Interaction::SCALAR,
  Interaction::MATRIX,
  Interaction::EIGENVALUES,
  Interaction::AXRHOM,
  Interaction::SPANSKEW
};

class STClientData : public wxClientData {
public:
  STClientData(Interaction::SubType st) 
    : mST(st){
  }
  Interaction::SubType GetData() {return mST;}
  private:
  Interaction::SubType mST;
};

InterEditPanel::InterEditPanel(wxWindow* parent,wxWindowID id)
  : InterEditPanelBase(parent,id),
    mInter(NULL),
    mLoading(false),
    mDialogMode(true) {

  Enable(false);

  mOrientEigenvalueCtrl = new OrientDialogCombo(mEigenEditPanel);
  mOrientAxRhomCtrl     = new OrientDialogCombo(mAxRhomEditPanel);
  mOrientSpanSkewCtrl   = new OrientDialogCombo(mSpanSkewEditPanel);

  mEigenEditPanel->   GetSizer()->Add(mOrientEigenvalueCtrl,1.0,wxALL);
  mAxRhomEditPanel->  GetSizer()->Add(mOrientAxRhomCtrl,1.0,wxALL);
  mSpanSkewEditPanel->GetSizer()->Add(mOrientSpanSkewCtrl,1.0,wxALL);

  mSubTypeCombo->SetId(SUBTYPE_COMBO);
  mSpin2Combo->SetId(SPIN2_COMBO);

  UpdateSubTypeCombo();
}							     
							     
void InterEditPanel::SetInter(Interaction* inter,Spin* withRespectTo) {
  interChangeConnect.disconnect();
  mWithRespectTo=withRespectTo;
  mInter=inter;
  Enable(inter != NULL);
  if(inter != NULL) {
    interChangeConnect=mInter->sigChange.connect(mem_fun(this,&InterEditPanel::OnInterChange));
    LoadFromInter();
  }
}


 
void InterEditPanel::OnPageChange(wxChoicebookEvent& e) {
  if (mLoading) {
    return;
  }
  Interaction::Type type=TypeOrders[e.GetSelection()];
  if(type==Interaction::SCALAR) {
    mScalarCtrl->SetValue(wxT("0.0"));
  } else if(type==Interaction::MATRIX) {
    mMatXXCtrl->SetValue(wxT("0.0"));
    mMatXYCtrl->SetValue(wxT("0.0"));
    mMatXZCtrl->SetValue(wxT("0.0"));
                           
    mMatYXCtrl->SetValue(wxT("0.0"));
    mMatYYCtrl->SetValue(wxT("0.0"));
    mMatYZCtrl->SetValue(wxT("0.0"));
                           
    mMatZXCtrl->SetValue(wxT("0.0"));
    mMatZYCtrl->SetValue(wxT("0.0"));
    mMatZZCtrl->SetValue(wxT("0.0"));
  } else if(type==Interaction::EIGENVALUES) {
    mEigenXXCtrl->SetValue(wxT("0.0"));
    mEigenYYCtrl->SetValue(wxT("0.0"));
    mEigenZZCtrl->SetValue(wxT("0.0"));

    Orientation o;
    o.SetQuaternion(1.0,0.0,0.0,0.0);
    mOrientEigenvalueCtrl->SetOrient(o);
  } else if(type==Interaction::AXRHOM) {
    mAxCtrl->       SetValue(wxT("0.0"));
    mRhomCtrl->     SetValue(wxT("0.0"));
    mAxRhomIsoCtrl->SetValue(wxT("0.0"));

    Orientation o;
    o.SetQuaternion(1.0,0.0,0.0,0.0);
    mOrientAxRhomCtrl->SetOrient(o);
  } else if(type==Interaction::SPANSKEW) {
    mSpanCtrl->       SetValue(wxT("0.0"));
    mSkewCtrl->       SetValue(wxT("0.0"));
    mSpanSkewIsoCtrl->SetValue(wxT("0.0"));

    Orientation o;
    o.SetQuaternion(1.0,0.0,0.0,0.0);
    mOrientSpanSkewCtrl->SetOrient(o);
  }
  SaveToInter();        
  e.Skip();
}

void InterEditPanel::UpdateSubTypeCombo(bool subtypeWarning) {
  //TODO: We need to test if the spin is an election, because then
  //slightly different options should become avaliable
  mSubTypeCombo->Clear();
  if(mInter==NULL) {
    return;
  }
  Interaction::SubType st = mInter->GetSubType();
  if(true) {//If nucleus
    for(long i=0;i<NuclearSTLen;i++) {
      mSubTypeCombo->Append(wxString(Interaction::GetSubTypeName(NuclearST[i]),wxConvUTF8),new STClientData(NuclearST[i]));
    }
  } else { //Else electron
    for(long i=0;i<ElectronSTLen;i++) {
      mSubTypeCombo->Append(wxString(Interaction::GetSubTypeName(ElectronST[i]),wxConvUTF8),new STClientData(ElectronST[i]));
    }
  }
  SetSubTypeSelection(st);
  STClientData* data=dynamic_cast<STClientData*>(mSubTypeCombo->GetClientObject(0));
  cout << Interaction::GetSubTypeName(data->GetData()) << endl;
}

void InterEditPanel::LoadFromInter() {
  mLoading=true;

  if(mInter->GetIsLinear()) {
	mSpin2Combo->Enable(false);
  } else if(mInter->GetIsBilinear()) {
	mSpin2Combo->Enable(true);
  } else {
	//Interaction is quadratic
	mSpin2Combo->Enable(false);
  }
  UpdateSubTypeCombo();

  //Populate the spin 2 combobox with every other spin
  mSpin2Combo->Clear();
  
  SpinSysManager* SSMgr=wxGetApp().GetSpinSysManager();
  const SpinSysPtr* head=SSMgr->Get();

  long spinCount=(*head)->GetSpinCount();

  for(long i=0;i<spinCount;i++) {
    Spin* spin=(*head)->GetSpin(i);
    mSpin2Combo->Append(wxString() << i << wxT(" ") << wxString(spin->GetLabel(),wxConvUTF8),(void*)spin);
  }
  if(mInter->GetIsBilinear()) {
    long Spin2Number=GetSS()->GetSpinNumber(mInter->GetOtherSpin(mWithRespectTo));
    mSpin2Combo->SetSelection(Spin2Number);
  }

  if(mInter->GetType()==Interaction::SCALAR) {
    double scalar;
    mInter->GetScalar(&scalar);
    mScalarCtrl->SetValue(wxString() << scalar);
    mTypeChoiceBook->SetSelection(0);
  } else if(mInter->GetType()==Interaction::MATRIX) {
    Matrix3 mat;
    mInter->GetMatrix(&mat);

    mMatXXCtrl->SetValue(wxString() << mat.Get(0,0));
    mMatXYCtrl->SetValue(wxString() << mat.Get(0,1));
    mMatXZCtrl->SetValue(wxString() << mat.Get(0,2));
                            
    mMatYXCtrl->SetValue(wxString() << mat.Get(1,0));
    mMatYYCtrl->SetValue(wxString() << mat.Get(1,1));
    mMatYZCtrl->SetValue(wxString() << mat.Get(1,2));
                                
    mMatZXCtrl->SetValue(wxString() << mat.Get(2,0));
    mMatZYCtrl->SetValue(wxString() << mat.Get(2,1));
    mMatZZCtrl->SetValue(wxString() << mat.Get(2,2));

    mTypeChoiceBook->SetSelection(1);
  } else if(mInter->GetType()==Interaction::EIGENVALUES) {
    double xx,yy,zz;
    Orientation o;
    mInter->GetEigenvalues(&xx,&yy,&zz,&o);

    mEigenXXCtrl->SetValue(wxString() << xx);
    mEigenYYCtrl->SetValue(wxString() << yy);
    mEigenZZCtrl->SetValue(wxString() << zz);
    
    mOrientEigenvalueCtrl->SetOrient(o);

    mTypeChoiceBook->SetSelection(2);

  } else if(mInter->GetType()==Interaction::AXRHOM) {
    double ax,rhom,iso;
    Orientation o;
    mInter->GetAxRhom(&ax,&rhom,&iso,&o);
    mAxCtrl->       SetValue(wxString() <<ax);
    mRhomCtrl->     SetValue(wxString() << rhom);
    mAxRhomIsoCtrl->SetValue(wxString() << iso);

    mOrientAxRhomCtrl->SetOrient(o);

    mTypeChoiceBook->SetSelection(3);
  } else if(mInter->GetType()==Interaction::SPANSKEW) {
    double span,skew,iso;
    Orientation o;
    mInter->GetSpanSkew(&span,&skew,&iso,&o);
    mSpanCtrl->       SetValue(wxString() << span);
    mSkewCtrl->       SetValue(wxString() << skew);
    mSpanSkewIsoCtrl->SetValue(wxString() << iso);

    mOrientSpanSkewCtrl->SetOrient(o);

    mTypeChoiceBook->SetSelection(4);
  }

  mLoading=false;
}

void InterEditPanel::SaveToInter() {
  interChangeConnect.block();
  Interaction::Type type = TypeOrders[mTypeChoiceBook->GetSelection()];
  if(type==Interaction::SCALAR) {
    double scalar;
    mScalarCtrl->GetValue().ToDouble(&scalar);
    mInter->SetScalar(scalar);
  } else if(type==Interaction::MATRIX) {
    double xx,xy,xz;
    double yx,yy,yz;
    double zx,zy,zz;
    mMatXXCtrl->GetValue().ToDouble(&xx);
    mMatXYCtrl->GetValue().ToDouble(&xy);
    mMatXZCtrl->GetValue().ToDouble(&xz);
                         
    mMatYXCtrl->GetValue().ToDouble(&yx);
    mMatYYCtrl->GetValue().ToDouble(&yy);
    mMatYZCtrl->GetValue().ToDouble(&yz);
                         
    mMatZXCtrl->GetValue().ToDouble(&zx);
    mMatZYCtrl->GetValue().ToDouble(&zy);
    mMatZZCtrl->GetValue().ToDouble(&zz);

    mInter->SetMatrix(Matrix3(xx,xy,xz,
			      yx,yy,yz,
			      zx,zy,zz));

  } else if(type==Interaction::EIGENVALUES) {
    double xx,yy,zz;

    mEigenXXCtrl->GetValue().ToDouble(&xx);
    mEigenYYCtrl->GetValue().ToDouble(&yy);
    mEigenZZCtrl->GetValue().ToDouble(&zz);
 
    mInter->SetEigenvalues(xx,yy,zz,mOrientEigenvalueCtrl->GetOrient());

  } else if(type==Interaction::AXRHOM) {
    double ax,rhom,iso;

    mAxCtrl->       GetValue().ToDouble(&ax);
    mRhomCtrl->     GetValue().ToDouble(&rhom);
    mAxRhomIsoCtrl->GetValue().ToDouble(&iso);

    mInter->SetAxRhom(ax,rhom,iso,mOrientAxRhomCtrl->GetOrient());
  } else if(type==Interaction::SPANSKEW) {
    double span,skew,iso;

    mSpanCtrl->       GetValue().ToDouble(&span);
    mSkewCtrl->       GetValue().ToDouble(&skew);
    mSpanSkewIsoCtrl->GetValue().ToDouble(&iso);

    mInter->SetSpanSkew(span,skew,iso,mOrientSpanSkewCtrl->GetOrient());
  }
  interChangeConnect.unblock();
}


void InterEditPanel::onTextChange(wxCommandEvent& e) {
  if(mLoading) {  //If we are loading from a spin we should ignore
		  //this event
    return;
  }
  sigChange();
  SaveToInter();
  return;
}


void InterEditPanel::OnSubTypeChange(wxCommandEvent& e) {
  STClientData* data=dynamic_cast<STClientData*>(mSubTypeCombo->GetClientObject(mSubTypeCombo->GetSelection()));
  if(data==NULL) {
    cerr << "data==NULL" << endl;
    return;
  }
  Interaction::SubType st=data->GetData();
  Interaction::Form f=Interaction::GetFormFromSubType(st);

  if(f==Interaction::LINEAR || f==Interaction::QUADRATIC) {
    mInter->SetSubType(st,mWithRespectTo,NULL);
  } else {
    mInter->SetSubType(st,mWithRespectTo,(Spin*)mSpin2Combo->GetClientData());
  }

  sigChange();
}


void InterEditPanel::SetSubTypeSelection(SpinXML::Interaction::SubType st) {
  for(long i=0;i<NuclearSTLen;i++) {
    if(st==NuclearST[i]) {
      mSubTypeCombo->SetSelection(i);
    }
  }
}

void InterEditPanel::OnOrientChange(wxCommandEvent& e){
  SaveToInter();
}


BEGIN_EVENT_TABLE(InterEditPanel,wxPanel)

EVT_CHOICEBOOK_PAGE_CHANGED(wxID_ANY,     InterEditPanel::OnPageChange)
EVT_CHOICE                 (SUBTYPE_COMBO,InterEditPanel::OnSubTypeChange)
EVT_CHOICE                 (SPIN2_COMBO,  InterEditPanel::OnSubTypeChange)
EVT_TEXT                   (wxID_ANY,     InterEditPanel::onTextChange)
EVT_COMMAND                (wxID_ANY,EVT_ORIENT_EDIT,InterEditPanel::OnOrientChange)

END_EVENT_TABLE()

