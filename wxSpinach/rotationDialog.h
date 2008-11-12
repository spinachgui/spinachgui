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
        rotationDialogAdapter();
        virtual bool DoShowDialog( wxPropertyGrid* WXUNUSED(propGrid),wxPGProperty* WXUNUSED(property) );
    protected:
};


class RotationProperty : public wxLongStringProperty {
        DECLARE_DYNAMIC_CLASS(RotationProperty)
    public:
        RotationProperty();
        RotationProperty(const wxString& label, const wxString& name = wxPG_LABEL, const wxString& value = wxEmptyString);

        virtual wxPGEditorDialogAdapter* GetEditorDialog() const;
    protected:
};





#endif // __spinachrotationDialog__
