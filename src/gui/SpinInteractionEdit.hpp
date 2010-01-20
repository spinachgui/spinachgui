
#ifndef SPIN_INTERACTION_EDIT_H
#define SPIN_INTERACTION_EDIT_H

#include <gui/InteractionEdit.hpp>
#include <shared/spinsys.hpp>
#include <auto/SpinachGUI.h>


class SpinInterEditPanel : public SpinInterEditPanelBase, public sigc::trackable {
public:
  SpinInterEditPanel(wxWindow* parent,wxWindowID id= -1);
  ~SpinInterEditPanel();

  void SetDialogMode(bool dialogMode=true) {mDialogMode=dialogMode; mInterEdit->SetDialogMode();}

  void SetSpin(SpinXML::Spin* spin);

  //wx events originating from within
  void OnNewButton(wxCommandEvent& e);
  void OnDeleteButton(wxCommandEvent& e);
  void OnSSChange(wxCommandEvent& e);
  void OnInteractionChange(wxCommandEvent& e);

  //Slots for use with boost::signals
  void DirtySelected();

  void LoadFromSpin();

  void UpdateListBox();
  void InteractionChange();

  enum EDIT_MODE {
    EDIT_ALL,
    EDIT_LINEAR,
    EDIT_BILINEAR,
    EDIT_QUAD
  };
  void SetEditMode(EDIT_MODE mEditMode);


protected:
  void Clear();
  static wxString NameInteraction(SpinXML::Interaction* inter);
  
  DECLARE_EVENT_TABLE();

private:
  //GUI Stuff
  InterEditPanel* mInterEdit;
  ///Are we editing linear, bilinear, quadrapolar or all the
  ///interactions which apply to this spin
  EDIT_MODE mEditMode;
  ///A mapping of the list box index to the interaction index.
  ///Updated when UpdateListBox() is called. Will be the identity
  ///mapping if the edit mode is EDIT_ALL
  std::vector<long> mLBIndex2SpinIndex;

  struct ListBoxInteraction {
    SpinXML::Interaction* inter;
    bool modified;
  };

  long GetSelectedInterIndex() const;

  //Data
  SpinXML::Spin* mSpin;
  
  ///True when the GUI list box is being updated.  Events generated
  ///when mUpdatingListBox is true should usually be ignored
  bool mUpdatingListBox;
  std::vector<ListBoxInteraction> mTempInteractions;

  ///If in dialog mode do not checkpoint changes and show a star* next
  ///to changed interactions. On by default
  bool mDialogMode;
};

class InterPopup : public wxFrame {
public:
  InterPopup(wxWindow* Parent,SpinXML::Spin* spin,wxWindowID id=-1);
private:
  SpinInterEditPanel* mPanel;
};



#endif
