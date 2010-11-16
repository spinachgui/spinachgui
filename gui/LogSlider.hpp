
#ifndef LOG_SLIDER_H
#define LOG_SLIDER_H

#include <wx/control.h>
#include <sigc++/sigc++.h>

class LogSlider : public wxControl {
public:
    LogSlider(wxWindow* parent, double value);
    void OnPaint(wxPaintEvent& e);

    void OnLeftUp(wxMouseEvent&e);
    void OnLeftDown(wxMouseEvent&e);
    void OnMouseMove(wxMouseEvent&e);

    void SetValue(double value);
    double GetValue();

    sigc::signal<void,double> sigChange;
protected:
    DECLARE_EVENT_TABLE()
    void RealPaint(wxDC& dc);
    double mLogValue;
    double mLogValueDelta;
    //The pixel that is being hovered over
    int mStartDrag;
    ///The Number of orders of magnitude to show
    double mLogWidth; 
private:
};

#endif

