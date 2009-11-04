
#include <gui/SpinachApp.hpp>
#include <gui/SpinGrid.hpp>
#include <shared/nuclear_data.hpp>
#include <gui/StdEvents.hpp>
#include <iostream>
#include <gui/RightClickMenu.hpp>

using namespace std;

using namespace SpinXML;

const long ColumCount=10;  

//Event generated whenever the spin system changes
DECLARE_EVENT_TYPE(EVT_INTER_SELECT, -1)
DEFINE_EVENT_TYPE(EVT_INTER_SELECT)

DECLARE_EVENT_TYPE(EVT_INTER_UNSELECT, -1)
DEFINE_EVENT_TYPE(EVT_INTER_UNSELECT)


//============================================================//
// Utility functions.

//Enum for use with wxString FormatLinearInteractions()
enum AlgebrakeForm {
  LINEAR,
  BILINEAR,
  QUAD,
  ALL
};

//Write a string representing the interactions acting on a spin
wxString FormatLinearInteractions(Spin* spin,AlgebrakeForm form=ALL) {
  wxString str;
  vector<Interaction*> Interactions=spin->GetInteractions();
  long count=Interactions.size();
  bool first=true;
  for(long i=0;i<count;i++) {

    Interaction* inter=Interactions[i];
    AlgebrakeForm f=(inter->GetIsLinear() ?
		     LINEAR : (inter->GetIsBilinear() ? BILINEAR :
			       (QUAD)));
    if(f==form || form==ALL) {
      if(!first) {
	str << wxT(", ");
      }
      first=false;
      str << wxString(Interaction::GetSubTypeName(inter->GetSubType()),wxConvUTF8);
    }
  }
  return str;
}

//============================================================//
// SpinGrid

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
  {COL_QUAD,"Quadrapolar",100}
};		   



SpinGrid::SpinGrid(wxWindow* parent,wxWindowID id)
  :wxGrid(parent,id),mHead(wxGetApp().GetSpinSysManager()->Get()),mUpdating(false) {

  //Listen for all instances of the spin system changing because this
  //means we need to redraw the display
  CEventManager::Instance()->addListener(EventListenerPtr(this),EVT_SSCHANGE);
  CEventManager::Instance()->addListener(EventListenerPtr(this),EVT_SCHANGE);

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
  long sc=GetSS()->GetSpinCount();
  if(e.GetRow()==sc) {
    //User is trying to edit the blank line at the bottom of the grid,
    //so create a new spin for them
    GetSS()->InsertSpin(new Spin(GetSS().get(),new Vector3(0,0,0),"New Spin",NULL,1));
    SetupRow(sc);
    UpdateRow(sc);
    AppendRows(1);
  }
  if (e.GetCol()==COL_LINEAR || e.GetCol()==COL_QUAD) {
    cout << "OnEdit" << endl;
  }
}

void SpinGrid::OnEndEdit(wxGridEvent& e) {
  if(e.GetCol()==COL_LINEAR or e.GetCol()==COL_QUAD) {
    cout << "OnEndEdit" << endl;
  }
}


void SpinGrid::RefreshFromSpinSystem() {
  cout << "SpinGrid::RefreshFromSpinSystem()" << endl;
  mUpdating=true;

  if(GetNumberRows()) {
    //Clear grid only clears the underlying data rather. The cells
    //remain and are black. wxGrid::DeleteRows solves this
    DeleteRows(0,GetNumberRows());
  }
  AppendRows(GetSS()->GetSpinCount()+1);
  for (long i=0; i < GetSS()->GetSpinCount()+1; i++) {
    SetupRow(i);
    UpdateRow(i);
  }
  mUpdating=false;
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

  //The Linear, Bilienar and Quad colums should not be editable
  SetReadOnly(rowNumber,COL_LINEAR);
  SetReadOnly(rowNumber,COL_BILINEAR);
  SetReadOnly(rowNumber,COL_QUAD);
}

void SpinGrid::UpdateRow(long rowNumber) {
  if(rowNumber<0 || rowNumber>=GetSS()->GetSpinCount()) {
    return;
  }
  Spin* thisSpin = (*mHead)->GetSpin(rowNumber);
  double x,y,z;
  thisSpin->GetCoordinates(&x,&y,&z);

  //Setup the label and the element columns
  SetCellValue(rowNumber,COL_LABEL,wxString(thisSpin->GetLabel(),wxConvUTF8));

  //Setup the x,y,z coordinates
  SetCellValue(rowNumber,COL_X,wxString() << x);
  SetCellValue(rowNumber,COL_Y,wxString() << y);
  SetCellValue(rowNumber,COL_Z,wxString() << z);

  //Set the element and isotope
  long element=thisSpin->GetElement();
  wxString str(getElementSymbol(element),wxConvUTF8);
  str << wxT(" ") << wxString(getElementName(element),wxConvUTF8);
  SetCellValue(rowNumber,COL_ELEMENT,str);

  //Set the interactions
  SetCellValue(rowNumber,COL_LINEAR     ,FormatLinearInteractions(thisSpin,LINEAR));
  SetCellValue(rowNumber,COL_BILINEAR   ,FormatLinearInteractions(thisSpin,BILINEAR));
  SetCellValue(rowNumber,COL_QUAD,FormatLinearInteractions(thisSpin,QUAD));
}

void SpinGrid::OnCellChange(wxGridEvent& e) {
  if(mUpdating) {
    return;
  }
  if(e.GetCol()==COL_X) {
    double x;
    GetCellValue(e.GetRow(),e.GetCol()).ToDouble(&x);
    Chkpoint(wxT("Spin Coordinates"));
    (*mHead)->GetSpin(e.GetRow())->GetPosition().SetX(x);
  } else if(e.GetCol()==COL_Y) {
    double y;
    GetCellValue(e.GetRow(),e.GetCol()).ToDouble(&y);
    Chkpoint(wxT("Spin Coordinates"));
    (*mHead)->GetSpin(e.GetRow())->GetPosition().SetY(y);
  } else if(e.GetCol()==COL_Z) {
    double z;
    GetCellValue(e.GetRow(),e.GetCol()).ToDouble(&z);
    Chkpoint(wxT("Spin Coordinates"));
    (*mHead)->GetSpin(e.GetRow())->GetPosition().SetZ(z);
  } else if(e.GetCol()==COL_ELEMENT) {
    wxString content=GetCellValue(e.GetRow(),e.GetCol());
    long space=content.Find(wxT(" "));
    wxString symbol=content.SubString(0,space-1);
    long element=getElementBySymbol(symbol.char_str());
    if(element==-1) {
      cerr << "Reverse lookup of element symbol " << symbol.char_str() << " failed!" << endl;
    } else {
      UpdateRowIsotopes(e.GetRow());
      Chkpoint(wxT("Spin Element"));
      (*mHead)->GetSpin(e.GetRow())->SetElement(element);
    }
    cout << space << " " << symbol.char_str() << endl;
  } else if(e.GetCol()==COL_LABEL) {
    Chkpoint(wxT("Change Spin Label"));
    std::string label(GetCellValue(e.GetRow(),e.GetCol()).char_str());
    GetSS()->GetSpin(e.GetRow())->SetLabel(label);
  }
  CEventManager::Instance()->trigger(EvtSChange(e.GetRow()));
}

void SpinGrid::UpdateRowIsotopes(long row) {

}

void SpinGrid::OnCellSelect(wxGridEvent& e) {
  if(e.GetRow()==GetSS()->GetSpinCount()) {
    //The user clicked the last line, which means there is no spin to
    //edit
    e.Skip();
    return;
  }

  if(e.GetCol()==COL_LINEAR || e.GetCol()==COL_BILINEAR || e.GetCol()==COL_QUAD) {
    wxCommandEvent event(EVT_INTER_SELECT);
    event.SetEventObject( this );
    event.SetInt(e.GetRow());
    ProcessEvent(event);

  } else {
    wxCommandEvent event(EVT_INTER_UNSELECT);
    event.SetEventObject( this );
    ProcessEvent(event);
  }
  e.Skip();
}

void SpinGrid::OnRightClick(wxGridEvent& e) {
  RightClickMenu* menu=new RightClickMenu(this);
  if(e.GetRow()<(*mHead)->GetSpinCount()) {
    menu->OptionDeleteSpin(e.GetRow());
    menu->OptionShowSpinProperties(GetSS()->GetSpin(e.GetRow()));
  }
  menu->Build();
  PopupMenu(menu);
  delete menu;

}

void SpinGrid::OnDeleteSpinHover(wxCommandEvent& e) {
}

//============================================================//
// virtual bool HandleEvent(CEvent const& event);
// The McShafry style event handler

bool SpinGrid::HandleEvent(CEvent const& event) {
  if(event.getType()==EVT_SCHANGE) {
    //A single spin has changed, update one row
    UpdateRow(event.getDataPtr<EvtDataSChange>()->mSpinNumber);
  } else if(event.getType()==EVT_SSCHANGE) {
    //The entire system has changed, need to reload the grid.
    cout << "SpinGrid::HandleEvent received an SSCHANGE event" << endl;
    GetSSMgr().DumpHistory();
    RefreshFromSpinSystem();
  }
  return true;
}



BEGIN_EVENT_TABLE(SpinGrid,wxGrid)

EVT_GRID_SELECT_CELL     (         SpinGrid::OnCellSelect)
EVT_GRID_CMD_CELL_CHANGE (wxID_ANY,SpinGrid::OnCellChange)
EVT_GRID_EDITOR_HIDDEN   (         SpinGrid::OnEndEdit)
EVT_GRID_EDITOR_SHOWN    (         SpinGrid::OnEdit)

EVT_GRID_CELL_RIGHT_CLICK(         SpinGrid::OnRightClick)

END_EVENT_TABLE()


//============================================================//
// SpinGridPanel

SpinGridPanel::SpinGridPanel(wxWindow* parent,wxWindowID id) 
: wxPanel(parent,id){
  wxBoxSizer* sizer=new wxBoxSizer(wxVERTICAL);

  //NB: Construction must be in this order as wxGrid will emit a cell
  //selection event as they are being constucted. This would cause
  //this->OnInterUnSelect to be called which tries to do something
  //with mInterEdit which is currently uninialised.
  //
  //The bright side is that we don't need to explicity call
  //mInterEdit->SetSpin as the correct behavior obtained by default
  //(so long as mInterEdit is constructed)
  mInterEdit=new SpinInterEditPanel(this);
  mInterEdit->SetDialogMode(false);
  mGrid=new SpinGrid(this);

  sizer->Add(mGrid,1,wxEXPAND | wxALL);
  sizer->Add(mInterEdit,0,wxEXPAND | wxALL);

  SetSizer(sizer);

  //  mInterEdit->SetSpin(GetSS()->GetSpin(0));
}

void SpinGridPanel::OnInterSelect(wxCommandEvent& e) {
  mInterEdit->Show(true);
  mInterEdit->SetSpin(GetSS()->GetSpin(e.GetInt()));
  Layout();
}

void SpinGridPanel::OnInterUnSelect(wxCommandEvent& e) {
  mInterEdit->Show(false);
  mInterEdit->SetSpin(NULL);
  Layout();
}

BEGIN_EVENT_TABLE(SpinGridPanel,wxPanel)

EVT_COMMAND  (wxID_ANY, EVT_INTER_SELECT,   SpinGridPanel::OnInterSelect)
EVT_COMMAND  (wxID_ANY, EVT_INTER_UNSELECT, SpinGridPanel::OnInterUnSelect)

END_EVENT_TABLE()
