
#ifndef _EVENT_H_
#define _EVENT_H_

#include <wx/string.h>

#include <vector>
#include <list>

class EventNode;

class IEventListener {
  friend class EventNode;
public:
  ~IEventListener();
  virtual void OnChange() = 0;
  virtual void OnAnnihilation() = 0;
private:
  typedef std::list<EventNode*>::iterator SubItor;
  std::list<EventNode*> mEventNodeSubscriptions;
};


class EventNode {
public:
  EventNode();
  EventNode(const wxString& name);
  ~EventNode();

  EventNode* AddParent(EventNode* parent);
  EventNode* AddChild(EventNode* child);

  void Change(bool PropogateDown=true);

  void AddListener(IEventListener* el);
  void RemoveListener(IEventListener* el);

  void Dump() const;
private:
  void PrivateDump(long indentDepth) const;

  void PropogateChangeUp(long UID);
  void PropogateChangeDown(long UID);
  void SendAnnihilation() const;
  void SendChange() const;

  typedef std::list<EventNode*>::const_iterator graphItorConst;
  typedef std::list<EventNode*>::iterator graphItor;
  std::list<EventNode*> mParents;
  std::list<EventNode*> mChildren;

  typedef std::vector<IEventListener*>::iterator ListenerItor;
  std::vector<IEventListener*> mListeners;

  long LastUID;

  wxString mName;
};


#endif
