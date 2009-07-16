import wx
import spinsys
from wx import xrc

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



class MyApp(wx.App):

    def OnInit(self):
        self.res = xrc.XmlResource('res/gui.xrc')
        self.ss=spinsys.Spinsys()
        self.init_frame()
        self.filename=""
        self.filepath=""
        return True

    def init_frame(self):
        self.frame = self.res.LoadFrame(None, 'rootFrameBase')
        self.frame.SetSizeWH(800,600)

        self.toolbar = xrc.XRCCTRL(self.frame,'mRootToolbar')

        #Hide the uneeded panels
        self.spinPanel = xrc.XRCCTRL(self.frame, 'mSpinPanel')
        self.JCoupPanel = xrc.XRCCTRL(self.frame, 'mJCoupPanel')
        self.JCoupPanel.Show(False)
        self.ClusterPanel = xrc.XRCCTRL(self.frame, 'mClusterPanel')
        self.ClusterPanel.Show(False)
        
        #Populate the spin tree
        self.spinTree = xrc.XRCCTRL(self.frame,'mSpinTree')
        self.spinTree.AddRoot("Spin System")
        
        #Setup event handling
        self.frame.Bind(wx.EVT_MENU, self.onSpinButton, id=xrc.XRCID('mSpinTool'))
        self.frame.Bind(wx.EVT_MENU, self.onJButton, id=xrc.XRCID('mJTool'))
        self.frame.Bind(wx.EVT_MENU, self.onClusterButton, id=xrc.XRCID('mClusterTool'))

        self.frame.Bind(wx.EVT_MENU, self.onOpen, id=xrc.XRCID('mMenuItemOpen'))
        self.frame.Bind(wx.EVT_MENU, self.onSave, id=xrc.XRCID('mMenuItemSave'))
        self.frame.Bind(wx.EVT_MENU, self.onSaveAs, id=xrc.XRCID('mMenuItemSaveAs'))

        self.frame.Bind(wx.EVT_MENU, self.onExit, id=xrc.XRCID('mMenuItemExit'))

        self.updateSpinTree()

        self.frame.Show()
   
        print getARotation(self.frame)


    def updateSpinTree(self):
        spins=self.ss.getSpins()
        for spin in spins:
            self.spinTree.AppendItem(self.spinTree.GetRootItem(),spin[2])

    def onSpinButton(self,e):
        self.spinPanel.Show(True)
        self.JCoupPanel.Show(False)
        self.ClusterPanel.Show(False)
        self.frame.Layout();

    def onJButton(self,e):
        self.spinPanel.Show(False)
        self.JCoupPanel.Show(True)
        self.ClusterPanel.Show(False)
        self.frame.Layout();

    def onClusterButton(self,e):
        self.spinPanel.Show(False)
        self.JCoupPanel.Show(False)
        self.ClusterPanel.Show(True)
        self.frame.Layout();

    def onOpen(self,e):
        fd=wx.FileDialog(self.frame,style=wx.FD_OPEN, message="Open a Spin System",wildcard="Spin XML files (*.xml)|*.xml|All Files (*.*)|*.*") 
        if(fd.ShowModal()):
            self.ss.loadFromFile(fd.GetPath().encode('latin-1'))
            self.updateSpinTree()

    def onSaveAs(self,e):
        fd=wx.FileDialog(self.frame,style=wx.FD_SAVE, message="Save your Spin System",wildcard="Spin XML files (*.xml)|*.xml|All Files (*.*)|*.*") 
        if(fd.ShowModal()):
            self.ss.saveToFile(fd.GetPath().encode('latin-1'))

    def onSave(self,e):
        print "Impliment me"

    def onExit(self,e):
        exit(0)

if __name__ == '__main__':
    app = MyApp(False)
    app.MainLoop()
        
