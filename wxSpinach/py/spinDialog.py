#3rd Party/standard Imports
import wx
from wx import xrc 
import wx.grid

import spinsys

#C++ Imports
from nuclear_data import *

#Local Imports
from interactionEdit import SpinInteractionsEdit


class SpinDialog(wx.Frame):
    def __init__(self,parent,spin):
        self.spin=spin;
        pre = wx.PreFrame();
        wx.GetApp().res.LoadOnFrame(pre,parent,"SpinDialog")
        self.PostCreate(pre)

        self.SetMinSize((600,600));

        self.spinDialogPanel=xrc.XRCCTRL(self,'spinDialogPanel');
        self.interSizer=self.spinDialogPanel.GetSizer().GetItem(2).GetSizer();
        self.interEdit=SpinInteractionsEdit(self.spinDialogPanel,spin=spin)
        self.interSizer.Add(self.interEdit,1.0,wx.EXPAND | wx.ALL);

        #Find all the controls that need to be referenced
        self.spinDialogPanel=xrc.XRCCTRL(self,'spinDialogPanel');
        self.spinTitle=xrc.XRCCTRL(self,'spinTitle');

        self.xPosCtrl=xrc.XRCCTRL(self,'xpos');
        self.yPosCtrl=xrc.XRCCTRL(self,'ypos');
        self.zPosCtrl=xrc.XRCCTRL(self,'zpos');

        #self.xPosCtrl.SetValidator(wx.PyValidator());
        #self.yPosCtrl.SetValidator(wx.PyValidator());
        #self.zPosCtrl.SetValidator(wx.PyValidator());

        self.elementCombo=xrc.XRCCTRL(self,'elementCombo');
        self.isotopeCombo=xrc.XRCCTRL(self,'isotopeCombo');

        #self.Fit()
        self.LoadFromSpin();


    def Show(self):
        wx.Frame.Show(self);


    def LoadFromSpin(self):
        x,y,z=self.spin.GetCoordinates()
        label=self.spin.GetLabel()

        self.spinTitle.SetValue(str(label));

        self.xPosCtrl.SetValue(str(x));
        self.yPosCtrl.SetValue(str(y));
        self.zPosCtrl.SetValue(str(z));

        self.interEdit.LoadFromSpin();

        
