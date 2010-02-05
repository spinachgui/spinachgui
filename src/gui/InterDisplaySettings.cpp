
#include <gui/InterDisplaySettings.hpp>

using namespace SpinXML;

InterDisplaySettings::InterDisplaySettings(wxWindow* parent,Interaction::SubType st) 
  : InterDisplaySettingsBase(parent,wxID_ANY) {
  mCheckbox->SetLabel(wxString(Interaction::GetSubTypeName(st),wxConvUTF8));
}

