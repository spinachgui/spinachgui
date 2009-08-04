import wx
import spinsys
from wx import xrc 
from wx import glcanvas 
from math import *
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
        self.camZ=15.0
        self.mousex=0;
        self.mousey=0;
        self.xTranslate=0;
        self.yTranslate=0;

    def setSpinSys(self,ss):
        """Set the spin system that this gl display is displaying"""
        self.ss=ss

    def setDrawMode(self,mode):
        if mode=="wireframe":
            glDisable(GL_DEPTH_TEST)  
            glDisable(GL_LIGHTING);   
            glDisable(GL_LIGHT0);     
            glDisable(GL_LIGHT1);     
        else:
            glEnable(GL_DEPTH_TEST)  
            glEnable(GL_LIGHTING);   
            glEnable(GL_LIGHT0);     
            glEnable(GL_LIGHT1);     
        self.mode=mode

    def enableGL(self):
        """This has to be called after the frame has been shown"""
        self.SetCurrent()
	glClearColor(0.0, 0.0, 0.0, 0.0);
        self.setDrawMode('self')

        self.sphereWire = glGenLists(1);
       
        qobj = gluNewQuadric();
        gluQuadricDrawStyle(qobj,GLU_LINE);
        gluQuadricNormals(qobj,GLU_SMOOTH);

        glNewList(self.sphereWire,GL_COMPILE);
        gluSphere(qobj,1.0,14,14);
        glEndList();
        gluDeleteQuadric(qobj);

        self.sphereSolid = glGenLists(1);

        qobj = gluNewQuadric();
        gluQuadricDrawStyle(qobj,GLU_FILL);
        gluQuadricNormals(qobj,GLU_SMOOTH);

        glNewList(self.sphereSolid,GL_COMPILE);
        gluSphere(qobj,1.0,32,32);
        glEndList();
        gluDeleteQuadric(qobj);



	glDepthFunc(GL_LEQUAL);

	#glShadeModel(GL_SMOOTH);
        #Adpated from a detailed tutorail on opengl lighting located at
        #http://www.falloutsoftware.com/tutorials/gl/gl8.htm


	# We're setting up two light sources. One of them is located
	# on the left side of the model (x = -1.5f) and emits white light. The
	# second light source is located on the right side of the model (x = 1.5f)
	# emitting red light.

	# GL_LIGHT0: the white light emitting light source
	# Create light components for GL_LIGHT0
	ambientLight0 =  array([0.3, 0.3, 0.3, 1.0],float32);
	diffuseLight0 =  array([0.5, 0.5, 0.5, 1.0],float32);
	specularLight0 = array([0.6, 0.6, 0.6, 1.0],float32);
	position0 =      array([-1.5, 1.0,-4.0, 1.0],float32);	
	# Assign created components to GL_LIGHT0
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight0);
	glLightfv(GL_LIGHT0, GL_POSITION, position0);

	# GL_LIGHT1: the red light emitting light source
	# Create light components for GL_LIGHT1
	ambientLight1 =  array([0.1, 0.1, 0.2, 1.0 ],float32);
	diffuseLight1 =  array([0.1, 0.1, 0.2, 1.0 ],float32);
	specularLight1 = array([0.1, 0.1, 0.2, 1.0 ],float32);
	position1 =      array([1.5, 1.0, 4.0,1.0],float32);	
	# Assign created components to GL_LIGHT1
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight1);
	glLightfv(GL_LIGHT1, GL_POSITION, position1);

        #glEnable(GL_COLOR_MATERIAL);
        #glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

        self.Bind(wx.EVT_PAINT,self.onPaint)
        self.Bind(wx.EVT_MOTION,self.onMouseMove)
        self.Bind(wx.EVT_MOUSEWHEEL,self.onWheel)

    def onWheel(self,e):
        self.zoom=self.zoom-0.001*e.GetWheelRotation()/e.GetWheelDelta();
        if(self.zoom<0.001):
            self.zoom=0.001;
        self.Refresh()


    def onPaint(self,e):
	glColor3f(0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT)
        glClear(GL_DEPTH_BUFFER_BIT)
	glClearDepth(1.0);

        width,height = self.GetClientSizeTuple()
        glViewport(0,0,width,height);

        glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
        glOrtho(-width*self.zoom, width*self.zoom, -height*self.zoom, height*self.zoom, -40.0, 40.0);
        #gluPerspective(45.0,float(width)/float(height),0.1, 200.0);
        glMatrixMode(GL_MODELVIEW);


        #Take the opertunity to calculate the rotation matrix for the scene
        #TODO: This would be better handled on the CPU, it's only one
        #      matrix. Change when matrix classes have been written
        dotProduct=(self.xRotate*self.xRotate+self.yRotate*self.yRotate);
        norm=sqrt(dotProduct);

        glLoadIdentity();
        glRotatef(dotProduct,self.yRotate/norm,self.xRotate/norm,0);
        glTranslatef(self.xTranslate*self.zoom,self.yTranslate*self.zoom,0);
        self.xRotate=0
        self.yRotate=0
        self.xTranslate=0
        self.yTranslate=0
        glMultMatrixf(self.rotationMatrix);
        self.rotationMatrix=glGetFloatv(GL_MODELVIEW_MATRIX);

        glLoadIdentity();
        gluLookAt(self.camX,self.camY,self.camZ,0,0,-1,0,1,0);
        glMultMatrixf(self.rotationMatrix);

        if self.ss==None:
            self.SwapBuffers()
            return

        spinCount=self.ss.getSpinCount()

        qobj = gluNewQuadric();
        if self.mode=="wireframe":
            gluQuadricDrawStyle(qobj,GLU_LINE);
        else:
            gluQuadricDrawStyle(qobj,GLU_FILL);
        gluQuadricNormals(qobj,GLU_SMOOTH);

        #Define some openGL materials
        whiteSpecularMaterial = array([0.5, 0.5, 0.5],float32); 
        blueSpecularMaterial = array([0.06, 0.06, 0.4],float32); 
        redSpecularMaterial = array([0.9, 0.00, 0.00],float32); 

        viewport   = glGetIntegerv(GL_VIEWPORT);
        mvmatrix   = glGetDoublev(GL_MODELVIEW_MATRIX);
        projmatrix = glGetDoublev(GL_PROJECTION_MATRIX);
        worldFarX, worldFarY ,worldFarZ  = gluUnProject(self.mousex,height-self.mousey-1,1.0,mvmatrix,projmatrix,viewport);
        worldNearX,worldNearY,worldNearZ = gluUnProject(self.mousex,height-self.mousey-1,0.0,mvmatrix,projmatrix,viewport);

        #glPushMatrix();
        #glTranslatef(coords[0],coords[1],coords[2]);
        #glMultMatrixf(mat)
        #glScale(0.04,0.04,0.04)
        #glCallList(self.sphereWire);
        #glPopMatrix();


        #Move these to some sort of config file
        radius=0.3
        radius2=radius*radius

        for i in range(spinCount):
            thisSpin=self.ss.getSpinByIndex(i)
            coords=thisSpin.getCoords()
            glColor3f(1.0, 1.0, 1.0);
            #Draw in the single spin interaction tensor
            mat3=self.ss.GetTotalInteractionOnSpinAsMatrix(i)
            #Convert to a openGL 4x4 matrix
            mat=array([[abs(mat3.get(0,0)),abs(mat3.get(0,1)),abs(mat3.get(0,2)),0],
                       [abs(mat3.get(1,0)),abs(mat3.get(1,1)),abs(mat3.get(1,2)),0],
                       [abs(mat3.get(2,0)),abs(mat3.get(2,1)),abs(mat3.get(2,2)),0],
                       [0,0,0,1]],float32)
            #Apply the transformation matrix to warp the sphere
            #print mat
            glPushMatrix();
            glTranslatef(coords[0],coords[1],coords[2]);
            glMultMatrixf(mat)
            glScale(0.04,0.04,0.04)
            glCallList(self.sphereWire);
            glPopMatrix();
            
            #Cast a ray from the eye to worldx,worldy,worldz and see if it collides with anything.
            #There four equations which combine into quadratic equation. If the descrimiate indicates a real
            #solution then the ray hits the sphere.


            Rx=worldNearX-worldFarX
            Ry=worldNearY-worldFarY
            Rz=worldNearZ-worldFarZ

            A =    Rx**2+                         Ry**2                    + Rz**2
            B = 2*(Rx*(worldNearX-coords[0]) +    Ry*(worldNearY-coords[1]) + Rz*(worldNearZ-coords[2]))
            C =    (worldNearX-coords[0])**2 +    (worldNearY-coords[1])**2 + (worldNearZ-coords[2])**2 - radius2



            desc=B**2-4*A*C
            if(desc>0):
                glMaterialfv(GL_FRONT, GL_SPECULAR, redSpecularMaterial);
                #glMaterialfv(GL_FRONT, GL_DIFFUSE, redSpecularMaterial);
            else:
                glMaterialfv(GL_FRONT, GL_SPECULAR, whiteSpecularMaterial);
                #glMaterialfv(GL_FRONT, GL_DIFFUSE,  whiteSpecularMaterial);
            glPushMatrix();
            glTranslatef(coords[0],coords[1],coords[2]);
            glScale(radius,radius,radius);
            if(self.mode=="wireframe"):
                glCallList(self.sphereWire);
            else:
                glCallList(self.sphereSolid);
            glPopMatrix();
        
            #Now draw in bonds to nearby atoms

            glMaterialfv(GL_FRONT, GL_SPECULAR, blueSpecularMaterial);
            nearby=self.ss.getNearbySpins(i,1.8)
            glColor3f(1.0, 0.0, 0.0);
            for index in nearby:
                otherCoords=self.ss.getSpinByIndex(index).getCoords()
                bondLength=((coords[0]-otherCoords[0])*(coords[0]-otherCoords[0])+
                            (coords[1]-otherCoords[1])*(coords[1]-otherCoords[1])+
                            (coords[2]-otherCoords[2])*(coords[2]-otherCoords[2]))**0.5

                #Now we need to find the rotation between the z axis
                angle=acos((otherCoords[2]-coords[2])/bondLength)
                glPushMatrix();
                glTranslatef(coords[0],coords[1],coords[2]);
                glRotate(angle/2/pi*360,coords[1]-otherCoords[1],otherCoords[0]-coords[0],0)
                gluCylinder(qobj,0.1,0.1,bondLength,10,10)
                glPopMatrix();

        glColor3f(0.0, 0.0, 1.0);


        self.SwapBuffers()

    def onMouseMove(self,e):
        if(e.Dragging() and e.RightIsDown()):
            self.xTranslate=self.xTranslate+(e.GetX()-self.mousex)
            self.yTranslate=self.yTranslate-(e.GetY()-self.mousey)
        elif(e.Dragging() and e.LeftIsDown()):
            self.xRotate=self.xRotate+(e.GetX()-self.mousex);
            self.yRotate=self.yRotate+(e.GetY()-self.mousey);

        self.mousex=e.GetX();
        self.mousey=e.GetY();
        self.Refresh()



class MyApp(wx.App):

    def OnInit(self):
        self.res = xrc.XmlResource('res/gui.xrc')
        self.ssroot=spinsys.SpinsysXMLRoot()
        self.ss=self.ssroot.getRoot()
        self.filename=""
        self.filepath=""
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

        self.frame.Bind(wx.EVT_MENU, self.onWireframe, id=xrc.XRCID('mMenuItemWireframe'))
        self.frame.Bind(wx.EVT_MENU, self.onFilled, id=xrc.XRCID('mMenuItemFilled'))

        self.updateSpinTree()

        self.frame.Show()

        self.glc.setSpinSys(self.ss)
        self.glc.enableGL()   
        
        #self.loadFromFile('data/hccch.xml')
        self.loadFromFile('data/tyrosine.log','g03')
        self.saveToFile('data/tyrosine.xml')


 
    def updateSpinTree(self):
        count=self.ss.getSpinCount()
        print "Count=",count
        for i in range(count):
            spin=self.ss.getSpinByIndex(i)
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
        wildcard="Spin XML files (*.xml)|*.xml|G03 Log Files (*.log)|*.log|Plain XYZ Files (*.xyz)|*.xyz|All Files (*.*)|*.*"
        fd=wx.FileDialog(self.frame,style=wx.FD_OPEN, message="Open a Spin System",wildcard=wildcard) 
        if(fd.ShowModal()):
            fileExt=fd.GetPath()[-3:]
            if(fileExt=="log"):
                self.loadFromFile(fd.GetPath().encode('latin-1'),'g03')
            if(fileExt=="xyz"):
                self.loadFromFile(fd.GetPath().encode('latin-1'),'xyz')
            else:
                self.loadFromFile(fd.GetPath().encode('latin-1'))
            
    def loadFromFile(self,filename,type="xml"):
        if type=="xml":
            self.ssroot.loadFromFile(filename)
        elif type=="g03":
            self.ssroot.loadFromG03File(filename)
        elif type=="xyz":
            self.ssroot.loadFromXYZFile(filename)
        self.ss=self.ssroot.getRoot();
        self.glc.setSpinSys(self.ss)
        self.updateSpinTree()

    def saveToFile(self,filename):
        #self.ssroot.setRoot(self.ss);
        self.ssroot.saveToFile(filename)   

    def onSaveAs(self,e):
        fd=wx.FileDialog(self.frame,style=wx.FD_SAVE, message="Save your Spin System",wildcard="Spin XML files (*.xml)|*.xml|All Files (*.*)|*.*") 
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

if __name__ == '__main__':
    app = MyApp(False)
    app.MainLoop()
        
