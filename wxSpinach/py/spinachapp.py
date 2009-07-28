import wx
import spinsys
from wx import xrc

import wx.glcanvas

from OpenGL.GLUT import *
from OpenGL.GLU import *
from OpenGL.GL import *

from numpy import *

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


class glDisplay(wx.glcanvas.GLCanvas):
    def __init__(self,parent,id=-1):
        super(glDisplay,self).__init__(parent,id)
        self.ss=None
        self.xRotate=0
        self.yRotate=0
        self.rotationMatrix=array([[1,0,0,0],[0,1,0,0],[0,0,1,0],[0, 0, 0, 1]], float64)
        self.zoom=0.01
        self.camX=0.0
        self.camY=0.0
        self.camZ=5.0

    def setSpinSys(self,ss):
        """Set the spin system that this gl display is displaying"""
        self.ss=ss


    def enableGL(self):
        """This has to be called after the frame has been shown"""
        print "enableGL()"
        self.SetCurrent()
	glClearColor(0.0, 0.0, 0.0, 0.0);


        qobj = gluNewQuadric();
        gluQuadricDrawStyle(qobj,GLU_LINE);
        gluQuadricNormals(qobj,GLU_SMOOTH);

        self.list = glGenLists(1);

        glNewList(self.list,GL_COMPILE);
        gluSphere(qobj,0.8,20,20);
        glEndList();
        gluDeleteQuadric(qobj);

        self.Bind(wx.EVT_PAINT,self.onPaint)
        self.Bind(wx.EVT_MOTION,self.onMouseMove)

    def onPaint(self,e):
	glColor3f(0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT)

        glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
        width,height = self.GetClientSizeTuple()
        glOrtho(-width*self.zoom, width*self.zoom, -height*self.zoom, height*self.zoom, -10.0, 10.0);
        glMatrixMode(GL_MODELVIEW);


        #Take the opertunity to calculate the rotation matrix for the scene
        #TODO: This would be better handled on the CPU, it's only one
        #      matrix. Change when matrix classes have been written
        dotProduct=(self.xRotate*self.xRotate+self.yRotate*self.yRotate);
        norm=sqrt(dotProduct);

        glLoadIdentity();
        glRotatef(dotProduct,self.yRotate/norm,self.xRotate/norm,0);
        self.xRotate=0
        self.yRotate=0
        glMultMatrixf(self.rotationMatrix);
        self.rotationMatrix=glGetFloatv(GL_MODELVIEW_MATRIX);

        glLoadIdentity();
        gluLookAt(self.camX,self.camY,self.camZ,0,0,0,0,1,0);
        glMultMatrixf(self.rotationMatrix);

        #glLoadMatrixf(rotationMatrix);

        if self.ss==None:
            self.SwapBuffers()
            return

        ints=[]
        for i in range(self.ss.getInteractionCount()):
            ints.append(self.ss.getInteraction(i))
            
        spinCount=self.ss.getSpinCount()
        for i in range(spinCount):
            thisSpin=self.ss.getSpin(i)
            coords=thisSpin.getCoords()
            glColor3f(1.0, 1.0, 1.0);
            glPushMatrix();
            glTranslatef(coords[0],coords[1],coords[2]);
            #see if any interactions involve this spin
            for inter in ints:
                if inter.getSpin1Number()==thisSpin.getNumber() and inter.getFormAsString() == "matrix":
                    #Apply the transformation matrix to warp the sphere
                    mat=array([[inter.get(0,0),inter.get(0,1),inter.get(0,2),0],[inter.get(1,0),inter.get(1,1),inter.get(1,2),0],[inter.get(2,0),inter.get(2,1),inter.get(2,2),0],[0,0,0,1]],float32)
                    glMultMatrixf(mat)
            glCallList(self.list);
            glPopMatrix();
        
            glColor3f(0.0, 0.0, 1.0);
        self.SwapBuffers()

    def onMouseMove(self,e):
        if(e.Dragging()):
            self.xRotate=self.xRotate+(e.GetX()-self.mousex);
            self.yRotate=self.yRotate+(e.GetY()-self.mousey);

        self.mousex=e.GetX();
        self.mousey=e.GetY();
        self.Refresh()



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

        #Set up a openGL canvas
        self.glc = glDisplay(self.frame,-1)
        self.frame.GetSizer().Add(self.glc, 1, wx.EXPAND | wx.ALL)
        self.dc=wx.PaintDC(self.glc);

        
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

        self.glc.setSpinSys(self.ss)
        self.glc.enableGL()   
        
        self.loadFromFile('data/hccch.xml')





    def updateSpinTree(self):
        count=self.ss.getSpinCount()
        for i in range(count):
            spin=self.ss.getSpin(i)
            string=spin.getLabel() + " (" + spin.getIsotope()  + ")"
            self.spinTree.AppendItem(self.spinTree.GetRootItem(),string)

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
            loadFromFile(fd.GetPath().encode('latin-1'))
            
    def loadFromFile(self,filename):
        self.ss.loadFromFile(filename)
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
        
