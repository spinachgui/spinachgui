
#include <gui/InterDisplaySettings.hpp>

using namespace SpinXML;

InterDisplaySettings::InterDisplaySettings(wxWindow* parent,Interaction::SubType st) 
  : InterDisplaySettingsBase(parent,wxID_ANY) {
  mCheckbox->SetLabel(wxString(Interaction::GetSubTypeName(st),wxConvUTF8));
}

void InterDisplaySettings::SetColour(float r,float g,float b) {
    mColour->SetColour(wxColour(r*256,g*256,b*256));
    sigColour(r,g,b);
}

BEGIN_EVENT_TABLE(InterDisplaySettings,wxPanel)
EVT_CHECKBOX             (wxID_ANY, InterDisplaySettings::OnVisibleChange)
EVT_COLOURPICKER_CHANGED (wxID_ANY, InterDisplaySettings::OnColourChange)
END_EVENT_TABLE()


