

#include "rootFrame.h"
#include "rotationDialog.h"
#include "..\res\cluster.xpm"
#include "..\res\Jcoupling.xpm"
#include "..\res\S.xpm"

rootFrame::rootFrame( wxWindow* parent )
:rootFrameBase( parent ),mSpinSys(new SpinSystem) {
    mSpinPanel->Show(true);
    mJCoupPanel->Show(false);
    mClusterPanel->Show(false);

  //Add the 3D display
    mMolDisplay = new glMolDisplay(this);
    this->GetSizer()->Add(mMolDisplay,         1, wxALL|wxEXPAND, 5 );


    mSpinPropGrid = new wxPropertyGrid(mSpinPanel, wxID_ANY, wxDefaultPosition, wxSize( 250,-1 ), wxPG_DEFAULT_STYLE);
	mSpinPropGrid->SetMinSize( wxSize( 250,-1 ) );

	mCouplingPropGrid = new wxPropertyGrid(mJCoupPanel, wxID_ANY, wxDefaultPosition, wxSize( 250,-1 ), wxPG_DEFAULT_STYLE);
	mCouplingPropGrid->SetMinSize( wxSize( 250,-1 ) );

    this->popSpinPropGrid();
    this->popCouplingGrid();


  //Add everything to the panel sizers
	mSpinPanel->GetSizer()->Add( mSpinPropGrid,     1, wxALL|wxEXPAND, 5 );

	mJCoupPanel->GetSizer()->Add( mCouplingPropGrid, 1, wxALL|wxEXPAND, 5 );


  //Sort out the toolbar
    mRootToolbar->AddTool( EV_MODE_SHIELDING, wxT("Shielding"),           wxBitmap( S, wxBITMAP_TYPE_XPM  ), wxNullBitmap, wxITEM_RADIO, wxEmptyString, wxEmptyString );
    mRootToolbar->AddTool( EV_MODE_JCOUPLING, wxT("J Coupling"),          wxBitmap( Jcoupling, wxBITMAP_TYPE_XPM  ), wxNullBitmap, wxITEM_RADIO, wxEmptyString, wxEmptyString );
    mRootToolbar->AddTool( EV_MODE_CLUSTERS,  wxT("Cluster Definitions"), wxBitmap( cluster, wxBITMAP_TYPE_XPM  ), wxNullBitmap, wxITEM_RADIO, wxEmptyString, wxEmptyString );
    mRootToolbar->Realize(); // Called to draw the buttons


	this->Layout();

  //Now for the numerical stuff
  //Add some random test data

    Spin S1=Spin("Spin 1",0 ,0 , 0);
    this->addSpin(S1);
    Spin S2=Spin("Spin 2",0 ,11, 0);
    this->addSpin(S2);
    Spin S3=Spin("Spin 3",5,7  ,-10);
    this->addSpin(S3);

    SetFocusedSpin(0);
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

void rootFrame::enableGL() {
     mMolDisplay->enableGL();
}

//////////////////////////////////>> Spin system Editors <<==============//

void rootFrame::addSpin(Spin& S) {
    mSpinChoice->Insert(S.mName,0);
    mSpinSys->addSpin(S);
}

//==========================>> Logical Event Handlers <<==================//

void rootFrame::SetFocusedSpin(long index) {
    mActiveSpin=index;
    mSpinChoice->SetSelection(index);
}

unsigned long rootFrame::GetFocusedSpin() {
    return mActiveSpin;
}

////////////////////////////////////>> Event Handlers <<==================//

void rootFrame::OnIdle(wxIdleEvent& e) {
   // mMolDisplay->glTick();
   // e.RequestMore();
}

void rootFrame::OnMouseMove(wxMouseEvent& e) {
    long x=e.GetX();
    long y=e.GetY();
    wxString msg="";
    msg << "x=" << x << " y=" << y;
    mRootStatusBar->SetStatusText(msg);
    e.Skip();
}

void rootFrame::setModeShielding(wxCommandEvent& e) {
    mSpinPanel->Show(true);
    mJCoupPanel->Show(false);
    mClusterPanel->Show(false);
    this->Layout();
    e.Skip();
};

void rootFrame::setModeJCoupling(wxCommandEvent& e) {
    mSpinPanel->Show(false);
    mJCoupPanel->Show(true);
    mClusterPanel->Show(false);
    this->Layout();
    e.Skip();
};

void rootFrame::setModeClusters(wxCommandEvent& e) {
    mSpinPanel->Show(false);
    mJCoupPanel->Show(false);
    mClusterPanel->Show(true);
    this->Layout();
    e.Skip();
};

void rootFrame::onFocusedSpinChange(wxCommandEvent& e) {
    this->SetFocusedSpin(e.GetInt());
}

BEGIN_EVENT_TABLE(rootFrame, wxFrame)
  EVT_IDLE    (rootFrame::OnIdle)
  EVT_MOTION  (rootFrame::OnMouseMove)
  EVT_MENU    (EV_MODE_SHIELDING, rootFrame::setModeShielding)
  EVT_MENU    (EV_MODE_JCOUPLING, rootFrame::setModeJCoupling)
  EVT_MENU    (EV_MODE_CLUSTERS,  rootFrame::setModeClusters)
END_EVENT_TABLE()



