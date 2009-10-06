
#include <gui/SpinachApp.hpp>

#include <gui/SpinDialog.hpp>

IMPLEMENT_APP(SpinachApp);

bool SpinachApp::OnInit() {
  mSS = shared_ptr<SpinSystem>(new SpinSystem);
  mSS->LoadFromG03File("install/data/tryosine.log");
  mSS->SaveToXMLFile("install/data/tryosine.xml");

  mSSMgr = shared_ptr<SpinSysManager>(new SpinSysManager(shared_ptr<SpinSystem>(new SpinSystem)));
  const SpinSysPtr* head = mSSMgr->Get();

  mSSMgr->Checkpoint();
  (*head)->LoadFromG03File("install/data/tryosine.log");

  RootFrame* frame = new RootFrame(NULL);

  //SpinDialog* dialog = new SpinDialog(frame,mSS->GetSpin(0));
  //dialog->ShowModal();

  frame->Show();
  return true;
}
