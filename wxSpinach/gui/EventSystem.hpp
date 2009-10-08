
#ifndef __EVENT_SYS_H__
#define __EVENT_SYS_H__

#include <boost/shared_ptr.hpp>
#include <string>
#include <set>
#include <list>
#include <map>

using namespace std;



//===========================>> CEventType <<======================================//

typedef unsigned long EventTypeId;

class CEventType {
public:	
  CEventType(const string& eventTypeName) : mId(hash_name(eventTypeName.c_str())) {};
  const EventTypeId getId() const{return mId;}
  const string& getStr() const {return mEventName;}
  const bool operator< (CEventType const& e)const {return mId<e.mId;}
  bool operator == (CEventType const& e) const;

  friend ostream& operator<< (ostream&, const CEventType&);

private:
  EventTypeId hash_name(const char* typeName);

  EventTypeId mId;
  string mEventName;
};

ostream& operator<< (ostream& os, const CEventType& et);


//===========================>> IEventData <<======================================//

class IEventData {
public:
  virtual ~IEventData(){}
};

typedef boost::shared_ptr<IEventData> IEventDataPtr;

//===========================>> CEvent <<======================================//


class CEvent {
public:
  CEvent(const string& inEventTypeName,
	 float inTime=0.f,
	 IEventDataPtr inData = IEventDataPtr((IEventData*)NULL))
    :
    mType(inEventTypeName),
    mTime(inTime),
    mUserData(inData)
  {}
	
  CEvent(CEvent const& o)
    :mType(o.mType),
     mTime(o.mTime),
     mUserData(o.mUserData) {}
	
  CEventType const & getType()  const {return mType;}
  float getTime() const {return mTime;}
  IEventDataPtr getData() const {return mUserData;}
	
  template<typename _T> _T* getDataPtr() const {return reinterpret_cast<_T*>(mUserData.get());}
private:
  CEventType mType;
  float mTime;
  IEventDataPtr mUserData;
};


//==========================>> IEventListener <<======================//

class IEventListener {
public:
  explicit IEventListener(){}
  virtual ~IEventListener(){}
	
  virtual bool HandleEvent(CEvent const& event) {
    //Error message goes here...
  }
};

/// Class for testing the event system with. Use logging classes for most debugging
class CEventSnooper: public IEventListener {
public:
  CEventSnooper();
  ~CEventSnooper();
	
  bool HandleEvent(CEvent const& event);
};

//==========================>> CEventManager <<=====================//

// CEventManager Description
//
// This is the object which maintains the list of registered
// events and their listeners
//
// This is a many-to-many relationship, as both one listener can
// be configured to process multiple event types and of course
// multiple listeners can be registered to each event type.
//
// The interface to this contruct uses smart pointer wrapped
// objects, the purpose being to ensure that no object that the
// registry is referring to is destoyed before it is removed from
// the registry AND to allow for the registry to be the only
// place where this list is kept ... the application code does
// not need to maintain a second list.
//
// Simply tearing down the registry (e.g.: destroying it) will
// automatically clean up all pointed-to objects ( so long as
// there are no other oustanding references, of course ).

typedef boost::shared_ptr<IEventListener>   EventListenerPtr;

typedef boost::shared_ptr<CEvent>           EventPtr;

class CEventManager {
public:

  enum ePublicConstants	{
    kINFINITE = 0xffffffff
  };

  explicit CEventManager();
	
  ~CEventManager();
	
  static CEventManager* Instance();

  // Register a handler for a specific event type, implicitly
  // the event type will be added to the known event types if
  // not already known.
  //
  // The function will return false on failure for any
  // reason. The only really anticipated failure reason is if
  // the input event type is bad ( e.g.: known-ident number
  // with different signature text, or signature text is empty
  // )
	
  void addListener ( EventListenerPtr const & inListener, CEventType const & inType );

  // Remove a listener/type pairing from the internal tables
  //
  // Returns false if the pairing was not found.
	
  void delListener ( EventListenerPtr const & inListener, CEventType const & inType );

  // Fire off event - synchronous - do it NOW kind of thing -
  // analogous to Win32 SendMessage() API.
  //
  // returns true if the event was consumed, false if not. Note
  // that it is acceptable for all event listeners to act on an
  // event and not consume it, this return signature exists to
  // allow complete propogation of that shred of information
  // from the internals of this system to outside uesrs.

  bool trigger (CEvent const & inEvent ) const;

  // Fire off event - asynchronous - do it WHEN the event
  // system tick() method is called, normally at a judicious
  // time during game-loop processing.
  //
  // returns true if the message was added to the processing
  // queue, false otherwise.

  void queueEvent ( EventPtr const & inEvent );

  // Find the next-available instance of the named event type
  // and remove it from the processing queue.
  //
  // This may be done up to the point that it is actively being
  // processed ...  e.g.: is safe to happen during event
  // processing itself.
  //
  // if 'allOfType' is input true, then all events of that type
  // are cleared from the input queue.
  //
  // returns true if the event was found and removed, false
  // otherwise

  void abortEvent ( CEventType const & inType,  bool allOfType = false );

  // Allow for processing of any queued messages, optionally
  // specify a processing time limit so that the event
  // processing does not take too long. Note the danger of
  // using this artificial limiter is that all messages may not
  // in fact get processed.
  //
  // returns true if all messages ready for processing were
  // completed, false otherwise (e.g. timeout )

  bool tick ( unsigned long maxMillis = kINFINITE );

  // --- information lookup functions ---
	
  // Validate an event type, this does NOT add it to the
  // internal registry, only verifies that it is legal (
  // e.g. either the ident number is not yet assigned, or it is
  // assigned to matching signature text, and the signature
  // text is not empty ).

  bool validateType( CEventType const & inType ) const;

  bool validateTypeById(EventTypeId id);

private:
  typedef std::set<CEventType>				EvTypeSet;
  typedef std::pair<EvTypeSet::iterator,bool>		EvTypeSetIRes;
  typedef std::list<EventListenerPtr>			EvListenerPtrList;		//If adding and removing listeners is rare, why a list McShaffy?

  typedef std::map<EventTypeId, EvListenerPtrList>    	EvListenerPtrMap;
  typedef std::pair<EventTypeId, EvListenerPtrList>	        EvListenerPtrMapEntry;

  typedef std::pair<EvListenerPtrMap::iterator, bool >	EvListenerMapIRes;
	
  typedef std::list<EventPtr>				EvPtrList;
	
  enum eConstants {
    NUM_QUEUES=2		//Why is this an enum McShaffy?
  };
	
  ///A set of all known event types. When a new event type is encoutered, it's added to the set!
  EvTypeSet 		mTypeList;
  ///A map from from an EventTypeId to an specific std list of event listeners
  EvListenerPtrMap	mEventListenerPtrMap;
  EvPtrList 		mQueues[NUM_QUEUES];
	
  int 			mActiveQueue;

};


#endif
