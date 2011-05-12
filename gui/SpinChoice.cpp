
#include <gui/SpinChoice.hpp>
#include <gui/SpinachApp.hpp>
#include <shared/foreach.hpp>

#include <shared/spinsys.hpp>

using namespace SpinXML;
using namespace sigc;

SpinChoice::SpinChoice(wxWindow* parent,wxWindowID id,
					   const wxPoint& pos,
					   const wxSize& size,
					   const wxArrayString& choices,
					   long style,
					   const wxValidator& validator,
					   const wxString& name)
	: wxChoice(parent,id,pos,size,choices,style,validator,name) {
	
	GetRawSS()->sigNewSpin.connect(mem_fun(this,&SpinChoice::SlotNewSpin));
	sigAnySpinChange.connect(mem_fun(this,&SpinChoice::SlotAnySpinChange));
	sigAnySpinDying.connect(mem_fun(this,&SpinChoice::SlotAnySpinDying));

	foreach(Spin* spin,GetRawSS()->GetSpins()) {
		Append(wxString(spin->GetLabel(),wxConvUTF8),(void*)spin);
	}
}

Spin* SpinChoice::GetSpin() const {
	return (Spin*) GetClientData(GetSelection());
}

void SpinChoice::SetSpin(Spin* spin) {
	for(unsigned long i = 0;i < GetCount();i++) {
		if(GetClientData(i) == spin) {
			SetSelection(i);
		}
	}
}


void SpinChoice::OnChange(wxCommandEvent& e) {
	sigChange((Spin*) GetClientData(GetSelection()));
}

void SpinChoice::SlotAnySpinDying(Spin* spin) {
	for(unsigned long i = 0;i < GetCount();i++) {
		if(GetClientData(i) == spin) {
			Delete(i);
		}
	}
}

void SpinChoice::SlotNewSpin(Spin* spin,long) {
	Append(wxString(spin->GetLabel(),wxConvUTF8),(void*)spin);
}

void SpinChoice::SlotAnySpinChange(Spin* spin) {
	long selection = GetSelection();
	for(unsigned long i = 0;i < GetCount();i++) {
		if(GetClientData(i) == spin) {
			Delete(i);
			Insert(wxString(spin->GetLabel(),wxConvUTF8),i+1,(void*)spin);
			break;
		}
	}
	SetSelection(selection);
}


BEGIN_EVENT_TABLE(SpinChoice,wxChoice)

EVT_CHOICE  (wxID_ANY,SpinChoice::OnChange)

END_EVENT_TABLE()



