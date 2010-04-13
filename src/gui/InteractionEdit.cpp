
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
      mInter->ToScalar();
  } else if(type==Interaction::MATRIX) {
      mInter->ToMatrix();
  } else if(type==Interaction::EIGENVALUES) {
      mInter->ToEigenvalues();
  } else if(type==Interaction::AXRHOM) {
      mInter->ToAxRhom();
      cout << "Conversion done" << endl;
  } else if(type==Interaction::SPANSKEW) {
      mInter->ToSpanSkew();
  }
  LoadFromInter();
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
  cout << "Done update sub type combo" << endl;
}

void InterEditPanel::LoadFromInter() {
    cout << "Load from Inter()" << endl;
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
  
  SpinSystem* ss=GetSS();
  long spinCount=ss->GetSpinCount();

  for(long i=0;i<spinCount;i++) {
    Spin* spin=ss->GetSpin(i);
    mSpin2Combo->Append(wxString() << i << wxT(" ") << wxString(spin->GetLabel(),wxConvUTF8),(void*)spin);
  }
  if(mInter->GetIsBilinear()) {
    long Spin2Number=GetSS()->GetSpinNumber(mInter->GetOtherSpin(mWithRespectTo));
    mSpin2Combo->SetSelection(Spin2Number);
  }
  cout << "About the start testing types" << endl;

  if(mInter->GetType()==Interaction::SCALAR) {
    energy scalar;
    mInter->GetScalar(&scalar);
    mScalarCtrl->SetValue(wxString() << scalar[MHz]);
    mTypeChoiceBook->SetSelection(0);
  } else if(mInter->GetType()==Interaction::MATRIX) {
    Matrix3e mat;
    mInter->GetMatrix(&mat);
    
    cout << "Trace=" << mat.Trace()[Hz] << "Hz" <<  endl;

    mMatXXCtrl->SetValue(wxString() << mat.Get(0,0)[MHz]);
    mMatXYCtrl->SetValue(wxString() << mat.Get(0,1)[MHz]);
    mMatXZCtrl->SetValue(wxString() << mat.Get(0,2)[MHz]);
                            
    mMatYXCtrl->SetValue(wxString() << mat.Get(1,0)[MHz]);
    mMatYYCtrl->SetValue(wxString() << mat.Get(1,1)[MHz]);
    mMatYZCtrl->SetValue(wxString() << mat.Get(1,2)[MHz]);
                                
    mMatZXCtrl->SetValue(wxString() << mat.Get(2,0)[MHz]);
    mMatZYCtrl->SetValue(wxString() << mat.Get(2,1)[MHz]);
    mMatZZCtrl->SetValue(wxString() << mat.Get(2,2)[MHz]);

    mTypeChoiceBook->SetSelection(1);
  } else if(mInter->GetType()==Interaction::EIGENVALUES) {
    energy xx,yy,zz;
    Orientation o;
    mInter->GetEigenvalues(&xx,&yy,&zz,&o);

    mEigenXXCtrl->SetValue(wxString() << xx[MHz]);
    mEigenYYCtrl->SetValue(wxString() << yy[MHz]);
    mEigenZZCtrl->SetValue(wxString() << zz[MHz]);
    
    mOrientEigenvalueCtrl->SetOrient(o);

    mTypeChoiceBook->SetSelection(2);

  } else if(mInter->GetType()==Interaction::AXRHOM) {
      cout << "type==axrhom" << endl;
    energy ax,rhom,iso;
    Orientation o;
    mInter->GetAxRhom(&iso,&ax,&rhom,&o);
    mAxCtrl->       SetValue(wxString() << ax[MHz]);
    mRhomCtrl->     SetValue(wxString() << rhom[MHz]);
    mAxRhomIsoCtrl->SetValue(wxString() << iso[MHz]);
    cout << "Set everything but the orientation" << endl;
    mOrientAxRhomCtrl->SetOrient(o);

    mTypeChoiceBook->SetSelection(3);
  } else if(mInter->GetType()==Interaction::SPANSKEW) {
    energy span,iso;
    double skew;
    Orientation o;
    mInter->GetSpanSkew(&iso,&span,&skew,&o);
    mSpanCtrl->       SetValue(wxString() << span[MHz]);
    mSkewCtrl->       SetValue(wxString() << skew);
    mSpanSkewIsoCtrl->SetValue(wxString() << iso[MHz]);

    mOrientSpanSkewCtrl->SetOrient(o);

    mTypeChoiceBook->SetSelection(4);
  } else {
      cerr << "Interaction.type was undefined" << endl;
  }

  mLoading=false;
}

void InterEditPanel::SaveToInter() {
  interChangeConnect.block();
  Interaction::Type type = TypeOrders[mTypeChoiceBook->GetSelection()];
  if(type==Interaction::SCALAR) {
    double scalar;
    mScalarCtrl->GetValue().ToDouble(&scalar);
    mInter->SetScalar(scalar*MHz);
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

    mInter->SetMatrix(Matrix3e(xx*MHz,xy*MHz,xz*MHz,
                               yx*MHz,yy*MHz,yz*MHz,
                               zx*MHz,zy*MHz,zz*MHz));

  } else if(type==Interaction::EIGENVALUES) {
    double xx,yy,zz;

    mEigenXXCtrl->GetValue().ToDouble(&xx);
    mEigenYYCtrl->GetValue().ToDouble(&yy);
    mEigenZZCtrl->GetValue().ToDouble(&zz);
 
    mInter->SetEigenvalues(xx*MHz,yy*MHz,zz*MHz,
                           mOrientEigenvalueCtrl->GetOrient());

  } else if(type==Interaction::AXRHOM) {
    double ax,rhom,iso;

    mAxCtrl->       GetValue().ToDouble(&ax);
    mRhomCtrl->     GetValue().ToDouble(&rhom);
    mAxRhomIsoCtrl->GetValue().ToDouble(&iso);

    mInter->SetAxRhom(iso*MHz,ax*MHz,rhom*MHz,
                      mOrientAxRhomCtrl->GetOrient());
  } else if(type==Interaction::SPANSKEW) {
    double span,skew,iso;

    mSpanCtrl->       GetValue().ToDouble(&span);
    mSkewCtrl->       GetValue().ToDouble(&skew);
    mSpanSkewIsoCtrl->GetValue().ToDouble(&iso);

    mInter->SetSpanSkew(iso*MHz,span*MHz,skew,
                        mOrientSpanSkewCtrl->GetOrient());
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
  cout << "OnSubTypeChange, st=" << Interaction::GetSubTypeName(st) << endl;
  Interaction::Form f=Interaction::GetFormFromSubType(st);

  if(f==Interaction::LINEAR || f==Interaction::QUADRATIC) {
    mInter->SetSubType(st,mWithRespectTo,NULL);
    cout << "setting subtype to a linear or quadratic form" << endl;
  } else {
    Spin* spin=(Spin*)mSpin2Combo->GetClientData(mSpin2Combo->GetSelection());
    cout << "setting spin 2 to " << spin << endl;
    mInter->SetSubType(st,mWithRespectTo,spin);
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

