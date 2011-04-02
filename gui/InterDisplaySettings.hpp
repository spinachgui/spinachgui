
#ifndef __InterDisplaySettings_H
#define __InterDisplaySettings_H

#include <auto/SpinachGUI.h>
#include <shared/spinsys.hpp>
#include <gui/MolSceneGraph.hpp>
#include <3d/displaySettings.hpp>

class InterDisplaySettings : public InterDisplaySettingsBase {
public:
    InterDisplaySettings(wxWindow* parent,SpinXML::Interaction::Type t);
    LogSlider* GetLogSlider() {return mLogSlider;}

    void SetColour(float r,float g,float b);


    void OnColourChange(wxColourPickerEvent& e);
    void OnVisibleChange(wxCommandEvent& e);

	void SlotColourChange(SpinXML::Interaction::Type t,ColourRGB c);
	void SlotVisible(SpinXML::Interaction::Type t,bool v);
	void SlotScaleChange(SpinXML::Interaction::Type t,double v);

    sigc::signal<void,bool>      sigVisible;
    sigc::signal<void,ColourRGB> sigColour;
protected:
    DECLARE_EVENT_TABLE();
private:
	SpinXML::Interaction::Type mType;
};

#endif

