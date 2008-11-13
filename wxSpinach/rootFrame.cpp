#include "rootFrame.h"
#include "rotationDialog.h"

rootFrame::rootFrame( wxWindow* parent )
:rootFrameBase( parent ) {
    mGLCanvas = new wxGLCanvas(this,-1,wxDefaultPosition,wxDefaultSize);
    mSpinPropGrid = new wxPropertyGrid(mLeftPanel, wxID_ANY, wxDefaultPosition, wxSize( 250,-1 ), wxPG_DEFAULT_STYLE);
	mSpinPropGrid->SetMinSize( wxSize( 250,-1 ) );

	mCouplingPropGrid = new wxPropertyGrid(mRightPanel, wxID_ANY, wxDefaultPosition, wxSize( 250,-1 ), wxPG_DEFAULT_STYLE);
	mCouplingPropGrid->SetMinSize( wxSize( 250,-1 ) );

    this->popSpinPropGrid();
    this->popCouplingGrid();

  //Add everything to the sizer
	mLeftPanel->GetSizer()->Add( mSpinPropGrid,     1, wxALL|wxEXPAND, 5 );
    this->GetSizer()->Insert(1, mGLCanvas,         1, wxALL|wxEXPAND, 5 );
	mRightPanel->GetSizer()->Add( mCouplingPropGrid, 1, wxALL|wxEXPAND, 5 );

	this->Layout();
}

void rootFrame::popSpinPropGrid() {
    // Populate all the fields in mSpinPropGrid
    // (isotropic shielding, anisotropic shilding... etc)
    mSpinPropGrid->Append( new wxPropertyCategory(wxT("Sheilding")) );
    mSpinPropGrid->Append( new wxFloatProperty(wxT("Isotropic (ppm)"), wxPG_LABEL, 0) );
    wxPGId pid=mSpinPropGrid->Append( new wxStringProperty(wxT("Anisotropic (ppm)"), wxPG_LABEL,  wxT("<composed>")) );
      mSpinPropGrid->AppendIn(pid,new RotationProperty(wxT("Rotation"), wxPG_LABEL,  wxT("ax 0,0,0")) );
      mSpinPropGrid->AppendIn(pid,new wxFloatProperty(wxT("x"),wxPG_LABEL,1.0) );
      mSpinPropGrid->AppendIn(pid,new wxFloatProperty(wxT("y"),wxPG_LABEL,1.0) );
      mSpinPropGrid->AppendIn(pid,new wxFloatProperty(wxT("z"),wxPG_LABEL,1.0) );


    mSpinPropGrid->Append( new wxPropertyCategory(wxT("Relaxation")) );
    mSpinPropGrid->Append( new wxBoolProperty(wxT("Redfield"), wxPG_LABEL, false) );
    mSpinPropGrid->Append( new wxFloatProperty(wxT("Rate"), wxPG_LABEL,0) );

    mSpinPropGrid->Append( new wxPropertyCategory(wxT("Coordinates")) );
    mSpinPropGrid->Append( new wxFloatProperty(wxT("x"), wxPG_LABEL, 0) );
    mSpinPropGrid->Append( new wxFloatProperty(wxT("y"), wxPG_LABEL, 0) );
    mSpinPropGrid->Append( new wxFloatProperty(wxT("z"), wxPG_LABEL, 0) );
}


void rootFrame::popCouplingGrid() {
    // Populate all the fields in mSpinPairPropGrid
    // (isotropic shielding, anisotropic shilding... etc)
    mCouplingPropGrid->Append( new wxPropertyCategory(wxT("Coupling")) );
    mCouplingPropGrid->Append( new wxFloatProperty(wxT("J-coupling"), wxPG_LABEL, 0) );
    mCouplingPropGrid->Append( new wxFloatProperty(wxT("Dipole coupling"), wxPG_LABEL, 0) );

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
    e.RequestMore();
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



