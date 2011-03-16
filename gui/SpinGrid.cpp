
#include <gui/SpinachApp.hpp>
#include <gui/SpinGrid.hpp>
#include <shared/nuclear_data.hpp>
#include <iostream>
#include <gui/RightClickMenu.hpp>
#include <wx/debug.h>

using namespace std;

using namespace SpinXML;

const long ColumCount=7; 
 
const wxColor defaultC(255,255,255);
const wxColor selectedC(200,255,255);
const wxColor hoverC(200,255,200);

//============================================================//
// Utility functions.


//============================================================//
// SpinGrid

const SpinGrid::SpinGridColum SpinGrid::columns[]={
    {COL_SELECTED,   "",20},
    {COL_LABEL,      "Label",105},    
    {COL_ELEMENT,    "Element",70},   
    {COL_ISOTOPES,   "Isotopes",70},  
    {COL_X,          "x",70},	    
    {COL_Y,          "y",70},	    
    {COL_Z,          "z",70}
};		   



SpinGrid::SpinGrid(wxWindow* parent)
    :wxGrid(parent,wxID_ANY),mUpdating(false),mLastHover(-1) {

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

	//Signals

    GetSS().sigReloaded.connect(mem_fun(this,&SpinGrid::RefreshFromSpinSystem));
    GetSS().sigNewSpin.connect(mem_fun(this,&SpinGrid::OnNewSpin));

	sigHover.connect(mem_fun(this,&SpinGrid::SlotHover));
	sigSelectChange.connect(mem_fun(this,&SpinGrid::SlotSelectChange));
	
    RefreshFromSpinSystem();
}

void SpinGrid::ColourRow(long rowNumber,const wxColor& c) {
	for(int i=0;i<GetNumberCols();i++){
		SetCellBackgroundColour(rowNumber,i,c);
	}
	Refresh();
}

void SpinGrid::SlotHover(Spin* spin) {
	if(mLastHover != -1) {
		ColourRow(mLastHover,wxColor(defaultC));
		mLastHover = -1;
	}
	if(spin == NULL) {
		return;
	}
	long spinN = GetSS()->GetSpinNumber(spin);
	ColourRow(spinN,wxColor(hoverC));
	mLastHover = spinN;
}

void SpinGrid::SlotSelectChange(set<Spin*> spins) {
	for(long i=0;i<GetNumberRows();i++) {
		ColourRow(i,defaultC);
	}
	for(set<Spin*>::iterator i = spins.begin();i!=spins.end();++i) {
		long row = GetSS()->GetSpinNumber(*i);
		ColourRow(row,selectedC);
	}
}


void SpinGrid::OnEdit(wxGridEvent& e) {
    long sc=GetSS()->GetSpinCount();
    if(e.GetRow()==sc) {
        //User is trying to edit the blank line at the bottom of the grid,
        //so create a new spin for them
        GetSS()->InsertSpin(new Spin(Vector3d(0,0,0),"New Spin",1));
    }
}

void SpinGrid::OnNewSpin(Spin* newSpin,long number) {
    //Somehow, somewhere a new spin has been created, so create a new
    //row for it at the end of the grid
    SetupRow(number);
	UpdateRow(number);
    AppendRows(1);
}

void SpinGrid::OnEndEdit(wxGridEvent& e) {
}

bool SpinGrid::DeleteRows(int pos,int numRows,bool updateLables) {
  bool r=wxGrid::DeleteRows(pos,numRows,updateLables);
  sigRowDelete(pos,numRows);
  return r;
}

void SpinGrid::RefreshFromSpinSystem() {
    mUpdating=true;
    sigClearing();

    if(GetNumberRows()) {
        //Clear grid only clears the underlying data rather. The cells
        //remain and are black. wxGrid::DeleteRows solves this
        DeleteRows(0,GetNumberRows());
    }
    long count=GetSS()->GetSpinCount();
    AppendRows(count+1);
    for (long i=0; i < count; i++) {
        SetupRow(i);
		UpdateRow(i);
    }
	SetupRow(count); //The last row is for clicking on to make a new spin
    mUpdating=false;
}

void SpinGrid::UpdateRow(long rowNumber) {
        length x,y,z;
		cout << rowNumber << endl;
        SpinView spin = GetSS().GetSpin(rowNumber);
		spin.GetCoordinates(&x,&y,&z);

        //Setup the label and the element columns
        SetCellValue(rowNumber,SpinGrid::COL_LABEL,wxString(spin->GetLabel(),wxConvUTF8));

        //Setup the x,y,z coordinates
        SetCellValue(rowNumber,SpinGrid::COL_X,wxString() << x);
        SetCellValue(rowNumber,SpinGrid::COL_Y,wxString() << y);
        SetCellValue(rowNumber,SpinGrid::COL_Z,wxString() << z);

        //Set the element and isotope
        long element=spin->GetElement();
        wxString str(getElementSymbol(element),wxConvUTF8);
        str << wxT(" ") << wxString(getElementName(element),wxConvUTF8);
        SetCellValue(rowNumber,SpinGrid::COL_ELEMENT,str);
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

void SpinGrid::OnCellChange(wxGridEvent& e) {
    if(mUpdating) {
        return;
    }
    if(e.GetCol()==COL_X || e.GetCol()==COL_Y || e.GetCol()==COL_Z) {
        double x;
        double y;
        double z;
        GetCellValue(e.GetRow(),COL_X).ToDouble(&x);
        GetCellValue(e.GetRow(),COL_Y).ToDouble(&y);
        GetCellValue(e.GetRow(),COL_Z).ToDouble(&z);

        Chkpoint(wxT("Spin Coordinates"));
        GetSS().GetSpin(e.GetRow()).SetCoordinates(x,y,z);
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
            GetSS().GetSpin(e.GetRow())->SetElement(element);
        }
        cout << space << " " << symbol.char_str() << endl;
    } else if(e.GetCol()==COL_LABEL) {
        Chkpoint(wxT("Change Spin Label"));
        std::string label(GetCellValue(e.GetRow(),e.GetCol()).char_str());
        GetSS()->GetSpin(e.GetRow())->SetLabel(label);
    } else if(e.GetCol()==COL_SELECTED){
        wxString value=GetCellValue(e.GetRow(),e.GetCol());
        if(value == wxT("1")) {
            AddSelection(GetSS()->GetSpin(e.GetRow()));
        } else {
            RemoveSelection(GetSS()->GetSpin(e.GetRow()));
        }
    }
}

void SpinGrid::UpdateRowIsotopes(long row) {

}

void SpinGrid::OnCellSelect(wxGridEvent& e) {
    if(e.GetRow()==GetSS()->GetSpinCount()) {
        //The user clicked the last line, which means there is no spin to
        //edit
        e.Skip();
        sigSelect((Spin*)NULL);
        return;
    }
    sigSelect(GetSS()->GetSpin(e.GetRow()));
    e.Skip();
}

void SpinGrid::OnRightClick(wxGridEvent& e) {
    RightClickMenu* menu=new RightClickMenu(this);
    menu->Build();
    PopupMenu(menu);
    delete menu;
}

void SpinGrid::OnDeleteSpinHover(wxCommandEvent& e) {
}

void SpinGrid::OnMouseMove(wxMouseEvent& e) {
    cout << "Mouse event" << endl;
    long y=e.GetY();

    int row=YToRow(y);
    if(row != wxNOT_FOUND) {
        for(int i=0;i<GetNumberCols();i++){
            SetCellBackgroundColour(row,i,wxColor(200,255,200));
        }
    }
}



BEGIN_EVENT_TABLE(SpinGrid,wxGrid)

EVT_GRID_SELECT_CELL     (         SpinGrid::OnCellSelect)
EVT_GRID_CMD_CELL_CHANGE (wxID_ANY,SpinGrid::OnCellChange)
EVT_GRID_EDITOR_HIDDEN   (         SpinGrid::OnEndEdit)
EVT_GRID_EDITOR_SHOWN    (         SpinGrid::OnEdit)

EVT_GRID_CELL_RIGHT_CLICK(         SpinGrid::OnRightClick)

//This doesn't work, at th emoment
EVT_MOTION               (         SpinGrid::OnMouseMove)

END_EVENT_TABLE()



