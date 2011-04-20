
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
	  mInter(NULL,GetFrame(),GetUnitSystem()),
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
	mInter=InteractionView(inter,GetFrame(),GetUnitSystem());
	Enable(inter != NULL);
	if(inter != NULL) {
		interChangeConnect=mInter.Get()->sigChange.connect(mem_fun(this,&InterEditPanel::OnInterChange));
		LoadFromInter();
	}
}


 
void InterEditPanel::OnPageChange(wxChoicebookEvent& e) {
	if (mLoading) {
		return;
	}
	Interaction::Storage storage=StorageOrders[e.GetSelection()];
	if(storage==Interaction::STORAGE_SCALAR) {
		mInter.ToScalar();
	} else if(storage==Interaction::MATRIX) {
		mInter.ToMatrix();
	} else if(storage==Interaction::EIGENVALUES) {
		mInter.ToEigenvalues();
	} else if(storage==Interaction::AXRHOM) {
		mInter.ToAxRhom();
	} else if(storage==Interaction::SPANSKEW) {
		mInter.ToSpanSkew();
	}
	LoadFromInter();
}

void InterEditPanel::UpdateSubTypeCombo(bool subtypeWarning) {
	//TODO: We need to test if the spin is an election, because then
	//slightly different options should become avaliable
	mSubTypeCombo->Clear();
	if(mInter.Get()==NULL) {
		return;
	}
	Interaction::Type t = mInter.GetType();
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

	if(mInter.GetIsLinear()) {
		mSpin2Combo->Enable(false);
	} else if(mInter.GetIsBilinear()) {
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
	if(mInter.GetIsBilinear()) {
		long Spin2Number=GetRawSS()->GetSpinNumber(mInter.Get()->GetOtherSpin(mWithRespectTo));
		mSpin2Combo->SetSelection(Spin2Number);
	}
	cout << "About the start testing types" << endl;

	if(mInter.GetStorage()==Interaction::STORAGE_SCALAR) {
		energy scalar = mInter.AsScalar();
		mScalarCtrl->SetValue(wxString() << scalar);
		mTypeChoiceBook->SetSelection(0);
	} else if(mInter.GetStorage()==Interaction::MATRIX) {
		Matrix3d mat = mInter.AsMatrix();
    
		mMatXXCtrl->SetValue(wxString() << mat(0,0));
		mMatXYCtrl->SetValue(wxString() << mat(0,1));
		mMatXZCtrl->SetValue(wxString() << mat(0,2));
                            
		mMatYXCtrl->SetValue(wxString() << mat(1,0));
		mMatYYCtrl->SetValue(wxString() << mat(1,1));
		mMatYZCtrl->SetValue(wxString() << mat(1,2));
                                
		mMatZXCtrl->SetValue(wxString() << mat(2,0));
		mMatZYCtrl->SetValue(wxString() << mat(2,1));
		mMatZZCtrl->SetValue(wxString() << mat(2,2));

		mTypeChoiceBook->SetSelection(1);
	} else if(mInter.GetStorage()==Interaction::EIGENVALUES) {
		Eigenvalues ev = mInter.AsEigenvalues();
		Orientation o = ev.mOrient;

		mEigenXXCtrl->SetValue(wxString() << ev.xx);
		mEigenYYCtrl->SetValue(wxString() << ev.yy);
		mEigenZZCtrl->SetValue(wxString() << ev.zz);
    
		mOrientEigenvalueCtrl->SetOrient(o);

		mTypeChoiceBook->SetSelection(2);

	} else if(mInter.GetStorage()==Interaction::AXRHOM) {
		cout << "type==axrhom" << endl;
		AxRhom ar = mInter.AsAxRhom();
		Orientation o = ar.mOrient;
		mAxCtrl->       SetValue(wxString() << ar.ax  );
		mRhomCtrl->     SetValue(wxString() << ar.rh);
		mAxRhomIsoCtrl->SetValue(wxString() << ar.iso );
		cout << "Set everything but the orientation" << endl;
		mOrientAxRhomCtrl->SetOrient(o);

		mTypeChoiceBook->SetSelection(3);
	} else if(mInter.GetStorage()==Interaction::SPANSKEW) {
		SpanSkew spanSkew = mInter.AsSpanSkew();
		Orientation o = spanSkew.mOrient;
		mSpanCtrl->       SetValue(wxString() << spanSkew.span);
		mSkewCtrl->       SetValue(wxString() << spanSkew.skew);
		mSpanSkewIsoCtrl->SetValue(wxString() << spanSkew.iso);

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
		mInter.SetScalar(scalar);
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

		mInter.SetMatrix(MakeMatrix3d(xx,xy,xz,
									  yx,yy,yz,
									  zx,zy,zz));

	} else if(storage==Interaction::EIGENVALUES) {
		double xx,yy,zz;

		mEigenXXCtrl->GetValue().ToDouble(&xx);
		mEigenYYCtrl->GetValue().ToDouble(&yy);
		mEigenZZCtrl->GetValue().ToDouble(&zz);
 
		mInter.SetEigenvalues(xx,yy,zz,mOrientEigenvalueCtrl->GetOrient());

	} else if(storage==Interaction::AXRHOM) {
		double ax,rhom,iso;

		mAxCtrl->       GetValue().ToDouble(&ax);
		mRhomCtrl->     GetValue().ToDouble(&rhom);
		mAxRhomIsoCtrl->GetValue().ToDouble(&iso);

		mInter.SetAxRhom(iso,ax,rhom, mOrientAxRhomCtrl->GetOrient());
	} else if(storage==Interaction::SPANSKEW) {
		double span,skew,iso;

		mSpanCtrl->       GetValue().ToDouble(&span);
		mSkewCtrl->       GetValue().ToDouble(&skew);
		mSpanSkewIsoCtrl->GetValue().ToDouble(&iso);

		mInter.SetSpanSkew(iso,span,skew,mOrientSpanSkewCtrl->GetOrient());
	}
	interChangeConnect.unblock();
}


void InterEditPanel::onTextChange(wxCommandEvent& e) {
	if(mLoading) {  //If we are loading from a spin we should ignore
		//this event
		return;
	}
	SaveToInter();
	sigChange();
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
		mInter.SetType(t,mWithRespectTo,NULL);
		cout << "setting subtype to a linear or quadratic form" << endl;
	} else {
		Spin* spin=(Spin*)mSpin2Combo->GetClientData(mSpin2Combo->GetSelection());
		if(spin == NULL) {
			//If spin is null then we are probably converting from a
			//linear or quadratic interaction where the mSpin2Combo is
			//greyed out and so all we can do is guess a spin to
			//couple mWithRespectTo to. Guess the first spin.
			
			spin = GetRawSS()->GetSpin(0);
		}

		mInter.SetType(t,mWithRespectTo,spin);
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

