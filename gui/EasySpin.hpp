
#ifndef __EASYSPIN_FRAME_HPP__
#define __EASYSPIN_FRAME_HPP__

#include <auto/easyspin.h>

class EasySpinFrame : public EasySpinFrameBase {
public:
    EasySpinFrame(wxWindow* parent,
                  wxWindowID id = wxID_ANY,
                  const wxString& title = wxT("Export to Easyspin"),
                  const wxPoint& pos = wxDefaultPosition,
                  const wxSize& size = wxDefaultSize,//Size( 700,500 ),
                  long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
    DECLARE_EVENT_TABLE();

    void OnCentreSweep(wxCommandEvent& e);
    void OnMaxMin(wxCommandEvent& e);


protected:
    wxString mStrMin;
    wxString mStrMax;
    wxString mStrCentre;
    wxString mStrSweep;
};

#endif
