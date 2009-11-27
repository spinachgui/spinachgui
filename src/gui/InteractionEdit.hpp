
#ifndef INTEREDIT_H
#define INTEREDIT_H

#include <gui/OrientationEdit.hpp>

#include <auto/SpinachGUI.h>
#include <shared/spinsys.hpp>

#include <gui/DialogCombo.hpp>


class InterEditPanel : public InterEditPanelBase {
public:
  InterEditPanel(wxWindow* parent, wxWindowID id=-1);

  void SetInter(SpinXML::Interaction* inter);
  void SetDialogMode(bool dialogMode=true) {mDialogMode=dialogMode;}

  void OnPageChange(wxChoicebookEvent& e);
  void LoadFromInter();
  void SaveToInter();
  void onTextChange(wxCommandEvent& e);
  void OnSubTypeChange(wxCommandEvent& e);
  void OnSpin2Change(wxCommandEvent& e);
  void OnInterFormChange(wxCommandEvent& e);
  void OnOrientChange(wxCommandEvent& e);


protected:
  DECLARE_EVENT_TABLE();


private:
  //GUI Stuff
  OrientDialogCombo* mOrientEigenvalueCtrl;
  OrientDialogCombo* mOrientAxRhomCtrl;
  OrientDialogCombo* mOrientSpanSkewCtrl;

  SpinXML::Interaction* mInter;

  ///Update the interaction subtype drop down box. If subtypeWarning
  ///is set to true the user will be informed that the interaction has
  ///an invalid subtype given it's algebrate form (e.g. a linear
  ///interaction being marked as subtype quadratic would qualify). If
  ///subtype warning is false the subtype will be silently changed.
  void UpdateSubTypeCombo(bool subtypeWarning=true);

  bool mLoading;

  ///Update SubTypeComboLookup
  void UpdateSubTypeComboLookup();
  ///A pointer to an array of valid subtypes given the interaction's
  ///form
  const SpinXML::Interaction::SubType* mSubTypeComboLookup;
  ///The length of SubTypeComboLookup
  long mSubTypeComboLookupLen;
  ///If in dialog mode do not checkpoint changes and show a star* next
  ///to changed interactions. On by default
  bool mDialogMode;
};


#endif
