#3rd Party/standard Imports
import wx
from wx import xrc 
import wx.grid

import spinsys

#C++ Imports
from nuclear_data import *

#Local python imports
from utils import *

INTERTYPE_SCALAR=0;
INTERTYPE_MATRIX=1;
INTERTYPE_EIGEN=2;
INTERTYPE_AXRHOM=3;
INTERTYPE_SPANSKEW=4;


class interactionEdit(wx.Panel):
    def __init__(self,parent,data,id=-1):
        wx.Panel.__init__(self,parent,id)
        self.data=data

        pre = wx.PrePanel();
        self.data.res.LoadOnPanel(pre,parent,"InterEditPanel");
        self.PostCreate(pre);

        self.typeChoiceBook=xrc.XRCCTRL(self,"TypeChoiceBook");

        self.scalarPage=self.data.res.LoadPanel(self.typeChoiceBook,"ScalarIntPanel")
        self.matrixPage=self.data.res.LoadPanel(self.typeChoiceBook,"MatrixIntPanel")
        self.eigenPage=self.data.res.LoadPanel(self.typeChoiceBook,"EigenIntPanel")
        self.axrhomPage=self.data.res.LoadPanel(self.typeChoiceBook,"AxRhomIntPanel")
        self.spanskewPage=self.data.res.LoadPanel(self.typeChoiceBook,"SpanSkewIntPanel")

        self.typeChoiceBook.AddPage(self.scalarPage,"Scalar")
        self.typeChoiceBook.AddPage(self.matrixPage,"Matrix")
        self.typeChoiceBook.AddPage(self.eigenPage,"Eigenvalues")
        self.typeChoiceBook.AddPage(self.axrhomPage,"Axiality Rhombicity")
        self.typeChoiceBook.AddPage(self.spanskewPage,"Span Skew")
        
        #Build the five "plug in" panels for the five different types of interaction
        

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
