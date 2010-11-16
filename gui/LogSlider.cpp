
#include <gui/LogSlider.hpp>
#include <wx/dcclient.h>
#include <iostream>
#include <cmath>
#include <stdexcept>

using namespace std;

LogSlider::LogSlider(wxWindow* parent,double value)
    : wxControl(parent,wxID_ANY) {
    if(value<=0) {
	throw std::runtime_error("In LogSlider value<=0");
    }
    mLogValue=log10(value);
    mLogWidth=4;
    mLogValueDelta=0;
}

void LogSlider::SetValue(double value) {
    if(value<=0) {
	throw std::runtime_error("In LogSlider value<=0");
    }
    mLogValue=log10(value);
    wxClientDC dc(this);
    RealPaint(dc);
    sigChange(pow(10,mLogValue));
}

double LogSlider::GetValue() {
    return pow(10.0,mLogValue);
}

void LogSlider::OnPaint(wxPaintEvent& e) {
    wxPaintDC dc(this);
    RealPaint(dc);
}

void LogSlider::RealPaint(wxDC& dc) {
    wxSize size=GetClientSize();
    int w=size.GetWidth();
    int h=size.GetHeight();

    dc.DrawRectangle(0,0,w,h);

    dc.DrawLine(0,h/2,w,h/2);

    wxPoint triangle[3];
    triangle[0]=wxPoint(0,6);
    triangle[1]=wxPoint(3,-2);
    triangle[2]=wxPoint(-3,-2);
    dc.DrawPolygon(3,triangle,w/2,4);

    double logMin=mLogValue+mLogValueDelta-mLogWidth/2;
    double logMax=mLogValue+mLogValueDelta+mLogWidth/2;
    
    int pixelsPer10=w/mLogWidth;

    int pixel02=pixelsPer10*log10(2);
    int pixel04=pixelsPer10*log10(4);
    int pixel06=pixelsPer10*log10(6);
    int pixel08=pixelsPer10*log10(8);

    for(double line=floor(logMin);line<ceil(logMax);line++) {
	int pixel=pixelsPer10 * (line-logMin);

	dc.DrawText(wxString() << pow(10,line),pixel,h/2-8);
	dc.DrawLine(pixel,h/2+10,pixel,h/2-10);

	dc.DrawLine(pixel+pixel02,h/2+4,pixel+pixel02,h/2-4);
	dc.DrawLine(pixel+pixel04,h/2+4,pixel+pixel04,h/2-4);
	dc.DrawLine(pixel+pixel06,h/2+4,pixel+pixel06,h/2-4);
	dc.DrawLine(pixel+pixel08,h/2+4,pixel+pixel08,h/2-4);
    }
}

void LogSlider::OnLeftDown(wxMouseEvent&e) {
    mStartDrag=e.GetX();
    mLogValueDelta=0;
    Refresh();
}

void LogSlider::OnLeftUp(wxMouseEvent&e) {
    mLogValue+=mLogValueDelta;
    mLogValueDelta=0;
    mStartDrag=0;
    sigChange(pow(10,mLogValue));
}

void LogSlider::OnMouseMove(wxMouseEvent&e) {
    if(e.Dragging()) {
	wxSize size=GetClientSize();
	int w=size.GetWidth();

	mLogValueDelta=double(mStartDrag-e.GetX())/double(w) * mLogWidth;
    }
    Refresh();
    sigChange(pow(10,mLogValue+mLogValueDelta));
}

BEGIN_EVENT_TABLE(LogSlider, wxControl)
EVT_LEFT_UP(LogSlider::OnLeftUp)
EVT_LEFT_DOWN(LogSlider::OnLeftDown)
EVT_MOTION(LogSlider::OnMouseMove)
EVT_PAINT(LogSlider::OnPaint)
END_EVENT_TABLE()
 


