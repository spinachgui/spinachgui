#ifndef __spinachrotationDialog__
#define __spinachrotationDialog__

#include "spinachGUI.h"
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include <wx/propgrid/propdev.h>

class rotationDialog : public rotationDialogBase {
public:
	rotationDialog( wxWindow* parent );
	wxString getAsString();
};

class rotationDialogAdapter : public wxPGEditorDialogAdapter {
    public:

        rotationDialogAdapter()
            : wxPGEditorDialogAdapter() {
        }

        virtual bool DoShowDialog( wxPropertyGrid* WXUNUSED(propGrid),
                                   wxPGProperty* WXUNUSED(property) ) {
            wxString s = GetValue();

            rotationDialog dialog(NULL);
            if (dialog.ShowModal() == wxID_OK) {
                s = dialog.getAsString();
            }

            SetValue(s);

            return true;
        }

    protected:
};


class RotationProperty : public wxLongStringProperty {
        DECLARE_DYNAMIC_CLASS(RotationProperty)
    public:
        RotationProperty() {
            RotationProperty("Default Rotation");
        }
        // Normal property constructor.
        RotationProperty(const wxString& label,
                         const wxString& name = wxPG_LABEL,
                         const wxString& value = wxEmptyString)
            : wxLongStringProperty(label,name,value) {

        }

        // Do something special when button is clicked.
        virtual wxPGEditorDialogAdapter* GetEditorDialog() const
        {
            return new rotationDialogAdapter();
        }

    protected:
};





#endif // __spinachrotationDialog__
