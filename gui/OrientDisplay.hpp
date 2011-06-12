
#include <3d/glgeometry.hpp>
#include <3d/glmode.hpp>
#include <3d/camera.hpp>
#include <gui/Display3D.hpp>
#include <gui/TextBitmap.hpp>
 
class OrientDisplay3D : public Display3D {
public:
    OrientDisplay3D(wxWindow* parent);
protected:
    virtual void DrawScene(); 
    virtual void DrawForeground();
    virtual void OnMouseOver3D(int stackLength,const GLuint* ClosestName);

    TextBitmap mTX;
    TextBitmap mTY;
    TextBitmap mTZ;
};

