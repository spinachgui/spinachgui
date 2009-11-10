
#include <gui/SpinSysManager.hpp>
#include <iostream>

using namespace boost;
using namespace SpinXML;
using namespace std;

#include <gui/StdEvents.hpp>

SpinSysManager::SpinSysManager(SpinSysPtr system) {
  HistoryItem hi;
  hi.state=SpinSysPtr(system);
  hi.message=wxT("");

  mHistory.push_back(hi);

  mPos=mHistory.begin();
  mHead=(*mPos).state;
}


const SpinSysPtr* SpinSysManager::Get() const {
  return &mHead;
}


void SpinSysManager::Checkpoint(const wxString& message) {
  return;
  HistoryItem hi;
  hi.state=SpinSysPtr(new SpinSystem(*mHead));
  hi.message=message;

  mHistory.erase(++mPos,mHistory.end());

  mHistory.push_back(hi);
  mPos=--mHistory.end();
  mHead=hi.state;
}



wxString SpinSysManager::GetUndoMessage() {
  if(CanUndo()) {
    std::list<HistoryItem>::iterator pos=mPos;
    return (*pos).message;
  } else {
    return wxString(wxT(""));
  }
}

wxString SpinSysManager::GetRedoMessage() {
  if(CanRedo()) {
    std::list<HistoryItem>::iterator pos=mPos;
    return (*++pos).message;
  } else {
    return wxString(wxT(""));
  }
}


void SpinSysManager::Undo() {
  if(CanUndo()) {
    mHead=(*--mPos).state;
  } else {
    cout << "Warning, can't SpinSysManager::Undo() any futher" << endl;
  }
}


void SpinSysManager::Redo() {
  if(CanRedo()) {
    mHead=(*++mPos).state;
  } else {
    cout << "Warning, can't SpinSysManager::Redo() any futher" << endl;
  }
}

void SpinSysManager::ClearHistory() {
  mHistory.erase(mHistory.begin(),--mHistory.end());
}

bool SpinSysManager::CanUndo() const {
  return false;
  return mPos != mHistory.begin();
}


bool SpinSysManager::CanRedo() const {
  return false;
  return mPos != --mHistory.end();
}

void SpinSysManager::DumpHistory() const {
  cout << "Dumping the contents of the history" << endl;
  for(ConstHistIter it=mHistory.begin();it != mHistory.end();++it) {
    cout << (*it).message.char_str() << endl;
  }
} 

