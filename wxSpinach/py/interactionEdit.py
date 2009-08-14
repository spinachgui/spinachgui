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

INTERTYPE_SCALAR=0;
INTERTYPE_MATRIX=1;
INTERTYPE_EIGEN=2;
INTERTYPE_AXRHOM=3;
INTERTYPE_SPANSKEW=4;

class ScalarEdit(wx.Panel):
    def __init__(self,parent,data,id=-1):
        pre = wx.PrePanel();
        data.res.LoadOnPanel(pre,parent,"ScalarIntPanel");
        self.PostCreate(pre);

        self.data=data

class MatrixEdit(wx.Panel):
    def __init__(self,parent,data,id=-1):
        pre = wx.PrePanel();
        data.res.LoadOnPanel(pre,parent,"MatrixIntPanel");
        self.PostCreate(pre);

        self.data=data

class EigenEdit(wx.Panel):
    def __init__(self,parent,data,id=-1):
        pre = wx.PrePanel();
        data.res.LoadOnPanel(pre,parent,"EigenIntPanel");
        self.PostCreate(pre);

        self.data=data
        
        self.orientEdit=OrientTextEditor(self,data);
        self.GetSizer().Add(self.orientEdit,1.0,wx.ALL);

class AxRhomEdit(wx.Panel):
    def __init__(self,parent,data,id=-1):
        pre = wx.PrePanel();
        data.res.LoadOnPanel(pre,parent,"AxRhomIntPanel");
        self.PostCreate(pre);

        self.data=data

        self.orientEdit=OrientTextEditor(self,data);
        self.GetSizer().Add(self.orientEdit,1.0,wx.ALL);

class SpanSkewEdit(wx.Panel):
    def __init__(self,parent,data,id=-1):
        pre = wx.PrePanel();
        data.res.LoadOnPanel(pre,parent,"SpanSkewIntPanel");
        self.PostCreate(pre);

        self.data=data

        self.orientEdit=OrientTextEditor(self,data);
        self.GetSizer().Add(self.orientEdit,1.0,wx.ALL);


class InterPopup(wx.Frame):
    """Adds a bit of text and mouse movement to the wx.PopupWindow"""
    def __init__(self, parent,data):
        wx.Frame.__init__(self, parent,style=wx.FRAME_FLOAT_ON_PARENT|wx.FRAME_NO_TASKBAR)
        self.data=data

        self.sizer=wx.BoxSizer()
        self.interPanel=interactionEdit(self,self.data)
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
    def __init__(self,parent,data,id=-1):
        wx.TextCtrl.__init__(self,parent,id)
        self.data=data

        self.win = InterPopup(self,self.data)

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
    def __init__(self,parent,data,id=-1):
        wx.Panel.__init__(self,parent,id)
        self.data=data

        pre = wx.PrePanel();
        self.data.res.LoadOnPanel(pre,parent,"InterEditPanel");
        self.PostCreate(pre);

        self.typeChoiceBook=xrc.XRCCTRL(self,"TypeChoiceBook");

        self.scalarPage=  ScalarEdit  (self.typeChoiceBook,self.data)
        self.matrixPage=  MatrixEdit  (self.typeChoiceBook,self.data)
        self.eigenPage=   EigenEdit   (self.typeChoiceBook,self.data)
        self.axrhomPage=  AxRhomEdit  (self.typeChoiceBook,self.data)
        self.spanskewPage=SpanSkewEdit(self.typeChoiceBook,self.data)

        self.typeChoiceBook.AddPage(self.scalarPage,"Scalar")
        self.typeChoiceBook.AddPage(self.matrixPage,"Matrix")
        self.typeChoiceBook.AddPage(self.eigenPage,"Eigenvalues")
        self.typeChoiceBook.AddPage(self.axrhomPage,"Axiality Rhombicity")
        self.typeChoiceBook.AddPage(self.spanskewPage,"Span Skew")
        
        

        

    def loadEditPanel(panelNumber):
        if(panelNumber==INTERTYPE_SCALAR):
            self.interEditHook
        elif(panelNumber==INTERTYPE_MATRIX):
            pass
        elif(panelNumber==INTERTYPE_EIGEN):
            pass
        elif(panelNumber==INTERTYPE_AXRHOM):
            pass
        elif(panelNumber==INTERTYPE_SPANSKEW):
            pass
