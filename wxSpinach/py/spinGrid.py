
#3rd Party/standard Imports
import wx
import wx.grid

import spinsys

#C++ Imports
from nuclear_data import *

#Local python imports
from utils import *
from interactionEdit import SpinInteractionsEdit,InterTextEditor,InterPopup

#Column descriptions

COL_SELECTED=0;   
COL_LABEL=1;       
COL_ELEMENT=2;    
COL_ISOTOPES=3;   
COL_X=4;          
COL_Y=5;          
COL_Z=6;          
COL_LINEAR=7;     
COL_BILINEAR=8;   
COL_QUADRAPOLAR=9;

columnOrder=[
    COL_SELECTED,
    COL_LABEL,
    COL_ELEMENT,
    COL_ISOTOPES,
    COL_X,
    COL_Y,
    COL_Z,
    COL_LINEAR,
    COL_BILINEAR,
    COL_QUADRAPOLAR    
]

#Name,Size
columnDetails={
    COL_SELECTED   :  ("Selected",73),
    COL_LABEL      :  ("Label",105),
    COL_ELEMENT    :  ("Element",70),
    COL_ISOTOPES   :  ("Isotopes",70),
    COL_X          :  ("x",70),
    COL_Y          :  ("y",70),
    COL_Z          :  ("z",70),
    COL_LINEAR     :  ("Linear",70),
    COL_BILINEAR   :  ("Bilinear",70),
    COL_QUADRAPOLAR:  ("Quadrapolar",100)
}





class InterCellEditor(wx.grid.PyGridCellEditor):
    """
    This is a custem cell editor for editing interactions. It's based off the exmaple code in
    GridCustEditor.py in the wxPython samples
    """
    def __init__(self,spin):
        self.spin=spin
        wx.grid.PyGridCellEditor.__init__(self)
        self.keycode=0;

    def Create(self, parent, id, evtHandler):
        """
        Called to create the control, which must derive from wx.Control.
        *Must Override*
        """
        self.tc = InterTextEditor(parent,self.spin,id)
        self.tc.SetInsertionPoint(0)
        self.SetControl(self.tc)

        if evtHandler:
            self.tc.PushEventHandler(evtHandler)


    def SetSize(self, rect):
        """
        Called to position/size the edit control within the cell rectangle.
        If you don't fill the cell (the rect) then be sure to override
        PaintBackground and do something meaningful there.
        """
        self.tc.SetDimensions(rect.x, rect.y, rect.width+2, rect.height+2,
                               wx.SIZE_ALLOW_MINUS_ONE)


    def Show(self, show, attr):
        """
        Show or hide the edit control.  You can use the attr (if not None)
        to set colours or fonts for the control.
        """
        wx.grid.PyGridCellEditor.Show(self,show, attr)


    def BeginEdit(self, row, col, grid):
        """
        Fetch the value from the table and prepare the edit control
        to begin editing.  Set the focus to the edit control.
        *Must Override*
        """

        self.startValue = grid.GetTable().GetValue(row, col)
        self.tc.SetValue(self.startValue)
        self.tc.SetInsertionPointEnd()
        self.tc.SetFocus()



    def EndEdit(self, row, col, grid):
        """
        Complete the editing of the current cell. Returns True if the value
        has changed.  If necessary, the control may be destroyed.
        *Must Override*
        """
        changed = False

        val = self.tc.GetValue()
        
        if val != self.startValue:
            changed = True
            grid.GetTable().SetValue(row, col, val) # update the table

        self.startValue = ''
        self.tc.SetValue('')
        return changed


    def Reset(self):
        """
        Reset the value in the control back to its starting value.
        *Must Override*
        """
        self._tc.SetValue(self.startValue)
        self._tc.SetInsertionPointEnd()
        pass

    def IsAcceptedKey(self, evt):
        """
        Return True to allow the given key to start editing: the base class
        version only checks that the event has no modifiers.  F2 is special
        and will always start the editor.
        """

        ## We can ask the base class to do it
        if  wx.grid.PyGridCellEditor.IsAcceptedKey(self,evt):
            return True;

        return False;    



    def StartingKey(self, e):
        """
        If the editor is enabled by pressing keys on the grid, this will be
        called to let the editor do something about that first key if desired.
        """
        key = evt.GetKeyCode()
        ch = None
        if key in [ wx.WXK_NUMPAD0, wx.WXK_NUMPAD1, wx.WXK_NUMPAD2, wx.WXK_NUMPAD3, 
                    wx.WXK_NUMPAD4, wx.WXK_NUMPAD5, wx.WXK_NUMPAD6, wx.WXK_NUMPAD7, 
                    wx.WXK_NUMPAD8, wx.WXK_NUMPAD9
                    ]:

            ch = ch = chr(ord('0') + key - wx.WXK_NUMPAD0)

        elif key < 256 and key >= 0 and chr(key) in string.printable:
            ch = chr(key)

        if ch is not None:
            # For this example, replace the text.  Normally we would append it.
            self_tc.AppendText(ch)
            self.tc.SetInsertionPointEnd()
        else:
            evt.Skip()



    def StartingClick(self):
        """
        If the editor is enabled by clicking on the cell, this method will be
        called to allow the editor to simulate the click on the control if
        needed.
        """
        self.keycode=0;

    def Destroy(self):
        """final cleanup"""
        wx.grid.PyGridCellEditor.Destroy()


    def Clone(self):
        """
        Create a new object which is the copy of this one
        *Must Override*
        """
        return InterCellEditor(self.spin)


class SpinGrid(wx.grid.Grid):
    def __init__(self,parent,id=-1):

        wx.grid.Grid.__init__(self,parent,id=-1)
        self.ss=wx.GetApp().ss;

        self.CreateGrid( self.ss.GetSpinCount()+1, len(columnOrder) );

        self.EnableEditing( True );
        self.EnableGridLines( True );
        self.EnableDragGridSize( True );
        self.EnableDragColMove(False);
        self.EnableDragColSize(True);

        self.SetMargins(0,0); 
        self.SetColLabelSize(20);

        i=0
        for column in columnOrder:
            self.SetColSize(i,columnDetails[column][1]);
            self.SetColLabelValue(i,columnDetails[column][0]);
            i=i+1


        self.SetColLabelAlignment( wx.ALIGN_CENTRE, wx.ALIGN_CENTRE );
	
        self.SetRowLabelSize(40);
        self.SetRowLabelAlignment( wx.ALIGN_CENTRE, wx.ALIGN_CENTRE );

	#Event Handling
        self.Bind(wx.grid.EVT_GRID_SELECT_CELL,self.onCellSelect,id=self.GetId());
        self.Bind(wx.grid.EVT_GRID_CMD_CELL_CHANGE,self.onCellChange,id=self.GetId());
        self.Bind(wx.grid.EVT_GRID_EDITOR_HIDDEN,self.onEndEdit,id=self.GetId());
        self.Bind(wx.grid.EVT_GRID_EDITOR_SHOWN,self.onEdit,id=self.GetId());


    
    def onEdit(self,e):
        if e.GetCol()==COL_LINEAR or e.GetCol()==COL_QUADRAPOLAR:
            self.hidePopup();

    def onEndEdit(self,e):
        if e.GetCol()==COL_LINEAR or e.GetCol()==COL_QUADRAPOLAR:
            self.showPopup();

    def showPopup(self):
        pos = self.ClientToScreen((10,10))
        self.win.SetPosition(pos)
        self.win.Show(True);
        wx.GetTopLevelParent(self).Raise(); #The window's appearence on the screen should be passive. It should not grab the focus away from the grid.

    def hidePopup(self):
        self.win.Show(False);


    def refreshFromSpinSystem(self):
        self.ClearGrid();
        self.AppendRows(self.ss.GetSpinCount());
        for i in range(self.ss.GetSpinCount()):
            thisSpin=self.ss.GetSpin(i);
            x,y,z=thisSpin.GetPosition().GetCoordinates();

            #Set the selected column renderers and editors to be boolian
            self.SetCellRenderer(i,COL_SELECTED,wx.grid.GridCellBoolRenderer())
            self.SetCellEditor(i,COL_SELECTED,wx.grid.GridCellBoolEditor())

            #Setup the label and the element columns
            self.SetCellValue(i,COL_LABEL,thisSpin.GetLabel());

            self.SetCellValue(i,COL_X,str(x));
            self.SetCellValue(i,COL_Y,str(y));
            self.SetCellValue(i,COL_Z,str(z));
            
            #Set up floating point editors, so the user can't enter something stupid
            self.SetCellEditor(i,COL_X,wx.grid.GridCellFloatEditor());
            self.SetCellEditor(i,COL_Y,wx.grid.GridCellFloatEditor());
            self.SetCellEditor(i,COL_Z,wx.grid.GridCellFloatEditor());
            
            #Set up the special interaction editors
            self.SetCellEditor(i,COL_LINEAR,InterCellEditor(self.ss.GetSpin(i)))
            #self.SetCellEditor(i,COL_BILINEAR,InterCellEditor())
            self.SetCellEditor(i,COL_QUADRAPOLAR,InterCellEditor(self.ss.GetSpin(i)))

        self.win = InterPopup(self,self.ss.GetSpin(0))


    def onCellChange(self,e):
        print "Cell change"
        if(e.GetCol()==COL_X):
            print "X Change"
            self.ss.GetSpin(e.GetRow()).GetPosition().SetX(float(self.GetCellValue(e.GetRow(),e.GetCol())));
        elif(e.GetCol()==COL_Y):
            self.ss.GetSpin(e.GetRow()).GetPosition().SetY(float(self.GetCellValue(e.GetRow(),e.GetCol())));
        elif(e.GetCol()==COL_Z):
            self.ss.GetSpin(e.GetRow()).GetPosition().SetZ(float(self.GetCellValue(e.GetRow(),e.GetCol())));
        wx.GetApp().em.Trigger();

    def onCellSelect(self,e):
        if e.GetCol()==COL_LINEAR or e.GetCol()==COL_QUADRAPOLAR:
            self.showPopup();
        else:
            self.hidePopup();
        e.Skip()

