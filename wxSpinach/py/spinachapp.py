#3rd Party/standard Imports
import wx
import wx.aui
from wx import xrc 
import wx.grid

import spinsys

#C++ Imports
from nuclear_data import *

#Local python imports
from glDisplay import glDisplay
from spinGrid import SpinGrid
from spinDialog import SpinDialog



def getARotation(parent):
    class rotDialog():
        def __init__(self):
            self.orientRes = xrc.XmlResource("res/gui.xrc")
            self.dlg = self.orientRes.LoadDialog(parent, "rotationDialog")


            self.eulerAPanel = xrc.XRCCTRL(self.dlg,'eulerAPanel')
            self.eulerBPanel = xrc.XRCCTRL(self.dlg,'eulerBPanel')
            self.angleAxisPanel = xrc.XRCCTRL(self.dlg,'angleAxisPanel')

            self.radioBox = xrc.XRCCTRL(self.dlg,'mRConRadioBox')

            self.eulerBPanel.Enable(False)
            self.angleAxisPanel.Enable(False)

            self.dlg.Bind(wx.EVT_RADIOBOX,self.onRadioBox,id=xrc.XRCID('mRConRadioBox'))

            
        def onRadioBox(self,e):
            self.eulerAPanel.Enable(False)
            self.eulerBPanel.Enable(False)
            self.angleAxisPanel.Enable(False)
            if e.Selection==0:
                self.eulerAPanel.Enable(True)
            elif e.Selection==1:
                self.eulerBPanel.Enable(True)
            elif e.Selection==2:
                self.angleAxisPanel.Enable(True)
            else:
                raise "A highly usual selection number was returned by a radioBox"
        def getRotation(self):
            convention=thisDialog.radioBox.GetSelection()
            if convention==0:
                v1=xrc.XRCCTRL(self.dlg,'alphaA')
                v2=xrc.XRCCTRL(self.dlg,'alphaA')
                v3=xrc.XRCCTRL(self.dlg,'gammaA')
            elif convention==1:
                v1=xrc.XRCCTRL(self.dlg,'alphaB')
                v2=xrc.XRCCTRL(self.dlg,'alphaB')
                v3=xrc.XRCCTRL(self.dlg,'gammaB')
            elif convention==2:
                v1=xrc.XRCCTRL(self.dlg,'omega')
                v2=xrc.XRCCTRL(self.dlg,'theta')
                v3=xrc.XRCCTRL(self.dlg,'phi')
            else:
                raise "A highly usual selection number was returned by a radioBox"
            return (convention,(float(v1.GetValue()),float(v2.GetValue()),float(v3.GetValue())))

    thisDialog=rotDialog()
    if(thisDialog.dlg.ShowModal() == wx.ID_OK):
        print "Dialog Okay"
        return thisDialog.getRotation()
    else:
        print "Dialog canceled"
        return None



class RootFrame(wx.Frame):
    def __init__(self):

        pre = wx.PreFrame();
        wx.GetApp().res.LoadOnFrame(pre,None,"rootFrameBase");
        self.PostCreate(pre);
        self.SetSize(wx.Size(1024,768));
        self.init_frame();

    def init_frame(self):
        self.toolbar = xrc.XRCCTRL(self,'mRootToolbar')

        #Hide the uneeded panels
        self.spinPanel = xrc.XRCCTRL(self, 'mSpinPanel')
        self.JCoupPanel = xrc.XRCCTRL(self, 'mJCoupPanel')
        self.JCoupPanel.Show(False)
        self.ClusterPanel = xrc.XRCCTRL(self, 'mClusterPanel')
        self.ClusterPanel.Show(False)
        
        #Populate the spin tree
        self.spinTree = xrc.XRCCTRL(self,'mSpinTree')
        self.spinTree.AddRoot("Spin System")
        self.updateSpinTree()
        
        #Setup event handling
        self.Bind(wx.EVT_MENU, self.onSpinButton, id=xrc.XRCID('mSpinTool'))
        self.Bind(wx.EVT_MENU, self.onJButton, id=xrc.XRCID('mJTool'))
        self.Bind(wx.EVT_MENU, self.onClusterButton, id=xrc.XRCID('mClusterTool'))

        self.Bind(wx.EVT_MENU, self.onOpen, id=xrc.XRCID('mMenuItemOpen'))
        self.Bind(wx.EVT_MENU, self.onSave, id=xrc.XRCID('mMenuItemSave'))
        self.Bind(wx.EVT_MENU, self.onSaveAs, id=xrc.XRCID('mMenuItemSaveAs'))

        self.Bind(wx.EVT_MENU, self.onExit, id=xrc.XRCID('mMenuItemExit'))

        self.Bind(wx.EVT_MENU, self.onWireframe, id=xrc.XRCID('mMenuItemWireframe'))
        self.Bind(wx.EVT_MENU, self.onFilled, id=xrc.XRCID('mMenuItemFilled'))

        #Setup the aui elements (this cannot be done from wxFormBuilder currently)

        self.auiPanel=xrc.XRCCTRL(self,'auiPanel');
        self.notebook=wx.aui.AuiNotebook(self.auiPanel,-1);

        self.spinGrid=SpinGrid(self.notebook)

        self.glc = glDisplay(self.notebook);
        self.dc=wx.PaintDC(self.glc);


        # add the panes to the manager
        self.notebook.AddPage(self.glc, '3D View')
        self.notebook.AddPage(self.spinGrid, 'Grid View')

        self.auiPanel.GetSizer().Add(self.notebook,1,wx.EXPAND);

        #Set up a openGL canvas
        

        #Set up the grid

        #self.loadFromFile('data/hccch.xml')
        self.loadFromFile('data/tyrosine.log','g03')
        #self.loadFromFile('../../../testing_kit/Gaussian/NMR spectroscopy/molecule_9.log','g03');
        #self.saveToFile('data/tyrosine.xml')

        self.testDia=SpinDialog(self,0);
        self.testDia.Show();

    def Show(self):
        wx.Frame.Show(self);
        self.glc.enableGL()   

 
    def updateSpinTree(self):
        count=wx.GetApp().ss.GetSpinCount()
        print "Count=",count
        for i in range(count):
            spin=wx.GetApp().ss.GetSpin(i)
            string=spin.GetLabel() + " (" + "<ISOTOPE>"  + ")"
            self.spinTree.AppendItem(self.spinTree.GetRootItem(),string)

    def onSpinButton(self,e):
        self.spinPanel.Show(True)
        self.JCoupPanel.Show(False)
        self.ClusterPanel.Show(False)
        self.Layout();

    def onJButton(self,e):
        self.spinPanel.Show(False)
        self.JCoupPanel.Show(True)
        self.ClusterPanel.Show(False)
        self.Layout();

    def onClusterButton(self,e):
        self.spinPanel.Show(False)
        self.JCoupPanel.Show(False)
        self.ClusterPanel.Show(True)
        self.Layout();

    def onOpen(self,e):
        wildcard="Spin XML files (*.xml)|*.xml|G03 Log Files (*.log)|*.log|Plain XYZ Files (*.xyz)|*.xyz|All Files (*.*)|*.*"
        fd=wx.FileDialog(self,style=wx.FD_OPEN, message="Open a Spin System",wildcard=wildcard) 
        if(fd.ShowModal()):
            fileExt=fd.GetPath()[-3:]
            print fileExt
            if(fileExt=="log"):
                print "Gaussian"
                self.loadFromFile(fd.GetPath().encode('latin-1'),'g03')
            if(fileExt=="xyz"):
                self.loadFromFile(fd.GetPath().encode('latin-1'),'xyz')
            else:
                self.loadFromFile(fd.GetPath().encode('latin-1'))
            
    def loadFromFile(self,filename,type="xml"):
        if type=="xml":
            wx.GetApp().ss.LoadFromFile(filename)
        elif type=="g03":
            wx.GetApp().ss.LoadFromG03File(filename)
        elif type=="xyz":
            wx.GetApp().ss.LoadFromXYZFile(filename)
        self.updateSpinTree()
        self.spinGrid.refreshFromSpinSystem()

    def saveToFile(self,filename):
        #self.ssroot.setRoot(self.ss);
        self.ssroot.saveToFile(filename)   

    def onSaveAs(self,e):
        fd=wx.FileDialog(self,style=wx.FD_SAVE, message="Save your Spin System",wildcard="Spin XML files (*.xml)|*.xml|All Files (*.*)|*.*") 
        if(fd.ShowModal()):
            saveToFile(fd.GetPath().encode('latin-1'))

    def onSave(self,e):
        print "Impliment me"

    def onFilled(self,e):
        self.glc.setDrawMode('filled')

    def onWireframe(self,e):
        self.glc.setDrawMode('wireframe')


    def onExit(self,e):
        exit(0)

        
        





class MyApp(wx.App):

    def OnInit(self):
        self.res = xrc.XmlResource('res/gui.xrc')
        self.ss=spinsys.SpinSystem()

        self.filename=""
        self.filepath=""

        self.rootFrame=RootFrame();
        self.rootFrame.Show()

        return True


if __name__ == '__main__':
    app = MyApp(False)
    app.MainLoop()
        
