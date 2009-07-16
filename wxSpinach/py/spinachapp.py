import wx
import spinsys
from wx import xrc


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

    def updateSpinTree(self):
        spins=self.ss.getSpins()
        for spin in spin:
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
        
