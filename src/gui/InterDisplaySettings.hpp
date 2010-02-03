
#ifndef __InterDisplaySettings_H
#define __InterDisplaySettings_H

#include <auto/SpinachGUI.h>
#include <shared/spinsys.hpp>

class InterDisplaySettings : public InterDisplaySettingsBase {
public:
  InterDisplaySettings(wxWindow* parent,SpinXML::Interaction::SubType st);
private:
};

#endif

