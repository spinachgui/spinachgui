#include "rootFrame.h"

rootFrame::rootFrame( wxWindow* parent )
:rootFrameBase( parent ) {
    mGLCanvas = new wxGLCanvas(this,-1,wxDefaultPosition,wxDefaultSize);
    this->GetSizer()->Insert(1, mGLCanvas, wxALL|wxEXPAND);

    mSpinPropGrid->Append( new wxPropertyCategory(wxT("Main")) );
    mSpinPropGrid->Append( new wxIntProperty(wxT("IntProperty"), wxPG_LABEL, 12345678) );

	this->Layout();
}


void rootFrame::ShowOptions(wxCommandEvent& e) {
    optionFrame* frame = new optionFrame(this);
    frame->SetIcon(wxICON(aaaa)); // To Set App Icon
    frame->Show();
}

void rootFrame::ShowCalc(wxCommandEvent& e) {
    calcFrame* frame = new calcFrame(this);
    frame->SetIcon(wxICON(aaaa)); // To Set App Icon
    frame->Show();
}

void rootFrame::OnIdle(wxIdleEvent& e) {
    glTick();
   // e.RequestMore();
}


void rootFrame::enableGL() {
    mGLCanvas->SetCurrent();

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glColor3f(0.0, 0.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10.0, 10.0, -10.0, 10.0, -10.0, 10.0);

    mGLCanvas->SwapBuffers();
}

void rootFrame::glTick() {
 	glClear(GL_COLOR_BUFFER_BIT);

	static float coord=5.0;
	coord+=0.1;
	if(coord>5.0){coord=-5.0;}

	glRectf(-5.0, coord, 5.0, -5.0);
	mGLCanvas->SwapBuffers();
}

BEGIN_EVENT_TABLE(rootFrame, wxFrame)
  EVT_IDLE    (rootFrame::OnIdle)
END_EVENT_TABLE()



