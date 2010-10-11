
#include <shared/interaction.hpp>
#include <Eigen/Eigenvalues> 

using namespace SpinXML;


Matrix3d SpinXML::ConvertToMatrix(const Matrix3d& I) {return I;}
Matrix3d SpinXML::ConvertToMatrix(const Eigenvalues& I) {
	//Undo the eigensystem decomposition
	Matrix3d in_eigen_frame(MakeMatrix3d(I.xx.si,0      ,0       , 
                                             0      ,I.yy.si,0       , 
                                             0      ,0      ,I.zz.si));
	Matrix3d rotMatrix=I.mOrient.GetAsMatrix();
 	return rotMatrix*in_eigen_frame*rotMatrix.inverse();
}
Matrix3d SpinXML::ConvertToMatrix(const AxRhom& I)  {return ConvertToMatrix(ConvertToEigenvalues(I));}
Matrix3d SpinXML::ConvertToMatrix(const SpanSkew& I){return ConvertToMatrix(ConvertToEigenvalues(I));}


Eigenvalues SpinXML::ConvertToEigenvalues(const Matrix3d& I) {
	EigenSolver<Matrix3d> solver(I, true); //true => compute eigenvectors
	Vector3d v1,v2,v3;//Eigenvectors
	
	double e1=real(solver.eigenvalues()[0]);
	double e2=real(solver.eigenvalues()[1]);
	double e3=real(solver.eigenvalues()[2]);
	return Eigenvalues(energy(e1),energy(e2),energy(e3),Orientation(solver.eigenvectors().real()));
}
Eigenvalues SpinXML::ConvertToEigenvalues(const Eigenvalues& I) {return I;}
Eigenvalues SpinXML::ConvertToEigenvalues(const AxRhom& I) {
    energy xx=I.iso - I.ax/6.0 + I.rh/2.0;
    energy yy=I.iso - I.ax/6.0 - I.rh/2.0;
    energy zz=I.iso + I.ax/3.0;
    return Eigenvalues(xx,yy,zz,I.mOrient);
}
Eigenvalues SpinXML::ConvertToEigenvalues(const SpanSkew& I) {
    energy xx=I.iso+I.span*I.skew/6.0-I.span/2.0;
    energy yy=I.iso-I.span*I.skew/3.0;
    energy zz=I.iso+I.span*I.skew/6.0+I.span/2.0;
    return Eigenvalues(xx,yy,zz,I.mOrient);
}

AxRhom SpinXML::ConvertToAxRhom(const Matrix3d& I) {return ConvertToAxRhom(ConvertToEigenvalues(I));}
AxRhom SpinXML::ConvertToAxRhom(const Eigenvalues& I) {
	energy xx=I.xx;	energy yy=I.yy;	energy zz=I.zz;
	//Sort so that xx <= yy <= zz
	energy tmp;
	if(zz < yy) tmp=zz; zz=yy; yy=tmp;
	if(zz < xx) tmp=zz; zz=xx; xx=tmp; //Now zz is definatly bigest
	if(yy < xx) tmp=xx; xx=yy; yy=tmp;

	energy iso = (xx+yy+zz)/3.0;
	energy ax=2*zz -  xx - yy;
	energy rh=xx - yy;
	return AxRhom(iso,ax,rh,I.mOrient);
}
AxRhom SpinXML::ConvertToAxRhom(const AxRhom& I) {return I;}
AxRhom SpinXML::ConvertToAxRhom(const SpanSkew& I) {return ConvertToAxRhom(ConvertToEigenvalues(I));}

SpanSkew SpinXML::ConvertToSpanSkew(const Matrix3d& I) {return ConvertToSpanSkew(ConvertToEigenvalues(I));}
SpanSkew SpinXML::ConvertToSpanSkew(const Eigenvalues& I) {
	energy xx=I.xx;	energy yy=I.yy;	energy zz=I.zz;
	//Sort so that xx <= yy <= zz
	energy tmp;
	if(zz < yy) tmp=zz; zz=yy; yy=tmp;
	if(zz < xx) tmp=zz; zz=xx; xx=tmp; //Now zz is the bigest
	if(yy < xx) tmp=xx; xx=yy; yy=tmp;

	energy iso = (xx+yy+zz)/3.0;
	energy span=zz - xx;
	double skew=span == energy(0.0) && span == energy(-0.0) ? 0.5 : ((3.0/2.0)*((iso-yy)/span)).si;
	return SpanSkew(iso,span,skew,I.mOrient);
}
SpanSkew SpinXML::ConvertToSpanSkew(const AxRhom& I) {return ConvertToSpanSkew(ConvertToEigenvalues(I));}
SpanSkew SpinXML::ConvertToSpanSkew(const SpanSkew& I) {return I;}
