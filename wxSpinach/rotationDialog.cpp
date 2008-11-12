#include "rotationDialog.h"

IMPLEMENT_DYNAMIC_CLASS(RotationProperty, wxLongStringProperty)


rotationDialog::rotationDialog( wxWindow* parent )
:
rotationDialogBase( parent )
{

}


wxString rotationDialog::getAsString() {
    return "Got From Dialog";
}
