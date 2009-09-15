
#include <gui/InteractionEdit.hpp>

using namespace SpinXML;

//============================================================//
// InterEditPanel

Interaction::Type TypeOrders[]={
  Interaction::SCALAR,
  Interaction::MATRIX,
  Interaction::EIGENVALUES,
  Interaction::AXRHOM,
  Interaction::SPANSKEW
};

InterEditPanel::InterEditPanel(wxWindow* parent,Interaction* inter,wxWindowID id)
  : InterEditPanelBase(parent,id),mInter(inter),mLoading(false) {

  mOrientEigenvalueCtrl = new DialogCombo(mEigenEditPanel);
  mOrientAxRhomCtrl     = new DialogCombo(mAxRhomEditPanel);
  mOrientSpanSkewCtrl   = new DialogCombo(mSpanSkewEditPanel);

  mEigenEditPanel->GetSizer()->Add(   mOrientEigenvalueCtrl,1.0,wxALL);
  mAxRhomEditPanel->GetSizer()->Add(  mOrientAxRhomCtrl,1.0,wxALL);
  mSpanSkewEditPanel->GetSizer()->Add(mOrientSpanSkewCtrl,1.0,wxALL);
}


void InterEditPanel::SetInter(Interaction* inter) {
  mInter=inter;
  LoadFromInter();
}

void InterEditPanel::OnQuadChecked(wxCommandEvent& e) {
  if(e.IsChecked()) {
    mInter->SetQuadratic();
  } else {
    mInter->SetLinear();
    //Following needs to be handled by an event system
    /*GetParent().UpdateListBox();*/
  }
}
 
void InterEditPanel::OnPageChange(wxCommandEvent& e) {
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
  } else if(type==Interaction::AXRHOM) {
    mAxCtrl->       SetValue(wxT("0.0"));
    mRhomCtrl->     SetValue(wxT("0.0"));
    mAxRhomIsoCtrl->SetValue(wxT("0.0"));
  } else if(type==Interaction::SPANSKEW) {
    mSpanCtrl->       SetValue(wxT("0.0"));
    mSkewCtrl->       SetValue(wxT("0.0"));
    mSpanSkewIsoCtrl->SetValue(wxT("0.0"));
  }
  SaveToInter();        
  e.Skip();
}

void InterEditPanel::LoadFromInter() {
  mLoading=true;
  //mTypeChoiceBook->SetSelection(Types[self.inter.GetType()][0]);
  if(mInter->GetType()==Interaction::SCALAR) {
    double scalar;
    mInter->GetScalar(&scalar);
    mScalarCtrl->SetValue(wxString() << scalar);
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

  } else if(mInter->GetType()==Interaction::EIGENVALUES) {
    double xx,yy,zz;
    Orientation* o;
    mInter->GetEigenvalues(&xx,&yy,&zz,&o);

    mEigenXXCtrl->SetValue(wxString() << xx);
    mEigenYYCtrl->SetValue(wxString() << yy);
    mEigenZZCtrl->SetValue(wxString() << zz);
  } else if(mInter->GetType()==Interaction::AXRHOM) {
    double ax,rhom,iso;
    Orientation* o;
    mInter->GetAxRhom(&ax,&rhom,&iso,&o);
    mAxCtrl->       SetValue(wxString() <<ax);
    mRhomCtrl->     SetValue(wxString() << rhom);
    mAxRhomIsoCtrl->SetValue(wxString() << iso);
  } else if(mInter->GetType()==Interaction::SPANSKEW) {
    double span,skew,iso;
    Orientation* o;
    mInter->GetSpanSkew(&span,&skew,&iso,&o);
    mSpanCtrl->       SetValue(wxString() << span);
    mSkewCtrl->       SetValue(wxString() << skew);
    mSpanSkewIsoCtrl->SetValue(wxString() << iso);
  }
  mQuadCheckbox->SetValue(mInter->GetIsQuadratic());

  mLoading=false;
}

void InterEditPanel::SaveToInter() {
  //Type=TypeOrders[self.typeChoiceBook.GetSelection()];
  Interaction::Type type=Interaction::SCALAR;
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

    mInter->SetEigenvalues(xx,yy,zz,new Orientation());

  } else if(type==Interaction::AXRHOM) {
    double ax,rhom,iso;

    mAxCtrl->       GetValue().ToDouble(&ax);
    mRhomCtrl->     GetValue().ToDouble(&rhom);
    mAxRhomIsoCtrl->GetValue().ToDouble(&iso);

    mInter->SetAxRhom(ax,rhom,iso,new Orientation());
  } else if(type==Interaction::SPANSKEW) {
    double span,skew,iso;

    mSpanCtrl->       GetValue().ToDouble(&span);
    mSkewCtrl->       GetValue().ToDouble(&skew);
    mSpanSkewIsoCtrl->GetValue().ToDouble(&iso);

    mInter->SetSpanSkew(span,skew,iso,new Orientation());
  }
}


void InterEditPanel::onTextChange(wxCommandEvent& e) {
  if(mLoading) {  //If we are loading from a spin we should ignore this event
    return;
  }
  SaveToInter();

  //Should be handled by some sort of event system
  /*GetParent().UpdateListBox();
    GetParent().interListBox.SetSelection(self.inter.index);*/

  return;
}


//============================================================//
// InterPopup

InterPopup::InterPopup(wxWindow* Parent, Spin* spin, wxWindowID id)
  : wxFrame(Parent,
	    id,
	    wxT("Popup panel"),
	    wxDefaultPosition,
	    wxDefaultSize,
	    wxFRAME_FLOAT_ON_PARENT|wxFRAME_NO_TASKBAR),
    mPanel(new SpinInterEditPanel(this,spin)) {
  wxBoxSizer* sizer=new wxBoxSizer(wxVERTICAL);
  sizer->Add(mPanel,1.0,wxALL | wxEXPAND);
  SetSizer(sizer);  
}

//============================================================//
// SpinInterEditPanel

SpinInterEditPanel::SpinInterEditPanel(wxWindow* parent,SpinXML::Spin* spin,wxWindowID id) 
  : SpinInterEditPanelBase(parent,id),mSpin(spin),mUpdatingListBox(false) {
  mInterEdit=new InterEditPanel(this,spin->GetInteraction(0)); //What if there are no interactions?
  GetSizer()->Add(mInterEdit,1,wxEXPAND | wxALL);


  LoadFromSpin();
};

SpinInterEditPanel::~SpinInterEditPanel() {
  Clear();
}

void SpinInterEditPanel::Clear() {
  long intCount=mTempInteractions.size();
  for(long i=0;i<intCount;i++) {
    delete mTempInteractions[i].inter;
  }
}

void SpinInterEditPanel::OnNewButton(wxCommandEvent& e) {
  CreateNew();
}

void SpinInterEditPanel::OnDeleteButton(wxCommandEvent& e) {
  MarkDeleted(mInterListBox->GetSelection());
}

void SpinInterEditPanel::LoadFromSpin() {
  Clear();
  std::vector<Interaction*> oldInteractions=mSpin->GetInteractions(); 
  for(long i=0;i<oldInteractions.size();i++) {              //Make sure all the interactions here are copies
    ListBoxInteraction lbi;
    lbi.inter=new Interaction(*(oldInteractions[i]));
    lbi.modified=false;
    mTempInteractions.push_back(lbi);
  }

  UpdateListBox();
  mInterListBox->SetSelection(0);
  InteractionChange();
}

void SpinInterEditPanel::UpdateListBox() {
  mUpdatingListBox=true;
  mInterListBox->Clear();
  long len=mTempInteractions.size();
  for (long i=0;i<len;i++) {
    Interaction* inter=mTempInteractions[i].inter;
    wxString quadStr;
    if(inter->GetIsQuadratic()) {
      quadStr=wxT("Quadratic");
    } else {
      quadStr=wxT("Linear");
    }
    wxString interTitle;
    interTitle << wxString(Interaction::GetSubTypeName(inter->GetSubType()),wxConvUTF8);
    interTitle << wxT("(") << quadStr << wxT(", ");
    interTitle << wxString(Interaction::GetTypeName(inter->GetType()),wxConvUTF8);
    interTitle << wxT(")");
    if(mTempInteractions[i].modified) {
      interTitle << wxT("*");
    } 
    mInterListBox->Append(interTitle);
  }
  mUpdatingListBox=false;
}

void SpinInterEditPanel::MarkDeleted(long n) {
  mTempInteractions.erase(mTempInteractions.begin() + n);
  UpdateListBox();
  mInterListBox->SetSelection(0);
  InteractionChange();
}

void SpinInterEditPanel::CreateNew() {
  Interaction* inter=new Interaction();
  inter->SetScalar(0.0);
  inter->SetSpin1(mSpin);

  ListBoxInteraction lbi;
  lbi.inter=inter;
  lbi.modified=true;

  mTempInteractions.push_back(lbi);

  UpdateListBox();
  mInterListBox->SetSelection(mInterListBox->GetCount()-1);
  InteractionChange();
}

void SpinInterEditPanel::SaveToSpin() {
  std::vector<Interaction*> oldInteractions=mSpin->GetInteractions();   
  long oldInterCount=oldInteractions.size();
  for(long i=0;i<oldInterCount;i++) { //Remove deleated interactions
    mSpin->RemoveInteraction(oldInteractions[i]);
  }

  for(long i=0;i<mTempInteractions.size();i++) {
    mSpin->InsertInteraction(mTempInteractions[i].inter);
  }
  //the SpinXML::SpinSystem is now responsible for looking after
  //the interaction points, so clear out our vector 
  mTempInteractions.resize(0);
}
       
void SpinInterEditPanel::OnInteractionChange(wxCommandEvent& e) {
  if(mUpdatingListBox) {
    return;
  }
  InteractionChange();
}

void SpinInterEditPanel::InteractionChange() {
  Interaction* inter=mTempInteractions[mInterListBox->GetSelection()].inter;
  //self.interEdit.SetInter(inter);
}


BEGIN_EVENT_TABLE(SpinInterEditPanel,wxPanel)

EVT_BUTTON(INTER_ADD   ,SpinInterEditPanel::OnNewButton)
EVT_BUTTON(INTER_DELETE,SpinInterEditPanel::OnDeleteButton)

END_EVENT_TABLE()
