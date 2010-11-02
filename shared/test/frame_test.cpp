#define BOOST_TEST_MODULE frame
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <shared/test/common.hpp>
#include <shared/orientation.hpp>
#include <shared/spin.hpp>
#include <shared/basic_math.hpp>
#include <iostream>

using namespace std;
using namespace SpinXML;

struct Setup {
    /*
      Setup reference frames linked like this:

      Lab (implicit)
      |
      |-FrameA
      |  |-SubFrameA1
      |  |-SubFrameA2
      |  |-SubFrameA3
      |     |-SubFrameA3a
      |     |-SubFrameA3b
      |
      |-FrameB
      |-FrameC





     */

#define SETUP_FRAME(fName,V,O,P)                                        \
    fName##_V(V),                                                       \
        fName##_O(Orientation(O)),                                       \
        fName(new Frame(fName##_V,fName##_O,P))                       
    Setup() :
        // Frames A,B and C are pure translational
        SETUP_FRAME(FrameA,Vector3d(1,0,0),Quaterniond(1,0,0,0),NULL),
        SETUP_FRAME(FrameB,Vector3d(0,2,0),Quaterniond(1,0,0,0),NULL),
        SETUP_FRAME(FrameC,Vector3d(1,2,3),Quaterniond(1,0,0,0),NULL),

        //A1 is another translation
        SETUP_FRAME(SubFrameA1,Vector3d(2,1,2),Quaterniond(1,0,0,0),FrameA),
        //A2 is a rotation
        SETUP_FRAME(SubFrameA2,Vector3d(0,0,0),AngleAxisd(pi/2,Vector3d(1,0,0)),FrameA),
        //A3 is a translation and a rotation of pi/2 about 1,0,0
        SETUP_FRAME(SubFrameA3,Vector3d(1,1,1),AngleAxisd(pi/2,Vector3d(1,0,0)),FrameA),

        //These are both rotational and translational.
        SETUP_FRAME(SubFrameA3a,Vector3d(2,1,2) ,AngleAxisd(3*pi/2,Vector3d(1,-1,1)),SubFrameA3),
        SETUP_FRAME(SubFrameA3b,Vector3d(-2,1,2),AngleAxisd(pi    ,Vector3d(0, 1,0)),SubFrameA3) {
    }
    ~Setup() {
        SAFE_DELETE(FrameA);
        SAFE_DELETE(FrameB);
        SAFE_DELETE(FrameC);

        SAFE_DELETE(SubFrameA1);
        SAFE_DELETE(SubFrameA2);
        SAFE_DELETE(SubFrameA3);

        SAFE_DELETE(SubFrameA3a);
        SAFE_DELETE(SubFrameA3b);
    }
#define DECLARE_FRAME(fName)                    \
    Vector3d fName##_V; Orientation fName##_O; Frame* fName;

    DECLARE_FRAME(FrameA);
    DECLARE_FRAME(FrameB);
    DECLARE_FRAME(FrameC);

    DECLARE_FRAME(SubFrameA1);
    DECLARE_FRAME(SubFrameA2);
    DECLARE_FRAME(SubFrameA3);

    DECLARE_FRAME(SubFrameA3a);
    DECLARE_FRAME(SubFrameA3b);
};



BOOST_FIXTURE_TEST_CASE( CorrectHomogenious, Setup ) {
    Vector4d v4(FrameA_V.x(),FrameA_V.y(),FrameA_V.z(),1.0);
    v4=FrameA->getTransformFromLab()*v4;
    BOOST_CHECK_CLOSE(v4.x(),FrameA_V.x(),0.01);
    BOOST_CHECK_CLOSE(v4.y(),FrameA_V.y(),0.01);
    BOOST_CHECK_CLOSE(v4.z(),FrameA_V.z(),0.01);
    BOOST_CHECK_CLOSE(v4.w(),1.0,0.01);

}

