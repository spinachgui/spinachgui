
#ifndef __EASYSPIN_FRAME_HPP__
#define __EASYSPIN_FRAME_HPP__

#include <auto/easyspin.h>
#include <shared/orientation.hpp>
#include <gui/OrientationEdit.hpp>

class EasySpinFrame : public EasySpinFrameBase {
public:
    EasySpinFrame(wxWindow* parent,
                  wxWindowID id = wxID_ANY,
                  const wxString& title = wxT("Export to Easyspin"),
                  const wxPoint& pos = wxDefaultPosition,
                  const wxSize& size = wxSize( 870,550 ),
                  long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
    DECLARE_EVENT_TABLE();

    //Exp events
    void OnCentreSweep(wxCommandEvent& e);
    void OnMin(wxCommandEvent& e);
    void OnMax(wxCommandEvent& e);

    void OnXBand(wxCommandEvent& e);
    void OnQBand(wxCommandEvent& e);
    void OnWBand(wxCommandEvent& e);

    void OnTempCheck(wxCommandEvent& e);
    void OnModAmpCheck(wxCommandEvent& e);

    void OnRangeUnit(wxCommandEvent& e);
    void OnModAmpUnit(wxCommandEvent& e);

    void OnCystal(wxCommandEvent& e);

    void OnAddOrient(wxCommandEvent& e);
    void OnDeleteOrient(wxCommandEvent& e);

    void OnShowSpaceGroups(wxCommandEvent& e);
    //Options Events
    void OnKnotsChange(wxSpinEvent& e);
    void OnAngularResText(wxCommandEvent& e);

    void SlotAngularResUnFocus();

    void OnInterpCheck(wxCommandEvent& e);

    //Methods
    void SetMWFreq(double freq);
    void SetMinMax(double min,double max);
    void SetKnots(unsigned long nKnots);
    void RepopulateCrystalOrients();
protected:
    wxString mStrMin;
    wxString mStrMax;
    wxString mStrCentre;
    wxString mStrSweep;

    std::vector<SpinXML::Orientation> mOrients;

    OrientEditPanel* mOrientEditPanel;
};


#endif
