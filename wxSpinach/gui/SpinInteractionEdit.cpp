
#include <gui/SpinInteractionEdit.hpp>
#include <gui/StdEvents.hpp>

using namespace SpinXML;

//============================================================//
// InterPopup

InterPopup::InterPopup(wxWindow* Parent, Spin* spin, wxWindowID id)
  : wxFrame(Parent,
	    id,
	    wxT("Popup panel"),
	    wxDefaultPosition,
	    wxDefaultSize,
	    wxFRAME_FLOAT_ON_PARENT|wxFRAME_NO_TASKBAR),
    mPanel(new SpinInterEditPanel(this)) {
  mPanel->SetSpin(spin);
  wxBoxSizer* sizer=new wxBoxSizer(wxVERTICAL);
  sizer->Add(mPanel,1.0,wxALL | wxEXPAND);
  SetSizer(sizer);  
}

//============================================================//
// SpinInterEditPanel


SpinInterEditPanel::SpinInterEditPanel(wxWindow* parent,wxWindowID id) 
  : SpinInterEditPanelBase(parent,id),
    mSpin(NULL),
    mUpdatingListBox(false),
    mEditMode(EDIT_ALL),
    mDialogMode(true) {
  mInterEdit=new InterEditPanel(this);
  GetSizer()->Add(mInterEdit,1,wxEXPAND | wxALL);
  Enable(false);
};

SpinInterEditPanel::~SpinInterEditPanel() {
  Clear();
}

void SpinInterEditPanel::SetSpin(Spin* spin) {
  mSpin=spin;
  LoadFromSpin();
}

void SpinInterEditPanel::Clear() {
  mTempInteractions.resize(0);
  Enable(false);
}

void SpinInterEditPanel::OnNewButton(wxCommandEvent& e) {
  Interaction* inter=new Interaction();
  inter->SetScalar(0.0);
  inter->SetSpin1(mSpin);
  mSpin->InsertInteraction(inter);

  ListBoxInteraction lbi;
  lbi.inter=inter;
  lbi.modified=true;

  mTempInteractions.push_back(lbi);

  UpdateListBox();
  mInterListBox->SetSelection(mInterListBox->GetCount()-1);
  mInterEdit->SetInter(inter);
  InteractionChange();
}

void SpinInterEditPanel::OnDeleteButton(wxCommandEvent& e) {
  long n=GetSelectedInterIndex();
  mTempInteractions.erase(mTempInteractions.begin() + n);
  UpdateListBox();
  mInterEdit->SetInter(NULL);
  InteractionChange();
}

void SpinInterEditPanel::LoadFromSpin() {
  Clear();
  if(mSpin==NULL) {
    return;
  } else {
    Enable(true);
  }

  std::vector<Interaction*> oldInteractions=mSpin->GetInteractions(); 
  for(long i=0;i<oldInteractions.size();i++) {              //Make sure all the interactions here are copies
    ListBoxInteraction lbi;
    lbi.inter=oldInteractions[i];
    lbi.modified=false;
    mTempInteractions.push_back(lbi);
  }

  UpdateListBox();
  InteractionChange();
}

void SpinInterEditPanel::UpdateListBox() {
  mUpdatingListBox=true;
  mInterListBox->Clear();
  long len=mTempInteractions.size();

  mLBIndex2SpinIndex.resize(0);

  for (long i=0;i<len;i++) {
    Interaction* inter=mTempInteractions[i].inter;
    if(inter->GetIsQuadratic()) {
      if(mEditMode!=EDIT_QUAD && mEditMode!=EDIT_ALL) {
	continue;
      }
    } else if (inter->GetIsBilinear()) {
      if(mEditMode!=EDIT_BILINEAR && mEditMode!=EDIT_ALL) {
	continue;
      }
    } else {
      if(mEditMode!=EDIT_LINEAR && mEditMode!=EDIT_ALL) {
	continue;
      }
    }
    wxString interTitle=NameInteraction(inter);
    if(mTempInteractions[i].modified && mDialogMode) {
      interTitle << wxT("*");
    } 
    mLBIndex2SpinIndex.push_back(i);
    mInterListBox->Append(interTitle);
  }
  mUpdatingListBox=false;
}

wxString SpinInterEditPanel::NameInteraction(Interaction* inter) {
  wxString quadStr;
  if(inter->GetIsQuadratic()) {
    quadStr=wxT("Quadratic");
  } else if (inter->GetIsBilinear()) {
    quadStr=wxT("Bilinear");
  } else {
    quadStr=wxT("Linear");
  }
  wxString interTitle;
  interTitle << wxString(Interaction::GetSubTypeName(inter->GetSubType()),wxConvUTF8);
  interTitle << wxT("(") << quadStr << wxT(", ");
  interTitle << wxString(Interaction::GetTypeName(inter->GetType()),wxConvUTF8);
  interTitle << wxT(")");

  return interTitle;
}
       
void SpinInterEditPanel::OnInteractionChange(wxCommandEvent& e) {
  if(mUpdatingListBox) {
    return;
  }
  InteractionChange();
}

void SpinInterEditPanel::InteractionChange() {
  long selected=GetSelectedInterIndex();
  if(selected >-1) {
    Interaction* inter=mTempInteractions[selected].inter;
    mInterEdit->SetInter(inter);
  } else {
    mInterEdit->SetInter(NULL);
  }
}

void SpinInterEditPanel::OnSSChange(wxCommandEvent& e) {
  if(mSpin==NULL) {
    return;
  }
  //If one of the interactions we're interested in just changed, it must
  //be the one currently being displayed, thus that is the one we mark as modifed
  long selected=GetSelectedInterIndex();
  if(selected >-1) {
    mTempInteractions[selected].modified=true;
  } 
  UpdateListBox();
  e.Skip();
}

long SpinInterEditPanel::GetSelectedInterIndex() const {
  long selected = mInterListBox->GetSelection();
  if(selected < 0) {
    return -1;
  } 
  return mLBIndex2SpinIndex[selected];
}


BEGIN_EVENT_TABLE(SpinInterEditPanel,wxPanel)

EVT_BUTTON     (INTER_ADD   ,SpinInterEditPanel::OnNewButton)
EVT_BUTTON     (INTER_DELETE,SpinInterEditPanel::OnDeleteButton)
EVT_LISTBOX    (wxID_ANY,SpinInterEditPanel::OnInteractionChange)
EVT_COMMAND    (wxID_ANY,EVT_SS_UPDATE,SpinInterEditPanel::OnSSChange)

END_EVENT_TABLE()
