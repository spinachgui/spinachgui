
#ifndef __DISPLAYSETTINGS_HPP__
#define __DISPLAYSETTINGS_HPP__

#include <shared/interaction.hpp>
#include <3d/opengl.hpp>

#include <map>

///A struct storing all 3D settings that are invariant over a single
///rendering pass, implying that this structure may be passed safely
///as a const reference. It would likely make sense to link settings
///here to a menu option of configuration file setting. Information
///such as the current render pass would not belong in this struct
struct DisplaySettings {
    DisplaySettings()
		: mSolidQuadric(gluNewQuadric()),
		  mWireQuadric(gluNewQuadric()) {
		gluQuadricDrawStyle(mSolidQuadric,GLU_FILL);
		gluQuadricNormals  (mWireQuadric,GLU_SMOOTH);
    }
    ~DisplaySettings() {
		gluDeleteQuadric(mSolidQuadric);
        gluDeleteQuadric(mWireQuadric);
    }
    void SetShowBonds(bool showBonds) {drawBonds=showBonds;}
    bool drawBonds;

    void SetScalling(double scalling,SpinXML::Interaction::Type t) {
        mScallings[t]=scalling;
    }
    void SetColour(float r,float g,float b,SpinXML::Interaction::Type t) {
        mInterColours[t]=Eigen::Vector3d(r,g,b);
    }
    void SetVisible(bool v,SpinXML::Interaction::Type t) {
        mVisible[t]=v;
    }

    GLUquadric* mSolidQuadric;
    GLUquadric* mWireQuadric;
    int width,height;
    const float* mRotationMatrix;

    ///The size of the displayed interaction in units of energy/length,
    ///e.g. MHz/Nm
    std::map<SpinXML::Interaction::Type,double> mScallings;
    ///The colour of the displayed interactions
    std::map<SpinXML::Interaction::Type,Eigen::Vector3d> mInterColours;
    ///The visibility of the displayed interactions
    std::map<SpinXML::Interaction::Type,bool> mVisible;
};


#endif
