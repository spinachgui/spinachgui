import wx
import spinsys
from wx import xrc


class MyApp(wx.App):

    def OnInit(self):
        self.res = xrc.XmlResource('res/gui.xrc')
        self.ss=spinsys.Spinsys()
        self.init_frame()
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
        self.root=self.spinTree.AddRoot("Spin System")
        
        #Setup event handling
        self.frame.Bind(wx.EVT_MENU, self.onSpinButton, id=xrc.XRCID('mSpinTool'))
        self.frame.Bind(wx.EVT_MENU, self.onJButton, id=xrc.XRCID('mJTool'))
        self.frame.Bind(wx.EVT_MENU, self.onClusterButton, id=xrc.XRCID('mClusterTool'))

        self.frame.Bind(wx.EVT_MENU, self.onLoadFromFile, id=xrc.XRCID('mMenuItemOpen'))
        self.frame.Bind(wx.EVT_MENU, self.onSaveToFile, id=xrc.XRCID('mMenuItemSaveAs'))

        self.frame.Show()

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
        
    def onNewSpinTool(self,e):
        spinElement=self.spinTree.AppendItem(self.root, "Spin 4")
        
        self.spinTree.AppendItem(spinElement,"x",wnd=wx.TextCtrl(self.spinTree, -1,"0.0"))
        self.spinTree.AppendItem(spinElement,"y",wnd=wx.TextCtrl(self.spinTree, -1,"0.0"))
        self.spinTree.AppendItem(spinElement,"z",wnd=wx.TextCtrl(self.spinTree, -1,"0.0"))

    def onLoadFromFile(self,e):
        print "hello"
        self.ss.loadFromFile("install/data/spinsys.xml")

    def onSaveToFile(self,e):
        print "world"
        self.ss.saveToFile("install/data/spinsys2.xml")

if __name__ == '__main__':
    app = MyApp(False)
    app.MainLoop()
        
