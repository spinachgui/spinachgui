
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
#include <iostream>               // for std::cout

#include <boost/filesystem.hpp>

using namespace boost::filesystem;

using namespace std;
using namespace SpinXML;

G03Loader gG03Loader;
SIMPSONLoader gSIMPSONLoader;
PDBLoader gPDBLoader;
XMLLoader gXMLLoader;

void makeDir (path p) {
	if(!exists(p)) {
		create_directory(p);
	}
}

class setup {
public:
	setup() : ss(new SpinSystem) {
		try {
			LoadIsotopes();
		} catch(runtime_error e) {
			cerr << "Isotopes not loaded" << endl;
			throw e;
		}
	}
	~setup() {
		SAFE_DELETE(ss);
	}
	SpinSystem* ss;
};

BOOST_FIXTURE_TEST_CASE( g03Load, setup) {
	path testOut("test_out/g03ToXMD");
    path testDir("examples/Gaussian/testing/");

	remove_all(testOut);
    if (!exists(testDir)) {
        cerr << "Couldn't find any test data" << endl;
        BOOST_CHECK(false);
    }
	makeDir(testOut);
    directory_iterator end_itr; // default construction yields past-the-end
    for (directory_iterator itr(testDir); itr != end_itr; ++itr ) {
        if (!is_directory(itr->status())) {
			cout << "Test-converting " << itr->path().filename() << endl;
			string inPath  = itr->path().string();
			string outPath = (testOut / itr->path()).string();

			ss->LoadFromFile(inPath.c_str(),&gG03Loader);
			ss->SaveToFile((outPath + ".xml").c_str(),&gXMLLoader);
        }
    }
}

BOOST_FIXTURE_TEST_CASE( pdbLoad, setup) {
	//	ss->LoadFromFile("examples/pdb/2X9B.pdb",mPDBLoader);
}

BOOST_FIXTURE_TEST_CASE( synthetic, setup) {
	Spin* spin0=new Spin(Vector3d(0    ,0    ,0	   ),"0,0,0",1);
	Spin* spinx=new Spin(Vector3d(3e-10,0    ,0	   ),"1,0,0",6);
	Spin* spiny=new Spin(Vector3d(0    ,3e-10,0	   ),"0,1,0",7);
	Spin* spinz=new Spin(Vector3d(0    ,0    ,3e-10),"0,0,1",8);
	
	Interaction* inter0=new Interaction(Eigenvalues(1*MHz,1*MHz,1*MHz   ,Orientation(AngleAxisd(0   ,Vector3d(1  ,0  ,0)))),
										Interaction::SHIELDING,spin0);
	Interaction* inter1=new Interaction(Eigenvalues(10*MHz,1*MHz,1*MHz  ,Orientation(AngleAxisd(PI/4,Vector3d(1  ,0  ,0)))),
										Interaction::SHIELDING,spinx);
	Interaction* inter2=new Interaction(Eigenvalues(1*MHz,10*MHz,1*MHz  ,Orientation(AngleAxisd(PI/4,Vector3d(0.5,0.5,0)))),
										Interaction::SHIELDING,spiny);
	Interaction* inter3=new Interaction(Eigenvalues(1*MHz,1*MHz,10*MHz  ,Orientation(AngleAxisd(PI/4,Vector3d(0  ,0  ,1)))),
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
    rootFrame->AddChild(new Frame(Vector3d(1*Angstroms,0,0),Orientation(AngleAxisd(2.0,Vector3d(1,2,3)))));
	//Rotation about the x axis

	Frame* withChildrenAndParent = new Frame(Vector3d(1*Angstroms,0,0),Orientation(EulerAngles(sqrt(0.5),sqrt(0.5),0)));

	spin0->SetPreferedFrame(withChildrenAndParent);
	spiny->SetPreferedFrame(withChildrenAndParent);

	inter2->SetPreferedFrame(withChildrenAndParent);

	rootFrame->AddChild(withChildrenAndParent);
	withChildrenAndParent->AddChild(new Frame(Vector3d(4*Angstroms,4*Angstroms,0),Orientation(EulerAngles(1,0.2,0.5))));

	ss->SaveToFile("test_out/synthetic.xml",&gXMLLoader);

	SpinSystem* LoadedSpinSystem = new SpinSystem;
	LoadedSpinSystem->LoadFromFile("test_out/synthetic.xml",&gXMLLoader);
	LoadedSpinSystem->SaveToFile("test_out/synthetic2.xml",&gXMLLoader);
	

	SAFE_DELETE(LoadedSpinSystem);
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
