
#include <gui/SpinSysManager.hpp>
#include <iostream>

using namespace boost;
using namespace SpinXML;
using namespace std;

SpinSysManager::SpinSysManager(SpinSysPtr system) {
  mHistory.push_back(system);
  mPos=mHistory.begin();
  mHead=*mPos;
}


const SpinSysPtr* SpinSysManager::Get() const {
  return &mHead;
}


void SpinSysManager::Checkpoint(wxString message) {
  SpinSysPtr NewSystem=SpinSysPtr(new SpinSystem(*mHead));
  mHistory.erase(++mPos,mHistory.end());
  mHistory.push_back(NewSystem);
  mPos=--mHistory.end();
  mHead=NewSystem;
}


void SpinSysManager::Undo() {
  if(CanUndo()) {
    mHead=*--mPos;
  } else {
    cout << "Warning, can't SpinSysManager::Undo() any futher" << endl;
  }
}


void SpinSysManager::Redo() {
  if(CanRedo()) {
    mHead=*++mPos;
  } else {
    cout << "Warning, can't SpinSysManager::Redo() any futher" << endl;
  }
}


bool SpinSysManager::CanUndo() const {
  return mPos != mHistory.begin();
}


bool SpinSysManager::CanRedo() const {
  return mPos != --mHistory.end();
}

EvtSChange::EvtSChange(long spinNumber) : CEvent("SCHANGE",IEventDataPtr(new EvtDataSChange(spinNumber))){
  
}
