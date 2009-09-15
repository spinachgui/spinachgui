
#ifndef INTEREDIT_H
#define INTEREDIT_H

#include <gui/OrientationEdit.hpp>

#include <res/SpinachGUI.h>
#include <shared/spinsys.hpp>

#include <wx/combo.h>
#include <wx/dcmemory.h>
#include <wx/settings.h>
#include <wx/filedlg.h>

class TextCtrlWithPopup : public wxTextCtrl {
public:
private:
  //  wxPanel mPanel;
  //  wxFrame
};


class DialogCombo : public wxComboCtrl {
public:
  DialogCombo(wxWindow* parent,wxWindowID id=-1) : wxComboCtrl(parent,id) {
    // make a custom bitmap showing "..."
    long bw = 14;
    long bh = 16;
    wxBitmap bmp(bw,bh);
    wxDC* dc = new wxMemoryDC(bmp);

    wxColour BGColour(255,254,255);

    // clear to a specific background colour
    dc->SetBackground(wxBrush(BGColour));
    dc->Clear();

    // draw the label onto the bitmap
    wxString label(wxT("..."));
    wxFont font = wxSystemSettings().GetFont(wxSYS_DEFAULT_GUI_FONT);
    font.SetWeight(wxFONTWEIGHT_BOLD);
    dc->SetFont(font);
    wxCoord tw,th;
    dc->GetTextExtent(label,&tw,&th);
    dc->DrawText(label, (bw-tw)/2, (bw-tw)/2);

    delete dc;

    // now apply a mask using the bgcolor
    bmp.SetMask(new wxMask(bmp, BGColour));

    // and tell the ComboCtrl to use it
    SetButtonBitmaps(bmp, true);
  }

  //Overridden from ComboCtrl, called when the combo button is clicked
  void OnButtonClick() {
    wxFileDialog* dlg;
    if (GetValue()) {
      dlg = new wxFileDialog(this, wxT("Choose File"), wxT(""),wxT(""),wxT("All (*.*)|*.*"), wxFD_OPEN);
    }
    if (dlg->ShowModal() == wxID_OK) {
      SetValue(dlg->GetPath());
    }
    dlg->Destroy();
    SetFocus();
  }

  /// Overridden from ComboCtrl to avoid assert since there is no ComboPopup
  void DoSetPopupControl(wxComboPopup* popup) {
    return;  
  }
};

class InterEditPanel : public InterEditPanelBase {
public:
  InterEditPanel(wxWindow* parent,SpinXML::Interaction* inter, wxWindowID id=-1);

  void SetInter(SpinXML::Interaction* inter);
  void OnQuadChecked(wxCommandEvent& e);
  void OnPageChange(wxCommandEvent& e);
  void LoadFromInter();
  void SaveToInter();
  void onTextChange(wxCommandEvent& e);
  
private:
  //GUI Stuff
  DialogCombo* mOrientEigenvalueCtrl;
  DialogCombo* mOrientAxRhomCtrl;
  DialogCombo* mOrientSpanSkewCtrl;

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
