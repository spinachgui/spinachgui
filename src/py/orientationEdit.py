


#3rd Party/standard Imports
import wx
from wx import xrc 
import wx.grid
import wx.combo

import spinsys

#Local python imports
from utils import *


UNDEFINED=spinsys.Orientation.UNDEFINED;  
EULER=spinsys.Orientation.EULER;     
ANGLE_AXIS=spinsys.Orientation.ANGLE_AXIS;     
QUATERNION=spinsys.Orientation.QUATERNION;
EIGENSYSTEM=spinsys.Orientation.EIGENSYSTEM;     

OrientTypeOrders=[EULER,ANGLE_AXIS,QUATERNION,EIGENSYSTEM];



#Choice book page,Humand readable name,panel name,list of textctrl names

eigenEdits=['eigsysXX','eigsysXY','eigsysXZ',
            'eigsysYX','eigsysYY','eigsysYZ',
            'eigsysZX','eigsysZY','eigsysZZ'];


OrientTypes={
    UNDEFINED:  (-1,"Undefined",""     ,""                ,[]                                ),
    EULER:      (0,"Euler Angles"            ,"ScalarIntPanel"  ,['alphaA','betaA','gammaA']       ),               
    ANGLE_AXIS: (1,"Angle Axis","AxRhomIntPanel"  ,['angleAxisAngle','angleAxisX','angleAxisY','angleAxisZ']),     
    QUATERNION: (2,"Quaternion"       ,"EigenIntPanel"   ,['quatRe','quatI','quatJ','quatK']),
    EIGENSYSTEM:(3,"Eigensystem"            ,"MatrixIntPanel"  ,eigenEdits                        ),
}


class OrientEditPanel(wx.Panel):
    def __init__(self,parent,id=-1):
        self.orient=None;
        pre = wx.PrePanel();
        wx.GetApp().res.LoadOnPanel(pre,parent,'OrientEditPanel');
        self.PostCreate(pre)

        self.loading=False;

        self.typeChoiceBook=xrc.XRCCTRL(self,"OrientTypeChoiceBook");
        self.Bind(wx.EVT_CHOICEBOOK_PAGE_CHANGED,self.OnPageChange);

        self.alphaA=xrc.XRCCTRL(self,'alphaA');
        self.betaA=xrc.XRCCTRL(self,'betaA');
        self.gammaA=xrc.XRCCTRL(self,'gammaA');

        self.AngleAxisAngle=xrc.XRCCTRL(self,'AngleAxisAngle');
        self.AngleAxisX=xrc.XRCCTRL(self,'AngleAxisX');
        self.AngleAxisY=xrc.XRCCTRL(self,'AngleAxisY');
        self.AngleAxisZ=xrc.XRCCTRL(self,'AngleAxisZ');

        self.quatRe=xrc.XRCCTRL(self,'quatRe');
        self.quatI=xrc.XRCCTRL(self,'quatI');
        self.quatJ=xrc.XRCCTRL(self,'quatJ');
        self.quatK=xrc.XRCCTRL(self,'quatK');

        self.eigsysXX=xrc.XRCCTRL(self,'eigsysXX');
        self.eigsysXY=xrc.XRCCTRL(self,'eigsysXY');
        self.eigsysXZ=xrc.XRCCTRL(self,'eigsysXZ');
                                                
        self.eigsysYX=xrc.XRCCTRL(self,'eigsysYX');
        self.eigsysYY=xrc.XRCCTRL(self,'eigsysYY');
        self.eigsysYZ=xrc.XRCCTRL(self,'eigsysYZ');
                                                
        self.eigsysZX=xrc.XRCCTRL(self,'eigsysZX');
        self.eigsysZY=xrc.XRCCTRL(self,'eigsysZY');
        self.eigsysZZ=xrc.XRCCTRL(self,'eigsysZZ');

        self.editCtrls=[]
        print OrientTypes
        for key,OrientType in OrientTypes.items():
            for ctrlName in OrientType[3]:
                print ctrlName
                ctrl=xrc.XRCCTRL(self,ctrlName)
                ctrl.SetValidator(NumericValidator());
                self.editCtrls.append(ctrl);
                self.Bind(wx.EVT_TEXT,self.onTextChange)

    def SetOrient(self,orient):
        self.orient=orient;
        self.LoadFromOrient();

    def onQuadChecked(self,e):
        if e.IsChecked():
            self.inter.SetQuadratic();
        else:
            self.inter.SetLinear();
        self.GetParent().UpdateListBox();

    def OnPageChange(self,e):
        if self.Loading:
            print "Skipping"
            return;
        print "Changing the type of this interaction"
        type=TypeOrders[e.GetSelection()]

        self.AngleAxisAngle=xrc.XRCCTRL(self,'AngleAxisAngle');
        self.AngleAxisX=xrc.XRCCTRL(self,'AngleAxisX');
        self.AngleAxisY=xrc.XRCCTRL(self,'AngleAxisY');
        self.AngleAxisZ=xrc.XRCCTRL(self,'AngleAxisZ');

        self.quatRe=xrc.XRCCTRL(self,'quatRe');
        self.quatI=xrc.XRCCTRL(self,'quatI');
        self.quatJ=xrc.XRCCTRL(self,'quatJ');
        self.quatK=xrc.XRCCTRL(self,'quatK');

        if(type==EULER):

            self.alphaA.SetValue("0.0");
            self.betaA.SetValue("0.0");
            self.gammaA.SetValue("0.0");

        elif(type==ANGLE_AXIS):
            self.AngleAxisAngle.SetValue("0.0");
            self.AngleAxisX.SetValue("0.0");
            self.AngleAxisY.SetValue("0.0");
            self.AngleAxisZ.SetValue("0.0");
        elif(type==QUATERNION):
            self.quatRe.SetValue("0.0");
            self.quatI.SetValue("0.0");
            self.quatJ.SetValue("0.0");
            self.quatK.SetValue("0.0");
        elif(type==EIGENSYSTEM):
            self.eigsysXX.SetValue("0.0");
            self.eigsysXY.SetValue("0.0");
            self.eigsysXZ.SetValue("0.0");
                                   
            self.eigsysYX.SetValue("0.0");
            self.eigsysYY.SetValue("0.0");
            self.eigsysYZ.SetValue("0.0");
                                   
            self.eigsysZX.SetValue("0.0");
            self.eigsysZY.SetValue("0.0");
            self.eigsysZZ.SetValue("0.0");


        self.SaveToInter();        
        e.Skip()



    def LoadFromOrient(self):
        self.Loading=True
        print self.orient
        self.typeChoiceBook.SetSelection(OrientTypes[self.orient.GetType()][0]);
        self.Loading=False;

    def SaveToOrient(self):
        Type=TypeOrders[self.typeChoiceBook.GetSelection()];
        if(Type==EULER):
            alpha=lazyFloat(self.alphaA.GetValue());
            beta=lazyFloat(self.betaA.GetValue())
            gamma=lazyFloat(self.gammaA.GetValue())
            self.orient.SetScalar(alpha,beta,gamma);
        elif(Type==ANGLE_AXIS):
            angle=lazyFloat(self.angleAxisAngle.GetValue());
            x=lazyFloat(self.angleAxisX.GetValue());
            y=lazyFloat(self.angleAxisY.GetValue());
            z=lazyFloat(self.angleAxisZ.GetValue());
        elif(Type==QUATERNION):
            re=lazyFloat(self.quatRe.GetValue());
            i=lazyFloat(self.quatI.GetValue());
            j=lazyFloat(self.quatJ.GetValue());
            k=lazyFloat(self.quatK.GetValue());
            self.orient.SetQuaternion(re,i,j,k);
        elif(Type==EIGENSYSTEM):
            vx=Vector3(lazyFloat(self.eigsysXX.GetValue()),lazyFloat(self.eigsysXY.GetValue()),lazyFloat(self.eigsysXZ.GetValue()));
            vy=Vector3(lazyFloat(self.eigsysYX.GetValue()),lazyFloat(self.eigsysYY.GetValue()),lazyFloat(self.eigsysYZ.GetValue()));
            vz=Vector3(lazyFloat(self.eigsysZX.GetValue()),lazyFloat(self.eigsysZY.GetValue()),lazyFloat(self.eigsysZZ.GetValue()));
            self.orient.SetEigensystem(vx,vy,vz);

        

    def onTextChange(self,e):
        if self.Loading: #If we are loading from a spin we should ignore this event
            return
        self.SaveToOrient();




class OrientPopup(wx.combo.ComboPopup):
    def __init__(self):

        wx.combo.ComboPopup.__init__(self)

    def Create(self,parent):

        self.panel=wx.Panel(parent);
        self.sizer=wx.BoxSizer();
        self.sizer.Add(self.panel);
        self.panel.SetSizer(self.sizer);

    def Init(self):
        print "init()ing"

    def GetStringValue(self):
        return "The Quick brown fox jumps over the lazy dog";

    def SetStringValue(self,value):
        print "User says ",value;



    def GetControl(self):
        return self.panel;






class OrientTextEditor(wx.combo.ComboCtrl):
    def __init__(self,parent,id=-1):
        wx.combo.ComboCtrl.__init__(self,parent,id);

        self.popup=OrientPopup();
        self.SetPopupControl(self.popup);




        
        

