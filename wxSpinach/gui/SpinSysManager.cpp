
#include <gui/SpinSysManager.hpp>
#include <iostream>

using namespace boost;
using namespace SpinXML;
using namespace std;

#include <gui/StdEvents.hpp>

SpinSysManager::SpinSysManager(SpinSysPtr system) {
  mHistory.push_back(system);
  mPos=mHistory.begin();
  mHead=*mPos;

  CEventManager::Instance()->addListener(EventListenerPtr(this),EVT_CHECKPOINT);
  CEventManager::Instance()->addListener(EventListenerPtr(this),EVT_SCHANGE);
  CEventManager::Instance()->addListener(EventListenerPtr(this),EVT_SSCHANGE);
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

bool SpinSysManager::HandleEvent(CEvent const& event) {
  cout << "SpinSysManager::HandleEvent" << endl;
  if(event.getType()==EVT_SCHANGE || event.getType()==EVT_SSCHANGE) {
    CEventManager::Instance()->trigger(CEvent(EVT_CHECKPOINT));
  } else if(event.getType()==EVT_CHECKPOINT) {
    Checkpoint(wxT("Unknown Change"));
  }
}


void SpinSysManager::Undo() {
  if(CanUndo()) {
    mHead=*--mPos;
    CEventManager::Instance()->trigger(CEvent(EVT_UNDO));    
  } else {
    cout << "Warning, can't SpinSysManager::Undo() any futher" << endl;
  }
}


void SpinSysManager::Redo() {
  if(CanRedo()) {
    mHead=*++mPos;
    CEventManager::Instance()->trigger(CEvent(EVT_REDO));
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

EvtSChange::EvtSChange(long spinNumber) : CEvent(EVT_SCHANGE,IEventDataPtr(new EvtDataSChange(spinNumber))){
  
}
