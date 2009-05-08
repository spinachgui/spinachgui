#include "rotationDialog.h"

IMPLEMENT_DYNAMIC_CLASS(RotationProperty, wxLongStringProperty)

//==============>> RotationProperty <<=================//

wxPGEditorDialogAdapter* RotationProperty::GetEditorDialog() const {
    return new rotationDialogAdapter();
}

RotationProperty::RotationProperty() {
    RotationProperty(_T("ax 0,0,0"));
}

RotationProperty::RotationProperty(const wxString& label, const wxString& name, const wxString& value)
: wxLongStringProperty(label,name,value) {
}

//==============>> rotationDialogAdapter  <<=================//

rotationDialogAdapter::rotationDialogAdapter() : wxPGEditorDialogAdapter() {
}


bool rotationDialogAdapter::DoShowDialog( wxPropertyGrid* WXUNUSED(propGrid),wxPGProperty* WXUNUSED(property) ) {
    wxString s = GetValue();

    rotationDialog dialog(NULL);
    if (dialog.ShowModal() == wxID_OK) {
        s = dialog.getAsString();
    }

    SetValue(s);

    return true;
}

//==============>> rotationDialog  <<=================//


rotationDialog::rotationDialog( wxWindow* parent )
: rotationDialogBase( parent )
{

}


wxString rotationDialog::getAsString() {
    return _T("Got From Dialog");
}
