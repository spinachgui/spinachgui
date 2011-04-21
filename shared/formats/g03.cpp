
#include <shared/formats/g03.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <boost/algorithm/string/trim.hpp>
#include <shared/nuclear_data.hpp>

#include <shared/basic_math.hpp>

using namespace std;
using namespace SpinXML;

void G03Loader::LoadFile(SpinSystem* ss,const char* filename) const {
	/*
	  This function really needs some work done on in, as it's not using C++
	  streams properly. This is due to it being adapted from matlab code (which uses
	  c style I/O
	*/
	vector<double>      isoHFC;
	vector<Eigenvalues> anisoHFC;

	ss->Clear();
	Spin* electron=new Spin(Vector3d(0,0,0),string("Unpaired Electron"),0);
	ss->InsertSpin(electron);

	ifstream fin(filename);
	cout << "Opening a g03 file:" << filename << endl;
	if(!fin.is_open()) {
		//Throw some sort of error here
		cerr << "Couldn't open file" << endl;
		throw runtime_error("Couldn't Open File");
	}
	long nAtoms=0;
	bool standardOrientFound=false; //Some files seem to have many standard orientation sections.
	while(!fin.eof()) {
		string line;
		char buff[500];  //TODO buletproof this
		fin.getline(buff,500); line=buff; //Read a line
		boost::algorithm::trim(line); //Remove whitespace

		if (line=="Standard orientation:") {
			cout << "Standard orientation found." << endl;
			nAtoms=0;
			//Skip 4 lines
			fin.getline(buff,500); line=buff; //Read a line
			fin.getline(buff,500); line=buff; //Read a line
			fin.getline(buff,500); line=buff; //Read a line
			fin.getline(buff,500); line=buff; //Read a line
			istringstream stream;

			fin.getline(buff,500); line=buff; stream.str(line); //Read a line
			while(line.find("--------") == string::npos && !fin.eof()) {
				int dummy1,atomicNumber,dummy3;
				length x,y,z;
				stream >> dummy1 >> atomicNumber >> dummy3 >> x >> y >> z;
				x=x*Angstroms;
				y=y*Angstroms;
				z=z*Angstroms;
				stream.clear();
				fin.getline(buff,500); line=buff; stream.str(line); //Read a line
				if(standardOrientFound) {
					//Update an existing spin
					if(ss->GetSpinCount() > nAtoms) {
						ss->GetSpin(nAtoms)->SetPosition(Vector3d(x,y,z));
					} else {
						cerr << "Error reading standard orientation: the number of centres is not consistant amoung the standard orientation tables" << endl;
						throw runtime_error("Error reading standard orientation: the number of centres is not consistant amoung the standard orientation tables");
					}
				} else {
					//Create a new spin
					string isotopeSymbol(getElementSymbol(atomicNumber));
					isotopeSymbol=isotopeSymbol; //We have no clue what the mass number is yet
					Spin* s=new Spin(Vector3d(x,y,z),string("A Spin")+isotopeSymbol,atomicNumber);
					ss->InsertSpin(s);
				}
				nAtoms++;
			}
			standardOrientFound=true;

		} else if(line=="g tensor (ppm):") {
		} else if(line=="g tensor [g = g_e + g_RMC + g_DC + g_OZ/SOC]:") {
			double XX = -1, YX = -1, ZX = -1;
			double XY = -1, YY = -1, ZY = -1;
			double XZ = -1, YZ = -1, ZZ = -1;

			istringstream stream;

			fin.getline(buff,500);
			for(long i=0;i<500;i++) if(buff[i]=='D') buff[i]='e';   //Replace fortran's "D" with "e"
			for(long i=0;i<500;i++) if(buff[i]=='X'||buff[i]=='Y'||buff[i]=='Z'||buff[i]=='=') buff[i]=' ';   //Get rid of the junk
			stream.str(buff);
			stream >> XX >> YX >> ZX;

			fin.getline(buff,500);
			for(long i=0;i<500;i++) if(buff[i]=='D') buff[i]='e';   //Replace fortran's "D" with "e"
			for(long i=0;i<500;i++) if(buff[i]=='X'||buff[i]=='Y'||buff[i]=='Z'||buff[i]=='=') buff[i]=' ';   //Get rid of the junk
			stream.str(buff);
			stream >> XY >> YY >> ZY;

			fin.getline(buff,500);
			for(long i=0;i<500;i++) if(buff[i]=='D') buff[i]='e';   //Replace fortran's "D" with "e"
			for(long i=0;i<500;i++) if(buff[i]=='X'||buff[i]=='Y'||buff[i]=='Z'||buff[i]=='=') buff[i]=' ';   //Get rid of the junk
			stream.str(buff);
			stream >> XZ >> YZ >> ZZ;
			
			Matrix3d gTensor;
			gTensor <<
				XX, YX, ZX,
				XY, YY, ZY,
				XZ, YZ, ZZ;

			Interaction* inter=new Interaction(gTensor, Interaction::G_TENSER,electron); 
			ss->InsertInteraction(inter);
		} else if(line=="SCF GIAO Magnetic shielding tensor (ppm):") {
			for(long i=0;i<nAtoms;i++) {
				long centerNumber; 
				string element;
				string Isotropic,Anisotropy,Eigenvalues; //Should always end up with the value "Isotropic" or "Anisotropy" etc. or file is corrupt
				string equils,equils2;
				double isotropicValue,anisotropicValue;
				string sXX,sYX,sZX, //Should end up "<variable name>="
					sXY,sYY,sZY,
					sXZ,sYZ,sZZ;
				double XX,YX,ZX, //Should end up "<variable name>="
					XY,YY,ZY,
					XZ,YZ,ZZ;
				double ev1,ev2,ev3;

				fin >> centerNumber >> element >> Isotropic >> equils >> isotropicValue >> Anisotropy >> equils2 >> anisotropicValue;
				fin >> sXX >> XX >> sYX >> YX >> sZX >> ZX;
				fin >> sXY >> XY >> sYY >> YY >> sZY >> ZY;
				fin >> sXZ >> XZ >> sYZ >> YZ >> sZZ >> ZZ;
				fin >> Eigenvalues >> ev1 >> ev2 >> ev3;

				//Check we got the input we expected
				if(Isotropic!="Isotropic" || Anisotropy!="Anisotropy" || Eigenvalues!="Eigenvalues:"
				   ||sXX!="XX="||sYX!="YX="||sZX!="ZX=" 
				   ||sXY!="XY="||sYY!="YY="||sZY!="ZY="
				   ||sXZ!="XZ="||sYZ!="YZ="||sZZ!="ZZ=") {
					ostringstream eout;
					eout << "Error reading GIAO Magnetic shielding tensor" << endl;
					eout << "Expected 'Isotropic' got " << Isotropic << endl;
					eout << "Expected 'Anisotropy' got " << Anisotropy << endl;
					eout << "Expected 'Eigenvalues:' got " << Eigenvalues << endl;

					eout << "XX=" << sXX;
					eout << " XY=" << sXY;
					eout << " XZ=" << sXZ << endl;

					eout << "YX=" << sYX;
					eout << " YY=" << sYY;
					eout << " YZ=" << sYZ << endl;

					eout << "ZX=" << sZX;
					eout << " ZY=" << sZY;
					eout << " ZZ=" << sZZ << endl;
					cerr << eout.str();
					throw runtime_error(eout.str());
				} else {
					//Store the interaction
				}
			}
		} else if(line=="Total nuclear spin-spin coupling J (Hz):") {
			long rows = nAtoms/5; //The number of rows the matrix is split over with the full 5 colums.
			//I'm relying on the fact that division is rounded down
			for(long i=0;i<rows;i++) {  //For each big row
				//Skip the center number labels on the top row
				for(long k=0;k<5;k++) {
					long topCenterNumber;
					fin >> topCenterNumber;
					if(i*5+k+1 != topCenterNumber) {
						ostringstream eout;
						eout << "Error reading J couplings, found topCenterNumber=" << topCenterNumber << " expected " << i*5+k+1 << endl; 
						cerr << eout.str();
						throw runtime_error(eout.str());
					}
				}

				for(long j=i*5;j<nAtoms;j++) {  //For each little row in the big row
					long centerNumber;
					fin >> centerNumber;
					if(centerNumber != j+1) {
						ostringstream eout;
						eout << "Error reading J Couplings, found centerNumber=" << centerNumber << " expected " << j+1 << endl;
						cerr << eout.str();
						throw runtime_error(eout.str());
					}
					for(long k=0;k<5 && k < j+1-i*5;k++) {
						string JCouplingStr;
						energy JCoupling;
						fin >> JCouplingStr;
						//Fotran double precision output isn't compatable with C++ iostreams, so we have to tweek it
						//Replace the D with an E
						long Dpos=JCouplingStr.find('D');
						if(Dpos >= 0) {
							JCouplingStr.replace(Dpos,1,"e");
						} else {
							ostringstream eout;
							eout << "Was expecting a J coupling value in the form +-0.00000D00 but there was no D. Got" << JCouplingStr << endl;
							cerr << eout.str();
							throw runtime_error(eout.str());
						}
						//Pack it back into a istream and parse
						istringstream stream;
						stream.str(JCouplingStr);
						stream >> JCoupling;
						Interaction* inter=new Interaction(JCoupling/Hz,Interaction::SCALAR,ss->GetSpin(j+2),ss->GetSpin(i*5+k+2));
						ss->InsertInteraction(inter);
					}
				}
			}
		} else if(line=="Isotropic Fermi Contact Couplings") {
			if (isoHFC.size() > 0) {
				//Sometimes Gaussian seems to just feel like giving
				//you these twice. Assume the last occurence is the
				//one we want
				isoHFC.clear();
			}
			cout << "Isotropic couplings found" << endl;
			//Skip a line
			fin.getline(buff,500); line=buff; //Read a line
			istringstream stream;
			for (long i=0;i<nAtoms;i++) {
				string dummy1,dummy2,dummy3;
				energy isoCoupling;
				fin.getline(buff,500); line=buff; //Read a line
				stream.clear(); stream.str(line);
				//Read the coupling strength (in megaherz)
				stream >> dummy1 >> dummy2 >> dummy3 >> isoCoupling;
        
				isoHFC.push_back(isoCoupling*MHz);
			}          
		}
		if(line=="Anisotropic Spin Dipole Couplings in Principal Axis System") {
			if (anisoHFC.size() > 0) {
				//Sometimes Gaussian seems to just feel like giving
				//you these twice. Assume the last occurence is the
				//one we want
				anisoHFC.clear();
			}
			cout << "Anisotropic couplings found" << endl;
			//Skip 4 lines
			fin.getline(buff,500); line=buff; //Read a line
			fin.getline(buff,500); line=buff; //Read a line
			fin.getline(buff,500); line=buff; //Read a line
			fin.getline(buff,500); line=buff; //Read a line
			istringstream stream;
			for (long i=0;i<nAtoms;i++) {
				string dummy1,dummy2,dummy3,dummy4,dummy5,isotope;
				energy eigenvalue1,eigenvalue2,eigenvalue3;
				double x1,y1,z1,x2,y2,z2,x3,y3,z3;
				//Read the rotation matrix and eigenvalues (in megaherz)
				fin.getline(buff,500); line=buff; stream.clear(); stream.str(line); //Read a line
				stream                      >> dummy1 >> dummy2 >> eigenvalue1 >> dummy3 >> dummy4 >> x1 >> y1 >> z1;

				fin.getline(buff,500); line=buff; stream.clear(); stream.str(line); //Read a line
				stream >> dummy5 >> isotope >> dummy1 >> dummy2 >> eigenvalue2 >> dummy3 >> dummy4 >> x2 >> y2 >> z2;

				fin.getline(buff,500); line=buff; stream.clear(); stream.str(line); //Read a line
				stream                      >> dummy1 >> dummy2 >> eigenvalue3 >> dummy3 >> dummy4 >> x3 >> y3 >> z3;
				//Skip a line
				fin.getline(buff,500); line=buff; //Read a line

				Matrix3d mat3 = MakeMatrix3d(x1,y1,z1,
											 x2,y2,z2,
											 x3,y3,z3);

				double det = mat3.determinant();
				if(det < 0) {
					//Sometimes gaussian produces inversions rather
					mat3 = -mat3;
				}
				
				Orientation o(mat3);

				anisoHFC.push_back(Eigenvalues(eigenvalue1*MHz,eigenvalue2*MHz,eigenvalue3*MHz,o));
			}
		}
	}

	for(unsigned long i = 0;i<isoHFC.size();i++) {
		double      iso       = isoHFC[i];
		Eigenvalues toCompose = anisoHFC[i];
		toCompose.xx += iso;
		toCompose.yy += iso;
		toCompose.zz += iso;
		Interaction* inter=new Interaction(toCompose, Interaction::HFC, ss->GetSpin(i+1),ss->GetSpin(0)); 
		ss->InsertInteraction(inter);

	}
	ss->SetupLabFrame();
	cout << "Finished loading the g03 file, saving ss->GetSpinCount()=" << ss->GetSpinCount() << endl;
}

