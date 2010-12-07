
#define BOOST_TEST_MODULE format
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN 
#include <boost/test/unit_test.hpp>
#include <shared/test/common.hpp>

//#define BOOST_SYSTEM_NO_DEPRECATED

#include <shared/formats/xyz.hpp>
#include <shared/formats/g03.hpp>
#include <shared/formats/xml.hpp>
#include <shared/formats/castep.hpp>
#include <shared/formats/simpson.hpp>
#include <shared/formats/pdb.hpp>

#include <shared/nuclear_data.hpp>


#include <iostream>

#include "boost/filesystem.hpp"   // includes all needed Boost.Filesystem declarations
#include <iostream>               // for std::cout


using namespace std;
using namespace SpinXML;
using namespace boost::filesystem;  


class setup {
public:
	setup() : ss(new SpinSystem) {
		FindFullPath(boost::unit_test::framework::master_test_suite().argv[0]);

		mG03Loader     = new G03Loader;
		mSIMPSONLoader = new SIMPSONLoader;
		mPDBLoader     = new PDBLoader;
		
		path schema_path = full_path;
		schema_path.remove_filename();
		schema_path/="data";
		schema_path/="spinxml_schema.xsd";
		string str = schema_path.string();
		str = "file://" + str;
		mXMLLoader = new XMLLoader(str.c_str());

		try {
			LoadIsotopes();
		} catch(runtime_error e) {
			cerr << "Isotopes not loaded" << endl;
			throw e;
		}
		unitSystem = new UnitSystem;
	}
	~setup() {
		SAFE_DELETE(ss);
		SAFE_DELETE(unitSystem);

		SAFE_DELETE(mG03Loader);
		SAFE_DELETE(mSIMPSONLoader);
		SAFE_DELETE(mXMLLoader);
		SAFE_DELETE(mPDBLoader);
	}
	void FindFullPath(const char* argv0) {
		full_path = system_complete( path( argv0 ));
	}

	SpinXML::ISpinSystemLoader* mG03Loader;	   
	SpinXML::ISpinSystemLoader* mSIMPSONLoader;
	SpinXML::ISpinSystemLoader* mPDBLoader;
	SpinXML::ISpinSystemLoader* mXMLLoader;    
	path full_path;

	SpinSystem* ss;

	UnitSystem* unitSystem;
};

BOOST_FIXTURE_TEST_CASE( g03Load, setup) {
	//ss->LoadFromFile("examples/Gaussian/ESR spectroscopy/cpdyad_cation.log",mG03Loader);
}

BOOST_FIXTURE_TEST_CASE( pdbLoad, setup) {
	ss->LoadFromFile("examples/pdb/2X9B.pdb",mPDBLoader);
}

BOOST_FIXTURE_TEST_CASE( synthetic, setup) {
	/*	Spin* spin0=new Spin(Vector3d(0    ,0    ,0	   ),"0,0,0",1);
	Spin* spinx=new Spin(Vector3d(3e-10,0    ,0	   ),"1,0,0",6);
	Spin* spiny=new Spin(Vector3d(0    ,3e-10,0	   ),"0,1,0",7);
	Spin* spinz=new Spin(Vector3d(0    ,0    ,3e-10),"0,0,1",8);
	
	Interaction* inter0=new Interaction(Eigenvalues(10e-6*eV,1e-6*eV,1e-6*eV  ,Orientation(AngleAxisd(0   ,Vector3d(1  ,0  ,0)))),
										Interaction::SHIELDING,spin0);
	Interaction* inter1=new Interaction(Eigenvalues(10e-6*eV,1e-6*eV,1e-6*eV  ,Orientation(AngleAxisd(PI/4,Vector3d(1  ,0  ,0)))),
										Interaction::SHIELDING,spinx);
	Interaction* inter2=new Interaction(Eigenvalues(10e-6*eV,1e-6*eV,1e-6*eV  ,Orientation(AngleAxisd(PI/4,Vector3d(0.5,0.5,0)))),
										Interaction::SHIELDING,spiny);
	Interaction* inter3=new Interaction(Eigenvalues(10e-6*eV,1.1e-6*eV,1e-6*eV,Orientation(AngleAxisd(PI/4,Vector3d(0  ,0  ,1)))),
										Interaction::SHIELDING,spinz);
	ss->InsertSpin(spin0);
	ss->InsertSpin(spinx);
	ss->InsertSpin(spiny);
	ss->InsertSpin(spinz);
	
	ss->InsertInteraction(inter0);
	ss->InsertInteraction(inter1);
	ss->InsertInteraction(inter2);
	ss->InsertInteraction(inter3);

	Frame* rootFrame  = ss->GetLabFrame();
    rootFrame->AddChild(new Frame(Vector3d(1,0,0),Orientation(),unitSystem));
	//Rotation about the x axis

	Frame* withChildrenAndParent = new Frame(Vector3d(1,0,0),Orientation(Quaterniond(sqrt(0.5),sqrt(0.5),0,0)),unitSystem);
	rootFrame->AddChild(withChildrenAndParent);
	withChildrenAndParent->AddChild(new Frame(Vector3d(4,4,0),Orientation(EulerAngles(1,0.2,0.5)),unitSystem));

	ss->SaveToFile("test_out/synthetic.xml",mXMLLoader);

	SpinSystem* LoadedSpinSystem = new SpinSystem;
	LoadedSpinSystem->LoadFromFile("test_out/synthetic.xml",mXMLLoader);
	LoadedSpinSystem->SaveToFile("test_out/synthetic2.xml",mXMLLoader);
	

	SAFE_DELETE(LoadedSpinSystem);*/
}


/*
        //Execute this testing code once when the main window is
        //created. Doing this in an onrezise should ensure that the window
        //is on the screne at the time.
        LoadFromFile(wxT("data/OFNAPH01_NMR.magres"),
                     wxT("data/"),
                     wxT("OFNAPH01_NMR.magres"));
        LoadFromFile(wxT("data/tryosine.log"),
                     wxT("data/"),
                     wxT("tryosine.log"));
        //GetSS()->CalcNuclearDipoleDipole();
        for(unsigned long i=0;i<wxGetApp().GetIOFilters().size();i++) {
            ISpinSystemLoader* saver=wxGetApp().GetIOFilters()[i];
            if(saver->GetFilterType()==ISpinSystemLoader::SAVE ||
               saver->GetFilterType()==ISpinSystemLoader::LOADSAVE) {
                GetSS()->SaveToFile((string("test.")+saver->GetFilter()).c_str(),saver);
            }
        }
        GetSS()->Clear();
 /


*/
