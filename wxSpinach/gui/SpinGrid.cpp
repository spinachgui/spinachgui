
#include <gui/SpinachApp.hpp>
#include <gui/SpinGrid.hpp>
#include <shared/nuclear_data.hpp>

#include <iostream>

using namespace std;

using namespace SpinXML;

const long ColumCount=10;  

const CEventType EV_SSCHANGE("SSCHANGE");
const CEventType EV_SCHANGE("SCHANGE");

const SpinGrid::SpinGridColum SpinGrid::columns[]={
  {COL_SELECTED,   "Selected",73},
  {COL_LABEL,      "Label",105},    
  {COL_ELEMENT,    "Element",70},   
  {COL_ISOTOPES,   "Isotopes",70},  
  {COL_X,          "x",70},	    
  {COL_Y,          "y",70},	    
  {COL_Z,          "z",70},	    
  {COL_LINEAR,     "Linear",70},    
  {COL_BILINEAR,   "Bilinear",70},
  {COL_QUADRAPOLAR,"Quadrapolar",100}
};		   



SpinGrid::SpinGrid(wxWindow* parent,wxWindowID id)
  :wxGrid(parent,id),mHead(wxGetApp().GetSpinSysManager()->Get()),mInterPopup(NULL),mPopupLock(false) {

  //Listen for all instances of the spin system changing because this
  //means we need to redraw the display
  CEventManager::Instance()->addListener(EventListenerPtr(this),EV_SSCHANGE);
  CEventManager::Instance()->addListener(EventListenerPtr(this),EV_SCHANGE);


  CreateGrid(0, ColumCount);

  EnableEditing(true);
  EnableGridLines(true);
  EnableDragGridSize(true);
  EnableDragColMove(false);
  EnableDragColSize(true);

  SetMargins(0,0); 
  SetColLabelSize(20);

  for(long i=0;i<ColumCount;i++) {
    SetColSize(i,columns[i].width);
    SetColLabelValue(i,wxString(columns[i].name,wxConvUTF8));
  }

  SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
  	
  SetRowLabelSize(40);
  SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

  RefreshFromSpinSystem();
}
    

void SpinGrid::OnEdit(wxGridEvent& e) {
  if (e.GetCol()==COL_LINEAR || e.GetCol()==COL_QUADRAPOLAR) {
    cout << "OnEdit" << endl;
    HidePopup();
  }
}

void SpinGrid::OnEndEdit(wxGridEvent& e) {
  if(e.GetCol()==COL_LINEAR or e.GetCol()==COL_QUADRAPOLAR) {
    cout << "OnEndEdit" << endl;
    ShowPopup((*mHead)->GetSpin(e.GetRow()));
  }
}

void SpinGrid::ShowPopup(Spin* spin) {
  mPopupLock=true;
  cout << "Show Popup" << endl;
  if(mInterPopup == NULL) {
    mInterPopup = new InterPopup(this,spin);
    wxPoint pos = ClientToScreen(wxPoint(10,10));
    mInterPopup->SetPosition(pos);
    mInterPopup->Show(true);

    wxGetTopLevelParent(this)->Raise(); //The window's appearence on the screen should be passive. It should not grab the focus away from the grid.
  }
  mPopupLock=false;
}

void SpinGrid::HidePopup() {
  cout << "HidePopup" << endl;
  if (mInterPopup != NULL) {
    mInterPopup->Show(false);
    mInterPopup->Destroy(); //Marks this frame for iminent deletion
    mInterPopup = NULL;     
  }
}

void SpinGrid::RefreshFromSpinSystem() {
  ClearGrid();
  AppendRows((*mHead)->GetSpinCount()+1);
  for (long i=0; i < (*mHead)->GetSpinCount(); i++) {
    SetupRow(i);
    UpdateRow(i);
  }
}

void SpinGrid::SetupRow(long rowNumber) {
  //Set the selected column renderers and editors to be boolian
  SetCellRenderer(rowNumber,COL_SELECTED,new wxGridCellBoolRenderer());
  SetCellEditor(rowNumber,COL_SELECTED,new wxGridCellBoolEditor());

  //
  long count=getElementCount();

  wxArrayString strArray;
  for(long i=0;i<count;i++) {
    wxString str(getElementSymbol(i),wxConvUTF8);
    str << wxT(" ") << wxString(getElementName(i),wxConvUTF8);
    strArray.Add(str);
  }

  SetCellEditor(rowNumber,COL_ELEMENT,new wxGridCellChoiceEditor(strArray));

  //Set up floating point editors, so the user can't enter something stupid
  SetCellEditor(rowNumber,COL_X,new wxGridCellFloatEditor());
  SetCellEditor(rowNumber,COL_Y,new wxGridCellFloatEditor());
  SetCellEditor(rowNumber,COL_Z,new wxGridCellFloatEditor());
}

void SpinGrid::UpdateRow(long rowNumber) {
  Spin* thisSpin = (*mHead)->GetSpin(rowNumber);
  double x,y,z;
  thisSpin->GetCoordinates(&x,&y,&z);

  //Setup the label and the element columns
  SetCellValue(rowNumber,COL_LABEL,wxString(thisSpin->GetLabel(),wxConvUTF8));

  SetCellValue(rowNumber,COL_X,wxString() << x);
  SetCellValue(rowNumber,COL_Y,wxString() << y);
  SetCellValue(rowNumber,COL_Z,wxString() << z);
}

void SpinGrid::OnCellChange(wxGridEvent& e) {
  if(e.GetCol()==COL_X) {
    double x;
    GetCellValue(e.GetRow(),e.GetCol()).ToDouble(&x);
    (*mHead)->GetSpin(e.GetRow())->GetPosition()->SetX(x);
  } else if(e.GetCol()==COL_Y) {
    double y;
    GetCellValue(e.GetRow(),e.GetCol()).ToDouble(&y);
    (*mHead)->GetSpin(e.GetRow())->GetPosition()->SetY(y);
  } else if(e.GetCol()==COL_Z) {
    double z;
    GetCellValue(e.GetRow(),e.GetCol()).ToDouble(&z);
    (*mHead)->GetSpin(e.GetRow())->GetPosition()->SetZ(z);
  }
  CEventManager::Instance()->trigger(CEvent("SCHANGE"));
}

void SpinGrid::OnCellSelect(wxGridEvent& e) {
  if(e.GetRow()==(*mHead)->GetSpinCount()) {
    //The user has clicked the blank row at the bottom of the grid.
    //We should create a new spin
    (*mHead)->InsertSpin(new Spin((*mHead).get(),new Vector3(0,0,0),"New Spin",(*mHead)->GetRootFrame(),0));
    UpdateRow((*mHead)->GetSpinCount()-1);
    SetupRow((*mHead)->GetSpinCount()-1);        
    AppendRows(1);
    e.Skip();
    return;
  }
  if(mPopupLock) {
    cout << "OnCellSelect() called, but locked " << endl;
    e.Skip();
  } else if(e.GetCol()==COL_LINEAR or e.GetCol()==COL_QUADRAPOLAR) {
    HidePopup();
    ShowPopup((*mHead)->GetSpin(e.GetRow()));
    e.Skip();
  } else {
    HidePopup();
    e.Skip();
  }
}

void SpinGrid::OnRightClick(wxGridEvent& e) {
  wxMenu* menu = new wxMenu();

  menu->Append(MENU_NEW_SPIN, wxT("Spin Properties..."));    

  if(e.GetRow()<(*mHead)->GetSpinCount()) {
    menu->Append(MENU_SPIN_DIALOG, wxT("Spin Properties..."));    
    menu->Append(MENU_DELETE_SPINS, wxT("Delete Spins..."));
  }
  PopupMenu(menu);
  delete menu;

}

//============================================================//
// virtual bool HandleEvent(CEvent const& event);
// The McShafry style event handler

bool SpinGrid::HandleEvent(CEvent const& event) {
  cout << "SpinGrid just got an event of type " << event.getType().getStr() << endl;
  return true;
}



BEGIN_EVENT_TABLE(SpinGrid,wxGrid)

EVT_GRID_SELECT_CELL     (         SpinGrid::OnCellSelect)
EVT_GRID_CMD_CELL_CHANGE (wxID_ANY,SpinGrid::OnCellChange)
EVT_GRID_EDITOR_HIDDEN   (         SpinGrid::OnEndEdit)
EVT_GRID_EDITOR_SHOWN    (         SpinGrid::OnEdit)

EVT_GRID_CELL_RIGHT_CLICK(         SpinGrid::OnRightClick)


END_EVENT_TABLE()
