
#include <gui/SpinachApp.hpp>

#include <gui/SpinDialog.hpp>

IMPLEMENT_APP(SpinachApp);

bool SpinachApp::OnInit() {
  mSS = shared_ptr<SpinSystem>(new SpinSystem);
  mSS->LoadFromG03File("install/data/tryosine.log");
  mSS->SaveToXMLFile("install/data/tryosine.xml");

  RootFrame* frame = new RootFrame(NULL);

  SpinDialog* dialog = new SpinDialog(frame,mSS->GetSpin(0));
  dialog->ShowModal();

  //frame->Show();
  return false;
}
