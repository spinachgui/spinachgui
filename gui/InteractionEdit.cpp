
#include <boost/shared_ptr.hpp>

#include <gui/InteractionEdit.hpp>
#include <gui/SpinachApp.hpp>

#include <shared/basic_math.hpp>

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
  TYPE_COMBO,
  SPIN2_COMBO
};

const Interaction::Type NuclearT[] =
  {Interaction::EXCHANGE,   
   Interaction::SHIELDING,  
   Interaction::HFC,        
   Interaction::DIPOLAR,    
   Interaction::QUADRUPOLAR,
   Interaction::CUSTOM_LINEAR,     
   Interaction::CUSTOM_BILINEAR,     
   Interaction::CUSTOM_QUADRATIC};
const long NuclearSTLen = 8;

const Interaction::Type ElectronT[] = 
  {Interaction::EXCHANGE,   
   Interaction::G_TENSER,   
   Interaction::DIPOLAR,    
   Interaction::QUADRUPOLAR,
   Interaction::ZFS,        
   Interaction::CUSTOM_LINEAR,     
   Interaction::CUSTOM_BILINEAR,     
   Interaction::CUSTOM_QUADRATIC};
const long ElectronSTLen = 8;


Interaction::Storage StorageOrders[]={
  Interaction::STORAGE_SCALAR,
  Interaction::MATRIX,
  Interaction::EIGENVALUES,
  Interaction::AXRHOM,
  Interaction::SPANSKEW
};

class STClientData : public wxClientData {
public:
  STClientData(Interaction::Type t) 
    : mT(t){
  }
  Interaction::Type GetData() {return mT;}
  private:
  Interaction::Type mT;
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

  mSubTypeCombo->SetId(TYPE_COMBO);
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
  Interaction::Storage storage=StorageOrders[e.GetSelection()];
  if(storage==Interaction::STORAGE_SCALAR) {
      mInter->ToScalar();
  } else if(storage==Interaction::MATRIX) {
      mInter->ToMatrix();
  } else if(storage==Interaction::EIGENVALUES) {
      mInter->ToEigenvalues();
  } else if(storage==Interaction::AXRHOM) {
      mInter->ToAxRhom();
      cout << "Conversion done" << endl;
  } else if(storage==Interaction::SPANSKEW) {
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
  Interaction::Type t = mInter->GetType();
  if(true) {//If nucleus
    for(long i=0;i<NuclearSTLen;i++) {
      mSubTypeCombo->Append(wxString(Interaction::GetTypeName(NuclearT[i]),wxConvUTF8),new STClientData(NuclearT[i]));
    }
  } else { //Else electron
    for(long i=0;i<ElectronSTLen;i++) {
      mSubTypeCombo->Append(wxString(Interaction::GetTypeName(ElectronT[i]),wxConvUTF8),new STClientData(ElectronT[i]));
    }
  }
  SetTypeSelection(t);
  STClientData* data=dynamic_cast<STClientData*>(mSubTypeCombo->GetClientObject(0));
  cout << Interaction::GetTypeName(data->GetData()) << endl;
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
  
	long spinCount=GetSS().GetSpinCount();

	for(long i=0;i<spinCount;i++) {
		SpinView spin=GetSS().GetSpin(i);
		mSpin2Combo->Append(wxString() << i << wxT(" ") << wxString(spin.GetLabel(),wxConvUTF8),(void*)spin.Get());
	}
	if(mInter->GetIsBilinear()) {
		long Spin2Number=GetSS()->GetSpinNumber(mInter->GetOtherSpin(mWithRespectTo));
		mSpin2Combo->SetSelection(Spin2Number);
	}
	cout << "About the start testing types" << endl;

	if(mInter->GetStorage()==Interaction::STORAGE_SCALAR) {
		energy scalar;
		mInter->GetScalar(&scalar);
		mScalarCtrl->SetValue(wxString() << scalar * MHz);
		mTypeChoiceBook->SetSelection(0);
	} else if(mInter->GetStorage()==Interaction::MATRIX) {
		Matrix3d mat;
		mInter->GetMatrix(&mat);
    
		mMatXXCtrl->SetValue(wxString() << mat(0,0) * MHz);
		mMatXYCtrl->SetValue(wxString() << mat(0,1) * MHz);
		mMatXZCtrl->SetValue(wxString() << mat(0,2) * MHz);
                            
		mMatYXCtrl->SetValue(wxString() << mat(1,0) * MHz);
		mMatYYCtrl->SetValue(wxString() << mat(1,1) * MHz);
		mMatYZCtrl->SetValue(wxString() << mat(1,2) * MHz);
                                
		mMatZXCtrl->SetValue(wxString() << mat(2,0) * MHz);
		mMatZYCtrl->SetValue(wxString() << mat(2,1) * MHz);
		mMatZZCtrl->SetValue(wxString() << mat(2,2) * MHz);

		mTypeChoiceBook->SetSelection(1);
	} else if(mInter->GetStorage()==Interaction::EIGENVALUES) {
		energy xx,yy,zz;
		Orientation o(Quaterniond(1,0,0,0));
		mInter->GetEigenvalues(&xx,&yy,&zz,&o);

		mEigenXXCtrl->SetValue(wxString() << xx * MHz);
		mEigenYYCtrl->SetValue(wxString() << yy * MHz);
		mEigenZZCtrl->SetValue(wxString() << zz * MHz);
    
		mOrientEigenvalueCtrl->SetOrient(o);

		mTypeChoiceBook->SetSelection(2);

	} else if(mInter->GetStorage()==Interaction::AXRHOM) {
		cout << "type==axrhom" << endl;
		energy ax,rhom,iso;
		Orientation o(Quaterniond(1,0,0,0));
		mInter->GetAxRhom(&iso,&ax,&rhom,&o);
		mAxCtrl->       SetValue(wxString() << ax   * MHz);
		mRhomCtrl->     SetValue(wxString() << rhom * MHz);
		mAxRhomIsoCtrl->SetValue(wxString() << iso  * MHz);
		cout << "Set everything but the orientation" << endl;
		mOrientAxRhomCtrl->SetOrient(o);

		mTypeChoiceBook->SetSelection(3);
	} else if(mInter->GetStorage()==Interaction::SPANSKEW) {
		energy span,iso;
		double skew;
		Orientation o(Quaterniond(1,0,0,0));
		mInter->GetSpanSkew(&iso,&span,&skew,&o);
		mSpanCtrl->       SetValue(wxString() << span * MHz);
		mSkewCtrl->       SetValue(wxString() << skew);
		mSpanSkewIsoCtrl->SetValue(wxString() << iso  * MHz);

		mOrientSpanSkewCtrl->SetOrient(o);

		mTypeChoiceBook->SetSelection(4);
	} else {
		cerr << "Interaction.type was undefined" << endl;
	}

	mLoading=false;
}

void InterEditPanel::SaveToInter() {
  interChangeConnect.block();
  Interaction::Storage storage = StorageOrders[mTypeChoiceBook->GetSelection()];
  if(storage==Interaction::STORAGE_SCALAR) {
    double scalar;
    mScalarCtrl->GetValue().ToDouble(&scalar);
    mInter->SetScalar(scalar*MHz);
  } else if(storage==Interaction::MATRIX) {
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

    mInter->SetMatrix(MakeMatrix3d(xx*MHz,xy*MHz,xz*MHz,
								   yx*MHz,yy*MHz,yz*MHz,
								   zx*MHz,zy*MHz,zz*MHz));

  } else if(storage==Interaction::EIGENVALUES) {
    double xx,yy,zz;

    mEigenXXCtrl->GetValue().ToDouble(&xx);
    mEigenYYCtrl->GetValue().ToDouble(&yy);
    mEigenZZCtrl->GetValue().ToDouble(&zz);
 
    mInter->SetEigenvalues(xx*MHz,yy*MHz,zz*MHz,
                           mOrientEigenvalueCtrl->GetOrient());

  } else if(storage==Interaction::AXRHOM) {
    double ax,rhom,iso;

    mAxCtrl->       GetValue().ToDouble(&ax);
    mRhomCtrl->     GetValue().ToDouble(&rhom);
    mAxRhomIsoCtrl->GetValue().ToDouble(&iso);

    mInter->SetAxRhom(iso*MHz,ax*MHz,rhom*MHz,
                      mOrientAxRhomCtrl->GetOrient());
  } else if(storage==Interaction::SPANSKEW) {
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
  Interaction::Type t=data->GetData();
  cout << "OnSubTypeChange, st=" << Interaction::GetTypeName(t) << endl;
  Interaction::Form f=Interaction::GetFormFromType(t);

  if(f==Interaction::LINEAR || f==Interaction::QUADRATIC) {
    mInter->SetType(t,mWithRespectTo,NULL);
    cout << "setting subtype to a linear or quadratic form" << endl;
  } else {
    Spin* spin=(Spin*)mSpin2Combo->GetClientData(mSpin2Combo->GetSelection());
    cout << "setting spin 2 to " << spin << endl;
    mInter->SetType(t,mWithRespectTo,spin);
  }

  sigChange();
}


void InterEditPanel::SetTypeSelection(SpinXML::Interaction::Type t) {
  for(long i=0;i<NuclearSTLen;i++) {
    if(t==NuclearT[i]) {
      mSubTypeCombo->SetSelection(i);
    }
  }
}

void InterEditPanel::OnOrientChange(wxCommandEvent& e){
  SaveToInter();
}


BEGIN_EVENT_TABLE(InterEditPanel,wxPanel)

EVT_CHOICEBOOK_PAGE_CHANGED(wxID_ANY,     InterEditPanel::OnPageChange)
EVT_CHOICE                 (TYPE_COMBO,InterEditPanel::OnSubTypeChange)
EVT_CHOICE                 (SPIN2_COMBO,  InterEditPanel::OnSubTypeChange)
EVT_TEXT                   (wxID_ANY,     InterEditPanel::onTextChange)
EVT_COMMAND                (wxID_ANY,EVT_ORIENT_EDIT,InterEditPanel::OnOrientChange)

END_EVENT_TABLE()

