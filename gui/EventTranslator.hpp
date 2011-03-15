
#ifndef EVENT_TRANSLATOR
#define EVENT_TRANSLATOR

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <map>
#include <limits.h>
#include <wx/event.h>

template<typename Derived>
class EventTranslator {
public:
	EventTranslator(Derived* derived) : mDerived(derived) {}
	typedef boost::function<void (wxCommandEvent&)> CommandFType;

	void CommandEvtAttach(wxCommandEvent& e) {
		int id = e.GetId();
		std::cout << "Mtest=" << mTest << std::endl;
		std::cout << "In the command event, id=" << id << std::endl;
		std::cout << "mFunc.size() = " << mFunc.size() << std::endl;
		//CommandFType f = 
		//f(e);
	}
protected:
	void WxConnect(int id, CommandFType f) {
		mTest = 4455;
		wxCommandEvent ce;
		std::cout << id << std::endl;
		mFunc[id] = f;
		mFunc[id](ce);
		std::cout << "mFunc.size() = " << mFunc.size() << std::endl;
		wxObjectEventFunction afterCast = 
			(wxObjectEventFunction)(wxEventFunction)(&EventTranslator::CommandEvtAttach);
		mDerived->Connect(id,wxEVT_COMMAND_MENU_SELECTED,
						afterCast);
	}

	Derived* mDerived;
	int mTest;
	std::map<int,CommandFType> mFunc;
};


#endif
