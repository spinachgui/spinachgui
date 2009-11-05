
#ifndef _EVENT_H_
#define _EVENT_H_

class EventManager {
public:
  EventManager();
  EventManager(const EventManager&);
  
  ///Allows an IEventListener to indicate that it wishes to know about
  ///changes to a particular section of the spin system
  void Subscribe();
  void Unsubscripbe();

  ///Informs the EventManager that part of a spin system has changed.
  void PostEvent();

  

private:
};

#include <vector>
#include <list>

class IEventListener {
public:
  ~IEventListener() {
    //Remove self
  }
  virtual void OnChange() = 0;
  virtual void OnAnnihilation() = 0;
};


class EventNode {
public:
  EventNode();
  ~EventNode();

  void AddParent(EventNode* parent);
  void AddChild(EventNode* child);

  void Change(bool PropogateDown=true);

private:

  void PropogateChangeUp(long UID) const;
  void PropogateChangeDown(long UID) const;
  void SendAnnihilation() const;
  void SendChange() const;

  typedef std::list<EventNode*>::const_iterator graphItorConst;
  typedef std::list<EventNode*>::iterator graphItor;
  std::list<EventNode*> mParents;
  std::list<EventNode*> mChildren;

  std::vector<IEventListener*> mListeners;

  long LastUID;
};


#endif
