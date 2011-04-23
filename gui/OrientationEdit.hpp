
#ifndef ORIENTEDIT_H
#define ORIENTEDIT_H

#include <auto/SpinachGUI.h>
#include <shared/spinsys.hpp>
#include <gui/DialogCombo.hpp>

DECLARE_EVENT_TYPE(EVT_ORIENT_EDIT, -1)

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

class OrientDialogCombo : public DialogCombo<OrientEditDialog> {
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
    OrientDialogCombo(wxWindow* parent,wxWindowID id=-1);

    void SetOrient(const SpinXML::Orientation& orient);
    const SpinXML::Orientation& GetOrient() {return mOrient;}

    virtual void SetStringValue(const wxString& s);

protected:
    virtual OrientEditDialog* CreateDialog();
    virtual void ReadDialog(OrientEditDialog* dlg);
    virtual wxString GetStringFromDialog(OrientEditDialog* dlg);
private:
    SpinXML::Orientation mOrient;
};

#endif
