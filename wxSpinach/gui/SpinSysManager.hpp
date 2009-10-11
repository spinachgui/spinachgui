
#ifndef __SPINSYSMANAGER_H__
#define __SPINSYSMANAGER_H__

#include <gui/EventSystem.hpp>

#include <boost/shared_ptr.hpp>

#include <shared/spinsys.hpp>

#include <wx/string.h>

#include <list>

typedef boost::shared_ptr<SpinXML::SpinSystem> SpinSysPtr;
//typedef SpinXML::SpinSystem* SpinSysPtr;

class SpinSysManager : public IEventListener {
public:
  SpinSysManager(SpinSysPtr system);

  ///Get a reference to the head spin system
  const SpinSysPtr* Get() const;
  
  ///Save the changes to the spin system.
  void Checkpoint(const wxString& message=wxT(""));

  ///Undo one change
  void Undo();

  ///Redo one change
  void Redo();

  ///True if it is possible to undo a change
  bool CanUndo() const;

  ///True if it is possible to redo a change
  bool CanRedo() const;

  wxString GetUndoMessage();

  wxString GetRedoMessage();

  ///The McShafry style event handler
  virtual bool HandleEvent(CEvent const& event);

private:
  ///Default constructor is disabled
  SpinSysManager();
  ///Copy constructor is disabled
  SpinSysManager(const SpinSysManager&);

  SpinSysPtr mHead;

  struct HistoryItem {
    SpinSysPtr state;
    wxString message;
  };

  std::list<HistoryItem>::iterator mPos;
  std::list<HistoryItem> mHistory;
};


#endif
