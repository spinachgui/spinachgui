
#include <iostream>
#include <string>
#include <gui/Event.hpp>

using namespace std;

class TestListener : public IEventListener {
public:
  TestListener(std::string name) : mName(name) {}
  virtual void OnChange(const Event& e) {
    cout << mName << " (" << this
	 << ") recived a change event"
         << " part=" << e.part
         << " hint=" << e.hint << endl;
  }
private:
  std::string mName;
};

int main(int argc,char** argv) {

  cout << " Testing event system " << endl;
  cout << " Building the spin system " << endl;

  EventNode ss(0,0,wxT("Root"));
  EventNode* s1=ss.AddChild(new EventNode(1,0,wxT("Spin1")));
  EventNode* s2=ss.AddChild(new EventNode(1,1,wxT("Spin2")));
  EventNode* s3=ss.AddChild(new EventNode(1,2,wxT("Spin3")));
					     
  EventNode* i1=s1->AddChild(new EventNode(2,0,wxT("HFS 1")));
  EventNode* i2=s2->AddChild(new EventNode(2,1,wxT("HFS 2")));
  EventNode* i3=s3->AddChild(new EventNode(2,2,wxT("HFS 3")));
					     
  EventNode* j1=s1->AddChild(new EventNode(3,0,wxT("J 1")));
  EventNode* j2=s2->AddChild(new EventNode(3,1,wxT("J 2")));
  s2->AddChild(j1);
  s3->AddChild(j2);

  ss.Dump();

  TestListener* l1=new TestListener("S3 Listener");
  s3->AddListener(l1);

  TestListener* lss=new TestListener("SS Listener");
  ss.AddListener(lss);

  cout << "===START TEST, edit ss===" << endl;
  ss.Change(IEventListener::CHANGE,true);
  cout << "===END TEST===" << endl;

  cout << "===START TEST, edit i1===" << endl;
  i1->Change(IEventListener::CHANGE,true);
  cout << "===END TEST===" << endl;

  cout << "===START TEST, edit s3===" << endl;
  s3->Change(IEventListener::CHANGE,true);
  cout << "===END TEST===" << endl;

  delete l1;
  delete lss;
  return 0;
}

