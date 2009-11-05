#include <gui/Event.hpp>


long GetUID() {
  long static counter=0;
  return counter++;
}


EventNode::EventNode() : LastUID(-1) {

}

EventNode::~EventNode() {
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


void EventNode::AddParent(EventNode* parent) {
  mParents.push_back(parent);
  PropogateChangeUp(GetUID());
}

void EventNode::AddChild(EventNode* child) {
  mChildren.push_back(child);
  Change();
}

void EventNode::Change(bool PropogateDown) {
  long uid=GetUID();
  PropogateChangeUp(uid);
  if(PropogateDown) {
    PropogateChangeDown(uid);
  }
}

//Private functions

void EventNode::PropogateChangeUp(long UID) const {
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

void EventNode::PropogateChangeDown(long UID) const {
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

void EventNode::SendAnnihilation() const {
  //Tell all listeners that this node has changed.
  for(long i=0;i<mListeners.size();i++) {
    mListeners[i]->OnAnnihilation();
  }
}

void EventNode::SendChange() const {
  //Tell all listeners that this node has changed.
  for(long i=0;i<mListeners.size();i++) {
    mListeners[i]->OnChange();
  }
}


