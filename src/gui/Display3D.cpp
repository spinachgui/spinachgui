
#include <gui/Display3D.hpp>
#include <gui/SpinachApp.hpp>
#include <gui/RightClickMenu.hpp>
#include <wx/dcclient.h>

//============================================================//
// Scene graphs

GLfloat defaultMaterial[3] = {0.5, 0.5,  0.5}; 

SGNode::SGNode()
  : mDirty(true),
    mUseMaterial(false),
    mMaterial(defaultMaterial),
    mList(glGenLists(1)),
    mGeomOnlyList(glGenLists(1)),
    mIdentity(true) {
}

SGNode::~SGNode() {
  glDeleteLists(mList,1);
  glDeleteLists(mGeomOnlyList,1);
  sigDying(this);
}

void SGNode::AddNode(SGNode* node) {
  mChildren.push_back(node);
  node->sigDying.connect(mem_fun(this,&SGNode::RemoveNode));
}

void SGNode::RemoveNode(SGNode* node) {
  for(itor i=mChildren.begin();i!=mChildren.end();++i) {
    if(*(i)==node) {
      mChildren.erase(i);
      break;
    }
  }
}

void SGNode::SetMaterial(const float material[3],bool use) {
  mMaterial=material;
  mUseMaterial=use;
}

void SGNode::SetTranslation(const Vector3& v) {
  mIdentity=false;
  mat[0 ]=1;  mat[4 ]=0;  mat[8 ]=0;  mat[12]=v.GetX();
  mat[1 ]=0;  mat[5 ]=1;  mat[9 ]=0;  mat[13]=v.GetY();
  mat[2 ]=0;  mat[6 ]=0;  mat[10]=1;  mat[14]=v.GetZ();
  mat[3 ]=0;  mat[7 ]=0;  mat[11]=0;  mat[15]=1;
}

void SGNode::Draw(const SpinachDC& dc) {
  if(mDirty) {
    //glNewList(mList,GL_COMPILE);
    if(!mIdentity) {
      glPushMatrix();
      glMultMatrixf(mat);
    }
    if(mUseMaterial) {
      glMaterialfv(GL_FRONT,GL_SPECULAR,mMaterial);
    }
    RawDraw(dc);
    for(itor i=mChildren.begin();i!=mChildren.end();++i) {
      (*i)->Draw(dc);
    }
    if(!mIdentity) {
      glPopMatrix();
    }
    //glEndList();
  } 
  //glCallList(mList);
}


//============================================================//
// Display3D class

Display3D::Display3D(wxWindow* parent) 
  : wxGLCanvas(parent,(wxGLContext*)NULL,wxID_ANY,wxDefaultPosition,wxDefaultSize,WX_GL_DOUBLEBUFFER),
    mRootNode(NULL),
    mDC() {
  mGLContext=NULL;
  mGLEnabled=false;

  mSS=wxGetApp().GetSpinSysManager()->Get();

  mZoom=0.01;
  mCamX=0.0;
  mCamY=0.0;
  mCamZ=15.0;

  mXTranslate=0;
  mYTranslate=0;
  mXRotate=0;
  mYRotate=0;

  mRotationMatrix[0 ]=1.0;
  mRotationMatrix[1 ]=0.0;
  mRotationMatrix[2 ]=0.0;
  mRotationMatrix[3 ]=0.0;
		    
  mRotationMatrix[4 ]=0.0;
  mRotationMatrix[5 ]=1.0;
  mRotationMatrix[6 ]=0.0;
  mRotationMatrix[7 ]=0.0;
		    
  mRotationMatrix[8 ]=0.0;
  mRotationMatrix[9 ]=0.0;
  mRotationMatrix[10]=1.0;
  mRotationMatrix[11]=0.0;
		    
  mRotationMatrix[12]=0.0;
  mRotationMatrix[13]=0.0;
  mRotationMatrix[14]=0.0;
  mRotationMatrix[15]=1.0;
}

Display3D::~Display3D() {
  if(mRootNode) {
    delete mRootNode;
  }
  gluDeleteQuadric(mQFilled);
  gluDeleteQuadric(mQWireframe);
}


void Display3D::EnableGL() {
  
  if(mGLContext == NULL) {
    mGLContext = new wxGLContext(this);
  }
  this->SetCurrent(*mGLContext);
	
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClearDepth(1.0);
	
  //glShadeModel(GL_SMOOTH);
  //Adpated from a detailed tutorail on opengl lighting located at
  //http://www.falloutsoftware.com/tutorials/gl/gl8.htm
	
	
  // We're setting up two light sources. One of them is located
  // on the left side of the model (x = -1.5f) and emits white light. The
  // second light source is located on the right side of the model (x = 1.5f)
  // emitting red light.

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);     
  glEnable(GL_LIGHT1);

  // GL_LIGHT0: the white light emitting light source
  // Create light components for GL_LIGHT0
  GLfloat ambientLight0[] =  {0.0, 0.0, 0.0, 1.0};
  GLfloat diffuseLight0[] =  {0.5, 0.5, 0.5, 1.0};
  GLfloat specularLight0[] = {0.6, 0.6, 0.6, 1.0};
  GLfloat position0[] =      {-1.5, 1.0,-4.0, 1.0};	
  // Assign created components to GL_LIGHT0
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight0);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight0);
  glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight0);
  glLightfv(GL_LIGHT0, GL_POSITION, position0);
	
  // GL_LIGHT1: the red light emitting light source
  // Create light components for GL_LIGHT1
  GLfloat ambientLight1[] =  {0.1, 0.1, 0.1, 1.0};
  GLfloat diffuseLight1[] =  {0.1, 0.1, 0.1, 1.0};
  GLfloat specularLight1[] = {0.3, 0.3, 0.3, 1.0};
  GLfloat position1[] =      {1.5, 1.0, 4.0, 1.0};	
  // Assign created components to GL_LIGHT1
  glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight1);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight1);
  glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight1);
  glLightfv(GL_LIGHT1, GL_POSITION, position1);
	
  //Create a dictionary of colours
  glMatrixMode(GL_MODELVIEW);
}


void Display3D::ChangeViewport() {

  int width,height;
  GetClientSize(&width,&height);
	

  GetClientSize(&width,&height);
  glViewport(0,0,width,height);
	
  //From the documentation:
  //glDeleteTextures silently ignores 0's and names that do not correspond to existing textures.
  //so it's okay that self.tex will not exist at first
  glDeleteTextures(1,&mTexDepth);
  glGenTextures(1,&mTexDepth);
	
  glBindTexture(GL_TEXTURE_2D,mTexDepth);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	
  glTexImage2D(GL_PROXY_TEXTURE_2D,0,GL_RGBA,
	       height,width,
	       0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);

}


void Display3D::OnMouseMove(wxMouseEvent& e) {
  if(e.Dragging() && e.RightIsDown()) {
    mXTranslate=mXTranslate+(e.GetX()-mMouseX);
    mYTranslate=mYTranslate-(e.GetY()-mMouseY);
  }  else if(e.Dragging() && e.LeftIsDown()) {
    mXRotate=mXRotate+(e.GetX()-mMouseX);
    mYRotate=mYRotate+(e.GetY()-mMouseY);
  } else {

  }
  mMouseX=e.GetX();
  mMouseY=e.GetY();
  

  Refresh();
}


void Display3D::OnWheel(wxMouseEvent& e) {
  mZoom=mZoom-0.001*e.GetWheelRotation()/e.GetWheelDelta();
  if(mZoom<0.001) {
    mZoom=0.001;
  }
  ChangeViewport();
  Refresh();
}

void Display3D::OnRightClick(wxMouseEvent& e) {
  if(!e.Dragging()) {

    if(mHover>=0) {
      RightClickMenu* menu = new RightClickMenu(this);
      menu->OptionDeleteSpin(GetSS()->GetSpin(mHover));
      menu->OptionShowSpinProperties(GetSS()->GetSpin(mHover));
      menu->Build();
      PopupMenu(menu);
      delete menu;
    }
  }
}

void Display3D::OnLeftClick(wxMouseEvent& e) {
  if(!e.ShiftDown()) {
    //selected=[];
  }
  if (mHover != -1) {
    //selected.append(self.hover);
    Refresh();
  }

}

void Display3D::OnResize(wxSizeEvent& e) {
  if(mGLEnabled) {
    ChangeViewport();
  }
}


void Display3D::OnDeleteSpinHover(wxCommandEvent& e) {
  if(mHover>-1) {
    Chkpoint(wxT("Delete Spin"));
    GetSS()->RemoveSpin(mHover);
    Refresh();
  }
}

void Display3D::OnPaint(wxPaintEvent& e) {
  if(!mGLEnabled) {
    mGLEnabled=true;
    EnableGL();
  }

  wxPaintDC dc(this);

  int width,height;
  GetClientSize(&width,&height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-width*mZoom, width*mZoom,
	  -height*mZoom, height*mZoom,
	  -40.0, 40.0);

  glMatrixMode(GL_MODELVIEW);
  //Take the opertunity to calculate the rotation matrix for the scene
  //TODO: This would be better handled on the CPU, it's only one
  //matrix. Change when matrix classes have been written
  float dotProduct=(mXRotate*mXRotate+mYRotate*mYRotate);
  float norm=sqrt(dotProduct);
  glLoadIdentity();

  if(norm != 0){ //Prevent division by zero errors
    glRotatef(dotProduct,mYRotate/norm,mXRotate/norm,0);
  }
  glTranslatef(mXTranslate*mZoom,mYTranslate*mZoom,0);
  mXRotate=0;
  mYRotate=0;
  mXTranslate=0;
  mYTranslate=0;
  glMultMatrixf(mRotationMatrix);
  glGetFloatv(GL_MODELVIEW_MATRIX,mRotationMatrix);
  glLoadIdentity();
  gluLookAt(mCamX,mCamY,mCamZ,0,0,-1,0,1,0);
  glMultMatrixf(mRotationMatrix);

  glEnable(GL_DEPTH_TEST);	

  glClear(GL_DEPTH_BUFFER_BIT);
  glEnable(GL_LIGHTING);{
    glClear(GL_COLOR_BUFFER_BIT);
    if(mRootNode) {
      mRootNode->Draw(mDC);
    }
  } glDisable(GL_LIGHTING);   
	
  SwapBuffers();
}

BEGIN_EVENT_TABLE(Display3D,wxGLCanvas)

EVT_PAINT     (                       Display3D::OnPaint)
EVT_MOTION    (                       Display3D::OnMouseMove)
EVT_MOUSEWHEEL(                       Display3D::OnWheel)
EVT_RIGHT_UP  (                       Display3D::OnRightClick)
EVT_LEFT_UP   (                       Display3D::OnLeftClick)
EVT_SIZE      (                       Display3D::OnResize)


END_EVENT_TABLE()



