#ifndef _EVENT_H_
#define _EVENT_H_

#include <wx/string.h>

#include <vector>
#include <list>

class EventNode;
class Event;

class IEventListener {
  friend class EventNode;
public:
  enum REASON {
    CHANGE,   //This change occured on this node
    ADD,      //The changed occured on this node and it was the
	      //addtion of a child node
    REMOVAL   //The Node is about to be removed (e.g. the user deleted
              //a spin). This is not quite the analog of ADD
  };

  ~IEventListener();
  virtual void OnChange(const Event& e) = 0;


private:
  typedef std::list<EventNode*>::iterator SubItor;
  std::list<EventNode*> mEventNodeSubscriptions;
};

class Event {
public:
  Event(long _part,long _hint,IEventListener::REASON r) 
    : part(_part),hint(_hint),reason(r) {

  }
  long part;   //General Description of where in the spin system the change occured (eg. spin, interaction)
  long hint;   //More specific hint about which part of the spin system chagned (eg. 1 for spin 1)
  IEventListener::REASON reason;  //What exactly changed.
};

class EventNode {
public:
  EventNode();
  EventNode(long part,long hint,const wxString& name);
  ~EventNode();

  EventNode* AddParent(EventNode* parent);
  EventNode* AddChild(EventNode* child);
  void RemoveParent(EventNode* parent);
  void RemoveChild(EventNode* child);

  void Change(IEventListener::REASON r=IEventListener::CHANGE,bool PropogateDown=false);

  void AddListener(IEventListener* el,long Hint=0);
  void RemoveListener(IEventListener* el,long Hint=0);

  void Dump() const;

  void SetPartAndHint(long part,long hint) {mPart=part; mHint=hint;}
private:
  void PrivateDump(long indentDepth) const;

  void PropogateChangeUp(long UID,const Event& e);
  void PropogateChangeDown(long UID,const Event& e);
  void SendChange(const Event& e) const;

  typedef std::list<EventNode*>::const_iterator graphItorConst;
  typedef std::list<EventNode*>::iterator graphItor;
  std::list<EventNode*> mParents;
  std::list<EventNode*> mChildren;

  struct ListenerStruct {
    ListenerStruct(IEventListener* L,long n)
      : Listener(L),hint(n) {
    };
    bool operator==(const ListenerStruct& l) {
      return (Listener==l.Listener && hint==l.hint);
    }
    IEventListener* Listener;
    long hint;
  };

  typedef std::vector<ListenerStruct>::iterator ListenerItor;
  std::vector<ListenerStruct> mListeners;

  long LastUID;
  long mPart;
  long mHint;

  wxString mName;
};


#endif
