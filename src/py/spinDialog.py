#3rd Party/standard Imports
import wx
from wx import xrc 
import wx.grid

import spinsys

#C++ Imports
from nuclear_data import *

#Local Imports
from interactionEdit import SpinInteractionsEdit


class SpinDialog(wx.Dialog):
    def __init__(self,parent,spin):
        self.spin=spin;
        pre = wx.PreDialog();
        wx.GetApp().res.LoadOnDialog(pre,parent,"SpinDialog")
        self.PostCreate(pre)

        self.SetMinSize((600,600));

        self.spinDialogPanel=xrc.XRCCTRL(self,'spinDialogPanel');
        self.interSizer=self.spinDialogPanel.GetSizer().GetItem(2).GetSizer();
        self.interEdit=SpinInteractionsEdit(self.spinDialogPanel,spin=spin);
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

        self.okayCancel=xrc.XRCCTRL(self,"okayCancel");

        self.Bind(wx.EVT_BUTTON,self.onApply,id=wx.ID_APPLY);


        #self.Fit()
        self.LoadFromSpin();

    def onApply(self,e):
        self.SaveToSpin();


    def ShowModal(self):
        result=wx.Dialog.ShowModal(self);
        if(result==wx.ID_OK):
            print "Okay"
            self.SaveToSpin();
        elif(result==wx.ID_CANCEL):
            print "Cancled."

    def SaveToSpin(self):
        x=float(self.xPosCtrl.GetValue());
        y=float(self.yPosCtrl.GetValue());
        z=float(self.zPosCtrl.GetValue());
        self.spin.SetCoordinates(x,y,z);
        print self.spin.GetPosition();

        label=self.spinTitle.GetValue();
        self.spin.SetLabel(label.encode('latin-1'))

        self.interEdit.SaveToSpin();
        


    def LoadFromSpin(self):
        x,y,z=self.spin.GetCoordinates()
        label=self.spin.GetLabel()

        self.spinTitle.SetValue(str(label));

        self.xPosCtrl.SetValue(str(x));
        self.yPosCtrl.SetValue(str(y));
        self.zPosCtrl.SetValue(str(z));

        self.interEdit.LoadFromSpin();

        
