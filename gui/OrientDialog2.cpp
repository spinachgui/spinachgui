
#include <gui/OrientDialog2.hpp>
#include <shared/basic_math.hpp>
#include <gui/TextCtrlFocus.hpp>
#include <iostream>

using namespace Eigen;
using namespace sigc;
using namespace std;
using namespace SpinXML;

OrientDialog2::OrientDialog2(wxWindow* parent)
    : OrientDialog2Base(parent,wxID_ANY,wxT("Orientation"),wxDefaultPosition, wxSize( 600,350 )) {
    mOrientDisplay3D->sigRotate.connect(mem_fun(this,&OrientDialog2::SlotRotate));
}

void OrientDialog2::SlotRotate(Eigen::Matrix3f rotation) {
    Matrix3d rotationd = Matrix3f2Matrix3d(rotation);
    Orientation o(rotationd);
    SetOrient(o);
}

void OrientDialog2::SetOrient(const Orientation& orient,maybeOrientType typeToKeep) {
    EulerAngles ea  = orient.GetAsEuler();
    Matrix3d    mat = orient.GetAsDCM();
    AngleAxisd  aa  = orient.GetAsAngleAxis();
    Quaterniond q   = orient.GetAsQuaternion();

    if(!typeToKeep || typeToKeep.get() != Orientation::EULER) {
        mAlpha->ChangeValue(wxString() << ea.alpha /PI*180);
        mBeta ->ChangeValue(wxString() << ea.beta  /PI*180);
        mGamma->ChangeValue(wxString() << ea.gamma /PI*180);
    }
    if(!typeToKeep || typeToKeep.get() != Orientation::DCM) {
        m00->ChangeValue(wxString() << mat(0,0));
        m01->ChangeValue(wxString() << mat(0,1));
        m02->ChangeValue(wxString() << mat(0,2));

        m10->ChangeValue(wxString() << mat(1,0));
        m11->ChangeValue(wxString() << mat(1,1));
        m12->ChangeValue(wxString() << mat(1,2));

        m20->ChangeValue(wxString() << mat(2,0));
        m21->ChangeValue(wxString() << mat(2,1));
        m22->ChangeValue(wxString() << mat(2,2));
    }
    if(!typeToKeep || typeToKeep.get() != Orientation::ANGLE_AXIS) {
        mAngle->ChangeValue(wxString() << aa.angle());
        mX    ->ChangeValue(wxString() << aa.axis().x());
        mY    ->ChangeValue(wxString() << aa.axis().y());
        mZ    ->ChangeValue(wxString() << aa.axis().z());
    }
    if(!typeToKeep || typeToKeep.get() != Orientation::QUATERNION) {
        mRe->ChangeValue(wxString() << q.w());
        mI ->ChangeValue(wxString() << q.x());
        mJ ->ChangeValue(wxString() << q.y());
        mK ->ChangeValue(wxString() << q.z());
    }
    Matrix3f mat3f = Matrix3d2Matrix3f(mat);
    mOrientDisplay3D->GetCamera()->SetRotation(mat3f);
    mOrientDisplay3D->Refresh();
}

void OrientDialog2::OnEulerEdit(wxCommandEvent& e) {
    double alpha,beta,gamma;
    mAlpha->GetValue().ToDouble(&alpha);
    mBeta ->GetValue().ToDouble(&beta);
    mGamma->GetValue().ToDouble(&gamma);
    EulerAngles ea(alpha*PI/180,beta*PI/180,gamma*PI/180);
    SetOrient(Orientation(ea),Orientation::EULER);
}
void OrientDialog2::OnAngleAxisEdit(wxCommandEvent& e) {
    double angle,x,y,z;
    mAngle->GetValue().ToDouble(&angle);
    mX->GetValue().ToDouble(&x);
    mY->GetValue().ToDouble(&y);
    mZ->GetValue().ToDouble(&z);
    AngleAxisd aa(angle,Vector3d(x,y,z));
    SetOrient(Orientation(aa),Orientation::ANGLE_AXIS);
}
void OrientDialog2::OnQuaternionEdit(wxCommandEvent& e) {
    double re,i,j,k;
    mRe->GetValue().ToDouble(&re);
    mI->GetValue().ToDouble(&i);
    mJ->GetValue().ToDouble(&j);
    mK->GetValue().ToDouble(&k);
    Quaterniond q(re,i,j,k);
    SetOrient(Orientation(q),Orientation::QUATERNION);
}
void OrientDialog2::OnDCMEdit(wxCommandEvent& e) {
    double
        mat00,mat10,mat20,
        mat01,mat11,mat21,
        mat02,mat12,mat22;

    m00->GetValue().ToDouble(&mat00);
    m10->GetValue().ToDouble(&mat10);
    m20->GetValue().ToDouble(&mat20);
              
    m01->GetValue().ToDouble(&mat01);
    m11->GetValue().ToDouble(&mat11);
    m21->GetValue().ToDouble(&mat21);
              
    m02->GetValue().ToDouble(&mat02);
    m12->GetValue().ToDouble(&mat12);
    m22->GetValue().ToDouble(&mat22);

    SetOrient(Orientation(MakeMatrix3d(mat00,mat10,mat20,
                                       mat01,mat11,mat21,
                                       mat02,mat12,mat22)),Orientation::DCM);
}

void OrientDialog2::OnIdentity(wxCommandEvent& e) {
    SetOrient(EulerAngles(0,0,0));
}

BEGIN_EVENT_TABLE(OrientDialog2,wxDialog)
EVT_TEXT(ID_EULER    ,OrientDialog2::OnEulerEdit)
EVT_TEXT(ID_ANGLEAXIS,OrientDialog2::OnAngleAxisEdit)
EVT_TEXT(ID_QUAT     ,OrientDialog2::OnQuaternionEdit)
EVT_TEXT(ID_MATRIX   ,OrientDialog2::OnDCMEdit)

EVT_BUTTON(ID_IDENTITY,OrientDialog2::OnIdentity)

END_EVENT_TABLE()

