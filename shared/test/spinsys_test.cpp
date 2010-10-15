
#define BOOST_TEST_MODULE spinsys
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <shared/test/common.hpp>
#include <shared/spinsys.hpp>
#include <shared/spin.hpp>
#include <vector>
#include <shared/nuclear_data.hpp>

using namespace sigc;
using namespace std;
using namespace SpinXML;

struct Setup {
    Setup()
        : blank(new SpinSystem), populated(new SpinSystem),
          newSpinCount(0),reloadedCount(0),dyingCount(0) {
        //Distribute the spins on a regular grid on a 5x5x5 angstrom cube
        for(long i=0;i<5;i++) {
            for(long j=0;j<5;j++) {
                for(long k=0;k<5;k++) {
                    Spins.push_back(new Spin(Vector3d(i*1e-10,j*1e-10,k*1e-10),"Hydrogen",1));
                }
            }
        }
        for(unsigned long i=0;i<Spins.size();i++) {
            populated->InsertSpin(Spins[i]);
        }

        //Link up the 
        blank->sigNewSpin.connect(mem_fun(this,&Setup::onNewSpinBlank));
        blank->sigReloaded.connect(mem_fun(this,&Setup::onReloaded));
        blank->sigDying.connect(mem_fun(this,&Setup::onDyingBlank));
        
        static bool firstTime=true;
        if(firstTime) {
            LoadIsotopes();
            firstTime=false;
        }
    }
    ~Setup() {
        SAFE_DELETE(blank);
        SAFE_DELETE(populated);
    }

    SpinSystem* blank;
	
    SpinSystem* populated;
    vector<Spin*> Spins;
    vector<Interaction*> Inters;

    long newSpinCount;
    void onNewSpinBlank(Spin*,long) {
        newSpinCount++;
    }

    long reloadedCount;
    void onReloaded() {
        reloadedCount++;
    }

    long dyingCount;
    void onDyingBlank() {
        dyingCount++;
    }

};


//Tests InsertSpin,sigNewSpin,GetSpinCount,GetSpin(long),GetSpinNumber(Spin*)
BOOST_FIXTURE_TEST_CASE( InsertSpin, Setup ) {
    Spin* spin = new Spin(Vector3d(),"Test Hydrogen",1);
    blank->InsertSpin(spin);
    BOOST_CHECK_EQUAL(newSpinCount,1);
    BOOST_CHECK_EQUAL(blank->GetSpinCount(),1);
    BOOST_CHECK_EQUAL(blank->GetSpin(0),spin);
    BOOST_CHECK_EQUAL(blank->GetSpinNumber(spin),0);
}

BOOST_FIXTURE_TEST_CASE( RemoveSpinN, Setup) {
    //Removing spin 5 should leave 4 invarient but move 6 back to 5
    Spin* spin4 = populated->GetSpin(4);
    Spin* spin5 = populated->GetSpin(5);
    Spin* spin6 = populated->GetSpin(6);
	
    Spin* removed=populated->RemoveSpin(5);
    BOOST_CHECK_EQUAL(removed,spin5);

    BOOST_CHECK_EQUAL(populated->GetSpin(4),spin4);
    BOOST_CHECK_EQUAL(populated->GetSpin(5),spin6);
    
    delete removed;
}

BOOST_FIXTURE_TEST_CASE( RemoveSpinP, Setup) {
    //Repate the RemoveSpinN with RemoveSpin(Spin*)
    //Removing spin 5 should leave 4 invarient but move 6 back to 5
    Spin* spin4 = populated->GetSpin(4);
    Spin* spin5 = populated->GetSpin(5);
    Spin* spin6 = populated->GetSpin(6);
	
    Spin* removed=populated->RemoveSpin(spin5);
    BOOST_CHECK_EQUAL(removed,spin5);    

    BOOST_CHECK_EQUAL(populated->GetSpin(4),spin4);
    BOOST_CHECK_EQUAL(populated->GetSpin(5),spin6);

    delete removed;
}


BOOST_FIXTURE_TEST_CASE( NuclearDipoleDipole, Setup) {
    //Insert two hydrogens a distance of 1 Angstrom appart
    blank->InsertSpin(new Spin(Vector3d(0,0,0),"Hydrogen 1",1));
    blank->InsertSpin(new Spin(Vector3d(1e-10,0,0),"Hydrogen 2",1));
    blank->CalcNuclearDipoleDipole();

    BOOST_CHECK(false); //Test not finished!
    BOOST_CHECK_CLOSE(1.0,1.0,0.001);
}


BOOST_FIXTURE_TEST_CASE( DeleteSpin, Setup ) {
    //Deleting a spin should remove it from a spin system automatically
    long count1 = populated->GetSpinCount();

    Spin* spin4 = populated->GetSpin(4);
    delete spin4;

    long count2 = populated->GetSpinCount();

    BOOST_CHECK_EQUAL(count1,count2);
}



