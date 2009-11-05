
#ifndef _EVENT_H_
#define _EVENT_H_

class EventManager {
public:
  EventManager();
  
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

long GetUID();

class EventNode {
public:
  EventNode() : LastUID(-1) {

  }

  ~EventNode() {
    PropogateChangeUp(GetUID());
    for(graphItor i=mParents.begin();i != mParents.end();++i) {
      EventNode* parent=*i;
      for(graphItor j=parent->mChildren.begin();j != parent->mChildren.end();++j) {
	if(*j==this) {
	  parent->mChildren.erase(j);
	}
      }
    }
    for(graphItor i=mChildren.begin();i != mChildren.end();++i) {
      EventNode* child=*i;
      for(graphItor j=child->mParents.begin();j != child->mParents.end();++j) {
	if(*j==this) {
	  child->mParents.erase(j);
	}
      }
    }
  }

  void AddParent(EventNode* parent) {
    mParents.push_back(parent);
    PropogateChangeUp(GetUID());
  }

  void AddChild(EventNode* child) {
    mChildren.push_back(child);
    Change();
  }

  void Change(bool PropogateDown=true) {
    long uid=GetUID();
    PropogateChangeUp(uid);
    if(PropogateDown) {
      PropogateChangeDown(uid);
    }
  }
  void Annihilate() {
    //Propogate the change upwards then delete this node.
  }
private:

  void PropogateChangeUp(long UID) const {
    if(LastUID==UID) {
      //This propogation has already touched this node
      return;
    } else {
      LastUID==UID;
    }
    SendChange();
    for(graphItorConst i=mParents.begin();i != mParents.end();++i) {
      (*i)->PropogateChangeUp(UID);
    }
  }

  void PropogateChangeDown(long UID) const {
    if(LastUID==UID) {
      //This propogation has already touched this node
      return;
    } else {
      LastUID==UID;
    }
    SendChange();
    for(graphItorConst i=mChildren.begin();i != mChildren.end();++i) {
      (*i)->PropogateChangeDown(GetUID());
    }
  }

  void SendAnnihilation() const {
    //Tell all listeners that this node has changed.
    for(long i=0;i<mListeners.size();i++) {
      mListeners[i]->OnAnnihilation();
    }
  }

  void SendChange() const {
    //Tell all listeners that this node has changed.
    for(long i=0;i<mListeners.size();i++) {
      mListeners[i]->OnChange();
    }
  }

  typedef std::list<EventNode*>::const_iterator graphItorConst;
  typedef std::list<EventNode*>::iterator graphItor;
  std::list<EventNode*> mParents;
  std::list<EventNode*> mChildren;

  std::vector<IEventListener*> mListeners;

  long LastUID;
};

#endif
