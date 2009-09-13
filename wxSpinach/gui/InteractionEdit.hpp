
#ifndef INTEREDIT_H
#define INTEREDIT_H

#include <gui/OrientationEdit.hpp>

#include <res/SpinachGUI.h>
#include <shared/spinsys.hpp>

class InterEditPanel : public InterEditPanelBase {
public:
  InterEditPanel(wxWindow* parent,wxWindowID id=-1);
private:
  //GUI Stuff
  OrientTextCtrl* mOrientEigenvalueCtrl;
  OrientTextCtrl* mOrientAxRhomCtrl;
  OrientTextCtrl* mOrientSpanSkewCtrl;

  SpinXML::Interaction* mInter;
};


class SpinInterEditPanel : public SpinInterEditPanelBase {
public:
  SpinInterEditPanel(wxWindow* parent,SpinXML::Spin* spin,wxWindowID id= -1);
  ~SpinInterEditPanel();

  void OnNewButton(wxCommandEvent& e);
  void OnDeleteButton(wxCommandEvent& e);
  void OnInteractionChange(wxCommandEvent& e);

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

#endif
