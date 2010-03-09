
#ifndef __InterDisplaySettings_H
#define __InterDisplaySettings_H

#include <auto/SpinachGUI.h>
#include <shared/spinsys.hpp>
#include <gui/MolSceneGraph.hpp>

class InterDisplaySettings : public InterDisplaySettingsBase {
public:
    InterDisplaySettings(wxWindow* parent,SpinXML::Interaction::SubType st);
    LogSlider* GetLogSlider() {return mLogSlider;}

    void SetVisible(bool v) {
        mCheckbox->SetValue(v);
        sigVisible(v);
    }
    void SetColour(float r,float g,float b);

    sigc::signal<void,bool>    sigVisible;
    sigc::signal<void,float,float,float> sigColour;

    void OnColourChange(wxColourPickerEvent& e) {
        const wxColour& c=e.GetColour();
        sigColour(double(c.Red())/256.0,
                  double(c.Green())/256.0,
                  double(c.Blue())/256.0);
    }
    void OnVisibleChange(wxCommandEvent& e) {
        sigVisible(e.IsChecked());
    }
protected:
    DECLARE_EVENT_TABLE();
private:
};

#endif

