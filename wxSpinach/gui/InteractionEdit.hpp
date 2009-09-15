
#ifndef INTEREDIT_H
#define INTEREDIT_H

#include <gui/OrientationEdit.hpp>

#include <res/SpinachGUI.h>
#include <shared/spinsys.hpp>

#include <gui/DialogCombo.hpp>

class InterEditPanel : public InterEditPanelBase {
public:
  InterEditPanel(wxWindow* parent, wxWindowID id=-1);

  void SetInter(SpinXML::Interaction* inter);

  void OnQuadChecked(wxCommandEvent& e);
  void OnPageChange(wxChoicebookEvent& e);
  void LoadFromInter();
  void SaveToInter();
  void onTextChange(wxCommandEvent& e);

protected:
  DECLARE_EVENT_TABLE();
private:
  //GUI Stuff
  OrientDialogCombo* mOrientEigenvalueCtrl;
  OrientDialogCombo* mOrientAxRhomCtrl;
  OrientDialogCombo* mOrientSpanSkewCtrl;

  SpinXML::Interaction* mInter;

  bool mLoading;
};



class SpinInterEditPanel : public SpinInterEditPanelBase {
public:
  SpinInterEditPanel(wxWindow* parent,SpinXML::Spin* spin,wxWindowID id= -1);
  ~SpinInterEditPanel();

  void OnNewButton(wxCommandEvent& e);
  void OnDeleteButton(wxCommandEvent& e);
  void OnInteractionChange(wxCommandEvent& e);
  void OnSSChange(wxCommandEvent& e);

  void MarkDeleted(long n);

  void CreateNew();

  void LoadFromSpin();
  void SaveToSpin();
       
  void UpdateListBox();
  void InteractionChange();

protected:
  void Clear();

  DECLARE_EVENT_TABLE();

private:
  //GUI Stuff
  InterEditPanel* mInterEdit;

  //Data

  struct ListBoxInteraction {
    SpinXML::Interaction* inter;
    bool modified;
  };

  const static char* TypeNames[];
  const static char* SubTypeNames[];

  SpinXML::Spin* mSpin;
  
  ///True when the GUI list box is being updated.
  ///Events generated when mUpdatingListBox is true
  ///should usually be ignored
  bool mUpdatingListBox;
  std::vector<ListBoxInteraction> mTempInteractions;
};

class InterPopup : public wxFrame {
public:
  InterPopup(wxWindow* Parent,SpinXML::Spin* spin,wxWindowID id=-1);
private:
  SpinInterEditPanel* mPanel;
};


#endif
