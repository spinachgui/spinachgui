
#include <iostream>
#include <gui/Event.hpp>

using namespace std;

class TestListener : public IEventListener {
public:
  virtual void OnChange() {
    cout << "TestListener(" << this
	 << ") recived a change event" << endl;
  }

  virtual void OnAnnihilation() {
    cout << "TestListener(" << this
	 << ") recived an annihilation event" << endl;
  }
};

int main(int argc,char** argv) {

  cout << " Testing event system " << endl;
  cout << " Building the spin system " << endl;

  EventNode ss(wxT("Root"));
  EventNode* s1=ss.AddChild(new EventNode(wxT("Spin1")));
  EventNode* s2=ss.AddChild(new EventNode(wxT("Spin2")));
  EventNode* s3=ss.AddChild(new EventNode(wxT("Spin3")));

  EventNode* i1=s1->AddChild(new EventNode(wxT("HFS 1")));
  EventNode* i2=s2->AddChild(new EventNode(wxT("HFS 2")));
  EventNode* i3=s3->AddChild(new EventNode(wxT("HFS 3")));

  EventNode* j1=s1->AddChild(new EventNode(wxT("J 1")));
  EventNode* j2=s2->AddChild(new EventNode(wxT("J 2")));
  s2->AddChild(j1);
  s3->AddChild(j2);

  ss.Dump();

  TestListener* l=new TestListener;
  s3->AddListener(l);

  cout << "===START TEST, edit ss===" << endl;
  ss.Change(true);
  cout << "===END TEST===" << endl;

  cout << "===START TEST, edit i1===" << endl;
  i1->Change(true);
  cout << "===END TEST===" << endl;

  cout << "===START TEST, edit s3===" << endl;
  s3->Change(true);
  cout << "===END TEST===" << endl;

  delete l;
  return 0;
}

