
#include <gui/InteractionEdit.hpp>

using namespace SpinXML;

//============================================================//
// InterEditPanel

InterEditPanel::InterEditPanel(wxWindow* parent,wxWindowID id)
  : InterEditPanelBase(parent,id),mInter(NULL) {

  mOrientEigenvalueCtrl = new OrientTextCtrl(mEigenEditPanel);
  mOrientAxRhomCtrl     = new OrientTextCtrl(mAxRhomEditPanel);
  mOrientSpanSkewCtrl   = new OrientTextCtrl(mSpanSkewEditPanel);

  mEigenEditPanel->GetSizer()->Add(   mOrientEigenvalueCtrl,1.0,wxALL);
  mAxRhomEditPanel->GetSizer()->Add(  mOrientAxRhomCtrl,1.0,wxALL);
  mSpanSkewEditPanel->GetSizer()->Add(mOrientSpanSkewCtrl,1.0,wxALL);
}

//============================================================//
// SpinInterEditPanel

SpinInterEditPanel::SpinInterEditPanel(wxWindow* parent,SpinXML::Spin* spin,wxWindowID id) 
  : SpinInterEditPanelBase(parent,id),mSpin(spin),mUpdatingListBox(false) {
  mInterEdit=new InterEditPanel(this);
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
