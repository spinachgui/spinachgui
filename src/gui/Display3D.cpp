
#include <gui/Display3D.hpp>
#include <gui/SpinachApp.hpp>
#include <gui/RightClickMenu.hpp>
#include <wx/dcclient.h>
#include <wx/image.h>
#include <wx/dcmemory.h>
#include <iostream>
#include <wx/file.h>

using namespace std;

//============================================================//
// Device Context

SpinachDC::SpinachDC()
        : depthOnly(false),
          mSolidQuadric(gluNewQuadric()),
          mWireQuadric(gluNewQuadric()) {
        gluQuadricDrawStyle(mSolidQuadric,GLU_FILL);
        gluQuadricNormals  (mSolidQuadric,GLU_SMOOTH);
	
        gluQuadricDrawStyle(mWireQuadric,GLU_LINE);
        gluQuadricNormals  (mWireQuadric,GLU_SMOOTH);
        mScallings[Interaction::ST_HFC             ]=1.0;
        mScallings[Interaction::ST_G_TENSER        ]=1.0;
        mScallings[Interaction::ST_ZFS             ]=1.0;
        mScallings[Interaction::ST_EXCHANGE        ]=1.0;
        mScallings[Interaction::ST_SHIELDING       ]=1.0;
        mScallings[Interaction::ST_SCALAR          ]=1.0;
        mScallings[Interaction::ST_QUADRUPOLAR     ]=1.0;
        mScallings[Interaction::ST_DIPOLAR         ]=1.0;
        mScallings[Interaction::ST_CUSTOM_LINEAR   ]=1.0;
        mScallings[Interaction::ST_CUSTOM_BILINEAR ]=1.0;
        mScallings[Interaction::ST_CUSTOM_QUADRATIC]=1.0;
}
SpinachDC::~SpinachDC() {
        gluDeleteQuadric(mSolidQuadric);
        gluDeleteQuadric(mWireQuadric);
}


//============================================================//
// Scene graphs


void OpenGLText::RawDraw(SpinachDC& dc) {
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D); {
    glDisable(GL_LIGHTING);
    glTranslatef(40,dc.height-40,0);
    glBindTexture(GL_TEXTURE_2D,texName);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
    glBegin(GL_QUADS); {
      glTexCoord2f(0,0); glVertex2f(0,0);
      glTexCoord2f(1,0); glVertex2f(w,0);
      glTexCoord2f(1,1); glVertex2f(w,h);
      glTexCoord2f(0,1); glVertex2f(0,h);
    } glEnd();
  } glDisable(GL_TEXTURE_2D);
}

void OpenGLText::UpdateString(const wxString& str) {
  wxMemoryDC memDC;
  wxSize s=memDC.GetTextExtent(str);
      
  w=s.GetWidth();
  h=s.GetHeight();

  wxBitmap bit(w,h); 
  memDC.SelectObject(bit);
  memDC.SetTextForeground(wxColor(255,255,255));
  memDC.SetTextBackground(wxColor(0,0,0));
  memDC.DrawText(str,0,0);
  wxImage img(bit.ConvertToImage());
  img.AddHandler(new wxPNGHandler);
  img.SaveFile(wxT("TextImg.png"), wxBITMAP_TYPE_PNG);
  unsigned char* raw=img.GetData();

  raw[0]=254;
  raw[1]=0;
  raw[2]=0;
  raw[3]=254;
  raw[4]=0;
  raw[5]=0;
  raw[6]=254;
  raw[7]=0;
  raw[8]=0;
  raw[9]=254;
  raw[10]=0;
  raw[11]=0;

  //openGL stuff
  glPixelStorei(GL_UNPACK_ALIGNMENT,1);
  glEnable(GL_TEXTURE_2D);
  glGenTextures(1,&texName);
  glBindTexture(GL_TEXTURE_2D,texName);

  // when texture area is small, bilinear filter the closest mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                     GL_LINEAR_MIPMAP_NEAREST );
    // when texture area is large, bilinear filter the first mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    // if wrap is true, the texture wraps over at the edges (repeat)
    //       ... false, the texture ends at the edges (clamp)
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
    /*glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);*/
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,w,h,0,GL_RGB,GL_UNSIGNED_BYTE,raw);
  //gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGB, w, h,GL_RGB, GL_UNSIGNED_BYTE, raw);
}


GLfloat defaultMaterial[3] = {0.5, 0.5,  0.5}; 

SGNode::SGNode()
  : mDirty(true),
    mUseMaterial(false),
    mMaterial(defaultMaterial),
    mIdentity(true) {
}

SGNode::~SGNode() {
  while(mChildren.begin() != mChildren.end()) {
    /*
      Okay, so this loop looks like it will never end. What's actually
      happening is that when the destructor of a SGNode is called, it
      signals to its parent and the parent removes the pointer from
      mChildren (see SGNode::AddNode)

      While this means it's possible to delete a node without care or
      worry, it does lead to this confuseing loop.

      This is perhaps not my greatest achivement in the field of
      writing readable code!
     */
    delete *(mChildren.begin());
  }
  sigDying(this);
}

void SGNode::AddNode(SGNode* node) {
  mChildren.push_back(node);
  node->sigDying.connect(mem_fun(this,&SGNode::RemoveNode));
  node->sigDirty.connect(mem_fun(this,&SGNode::OnChildDirty));
}

void SGNode::RemoveNode(SGNode* node) {
  for(itor i=mChildren.begin();i!=mChildren.end();++i) {
    if(*(i)==node) {
      mChildren.erase(i);
      //If we have actually removed a node, we need to recompile this
      //display list as well, because it still contains a reference to a
      //dangleing display list
      Dirty();
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

void SGNode::Draw(SpinachDC& dc) {
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
}

void SGNode::GetPovRayString(wxString& str) {
  ToPovRay(str);
  str << wxT("union{ \n");
  for(itor i=mChildren.begin();i!=mChildren.end();++i) {
    (*i)->GetPovRayString(str);
  }
  str << wxT("matrix<")
      << mat[0 ] << wxT(",") << mat[1 ] << wxT(",") << mat[2 ] << wxT(",\n")
      << mat[4 ] << wxT(",") << mat[5 ] << wxT(",") << mat[6 ] << wxT(",\n")
      << mat[8 ] << wxT(",") << mat[9 ] << wxT(",") << mat[10] << wxT(",\n")
      << mat[12] << wxT(",") << mat[13] << wxT(",") << mat[14]
      << wxT(">\n");
  str << wxT("}\n");
}

//============================================================//
// Display3D class
int gl_attribs[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 24, 0};

Display3D::Display3D(wxWindow* parent) 
  : wxGLCanvas(parent,(wxGLContext*)NULL,wxID_ANY,
	       wxDefaultPosition,wxDefaultSize,
	       0,wxT("Display3D"),
	       gl_attribs),
    mRootNode(NULL),
    mDC(),
    mHover(-1) {
  mGLContext=NULL;
  mGLEnabled=false;

  mSS=GetSS();

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
}


void Display3D::EnableGL() {
  if(mGLContext == NULL) {
    mGLContext = new wxGLContext(this);
  }
  this->SetCurrent(*mGLContext);
	
  glClearColor(0.0, 0.0, 0.2, 0.0);
  glClearDepth(1.0);
	
  glShadeModel(GL_SMOOTH);
  //Adpated from a detailed tutorail on opengl lighting located at
  //http://www.falloutsoftware.com/tutorials/gl/gl8.htm
	
  glEnable(GL_TEXTURE_2D);
	
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

  //Generate a texture for saving the depth buffer to
  glGenTextures(1,&mTexDepth);
  //Generate a texture for saving the depth buffer to
  //glGenFramebuffers(1,&mFB);
  cout << "OpenGL version is " << glGetString(GL_VERSION) << endl;
}

void Display3D::ResetView() {
  mRotationMatrix[0 ]=1;  mRotationMatrix[1 ]=0;  mRotationMatrix[2 ]=0;  mRotationMatrix[3 ]=0;
  mRotationMatrix[4 ]=0;  mRotationMatrix[5 ]=1;  mRotationMatrix[6 ]=0;  mRotationMatrix[7 ]=0;
  mRotationMatrix[8 ]=0;  mRotationMatrix[9 ]=0;  mRotationMatrix[10]=1;  mRotationMatrix[11]=0;
  mRotationMatrix[12]=0;  mRotationMatrix[13]=0;  mRotationMatrix[14]=0;  mRotationMatrix[15]=1;
  Refresh();
}

void Display3D::ChangeViewport() {
  GetClientSize(&mWidth,&mHeight);
  glViewport(0,0,mWidth,mHeight);
	
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
	       mHeight,mWidth,
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
}

void Display3D::OnPaint(wxPaintEvent& e) {
  if(!mGLEnabled) {
    mGLEnabled=true;
    EnableGL();
  }

  /*wxString povray;

  povray << wxT("camera {\n  location <")
	 << mCamX << wxT(",") 
	 << mCamY << wxT(",")
	 << mCamZ << wxT(">\n  look_at <0,0,0>\n}\n");
  povray << wxT("light_source {\n<1,0,0>\ncolor rgb <1,1,1>\n}\n");
  povray << wxT("light_source {\n<0,1,0>\ncolor rgb <1,1,1>\n}\n");
  povray << wxT("background{rgb<1,1,1>}\n\n");

  mRootNode->GetPovRayString(povray);
  wxFile f(wxT("povray.pov"),wxFile::write);
  f.Write(povray);
  f.Close();*/

  wxPaintDC dc(this);

  int width,height;
  GetClientSize(&width,&height);
  mDC.width=width; mDC.height=height;

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

  mDC.mRotationMatrix=mRotationMatrix;


  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glClear(GL_DEPTH_BUFFER_BIT);

  if(mRootNode) {
    mRootNode->Draw(mDC);
  }

  //glBindFramebuffer(GL_FRAMEBUFFER,&mFB);

  glEnable(GL_TEXTURE_2D); {
    glBindTexture(GL_TEXTURE_2D,mTexDepth);
    glCopyTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,0,0,width,height,0);
  } glDisable(GL_TEXTURE_2D);
	
  glDepthFunc(GL_LEQUAL);

  glEnable(GL_LIGHTING);{
   glClear(GL_DEPTH_BUFFER_BIT);
   glClear(GL_COLOR_BUFFER_BIT);
    if(mRootNode) {
      mRootNode->Draw(mDC);
    }
    //Now draw the forground objects
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,mWidth,0,mHeight,-50,50);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    mForgroundNode->Draw(mDC);
  } glDisable(GL_LIGHTING);

  glEnable(GL_TEXTURE_2D); {
    glBindTexture(GL_TEXTURE_2D,mTexDepth);
	
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,width,height,0);
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();
	
    long littleWidth=120;
    long littleHeight=round(littleWidth*height/float(width));
	
    glBegin(GL_QUADS); {
      glTexCoord2f(0,0);     glVertex2f(width-littleWidth,0);
      glTexCoord2f(1.0,0);   glVertex2f(width,0);
      glTexCoord2f(1.0,1.0); glVertex2f(width,littleHeight);
      glTexCoord2f(0,1.0);   glVertex2f(width-littleWidth,littleHeight);
    } glEnd();
  }glDisable(GL_TEXTURE_2D);

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



