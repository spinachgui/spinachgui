
#ifndef LOG_SLIDER_H
#define LOG_SLIDER_H

#include <wx/control.h>
#include <sigc++/sigc++.h>

class LogSlider : public wxControl {
public:
  LogSlider(wxWindow* parent);
  void OnPaint(wxPaintEvent& e);

  void OnLeftUp(wxMouseEvent&e);
  void OnMouseMove(wxMouseEvent&e);

  sigc::signal<void,double> sigChange;
protected:
  DECLARE_EVENT_TABLE()
private:
};

#endif

