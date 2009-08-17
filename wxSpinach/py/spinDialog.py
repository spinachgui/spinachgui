#3rd Party/standard Imports
import wx
from wx import xrc 
import wx.grid

import spinsys

#C++ Imports
from nuclear_data import *

#Local Imports
from interactionEdit import interactionEdit


class SpinDialog(wx.Frame):
    def __init__(self,parent,data,index):
        self.data=data
        self.index=index;
        self.parent=parent;
        pre = wx.PreFrame();
        self.data.res.LoadOnFrame(pre,parent,"SpinDialog")
        self.PostCreate(pre)

        self.spinDialogPanel=xrc.XRCCTRL(self,'spinDialogPanel');
        self.interSizer=self.spinDialogPanel.GetSizer().GetItem(2).GetSizer();
        self.interEdit=interactionEdit(self.spinDialogPanel,self.data)
        self.interSizer.Add(self.interEdit,1.0,wx.EXPAND | wx.ALL);

    def Show(self):
        wx.Frame.Show(self);

