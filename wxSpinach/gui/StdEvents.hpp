
#ifndef STD_EVENTS_H
#define STD_EVENTS_H

#include <gui/EventSystem.hpp>

/* Define standard event types that are useful at an application
   level. NB: Other event types can be defined elsewhere as needed. NB
   2: There really isn't any need to include this file in any header
   file. It should only be included by source files.*/

extern const CEventType EVT_CHECKPOINT;
extern const CEventType EVT_UNDO;
extern const CEventType EVT_REDO;
extern const CEventType EVT_SSCHANGE;
extern const CEventType EVT_SCHANGE;

#endif
