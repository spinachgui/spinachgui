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

void Event::PushHistory(long part, long hint) {
  History.push_back(PartHint(part,hint));
}

long Event::GetHintGivenPart(long part) const {
  for(long i=History.size()-1;i!=0;i--) {
    if(History[i].part==part) {
      return History[i].hint;
    }
  }
  return -1;
}

void Event::Dump() const {
  cout << "  Event Object:" << endl;
  for(long i=0;i<History.size();i++) {
    cout << "     part=" << History[i].part
	 << " hint=" << History[i].hint << endl;
  }
}

EventNode::EventNode() : LastUID(-1) {

}

EventNode::EventNode(long part,long hint,const wxString& name) 
  : LastUID(-1), mName(name),mPart(part),mHint(hint){
}

EventNode::~EventNode() {
  Event e(IEventListener::REMOVAL);
  PropogateChangeUp(GetUID(),e);
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
  Event e(IEventListener::CHANGE);
  PropogateChangeUp(GetUID(),e);
  return parent;
}

EventNode* EventNode::AddChild(EventNode* child) {
  child->mParents.push_back(this);
  mChildren.push_back(child);
  Change(IEventListener::ADD);
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
  Change(IEventListener::REMOVAL);
  return;
}

void EventNode::Change(IEventListener::REASON r) {
  long uid=GetUID();
  Event e(r);
  PropogateChangeUp(uid,e);
}

//Private functions

void EventNode::PropogateChangeUp(long UID,Event& e) {
  if(LastUID==UID) {
    //This propogation has already touched this node
    return;
  } else {
    LastUID=UID;
  }
  e.PushHistory(mPart,mHint);
  SendChange(e);
  for(graphItorConst i=mParents.begin();i != mParents.end();++i) {
    (*i)->PropogateChangeUp(UID,e);
  }
}


void EventNode::SendChange(const Event& e) const {
  //Tell all listeners that this node has changed.
  for(long i=0;i<mListeners.size();i++) {
    mListeners[i].Listener->OnChange(e);
  }
}

void EventNode::AddListener(IEventListener* el,long hint) {
  mListeners.push_back(ListenerStruct(el,hint));
  el->mEventNodeSubscriptions.push_back(this);
}

void EventNode::RemoveListener(IEventListener* el,long hint) {
  ListenerItor i=find(mListeners.begin(),mListeners.end(),ListenerStruct(el,hint));
  mListeners.erase(i);
  IEventListener::SubItor j=find(el->mEventNodeSubscriptions.begin(),el->mEventNodeSubscriptions.end(),this);
  el->mEventNodeSubscriptions.erase(j);
}

//============================================================
// Debuging Functions

void EventNode::Dump() const {
  PrivateDump(0);
  cout << endl;
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
  cout << ")";
  cout << " Listeners=" << mListeners.size() << " (";
  for(cListenerItor i = mListeners.begin(); i != mListeners.end(); ++i) {
    cout << i->Listener << " " << i->hint;
  }
  cout << ")" << endl;
  indentDepth++;
  for(graphItorConst i=mChildren.begin();i != mChildren.end(); ++i) {
    (*i)->PrivateDump(indentDepth);
  }

}
