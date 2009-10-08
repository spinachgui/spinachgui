
#include <gui/SpinachApp.hpp>

#include <gui/SpinDialog.hpp>

IMPLEMENT_APP(SpinachApp);

bool SpinachApp::OnInit() {
  mSS = shared_ptr<SpinSystem>(new SpinSystem);
  mSS->LoadFromG03File("data/tryosine.log");
  mSS->SaveToXMLFile("data/tryosine.xml");

  mSSMgr = shared_ptr<SpinSysManager>(new SpinSysManager(mSS));
  
  const SpinSysPtr* head = mSSMgr->Get();

  cout << *head << endl;

  mSSMgr->Checkpoint();
  (*head)->InsertSpin(new Spin((*head).get(),new Vector3(0,0,0),"My Mistake",NULL,1));
  cout << *head << endl;

  (*head)->SaveToXMLFile("try_extra_spin.xml");

  mSSMgr->Undo();
  (*head)->SaveToXMLFile("try_extra_spin_undo.xml");
  cout << *head << endl;


  mSSMgr->Redo();
  (*head)->SaveToXMLFile("try_extra_spin_undo_redo.xml");

  mSSMgr->Undo();
  (*head)->InsertSpin(new Spin((*head).get(),new Vector3(1,1,1),"My Corrected Spin",NULL,1));
  mSSMgr->Checkpoint();
  (*head)->SaveToXMLFile("try_extra_spin_undo_corrected.xml");

  RootFrame* frame = new RootFrame(NULL);

  //SpinDialog* dialog = new SpinDialog(frame,mSS->GetSpin(0));
  //dialog->ShowModal();

  frame->Show();
  return true;
}
