
#include <gui/SpinachApp.hpp>
#include <gui/SpinGrid.hpp>
#include <shared/nuclear_data.hpp>
#include <iostream>
#include <gui/RightClickMenu.hpp>
#include <wx/debug.h>
#include <shared/foreach.hpp>

using namespace std;
using namespace Eigen;
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
    {COL_ISOTOPES,   "Mass Number",70},  
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

    GetRawSS()->sigReloaded.connect(mem_fun(this,&SpinGrid::RefreshFromSpinSystem));
    GetRawSS()->sigNewSpin.connect(mem_fun(this,&SpinGrid::OnNewSpin));
	sigAnySpinDying.connect(mem_fun(this,&SpinGrid::SlotAnySpinDie));

	sigHover.connect(mem_fun(this,&SpinGrid::SlotHover));
	sigSelectChange.connect(mem_fun(this,&SpinGrid::SlotSelectChange));
	
    RefreshFromSpinSystem();
}

void SpinGrid::SlotAnySpinDie(Spin* spin) {
	//Quick hack, we should probably just delete the row
	RefreshFromSpinSystem();
	return;
}

void SpinGrid::ColourRow(long rowNumber,const wxColor& c) {
	for(int i=0;i<GetNumberCols();i++){
		SetCellBackgroundColour(rowNumber,i,c);
	}
	Refresh();
}

void SpinGrid::SlotHover(Spin* spin) {
	//Change the colour of the last spin hovered over back to how it
	//was before
	if(mLastHover != -1) {
		Spin* lastSpin = GetRawSS()->GetSpin(mLastHover);
		cout << GetSelection().size() << endl;
		if(GetSelection().find(lastSpin) == GetSelection().end()) {
			ColourRow(mLastHover,defaultC);
		} else {
			ColourRow(mLastHover,selectedC);
		}
		mLastHover = -1;
	}
	//Now paint the new spin
	if(spin == NULL) {
		return;
	}
	long spinN = GetRawSS()->GetSpinNumber(spin);
	ColourRow(spinN,hoverC);
	mLastHover = spinN;
}

void SpinGrid::SlotSelectChange(set<Spin*> spins) {
	for(long i=0;i<GetNumberRows();i++) {
		ColourRow(i,defaultC);
	}
	foreach(Spin* spin,spins) {
		long row = GetRawSS()->GetSpinNumber(spin);
		ColourRow(row,selectedC);
		SetGridCursor(row,COL_LABEL);
		//According to the wxWidgets documentation SetGridCursor
		//should call MakeCellVisible, but that doesn't seem to be the
		//case
		MakeCellVisible(row,COL_LABEL);
	}
	SetFocus();
}


void SpinGrid::OnEdit(wxGridEvent& e) {
    long sc=GetRawSS()->GetSpinCount();
    if(e.GetRow()==sc) {
        //User is trying to edit the blank line at the bottom of the grid,
        //so create a new spin for them
        GetRawSS()->InsertSpin(new Spin(Vector3d(0,0,0),"New Spin",1));
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
    long count=GetRawSS()->GetSpinCount();
    AppendRows(count+1);
    for (long i=0; i < count; i++) {
        SetupRow(i);
		UpdateRow(i);
    }
	SetupRow(count); //The last row is for clicking on to make a new spin
    mUpdating=false;
}

void SpinGrid::UpdateRow(long rowNumber) {
		cout << rowNumber << endl;
        Spin* spin = GetRawSS()->GetSpin(rowNumber);
		Vector3d v = spin->GetPosition();
		Vector3d vprime = FromLabVec3d(GetFrame(),v);
        //Set the label
        SetCellValue(rowNumber,SpinGrid::COL_LABEL,wxString(spin->GetLabel(),wxConvUTF8));

        //Set the x,y,z coordinates
        SetCellValue(rowNumber,SpinGrid::COL_X,wxString() << vprime.x() / GetUnitSystem()->lengthUnit);
        SetCellValue(rowNumber,SpinGrid::COL_Y,wxString() << vprime.y() / GetUnitSystem()->lengthUnit);
        SetCellValue(rowNumber,SpinGrid::COL_Z,wxString() << vprime.z() / GetUnitSystem()->lengthUnit);

        //Set the element and isotope
        long element=spin->GetElement();
        long isotope=spin->GetIsotope();
        wxString str(getElementSymbol(element),wxConvUTF8);
        str << wxT(" ") << wxString(getElementName(element),wxConvUTF8);
        SetCellValue(rowNumber,SpinGrid::COL_ELEMENT,str);
		SetCellValue(rowNumber,SpinGrid::COL_ISOTOPES,wxString() << isotope + element);
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
		Vector3d vprime = ToLabVec3d(GetFrame(),Vector3d(x,y,z));
		
        GetRawSS()->GetSpin(e.GetRow())->SetPosition(vprime * GetUnitSystem()->lengthUnit);
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
            GetRawSS()->GetSpin(e.GetRow())->SetElement(element);
        }
    } else if(e.GetCol()==SpinGrid::COL_ISOTOPES) {
		long iso;
        GetCellValue(e.GetRow(),COL_ISOTOPES).ToLong(&iso);
		Spin* spin =GetRawSS()->GetSpin(e.GetRow());
		spin->SetIsotope(iso-spin->GetElement());
	} else if(e.GetCol()==COL_LABEL) {
        Chkpoint(wxT("Change Spin Label"));
        std::string label(GetCellValue(e.GetRow(),e.GetCol()).char_str());
        GetRawSS()->GetSpin(e.GetRow())->SetLabel(label);
    } else if(e.GetCol()==COL_SELECTED){
        wxString value=GetCellValue(e.GetRow(),e.GetCol());
        if(value == wxT("1")) {
            AddSelection(GetRawSS()->GetSpin(e.GetRow()));
        } else {
            RemoveSelection(GetRawSS()->GetSpin(e.GetRow()));
        }
    }
}

void SpinGrid::UpdateRowIsotopes(long row) {

}

void SpinGrid::OnCellSelect(wxGridEvent& e) {
    if(e.GetRow()==GetRawSS()->GetSpinCount()) {
        //The user clicked the last line, which means there is no spin to
        //edit
        e.Skip();
        sigSelect((Spin*)NULL);
        return;
    }
    sigSelect(GetRawSS()->GetSpin(e.GetRow()));
    e.Skip();
}

void SpinGrid::OnRightClick(wxGridEvent& e) {
    RightClickMenu* menu=new RightClickMenu(this);
    menu->Build();
    PopupMenu(menu);
    delete menu;
}

void SpinGrid::OnDeleteSpinHover(wxCommandEvent& e) {
	//Currently there isn't a clear way to do hovering
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



