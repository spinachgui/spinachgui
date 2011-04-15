

#include <gui/State.hpp>
#include <iostream>

//XPM resources
#include "../res/bonds.xpm"
#include "../res/document_new.xpm"
#include "../res/document_open.xpm"
#include "../res/document_save.xpm"
#include "../res/epr.xpm"
#include "../res/nmr.xpm"
#include "../res/nmr_epr.xpm"

using namespace std;

TBNew::TBNew()
	: Action(wxT("New"),wxT("New"),wxBitmap( document_new_xpm )) {
}
void TBNew::Exec(wxCommandEvent& e) {
	cout << "New" << endl;
}


TBOpen::TBOpen()
	: Action(wxT("Open"),wxT("Open"),wxBitmap( document_open_xpm )) {
}
void TBOpen::Exec(wxCommandEvent& e) {
	cout << "Open" << endl;
}

TBSave::TBSave()
	: Action(wxT("Save"),wxT("Save"),wxBitmap( document_save_xpm )) {
}
void TBSave::Exec(wxCommandEvent& e) {
	cout << "Save" << endl;
}

TBNmrEpr::TBNmrEpr()
	: Action(wxT("Show All Interactions"),wxT("Show All Interactions"),wxBitmap( nmr_epr_xpm )) {
}
void TBNmrEpr::Exec(wxCommandEvent& e) {
	cout << "NMR EPR" << endl;
}

TBNmr::TBNmr()
	: Action(wxT("NMR Mode"),wxT("NMR Mode"),wxBitmap( nmr_xpm )) {
}
void TBNmr::Exec(wxCommandEvent& e) {
	cout << "NMR" << endl;
}

TBEpr::TBEpr()
	: Action(wxT("EPR Mode"),wxT("EPR Mode"),wxBitmap( epr_xpm )) {
}
void TBEpr::Exec(wxCommandEvent& e) {
	cout << "EPR" << endl;
}

TBBonds::TBBonds()
	: Action(wxT("Toggle Bonds"),wxT("Toggle Bonds"),wxBitmap( bonds_xpm )) {
}
void TBBonds::Exec(wxCommandEvent& e) {
	cout << "Toggle Bonds" << endl;
}


