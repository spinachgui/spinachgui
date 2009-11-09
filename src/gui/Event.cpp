#include <gui/Event.hpp>

#include <iostream>
#include <algorithm>
using namespace std;

long GetUID() {
  long static counter=0;
  return counter++;
}

IEventListener::~IEventListener() {
  //Remove self
  while(mEventNodeSubscriptions.begin()!=mEventNodeSubscriptions.end()) {

    (*mEventNodeSubscriptions.begin())->RemoveListener(this);
  }
}


EventNode::EventNode() : LastUID(-1) {

}

EventNode::EventNode(const wxString& name) : LastUID(-1), mName(name){

}

EventNode::~EventNode() {
  PropogateChangeUp(GetUID());
  while(mParents.begin() != mParents.end()) {
    graphItor i=mChildren.begin();
    EventNode* parent=*i;
    for(graphItor j=parent->mChildren.begin();j != parent->mChildren.end();++j) {
      if((*j)==this) {
	graphItor e=j;
	--j; //E will soon point to an invalid position
	parent->mChildren.erase(j);
      }
    }
    mParents.erase(i);
  }
  while(mChildren.begin() != mChildren.end()) {
    graphItor i=mChildren.begin();
    EventNode* child=*i;
    for(graphItor j=child->mParents.begin();j != child->mParents.end();++j) {
      if((*j)==this) {
	graphItor e=j;
	--j; //E will soon point to an invalid position
	child->mParents.erase(e);
      }
    }
    if(child->mParents.size() == 0) {
      //We've left this child an orphan. In the harsh reality real
      //world there's only one thing we can do
      delete child;
    }
    mChildren.erase(i);
  }
}


EventNode* EventNode::AddParent(EventNode* parent) {
  mParents.push_back(parent);
  PropogateChangeUp(GetUID());
  return parent;
}

EventNode* EventNode::AddChild(EventNode* child) {
  child->mParents.push_back(this);
  mChildren.push_back(child);
  Change();
  return child;
}

void EventNode::RemoveParent(EventNode* parent) {
  graphItor i = find(mParents.begin(),mParents.end(),parent);
  if(i == mParents.end()) {
    return;
  }
  mParents.erase(i);
  return;
}

void EventNode::RemoveChild(EventNode* child) {
  graphItor i = find(mChildren.begin(),mChildren.end(),child);
  if(i == mChildren.end()) {
    return;
  }
  mParents.erase(i);
  return;
}

void EventNode::Change(bool PropogateDown) {
  long uid=GetUID();
  PropogateChangeUp(uid);
  if(PropogateDown) {
    PropogateChangeDown(uid);
  }
}

//Private functions

void EventNode::PropogateChangeUp(long UID) {
  if(LastUID==UID) {
    //This propogation has already touched this node
    return;
  } else {
    LastUID=UID;
  }
  SendChange();
  for(graphItorConst i=mParents.begin();i != mParents.end();++i) {
    (*i)->PropogateChangeUp(UID);
  }
}

void EventNode::PropogateChangeDown(long UID) {
  cout << " Propogate Down" << endl;
  if(LastUID!=UID) {
    LastUID=UID;
    SendChange();
  }

  for(graphItorConst i=mChildren.begin();i != mChildren.end();++i) {
    cout << " Sending the event down the tree" << endl;
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
  cout << " Sending a change" << endl;
  for(long i=0;i<mListeners.size();i++) {
    mListeners[i]->OnChange();
  }
}

void EventNode::AddListener(IEventListener* el) {
  mListeners.push_back(el);
  el->mEventNodeSubscriptions.push_back(this);
}

void EventNode::RemoveListener(IEventListener* el) {
  ListenerItor i=find(mListeners.begin(),mListeners.end(),el);
  mListeners.erase(i);
  IEventListener::SubItor j=find(el->mEventNodeSubscriptions.begin(),el->mEventNodeSubscriptions.end(),this);
  el->mEventNodeSubscriptions.erase(j);
}

//============================================================
// Debuging Functions

void EventNode::Dump() const {
  PrivateDump(0);
}

void EventNode::PrivateDump(long indentDepth) const {
  for(long i=0;i<indentDepth;i++) {
    cout << "  ";
  }
  cout << "Node " << this;
  cout << " name=" << mName.char_str();
  cout << " parents=" << mParents.size() << " (";
  for(graphItorConst i=mParents.begin(); i!= mParents.end();++i) {
    cout << (*i)->mName.char_str() << ",";
  }
  cout << ") children=" << mChildren.size() << " (";
  for(graphItorConst i=mChildren.begin(); i!= mChildren.end();++i) {
    cout << (*i)->mName.char_str() << ",";
  }
  cout << ")" << endl;
  indentDepth++;
  for(graphItorConst i=mChildren.begin();i != mChildren.end(); ++i) {
    (*i)->PrivateDump(indentDepth);
  }
}
