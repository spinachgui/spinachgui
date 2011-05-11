
#ifndef DIALOG_COMBO_H
#define DIALOG_COMBO_H

#include <wx/combo.h>
#include <wx/dcmemory.h>
#include <wx/settings.h>
#include <wx/filedlg.h>



template<class DialogClass> class DialogCombo : public wxComboCtrl {
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
protected:
    virtual DialogClass* CreateDialog() = 0;
    virtual void ReadDialog(DialogClass* dlg) =0;
    virtual wxString GetStringFromDialog(DialogClass* dlg) = 0;

public:
    //Overridden from ComboCtrl, called when the combo button is clicked
    void OnButtonClick() {
        DialogClass* dlg;
        if (GetValue()) {
            dlg = CreateDialog();
        }
        if (dlg->ShowModal() == wxID_OK) {
            SetValue(GetStringFromDialog(dlg));
            ReadDialog(dlg);
        }
        dlg->Destroy();
        SetFocus();
    }

    /// Overridden from ComboCtrl to avoid assert since there is no ComboPopup
    void DoSetPopupControl(wxComboPopup* popup) {
        return;  
    }

};

#endif
