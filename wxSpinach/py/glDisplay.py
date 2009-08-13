#3rd Party/Standard Imports
import wx
import wx.aui
from wx import glcanvas 


from OpenGL.GLUT import *
from OpenGL.GLU import *
from OpenGL.GL import *

from numpy import *

import time
from math import *

#C++ Imports
from nuclear_data import *

#Local Imports
from utils import *

#Define some openGL materials
whiteMaterial = array([0.5, 0.5, 0.5],float32); 
blueMaterial = array([0.06, 0.06, 0.4],float32); 
redMaterial = array([0.9, 0.00, 0.00],float32); 

hoverMaterial=redMaterial
selectedMaterial=blueMaterial



class glDisplay(wx.glcanvas.GLCanvas):
    def __init__(self,parent,id=-1):
        super(glDisplay,self).__init__(parent,id)
        self.parent=parent
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
        self.selected=[]  #List containing all spins which are currently selected
        self.hover=-1   #The closest spin currently sitting under the mouse

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
        gluSphere(qobj,1.0,12,12);
        glEndList();
        gluDeleteQuadric(qobj);



	glDepthFunc(GL_LEQUAL);

        #Reserve Space for the display lists
        self.bondDispList=glGenLists(1);

	#glShadeModel(GL_SMOOTH);
        #Adpated from a detailed tutorail on opengl lighting located at
        #http://www.falloutsoftware.com/tutorials/gl/gl8.htm


	# We're setting up two light sources. One of them is located
	# on the left side of the model (x = -1.5f) and emits white light. The
	# second light source is located on the right side of the model (x = 1.5f)
	# emitting red light.

	# GL_LIGHT0: the white light emitting light source
	# Create light components for GL_LIGHT0
	ambientLight0 =  array([0.0, 0.0, 0.0, 1.0],float32);
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
	ambientLight1 =  array([0.1, 0.1, 0.1, 1.0 ],float32);
	diffuseLight1 =  array([0.1, 0.1, 0.1, 1.0 ],float32);
	specularLight1 = array([0.3, 0.3, 0.3, 1.0 ],float32);
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
        self.Bind(wx.EVT_RIGHT_UP,self.onRightClick)
        self.Bind(wx.EVT_LEFT_UP,self.onLeftClick)

        #Create a dictionary of colours
        self.colourDict={}
        for i in range(getElementCount()):
            self.colourDict[getElementSymbol(i)]=array([getElementR(i), getElementG(i), getElementB(i)],float32);

        self.genBondList()

    def onWheel(self,e):
        self.zoom=self.zoom-0.001*e.GetWheelRotation()/e.GetWheelDelta();
        if(self.zoom<0.001):
            self.zoom=0.001;
        self.Refresh()

    def genBondList(self):
        self.bondDispList
        glNewList(self.bondDispList,GL_COMPILE);

        qobj = gluNewQuadric();
        if self.mode=="wireframe":
            gluQuadricDrawStyle(qobj,GLU_LINE);
        else:
            gluQuadricDrawStyle(qobj,GLU_FILL);
        gluQuadricNormals(qobj,GLU_SMOOTH);

        for i in range(self.ss.getSpinCount()):   #Draw the spins and the bonds
            thisSpin=self.ss.getSpinByIndex(i)
            coords=thisSpin.getCoords()
        
            #draw in bonds to nearby atoms

            glMaterialfv(GL_FRONT, GL_SPECULAR, blueMaterial);
            glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, whiteMaterial);
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
                gluCylinder(qobj,0.1,0.1,bondLength,7,7)
                glPopMatrix();

        glEndList();


    def onPaint(self,e):
        t1=time.time()
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

        if norm != 0: #Prevent division by zero errors
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

        #Move these to some sort of config file
        radius=0.3
        radius2=radius*radius


        #Find out if any of the spins are under the mouse and record the closest
        self.hover=-1     #The index of the spin the user is hovering over
        self.hoverDist=0  #This distance to that spin

        viewport   = glGetIntegerv(GL_VIEWPORT);
        mvmatrix   = glGetDoublev(GL_MODELVIEW_MATRIX);
        projmatrix = glGetDoublev(GL_PROJECTION_MATRIX);
        worldFarX, worldFarY ,worldFarZ  = gluUnProject(self.mousex,height-self.mousey-1,1.0,mvmatrix,projmatrix,viewport);
        worldNearX,worldNearY,worldNearZ = gluUnProject(self.mousex,height-self.mousey-1,0.0,mvmatrix,projmatrix,viewport);
        tsetup=time.time()
        for i in range(spinCount):  #Decide which spin is selected
            thisSpin=self.ss.getSpinByIndex(i);
            coords=thisSpin.getCoords()

            #The distance from the near clipping plane is reused in the colision detection
            clipDist2=(worldNearX-coords[0])**2 +    (worldNearY-coords[1])**2 + (worldNearZ-coords[2])**2
            if clipDist2 > self.hoverDist and self.hover!=-1:
                continue; #We already found a closer spin, so there isn't any point in checking this one

            #Cast a ray from the eye to worldx,worldy,worldz and see if it collides with anything.
            #There four equations which combine into quadratic equation. If the descrimiate indicates a real
            #solution then the ray hits the sphere.
            Rx=worldNearX-worldFarX
            Ry=worldNearY-worldFarY
            Rz=worldNearZ-worldFarZ

            A =    Rx**2+                         Ry**2                    + Rz**2
            B = 2*(Rx*(worldNearX-coords[0]) +    Ry*(worldNearY-coords[1]) + Rz*(worldNearZ-coords[2]))
            C =    clipDist2 - radius2

            desc=B**2-4*A*C
            if desc > 0:
                self.hover=i
                self.hoverDist=clipDist2
            

        tspins=time.time()

        for i in range(spinCount):   #Draw the spins and the bonds
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

            glPushMatrix();
            glMultMatrixf(mat)
            glScale(0.04,0.04,0.04)
            glCallList(self.sphereWire);
            glPopMatrix();

            eValX=self.ss.getEigenValX(i).real;
            eValY=self.ss.getEigenValY(i).real;
            eValZ=self.ss.getEigenValZ(i).real;

            eVecX=self.ss.getEigenVecX(i);
            eVecY=self.ss.getEigenVecY(i);
            eVecZ=self.ss.getEigenVecZ(i);

            #Draw the three eigenvectors of the interactionx
            glBegin(GL_LINES);
            glVertex3f(0,0,0);
            glVertex3f(eVecX[0]*eValX,eVecX[1]*eValX,eVecX[2]*eValX);
            glEnd();

            glBegin(GL_LINES);
            glVertex3f(0,0,0);
            glVertex3f(eVecY[0]*eValY,eVecY[1]*eValY,eVecY[2]*eValY);
            glEnd();

            glBegin(GL_LINES);
            glVertex3f(0,0,0);
            glVertex3f(eVecZ[0]*eValZ,eVecZ[1]*eValZ,eVecZ[2]*eValZ);
            glEnd();

            glPopMatrix();


            if(self.hover==i):
                glMaterialfv(GL_FRONT, GL_SPECULAR, redMaterial);
                glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, whiteMaterial);
            elif(i in self.selected):
                glMaterialfv(GL_FRONT, GL_SPECULAR, selectedMaterial);
                glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, whiteMaterial);                
            else:
                letter,num=splitSymbol(thisSpin.getIsotope())
                if (letter in self.colourDict):
                     glMaterialfv(GL_FRONT, GL_SPECULAR, self.colourDict[letter]);
                     glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, self.colourDict[letter]);
            glPushMatrix();
            glTranslatef(coords[0],coords[1],coords[2]);
            glScale(radius,radius,radius);
            if(self.mode=="wireframe"):
                glCallList(self.sphereWire);
            else:
                glCallList(self.sphereSolid);
            glPopMatrix();
        
        glCallList(self.bondDispList);

        tbonds=time.time()

        glDisable(GL_LIGHTING);   
        glDisable(GL_LIGHT0);     
        glDisable(GL_LIGHT1);     
        glEnable(GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        for i in range(spinCount):  #Draw the J couplings
            #Do the two spin couplings
            thisSpin=self.ss.getSpinByIndex(i)
            coords=thisSpin.getCoords()            
            for j in range(i+1,spinCount):
                coordsJ=self.ss.getSpinByIndex(j).getCoords()
                scalar=abs(self.ss.GetTotalIsotropicInteractionOnSpinPair(i,j))/300;
                glColor4f(scalar,0,0,scalar);
                glBegin(GL_LINES);
                glVertex3f(coords[0],coords[1],coords[2]);
                glVertex3f(coordsJ[0],coordsJ[1],coordsJ[2]);
                glEnd();
        glDisable(GL_BLEND);
        glEnable(GL_LIGHTING);   
        glEnable(GL_LIGHT0);     
        glEnable(GL_LIGHT1);     
                
                

        glColor3f(0.0, 0.0, 1.0);

        tj=time.time()

        self.SwapBuffers()
        t2=time.time()
        print ("Render time=" +str(t2-t1) + "ms , approx fps=" +str(1/(t2-t1)) + 
               "s^-1 (setup=" + str(tsetup-t1) + ", spins=" + str(tspins-tsetup) + ", bonds="+
               str(tbonds-tspins)+", j="+str(tj-tbonds)+", tswap="+str(t2-tj)+")")
        
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

    def onLeftClick(self,e):
        if(not e.ShiftDown()):
            self.selected=[]
        if self.hover != -1:
            self.selected.append(self.hover);
        self.Refresh()


    def onRightClick(self,e):
        if(not e.Dragging()):
            menu = wx.Menu()
            menu.Append(-1, "Test Menu Item")
            if(self.hover>=0):
                menu.Append(-1, "Spin Properties...")
                menu.Bind(wx.EVT_MENU,self.parent.onDisplaySpinPropDialog);
            if(len(self.selected)==2):
                menu.Append(-1, "Coupling Properties")                    
   
            self.PopupMenuXY( menu, e.GetX(), e.GetY() )
            menu.Destroy()

