
#include <gui/EventSystem.hpp>
#include <string.h>
#include <iostream>
#include <stdexcept>

using namespace std;

//==============================>> Globals <<===========================//

CEventManager* gpEventManager = 0; //There can be only one...
const char* const pWildcardEventType="*";

//==============================>> CEventType <<========================//

EventTypeId CEventType::hash_name(const char* typeName) {
  // Relatively simple hash of arbitrary text string into a
  // 32-bit identifier Output value is
  // input-valid-deterministic, but no guarantees are made
  // about the uniqueness of the output per-input
  //
  // Input value is treated as lower-case to cut down on false
  // separations cause by human mistypes. Sure, it could be
  // construed as a programming error to mix up your cases, and
  // it cuts down on permutations, but in Real World Usage
  // making this text case-sensitive will likely just lead to
  // Pain and Suffering.
  //
  // This code lossely based upon the adler32 checksum by Mark
  // Adler and published as part of the zlib compression
  // library sources.
	
  // largest prime smaller than 65536
  unsigned long BASE = 65521L;
	
  // NMAX is the largest n such that 255n(n+1)/2 +
  // (n+1)(BASE-1) <= 2^32-1
  unsigned long NMAX = 5552;
	
#define DO1(buf,i)  {s1 += tolower(buf[i]); s2 += s1;}
#define DO2(buf,i)  DO1(buf,i); DO1(buf,i+1);
#define DO4(buf,i)  DO2(buf,i); DO2(buf,i+2);
#define DO8(buf,i)  DO4(buf,i); DO4(buf,i+4);
#define DO16(buf)   DO8(buf,0); DO8(buf,8);
	
  if (typeName == NULL) {
    return NULL;
  }
	
  if ( strcmp( typeName, pWildcardEventType ) == 0 ) {
    return 0;
  }
	
  unsigned long s1 = 0;
  unsigned long s2 = 0;
	
  for ( size_t len = strlen( typeName ); len > 0 ; ) {
    unsigned long k = len < NMAX ? len : NMAX;
		
    len -= k;
		
    while (k >= 16) {
      DO16(typeName);
      typeName += 16;
      k -= 16;
    }
		
    if (k != 0) do {
	s1 += *typeName++;
	s2 += s1;
      } while (--k);
		
    s1 %= BASE;
    s2 %= BASE;
  }
	
#pragma warning(push)
#pragma warning(disable : 4312)
	
  return (s2 << 16) | s1;
	
#pragma warning(pop)
#undef DO1
#undef DO2
#undef DO4
#undef DO8
#undef DO16
}

ostream& operator<< (ostream& os,const CEventType& et) {
	return os << "EventType:" << et.mEventName << " id:" << et.mId;
}

//==============================>> CEventSooper <<=========================//

CEventSnooper::CEventSnooper() {

};

CEventSnooper::~CEventSnooper() {

};


bool CEventSnooper::HandleEvent(CEvent const& event) {

};

//==============================>> CEventManager <<========================//


CEventManager::CEventManager(): mActiveQueue(0) {

}

CEventManager::~CEventManager() {
	gpEventManager=0;
}

void CEventManager::addListener ( EventListenerPtr const & inListener, CEventType const& inType ) {
  if(!validateType(inType)) {
    //This check probably only needs to be done in the debug build
    throw logic_error("Bad luck, your event name hash-collided with another event name");
  }
	
  EvTypeSet::iterator evIt = mTypeList.find(inType);
  if(evIt == mTypeList.end()) {
    //That was an unknown type, add it to the list
    EvTypeSetIRes ires= mTypeList.insert(inType);
		
    //Sanity checks...
    if(ires.second == false) {
      throw runtime_error("Okay, for some reason, we couldn't add an event to mTypeList");
    } else if(ires.first == mTypeList.end()) {
      //This may be just totally paranoid check, but it's in McShaffy's book.
      throw runtime_error("We added an event to mTypeList, but the list ended up empty.");
    }
    evIt = ires.first;
  }
  //See if this event type is present in map from EventTypeId's to EventListeners
  EvListenerPtrMap::iterator elmIt = mEventListenerPtrMap.find(inType.getId());
  if(elmIt == mEventListenerPtrMap.end()) {
    //No, it's not, we'll have to add it.
    EvListenerMapIRes elmIRes = mEventListenerPtrMap.insert(EvListenerPtrMapEntry(inType.getId(),EvListenerPtrList()));
    if(elmIRes.second == false) {
      throw runtime_error("While trying to add a EventListenerTable to mEventListenerMap, the insertion failed for some reason");
    } else if (elmIRes.first == mEventListenerPtrMap.end()) {
      //Surely this has to be McSaffy wanting his code to look good when published?
      throw runtime_error("While trying to add a EventListenerTable to mEventListenerMap, the insertion succeded, but the table ended empty");
    }
    elmIt = elmIRes.first;
  }
	
  EvListenerPtrList& evlTable = (*elmIt).second;
  for(EvListenerPtrList::iterator it=evlTable.begin(),itEnd = evlTable.end(); it != itEnd; it++) {
    bool listenerMatch = (*it == inListener);
    if(listenerMatch) {
      //This listener has already been added. Not fatal? Send a warning
      cout << "Warning:Trying to add a duplicate listener..." << endl;
      return;
    }
  }
  evlTable.push_back(inListener);
}

	
void CEventManager::delListener ( EventListenerPtr const & inListener, CEventType const & inType ) {
  //According to McSaffy, the removal of event listeners is quite rare, so this function
  //needent be super efficent.
  bool retVal=false;
  if(!validateType(inType)) {
    //This check probably only needs to be done in the debug build
    throw logic_error("Bad luck, your event name hash-collided with another event name. Also dude, you were trying to remove an eventListener");
  }
  for(EvListenerPtrMap::iterator it = mEventListenerPtrMap.begin(), itEnd=mEventListenerPtrMap.end(); it != itEnd; it++) {
    const EventTypeId thisEventId = it->first;
    EvListenerPtrList& eventListeners = it->second; 
    for(EvListenerPtrList::iterator it2 = eventListeners.begin(),it2End = eventListeners.end();it2 != it2End;it2++) {			
      if(*it2 == inListener) {
	eventListeners.erase(it2);
	retVal=true;
	break;	//Valid as there is only one of each listener to an event type.
      }
    }
  }
}


bool CEventManager::trigger(CEvent const & inEvent ) const {
  //Loop though listeners listening to all events, then
  //Loop though listeners listening to specific events.

  if(!validateType(inEvent.getType())) {
    //This check probably only needs to be done in the debug build
  }

  //Stage one, send the event to all listeners that care about all events
  EvListenerPtrMap::const_iterator itAll = mEventListenerPtrMap.find(0);  	//Were assuming here that the list of event listeners that care about
										//all events is at index 0. Which is correct by definition

  if(itAll != mEventListenerPtrMap.end()) {					//If there are actually any listeners interesting in all events
    EvListenerPtrList const& ListenList = itAll->second;
    for(EvListenerPtrList::const_iterator it2 = ListenList.begin(),it2End=ListenList.end();it2!=it2End;it2++) {
      (*it2)->HandleEvent(inEvent);
    }
  }
	
  //Stage send the event to all who are interested.
  EvListenerPtrMap::const_iterator it = mEventListenerPtrMap.find(inEvent.getType().getId());
		
  if(it == mEventListenerPtrMap.end()) {
    return false;
  }

  bool processed=false;

  EvListenerPtrList const& ListenList = it->second;
  for(EvListenerPtrList::const_iterator it2 = ListenList.begin(),it2End=ListenList.end();it2!=it2End;it2++) {
    if((*it2)->HandleEvent(inEvent)) {
      processed=true;
    }
  }

  return processed;
}

void CEventManager::queueEvent(EventPtr const & inEvent ) {
  if(!validateType(inEvent->getType())) {
    //This check probably only needs to be done in the debug build
  }
	
  //McSaffy tests that there is an interested listener before queing the event. I say,
  //Test when the event is triggered.
	
  mQueues[mActiveQueue].push_back(inEvent);
}

void CEventManager::abortEvent(CEventType const & inType,  bool allOfType) {
  cerr << "CEventManager::abortEvent isn't implimented" << endl;
}

bool CEventManager::tick(unsigned long maxMillis) {
  EvListenerPtrMap::const_iterator itWC = mEventListenerPtrMap.find(0);
	
  //Swap the active queue.
  int queueToProcess = mActiveQueue;
  mActiveQueue=(mActiveQueue+1) % NUM_QUEUES;
  mQueues[mActiveQueue].clear();
	
  while(mQueues[queueToProcess].size() > 0) {
    EventPtr event = mQueues[queueToProcess].front();	//Remember, the queues are linked lists.
    mQueues[queueToProcess].pop_front();
		
    CEventType const& eventType = event->getType();
    EvListenerPtrMap::const_iterator itListeners = mEventListenerPtrMap.find(eventType.getId());
		
    if(itWC != mEventListenerPtrMap.end()) {
      EvListenerPtrList const& table = itWC->second;
      bool processed = false;
      for(EvListenerPtrList::const_iterator it2=table.begin(), it2End=table.end(); it2 != it2End;it2++) {
	(*it2)->HandleEvent(*event);
      }
    }
		
    if(itListeners == mEventListenerPtrMap.end()) continue;
		
    unsigned int const EventTypeId = itListeners->first;
    EvListenerPtrList const& table = itListeners->second;
		
    for(EvListenerPtrList::const_iterator it=table.begin(),end=table.end();it != end; it++) {
      if((*it)->HandleEvent(*event)) {
	break;
      }
    }
  }
	
  return true; //As we aren't timing anything yet, the queue can only be flushed.
}


bool CEventManager::validateType( CEventType const & inType ) const {
  cerr << "You need to impliment validateType()!" << endl;
	return true;
}

bool CEventManager::validateTypeById(EventTypeId id) {
  cerr << "Not Implimented" << endl;
  return true;
}

CEventManager* CEventManager::Instance() {
  if(gpEventManager==0) {
    gpEventManager=new CEventManager;
  }
}

