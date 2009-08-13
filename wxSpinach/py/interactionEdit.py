#3rd Party/standard Imports
import wx
from wx import xrc 
import wx.grid

import spinsys

#C++ Imports
from nuclear_data import *

#Local python imports
from utils import *


class interactionEdit(wx.Panel):
    def __init__(self,parent,data,id=-1):
        wx.Panel.__init__(self,parent,id)
        self.data=data

        pre = wx.PrePanel();
        self.data.res.LoadOnPanel(pre,parent,"InterEditPanel");
        self.PostCreate(pre);

