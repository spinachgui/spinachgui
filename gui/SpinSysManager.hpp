
#ifndef __SPINSYSMANAGER_H__
#define __SPINSYSMANAGER_H__


#include <boost/shared_ptr.hpp>

#include <shared/spinsys.hpp>

#include <wx/string.h>

#include <list>

typedef boost::shared_ptr<SpinXML::SpinSystem> SpinSysPtr;
//typedef SpinXML::SpinSystem* SpinSysPtr;

class SpinSysManager  {
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

  ///Clear all the undo history
  void ClearHistory();

  ///True if it is possible to undo a change
  bool CanUndo() const;

  ///True if it is possible to redo a change
  bool CanRedo() const;

  wxString GetUndoMessage();

  wxString GetRedoMessage();

  //Debug functions
  ///Print out the history to the stdout
  void DumpHistory() const;

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

  typedef std::list<HistoryItem>::iterator HistIter;
  typedef std::list<HistoryItem>::const_iterator ConstHistIter;
  HistIter mPos;
  std::list<HistoryItem> mHistory;
};


#endif
