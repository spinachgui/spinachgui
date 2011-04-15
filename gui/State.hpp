
#ifndef __STATE__H__
#define __STATE__H__

#include <gui/Action.hpp>

//============================================================//
// Toolbar

struct TBNew : public Action {
	TBNew();
	virtual void Exec(wxCommandEvent& e);
};

struct TBOpen : public Action {
	TBOpen();
	virtual void Exec(wxCommandEvent& e);
};

struct TBSave : public Action {
	TBSave();
	virtual void Exec(wxCommandEvent& e);
};

struct TBNmrEpr : public Action {
	TBNmrEpr();
	virtual void Exec(wxCommandEvent& e);
};

struct TBNmr : public Action {
	TBNmr();
	virtual void Exec(wxCommandEvent& e);
};

struct TBEpr : public Action {
	TBEpr();
	virtual void Exec(wxCommandEvent& e);
};

struct TBBonds : public Action {
	TBBonds();
	virtual void Exec(wxCommandEvent& e);
};


#endif

