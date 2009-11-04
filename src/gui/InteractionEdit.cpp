
#include <boost/shared_ptr.hpp>

#include <gui/StdEvents.hpp>
#include <gui/InteractionEdit.hpp>
#include <gui/SpinachApp.hpp>

#include <iostream>
#include <map>

#include <wx/log.h>

#include <stdexcept>

using namespace std;
using namespace SpinXML;
using namespace boost;

//============================================================//
// InterEditPanel

//Static constructory stuff

enum {
  SUBTYPE_COMBO,
  SPIN2_COMBO
};

static bool DropDownSetup = false;

const static Interaction::SubType LinearSTLookup[] = {Interaction::ST_HFC,    	    
						      Interaction::ST_EXCHANGE, 
						      Interaction::ST_SHIELDING,
						      Interaction::ST_CUSTOM};
const static long LinearSTLookupLen = 4;

const static Interaction::SubType LinearSTLookupE[] = {Interaction::ST_G_TENSER,
						       Interaction::ST_EXCHANGE,
						       Interaction::ST_SHIELDING,
						       Interaction::ST_CUSTOM};
const static long LinearSTLookupELen = 4;

const static Interaction::SubType BilinearSTLookup[] = {Interaction::ST_SCALAR,
							Interaction::ST_DIPOLAR,
							Interaction::ST_CUSTOM}; 
const static long BilinearSTLookupLen = 3;

const static Interaction::SubType BilinearSTLookupE[] = {Interaction::ST_SCALAR,
							 Interaction::ST_DIPOLAR,
							 Interaction::ST_CUSTOM}; 
const static long BilinearSTLookupELen = 3;

const static Interaction::SubType QuadSTLookup[]  = {Interaction::ST_QUADRUPOLAR,
						     Interaction::ST_ZFS,
						     Interaction::ST_CUSTOM};     
const static long QuadSTLookupLen = 3;

const static Interaction::SubType QuadSTLookupE[]  = {Interaction::ST_QUADRUPOLAR,
						      Interaction::ST_ZFS,
						      Interaction::ST_CUSTOM};     
const static long QuadSTLookupELen = 3;


typedef std::map<Interaction::SubType,long> ReverseLookupMap;

static ReverseLookupMap LinearSTRevLookup;   
static ReverseLookupMap LinearSTRevLookupE;    
static ReverseLookupMap BilinearSTRevLookup;  
static ReverseLookupMap BilinearSTRevLookupE;
static ReverseLookupMap QuadSTRevLookup;        
static ReverseLookupMap QuadSTRevLookupE;    

static bool mDropDownsSetup;

Interaction::Type TypeOrders[]={
  Interaction::SCALAR,
  Interaction::MATRIX,
  Interaction::EIGENVALUES,
  Interaction::AXRHOM,
  Interaction::SPANSKEW
};

///Private function called once the first time an interaction edit
///panel is created. Initalises the static variables assocated with
///the class and does any other global set up that is required.

void InterEditPanel_StaticConstructor() {
  ReverseLookupMap* maps[] = {&LinearSTRevLookup,   
			      &LinearSTRevLookupE,  
			      &BilinearSTRevLookup, 
			      &BilinearSTRevLookupE,
			      &QuadSTRevLookup,	    
			      &QuadSTRevLookupE};   
  
  const Interaction::SubType* lookups[] = {LinearSTLookup,   
					   LinearSTLookupE,  
					   BilinearSTLookup, 
					   BilinearSTLookupE,
					   QuadSTLookup,	    
					   QuadSTLookupE};

  long lengths[] = {LinearSTLookupLen,
		    LinearSTLookupELen,
		    BilinearSTLookupLen,
		    BilinearSTLookupELen,
		    QuadSTLookupLen,
		    QuadSTLookupELen};
  for(long i=0;i<6;i++) {
    for(long j=0;j<lengths[i];j++) {
      (*maps[j])[lookups[i][j]]=j;
    }
  }
}

InterEditPanel::InterEditPanel(wxWindow* parent,wxWindowID id)
  : InterEditPanelBase(parent,id),
    mInter(NULL),
    mLoading(false),
    mSubTypeComboLookup(NULL),
    mSubTypeComboLookupLen(0),
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

  if(DropDownSetup) { //Fakes a static constructor
    DropDownSetup=true;
    InterEditPanel_StaticConstructor();
  }

}


void InterEditPanel::SetInter(Interaction* inter) {
  mInter=inter;
  Enable(inter != NULL);
  if(inter != NULL) {
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

    mOrientEigenvalueCtrl->SetOrient(Orientation());
  } else if(type==Interaction::AXRHOM) {
    mAxCtrl->       SetValue(wxT("0.0"));
    mRhomCtrl->     SetValue(wxT("0.0"));
    mAxRhomIsoCtrl->SetValue(wxT("0.0"));

    mOrientAxRhomCtrl->SetOrient(Orientation());
  } else if(type==Interaction::SPANSKEW) {
    mSpanCtrl->       SetValue(wxT("0.0"));
    mSkewCtrl->       SetValue(wxT("0.0"));
    mSpanSkewIsoCtrl->SetValue(wxT("0.0"));

    mOrientSpanSkewCtrl->SetOrient(Orientation());
  }
  SaveToInter();        
  e.Skip();
}

void InterEditPanel::UpdateSubTypeCombo(bool subtypeWarning) {
  //TODO: We need to test if the spin is an election, because then
  //slightly different options should become avaliable
  mSubTypeCombo->Clear();

  if(mInter->GetIsLinear()) {
    mSubTypeComboLookupLen = LinearSTLookupLen;
    mSubTypeComboLookup    = LinearSTLookup;
  } else if(mInter->GetIsBilinear()) {
    mSubTypeComboLookupLen = BilinearSTLookupLen;
    mSubTypeComboLookup    = BilinearSTLookup;
  } else {
    mSubTypeComboLookupLen = QuadSTLookupLen;
    mSubTypeComboLookup    = QuadSTLookup;
  }
  for(long i=0;i<mSubTypeComboLookupLen;i++) {
    mSubTypeCombo->Append(wxString(Interaction::GetSubTypeName(mSubTypeComboLookup[i]),wxConvUTF8));
  }

  Interaction::SubType st = mInter->GetSubType();
  bool found=false;
  for(long i=0;i<mSubTypeComboLookupLen;i++) {
	if(st==mSubTypeComboLookup[i]) {
	  mSubTypeCombo->SetSelection(i);
	  found=true;
	  break;
	}
  }
  if(!found) {
	if(subtypeWarning) {
	  wxString form(mInter->GetIsLinear() ? wxT("linear") : (mInter->GetIsBilinear() ? wxT("bilinear") : wxT("quadratic")));
	  wxLogError(wxString() << wxT("Warning, interaction has an invalid type (") 
				 << wxString(Interaction::GetSubTypeName(st),wxConvUTF8)
				 << wxT(") for its algebrate form (")
				 << form << wxT(")"));
	} else {
	  mSubTypeCombo->SetSelection(0);
	}
  }
}

void InterEditPanel::LoadFromInter() {
  mLoading=true;


  UpdateSubTypeCombo();

  if(mInter->GetIsLinear()) {
	mSpin2Combo->Enable(false);	
  } else if(mInter->GetIsBilinear()) {
	mSpin2Combo->Enable(true);
  } else {
	//Interaction is quadratic
	mSpin2Combo->Enable(false);
  }

  //Populate the spin 2 combobox with every other spin
  mSpin2Combo->Clear();
  
  SpinSysManager* SSMgr=wxGetApp().GetSpinSysManager();
  const SpinSysPtr* head=SSMgr->Get();

  long spinCount=(*head)->GetSpinCount();

  for(long i=0;i<spinCount;i++) {
    mSpin2Combo->Append(wxString() << i << wxT(" ") << wxString((*head)->GetSpin(i)->GetLabel(),wxConvUTF8));
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

    mInter->SetMatrix(new Matrix3(xx,xy,xz,
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
}


void InterEditPanel::onTextChange(wxCommandEvent& e) {
  if(mLoading) {  //If we are loading from a spin we should ignore
				  //this event
    return;
  }
  SaveToInter();

  wxCommandEvent event(EVT_SS_UPDATE,GetId());
  event.SetEventObject(this);
  GetEventHandler()->ProcessEvent(event);

  return;
}

void InterEditPanel::OnSpin2Change(wxCommandEvent& e) {
  mInter->SetSpin2((*(wxGetApp().GetSpinSysManager()->Get()))->GetSpin(mSpin2Combo->GetSelection()));
  wxCommandEvent event(EVT_SS_UPDATE,GetId());
  event.SetEventObject(this);
  GetEventHandler()->ProcessEvent(event);
}

void InterEditPanel::OnSubTypeChange(wxCommandEvent& e) {
  if(mSubTypeComboLookup==NULL) {
	wxLogError(wxT("mSubTypeComboLookup was null"));
	throw logic_error("mSubTypeComboLookup was null");
  }

  mInter->SetSubType(mSubTypeComboLookup[mSubTypeCombo->GetSelection()]);

  wxCommandEvent event(EVT_SS_UPDATE,GetId());
  event.SetEventObject(this);
  GetEventHandler()->ProcessEvent(event);
}

void InterEditPanel::OnInterFormChange(wxCommandEvent& e) {
  long selection=mFormBox->GetSelection();

  if(selection==0) { 
	//User set the interaction to linear
	mInter->SetLinear();
	mSpin2Combo->Enable(false);
  } else if (selection==2) {
	//User set the interaction to quadratic
	mInter->SetQuadratic();
	mSpin2Combo->Enable(false);
  } else {
	//User set the interaction to bilinear
	mInter->SetSpin2((*(wxGetApp().GetSpinSysManager()->Get()))->GetSpin(0));
	mSpin2Combo->Enable(true);
  }

  UpdateSubTypeCombo(false);

  wxCommandEvent event(EVT_SS_UPDATE,GetId());
  event.SetEventObject(this);
  GetEventHandler()->ProcessEvent(event);
}


BEGIN_EVENT_TABLE(InterEditPanel,wxPanel)

EVT_CHOICEBOOK_PAGE_CHANGED(wxID_ANY,     InterEditPanel::OnPageChange)
EVT_CHOICE                 (SUBTYPE_COMBO,InterEditPanel::OnSubTypeChange)
EVT_CHOICE                 (SPIN2_COMBO,  InterEditPanel::OnSpin2Change)
EVT_TEXT                   (wxID_ANY,     InterEditPanel::onTextChange)
EVT_RADIOBOX               (wxID_ANY,     InterEditPanel::OnInterFormChange)

END_EVENT_TABLE()

