
#include <gui/SpinachApp.hpp>

IMPLEMENT_APP(SpinachApp);

bool SpinachApp::OnInit() {
  mSS = shared_ptr<SpinSystem>(new SpinSystem);
  mSS->LoadFromG03File("install/data/tryosine.log");

  RootFrame* frame = new RootFrame(NULL);
  frame->Show();
  return true;
}
