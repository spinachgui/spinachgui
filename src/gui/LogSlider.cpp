
#include <gui/LogSlider.hpp>
#include <wx/dcclient.h>

LogSlider::LogSlider(wxWindow* parent) : wxControl(parent,wxID_ANY) {
  
}

void LogSlider::OnPaint(wxPaintEvent& e) {
  wxPaintDC dc(this);
  dc.DrawLine(0,0,5,5);
}

void LogSlider::OnLeftUp(wxMouseEvent&e) {
  if(e.Dragging()) {
    
  }
}

void LogSlider::OnMouseMove(wxMouseEvent&e) {
  if(e.Dragging()) {
    
  }
}

BEGIN_EVENT_TABLE(LogSlider, wxControl)
EVT_LEFT_UP(LogSlider::OnLeftUp)
EVT_MOTION(LogSlider::OnMouseMove)
EVT_PAINT(LogSlider::OnPaint)
END_EVENT_TABLE()
 


