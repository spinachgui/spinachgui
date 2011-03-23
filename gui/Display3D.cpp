
#include <gui/Display3D.hpp>
#include <gui/SpinachApp.hpp>
#include <gui/RightClickMenu.hpp>
#include <wx/dcclient.h>
#include <wx/image.h>
#include <wx/dcmemory.h>
#include <iostream>
#include <wx/file.h>

using namespace std;
using namespace SpinXML;

//============================================================//
// Device Context

SpinachDC::SpinachDC()
    : depthOnly(false),
      mSolidQuadric(gluNewQuadric()),
      mWireQuadric(gluNewQuadric()) {
    drawBonds=true;
    gluQuadricDrawStyle(mSolidQuadric,GLU_FILL);
    gluQuadricNormals  (mSolidQuadric,GLU_SMOOTH);

    gluQuadricDrawStyle(mWireQuadric,GLU_LINE);
    gluQuadricNormals  (mWireQuadric,GLU_SMOOTH);
    mScallings[Interaction::HFC             ]=1.0;
    mScallings[Interaction::G_TENSER        ]=1.0;
    mScallings[Interaction::ZFS             ]=1.0;
    mScallings[Interaction::EXCHANGE        ]=1.0;
    mScallings[Interaction::SHIELDING       ]=1.0;
    mScallings[Interaction::SCALAR          ]=1.0;
    mScallings[Interaction::QUADRUPOLAR     ]=1.0;
    mScallings[Interaction::DIPOLAR         ]=1.0;
    mScallings[Interaction::CUSTOM_LINEAR   ]=1.0;
    mScallings[Interaction::CUSTOM_BILINEAR ]=1.0;
    mScallings[Interaction::CUSTOM_QUADRATIC]=1.0;

    mInterColours[Interaction::HFC             ]=Vector3d(1.0,1.0,1.0);
    mInterColours[Interaction::G_TENSER        ]=Vector3d(1.0,1.0,1.0);
    mInterColours[Interaction::ZFS             ]=Vector3d(1.0,1.0,1.0);
    mInterColours[Interaction::EXCHANGE        ]=Vector3d(1.0,1.0,1.0);
    mInterColours[Interaction::SHIELDING       ]=Vector3d(1.0,1.0,1.0);
    mInterColours[Interaction::SCALAR          ]=Vector3d(1.0,1.0,1.0);
    mInterColours[Interaction::QUADRUPOLAR     ]=Vector3d(1.0,1.0,1.0);
    mInterColours[Interaction::DIPOLAR         ]=Vector3d(1.0,1.0,1.0);
    mInterColours[Interaction::CUSTOM_LINEAR   ]=Vector3d(1.0,1.0,1.0);
    mInterColours[Interaction::CUSTOM_BILINEAR ]=Vector3d(1.0,1.0,1.0);
    mInterColours[Interaction::CUSTOM_QUADRATIC]=Vector3d(1.0,1.0,1.0);

    mVisible[Interaction::HFC             ]=false;
    mVisible[Interaction::G_TENSER        ]=false;
    mVisible[Interaction::ZFS             ]=false;
    mVisible[Interaction::EXCHANGE        ]=false;
    mVisible[Interaction::SHIELDING       ]=false;
    mVisible[Interaction::SCALAR          ]=false;
    mVisible[Interaction::QUADRUPOLAR     ]=false;
    mVisible[Interaction::DIPOLAR         ]=false;
    mVisible[Interaction::CUSTOM_LINEAR   ]=false;
    mVisible[Interaction::CUSTOM_BILINEAR ]=false;
    mVisible[Interaction::CUSTOM_QUADRATIC]=false;

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
    : mTranslucent(false),
      mTranslucentLevel(1.0),
      mDirty(true),
      mUseMaterial(false),
      mMaterial(defaultMaterial),
      mIdentity(true),
      mPickingName(0) {
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

void SGNode::SetTranslation(const Vector3d& v) {
    mIdentity=false;
    mat[0 ]=1;  mat[4 ]=0;  mat[8 ]=0;  mat[12]=v.x()*Angstroms;
    mat[1 ]=0;  mat[5 ]=1;  mat[9 ]=0;  mat[13]=v.y()*Angstroms;
    mat[2 ]=0;  mat[6 ]=0;  mat[10]=1;  mat[14]=v.z()*Angstroms;
    mat[3 ]=0;  mat[7 ]=0;  mat[11]=0;  mat[15]=1;
}

void SGNode::Draw(SpinachDC& dc) {
    if(!mIdentity) {
        glPushMatrix();
        glMultMatrixf(mat);
    }
    if(mUseMaterial) {
        const static GLfloat white[4]={0.5f,0.5f,0.5f,0.5f};
        glMaterialfv(GL_FRONT,GL_SPECULAR, white);
        glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,mMaterial);
    }
    if(dc.pass == SpinachDC::PICKING) {
        glPushName(mPickingName);
    }
    if((dc.pass == SpinachDC::TRANSLUCENT && mTranslucent) ||
       (dc.pass == SpinachDC::SOLID && !mTranslucent) ||
       (dc.pass == SpinachDC::PICKING && !mTranslucent)) {
        RawDraw(dc);
    }
    if(dc.pass == SpinachDC::PICKING) {
        glPopName();
    }
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
	  mForgroundNode(NULL),
      mDC() {
    mGLContext=NULL;
    mGLEnabled=false;

    mSS=GetRawSS();

    mZoom=0.01 * OPENGL_SCALE;;
    mCamX=0.0;
	mCamY=0.0;
	mCamZ=1.0;

    mXTranslate=0 * OPENGL_SCALE;
    mYTranslate=0 * OPENGL_SCALE;
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

void Display3D::PrintTransformMatricese() {
    // get the current modelview matrix
    GLfloat mv[16];
    GLfloat p[16];
    glGetFloatv(GL_MODELVIEW_MATRIX , mv);
    glGetFloatv(GL_PROJECTION_MATRIX , p);

    cout << "(" << mv[0]  << "," << mv[1]  << "," << mv[2]  << "," << mv[3]  << endl
         << " " << mv[4]  << "," << mv[5]  << "," << mv[6]  << "," << mv[7]  << "," << endl
         << " " << mv[8]  << "," << mv[9]  << "," << mv[10] << "," << mv[11] << "," << endl
         << " " << mv[12] << "," << mv[13] << "," << mv[14] << "," << mv[15] << "," << ")" << endl;

    cout << "(" << p[0]  << "," << p[1]  << "," << p[2]  << "," << p[3]  << "," << endl
         << " " << p[4]  << "," << p[5]  << "," << p[6]  << "," << p[7]  << "," << endl
         << " " << p[8]  << "," << p[9]  << "," << p[10] << "," << p[11] << "," << endl
         << " " << p[12] << "," << p[13] << "," << p[14] << "," << p[15] << ")" << endl;
}


void Display3D::EnableGL() {
    //cout << "OpenGL version is " << glGetString(GL_VERSION) << endl;

    if(mGLContext == NULL) {
        mGLContext = new wxGLContext(this);
    }
    this->SetCurrent(*mGLContext);

    glClearColor(0.0, 0.0, 0.0, 0.0);
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
    GLfloat ambientLight0[] =  {0.4, 0.4, 0.4, 1.0};
    GLfloat diffuseLight0[] =  {0.6, 0.6, 0.6, 1.0};
    GLfloat specularLight0[] = {0.8, 0.8, 0.8, 1.0};
    GLfloat position0[] =      {-1.5, 1.0,-4.0, 1.0};
    // Assign created components to GL_LIGHT0
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight0);
    glLightfv(GL_LIGHT0, GL_POSITION, position0);

    // GL_LIGHT1: the red light emitting light source
    // Create light components for GL_LIGHT1
    GLfloat ambientLight1[] =  {0.4, 0.4, 0.4, 1.0};
    GLfloat diffuseLight1[] =  {0.6, 0.6, 0.6, 1.0};
    GLfloat specularLight1[] = {0.8, 0.8, 0.8, 1.0};
    GLfloat position1[] =      {1.5, 1.0, 4.0, 1.0};
    // Assign created components to GL_LIGHT1
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight1);
    glLightfv(GL_LIGHT1, GL_POSITION, position1);

    glMaterialf(GL_FRONT, GL_SHININESS, 10.0f);

    //Create a dictionary of colours
    glMatrixMode(GL_MODELVIEW);

    //Generate a texture for saving the depth buffer to
    glGenTextures(1,&mTexDepth);
    //Generate a texture for saving the depth buffer to
    //glGenFramebuffers(1,&mFB);
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


void Display3D::DoPickingPass() {
	
}

void Display3D::OnMouseMove(wxMouseEvent& e) {
    if(e.Dragging() && e.RightIsDown()) {
        mXTranslate=mXTranslate+(e.GetX()-mMouseX)*OPENGL_SCALE;
        mYTranslate=mYTranslate-(e.GetY()-mMouseY)*OPENGL_SCALE;
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
    mZoom=mZoom-(0.001 * OPENGL_SCALE)*e.GetWheelRotation()/e.GetWheelDelta();
    if(mZoom<0.001 * OPENGL_SCALE) {
        mZoom=0.001 * OPENGL_SCALE;
    }
    ChangeViewport();
    Refresh();
}

void Display3D::OnRightClick(wxMouseEvent& e) {
	if(!e.Dragging()) {
		if(GetHover()!=NULL) {
			RightClickMenu* menu = new RightClickMenu(this);
			menu->Build();
			PopupMenu(menu);
			delete menu;
		}
	}
}

void Display3D::OnLeftClick(wxMouseEvent& e) {
    if(!e.ShiftDown()) {
		AddSelection(GetHover());
    } else {
		AddSelection(GetHover());
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
    gluLookAt(mCamX*OPENGL_SCALE,mCamY*OPENGL_SCALE,mCamZ*OPENGL_SCALE,0,0,0,0,1,0);
    glMultMatrixf(mRotationMatrix);

    mDC.mRotationMatrix=mRotationMatrix;



    glDepthFunc(GL_LESS);
    //glClear(GL_DEPTH_BUFFER_BIT);

    /*if(mRootNode) {
      mRootNode->Draw(mDC);
      }*/

    //glBindFramebuffer(GL_FRAMEBUFFER,&mFB);

    /*glEnable(GL_TEXTURE_2D); {
      glBindTexture(GL_TEXTURE_2D,mTexDepth);
      glCopyTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,0,0,width,height,0);
      } glDisable(GL_TEXTURE_2D);*/

    //glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);  //Can move to initalisation
    glEnable(GL_LIGHTING);{
        glClear(GL_DEPTH_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT);
        if(mRootNode) {
            //Draw opaque objects first
            glDepthMask(GL_TRUE);
            mDC.pass=SpinachDC::SOLID;
            mRootNode->Draw(mDC);
            //Draw transparent/traslucent objects
            glEnable (GL_BLEND);
            glDepthMask(GL_FALSE);
            glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            mDC.pass=SpinachDC::TRANSLUCENT;
            mRootNode->Draw(mDC);
            glDisable (GL_BLEND);

            //Work out a line the world coordinates of the mouse
            GLint viewport[4];
            GLdouble mvmatrix[16],projmatrix[16];
            glGetIntegerv(GL_VIEWPORT,viewport);
            glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
            glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);
            GLdouble worldFarX,  worldFarY ,worldFarZ;
            GLdouble worldNearX, worldNearY,worldNearZ;
            gluUnProject(mMouseX,height-mMouseY-1,1.0,mvmatrix,projmatrix,viewport,&worldFarX, &worldFarY ,&worldFarZ);
            gluUnProject(mMouseX,height-mMouseY-1,0.0,mvmatrix,projmatrix,viewport,&worldNearX,&worldNearY,&worldNearZ);


            //Draw in opengl picking mode
            glDepthMask(GL_TRUE);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glClear(GL_DEPTH_BUFFER_BIT);
            gluPickMatrix(mMouseX,viewport[3]-mMouseY,3.0, 3.0, viewport);
            glMultMatrixd(projmatrix);

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glMultMatrixd(mvmatrix);

            mDC.pass=SpinachDC::PICKING;
            GLuint buff[2000];
            GLint hits;
            glSelectBuffer(2000,buff);  //glSelectBuffer goes before glRenderMode
            glRenderMode(GL_SELECT);

            glInitNames();
            mRootNode->Draw(mDC);

            hits=glRenderMode(GL_RENDER);
            GLuint* pbuff=buff;
            float closestDistance=HUGE_VAL;
            GLuint* closestNames=NULL;
            GLuint closestNameCount=0;
            if(hits >0) {
                for(long i=0;i<hits;i++) {
                    GLuint name_count = *(pbuff++);
                    float d1=float(*(pbuff++))/0x7fffffff;
                    float d2=float(*(pbuff++))/0x7fffffff;
                    float thisDistance=d1<d2 ? d1 : d2;
                    if(closestDistance > thisDistance) {
                        closestDistance=thisDistance;
                        closestNames=pbuff;
                        closestNameCount=name_count;
                    }
                    pbuff+=name_count;
                }
                switch(closestNames[0]){
                case LAYER_SPINS:
                    SetHover(GetSS()->GetSpin(closestNames[1]));
                    break;
                case LAYER_INTERACTIONS:
                    SetHover(NULL);
                    //NO_OP
                    break;
                case LAYER_BONDS:
                    SetHover(NULL);
                    //NO_OP
                    break;
                }
            } else {
				SetHover(NULL);
			}
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



