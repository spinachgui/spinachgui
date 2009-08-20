#3rd Party/standard Imports
import wx
from wx import xrc 
import wx.grid
import wx.lib.popupctl
import  wx.calendar     as  cal

import spinsys

#C++ Imports
from nuclear_data import *

#Local python imports
from utils import *
from orientationEdit import OrientTextEditor

UNDEFINED=spinsys.Interaction.UNDEFINED;  
SCALAR=spinsys.Interaction.SCALAR;     
MATRIX=spinsys.Interaction.MATRIX;     
EIGENVALUES=spinsys.Interaction.EIGENVALUES;
AXRHOM=spinsys.Interaction.AXRHOM;     
SPANSKEW=spinsys.Interaction.SPANSKEW;   

TypeOders=[SCALAR,MATRIX,EIGENVALUES,AXRHOM,SPANSKEW];

Types={
    UNDEFINED:(-1,"Undefined"),
    SCALAR:(0,"Scalar"),
    MATRIX:(1,"Matrix"),
    EIGENVALUES:(2,"Eigenvalues"),
    AXRHOM:(3,"Axiality-Rhombicty"),
    SPANSKEW:(4,"Span-Skew")
}


ST_ANY        =spinsys.Interaction.ST_ANY;
ST_NMR        =spinsys.Interaction.ST_NMR;       
ST_EPR        =spinsys.Interaction.ST_EPR;       
ST_HFC        =spinsys.Interaction.ST_HFC;       
ST_G_TENSER   =spinsys.Interaction.ST_G_TENSER;
ST_ZFS        =spinsys.Interaction.ST_ZFS;   
ST_EXCHANGE   =spinsys.Interaction.ST_EXCHANGE;
ST_SHIELDING  =spinsys.Interaction.ST_SHIELDING;
ST_SCALAR     =spinsys.Interaction.ST_SCALAR; 
ST_QUADRUPOLAR=spinsys.Interaction.ST_QUADRUPOLAR;
ST_DIPOLAR    =spinsys.Interaction.ST_DIPOLAR;
ST_CUSTOM     =spinsys.Interaction.ST_CUSTOM;   



SubTypes={
    ST_ANY:       (-3,"Any"),
    ST_NMR:       (-2,"NMR"),
    ST_EPR:       (-1,"EPR"),
    ST_HFC:       (0,"Hyperfine"),
    ST_G_TENSER:  (1,"G Tensor"),
    ST_ZFS:       (2,"Zero Field Splitting"),
    ST_EXCHANGE:  (3,"Exchange Coupling"),
    ST_SHIELDING: (4,"Chemical Shilding"),
    ST_SCALAR:    (5,"Scalar"),
    ST_QUADRUPOLAR:(6,"Quadrupolar"),
    ST_DIPOLAR:   (7,"Dipolar"),
    ST_CUSTOM:    (8,"Custem")
}

class ScalarEdit(wx.Panel):
    def __init__(self,parent,id=-1):
        pre = wx.PrePanel();
        wx.GetApp().res.LoadOnPanel(pre,parent,"ScalarIntPanel");
        self.PostCreate(pre);


class MatrixEdit(wx.Panel):
    def __init__(self,parent,id=-1):
        pre = wx.PrePanel();
        wx.GetApp().res.LoadOnPanel(pre,parent,"MatrixIntPanel");
        self.PostCreate(pre);

class EigenEdit(wx.Panel):
    def __init__(self,parent,id=-1):
        pre = wx.PrePanel();
        wx.GetApp().res.LoadOnPanel(pre,parent,"EigenIntPanel");
        self.PostCreate(pre);

        self.orientEdit=OrientTextEditor(self);
        self.GetSizer().Add(self.orientEdit,1.0,wx.ALL);

class AxRhomEdit(wx.Panel):
    def __init__(self,parent,id=-1):
        pre = wx.PrePanel();
        wx.GetApp().res.LoadOnPanel(pre,parent,"AxRhomIntPanel");
        self.PostCreate(pre);

        self.orientEdit=OrientTextEditor(self);
        self.GetSizer().Add(self.orientEdit,1.0,wx.ALL);

class SpanSkewEdit(wx.Panel):
    def __init__(self,parent,id=-1):
        pre = wx.PrePanel();
        wx.GetApp().res.LoadOnPanel(pre,parent,"SpanSkewIntPanel");
        self.PostCreate(pre);

        self.orientEdit=OrientTextEditor(self);
        self.GetSizer().Add(self.orientEdit,1.0,wx.ALL);


class InterPopup(wx.Frame):
    """Adds a bit of text and mouse movement to the wx.PopupWindow"""
    def __init__(self, parent):
        wx.Frame.__init__(self, parent,style=wx.FRAME_FLOAT_ON_PARENT|wx.FRAME_NO_TASKBAR)

        self.sizer=wx.BoxSizer()
        self.interPanel=interactionEdit(self)
        self.sizer.Add(self.interPanel,1.0,wx.EXPAND);
        self.SetSizer(self.sizer)
        

        size = self.interPanel.GetSize()
        self.SetSize((size.width, size.height));

        self.Bind(wx.EVT_KILL_FOCUS, self.onUnFocus, self)
        self.Bind(wx.EVT_SET_FOCUS, self.onFocus, self)

    def onUnFocus(self,e):
        print e.GetWindow();
        e.Skip();

    def onFocus(self,e):
        print e.GetWindow();
        e.Skip()



class InterTextEditor(wx.TextCtrl):
    def __init__(self,parent,id=-1):
        wx.TextCtrl.__init__(self,parent,id)

        self.win = InterPopup(self)

        self.Bind(wx.EVT_KILL_FOCUS, self.onUnFocus, self)
        self.Bind(wx.EVT_SET_FOCUS, self.onFocus, self)

    def onUnFocus(self,e):
        print e.GetWindow();
        #self.win.Show(False);
        e.Skip()

    def onFocus(self, e):

        # Show the popup right below or above the button
        # depending on available screen space...
        pos = self.ClientToScreen(self.GetPosition())
        size =  self.GetSize()
        self.win.SetPosition((pos[0], pos[1]+size[1]))

        self.win.Show(True);
        e.Skip()



class interactionEdit(wx.Panel):
    def __init__(self,parent,spin,id=-1):
        wx.Panel.__init__(self,parent,id=-1)
        pre = wx.PrePanel();
        wx.GetApp().res.LoadOnPanel(pre,parent,"InterEditPanel");
        self.PostCreate(pre);

        self.spin=spin

        self.typeChoiceBook=xrc.XRCCTRL(self,"TypeChoiceBook");
        self.interListBox=xrc.XRCCTRL(self,"interEditCtrl");
        self.interListBox.SetMinSize(wx.Size(-1,200));

        self.scalarPage=  ScalarEdit  (self.typeChoiceBook)
        self.matrixPage=  MatrixEdit  (self.typeChoiceBook)
        self.eigenPage=   EigenEdit   (self.typeChoiceBook)
        self.axrhomPage=  AxRhomEdit  (self.typeChoiceBook)
        self.spanskewPage=SpanSkewEdit(self.typeChoiceBook)

        self.typeChoiceBook.AddPage(self.scalarPage,"Scalar")
        self.typeChoiceBook.AddPage(self.matrixPage,"Matrix")
        self.typeChoiceBook.AddPage(self.eigenPage,"Eigenvalues")
        self.typeChoiceBook.AddPage(self.axrhomPage,"Axiality Rhombicity")
        self.typeChoiceBook.AddPage(self.spanskewPage,"Span Skew")
        
        self.interListBox.Bind(wx.EVT_LISTBOX,self.onInteractionChange);

        self.LoadFromSpin();

    def LoadFromSpin(self):
        self.interListBox.Clear();

        interactions=self.spin.GetInteractions();
        for inter in interactions:
            self.interListBox.Append(SubTypes[inter.GetSubType()][1] + " ("+Types[inter.GetType()][1]+")")
       
    def onInteractionChange(self,e):
        inter=self.spin.GetInteraction(e.GetSelection());
        self.typeChoiceBook.SetSelection(Types[inter.GetType()][0]);

