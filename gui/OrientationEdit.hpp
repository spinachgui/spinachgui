
#ifndef ORIENTEDIT_H
#define ORIENTEDIT_H

#include <auto/SpinachGUI.h>
#include <shared/spinsys.hpp>
#include <sigc++/sigc++.h>

//============================================================//
// OrientEditPanel

class OrientEditPanel : public OrientEditPanelBase {
public:
    OrientEditPanel(wxWindow* parent,
                    wxWindowID id = wxID_ANY,
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxDefaultSize,
                    long style = wxTAB_TRAVERSAL);

    void SetOrient(const SpinXML::Orientation& orient);
    const SpinXML::Orientation& GetOrient() {return mOrient;}  

    void OnTextChange(wxCommandEvent& e);
    void OnPageChange(wxChoicebookEvent& e);

    void LoadFromOrient();
    void SaveToOrient();


protected:
    DECLARE_EVENT_TABLE();
private:
    SpinXML::Orientation mOrient;
    bool mLoading;
    bool mDirty;
};

//============================================================//
// OrientEditDialog

class OrientEditDialog : public OrientDialogBase {
public:
    OrientEditDialog(wxWindow* parent,wxWindowID id=-1);

    int ShowModal();

    void SetOrient(const SpinXML::Orientation& orient) {mOrientEditPanel->SetOrient(orient);}
    const SpinXML::Orientation& GetOrient() {return mOrientEditPanel->GetOrient();}

    void SaveToOrient() {mOrientEditPanel->SaveToOrient();}

    void OnApply(wxCommandEvent& e);
protected:
    DECLARE_EVENT_TABLE();
private:
    OrientEditPanel* mOrientEditPanel;
};

//============================================================//
// OrientDialogCombo

class OrientDialogCombo : public wxButton {
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
    OrientDialogCombo(wxWindow* parent,wxWindowID id=-1);

    void SetOrient(const SpinXML::Orientation& orient);
    const SpinXML::Orientation& GetOrient() {return mOrient;}
    
    //Events
    void OnClick(wxCommandEvent& e);

    sigc::signal<void> sigChange;
protected:
    void ReadFromOrient();
    DECLARE_EVENT_TABLE();

private:
    SpinXML::Orientation mOrient;
};

#endif
