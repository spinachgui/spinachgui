
#ifndef STD_EVENTS_H
#define STD_EVENTS_H

#include <gui/EventSystem.hpp>
#include <wx/string.h>

/* Define standard event types that are useful at an application
   level. NB: Other event types can be defined elsewhere as needed. NB
   2: There really isn't any need to include this file in any header
   file. It should only be included by source files.*/

extern const CEventType EVT_CHECKPOINT;
extern const CEventType EVT_UNDO;
extern const CEventType EVT_REDO;
extern const CEventType EVT_SSCHANGE;
extern const CEventType EVT_SCHANGE;

//============================================================//
// Derived classes for events that need to carry a payload.
// (I wonder if this would be better done with a template?)

class EvtDataSChange : public IEventData {
public:
  explicit EvtDataSChange(long spinNumber) : mSpinNumber(spinNumber){}
  long  mSpinNumber;
};

class EvtSChange : public CEvent {
public:
  explicit EvtSChange(long spinNumber) 
  : CEvent(EVT_SCHANGE,IEventDataPtr(new EvtDataSChange(spinNumber))){
  }
};

//==============================>>

class EvtDataCheckpoint : public IEventData {
public:
  explicit EvtDataCheckpoint(const wxString& message) : mMessage(message){}
  wxString mMessage;
};

class EvtCheckpoint : public CEvent {
public:
  explicit EvtCheckpoint(const wxString& message) 
  : CEvent(EVT_CHECKPOINT,IEventDataPtr(new EvtDataCheckpoint(message))){
  }
};


#endif
