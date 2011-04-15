
#include <gui/Action.hpp>

Action::Action(const wxString& text,const wxString& tooltip,wxBitmap bitmap) 
	: mText(text),mTooltip(tooltip),mBitmap(bitmap) {
}

wxItemKind Action::GetItemKind() {
	return wxITEM_NORMAL;
}

const wxString& Action::GetText() {
	return mText;
}

const wxString& Action::GetTooltip() {
	return mTooltip;
}

wxBitmap Action::GetBitmap() {
	return mBitmap;
}

bool Action::Enabled() {
	return true;
}



