
#include <gui/InterDisplaySettings.hpp>

using namespace SpinXML;
using namespace sigc;

void InterDisplaySettings::OnColourChange(wxColourPickerEvent& e) {
	const wxColour& c=e.GetColour();
	ColourRGB rgbColour(double(c.Red())/256.0,
						double(c.Green())/256.0,
						double(c.Blue())/256.0);
	SetInterColour(rgbColour,mType);
}

void InterDisplaySettings::OnVisibleChange(wxCommandEvent& e) {
	SetInterVisible(e.IsChecked(),mType);
}

void InterDisplaySettings::SlotColourChange(SpinXML::Interaction::Type t,ColourRGB c) {
	if(t != mType) {
		return;
	}
	mColour->SetColour(wxColour(c.r*256,c.g*256,c.b*256));
}

void InterDisplaySettings::SlotVisible(SpinXML::Interaction::Type t,bool v) {
	if(t != mType) {
		return;
	}
	mCheckbox->SetValue(v);
}

void InterDisplaySettings::SlotScaleChange(SpinXML::Interaction::Type t,double v) {
	if(t != mType) {
		return;
	}
	mLogSlider->SetValue(v);
}


InterDisplaySettings::InterDisplaySettings(wxWindow* parent,Interaction::Type t) 
	: InterDisplaySettingsBase(parent,wxID_ANY),mType(t) {
	mCheckbox->SetLabel(wxString(Interaction::GetTypeName(t),wxConvUTF8));
	mCheckbox->SetValue(GetInterVisible(mType));

	ColourRGB c = GetInterColour(mType);
	mColour->SetColour(wxColour(c.r*256,c.g*256,c.b*256));

				//Connect the scalling sliders to the scalling
	GetLogSlider()->sigChange.connect(bind(&SetInterSize,mType));

	sigInterVisible.connect(mem_fun(this,&InterDisplaySettings::SlotVisible));
	sigInterColour.connect(mem_fun(this,&InterDisplaySettings::SlotColourChange));
}

BEGIN_EVENT_TABLE(InterDisplaySettings,wxPanel)
EVT_CHECKBOX             (wxID_ANY, InterDisplaySettings::OnVisibleChange)
EVT_COLOURPICKER_CHANGED (wxID_ANY, InterDisplaySettings::OnColourChange)
END_EVENT_TABLE()


