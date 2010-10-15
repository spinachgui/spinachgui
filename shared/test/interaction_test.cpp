#define BOOST_TEST_MODULE raw_interactions
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <shared/test/common.hpp>
#include <shared/interaction.hpp>
#include <stdexcept>

using namespace SpinXML;

struct Setup {
    Setup() :
        o(Quaterniond(1/sqrt(2.0),1/sqrt(2.0),0,0)),

        scalar(20.0*MHz),
		
        m(MakeMatrix3d(1.0,0.0,0.0,
                       0.0,1.0,0.0,
                       0.0,0.0,1.0)),

        xx(10.0*MHz),
        yy(8.0*MHz),
        zz(4.0*MHz),
        ev(xx,yy,zz,o),

        ar_iso(20.0*MHz),
        ax(20.0*MHz),
        rh(15.0*MHz),
        ar(ar_iso,ax,rh,o),

        ss_iso(20.0*MHz),
        span(10.0*MHz),
        skew(0.5),
        ss(ss_iso,span,skew,o),
        
        spin1(new Spin(Vector3d(0,0,0),"Spin 1",1)),
        spin2(new Spin(Vector3d(1,1,1),"Spin 2",1)),

        storage_scalar(new Interaction(scalar,Interaction::DIPOLAR,    spin1,spin2)),
        storage_matrix(new Interaction(m     ,Interaction::SHIELDING  ,spin1)),
        storage_ev(    new Interaction(ev    ,Interaction::QUADRUPOLAR,spin1)),
        storage_ar(    new Interaction(ar,    Interaction::QUADRUPOLAR,spin1)),
        storage_ss(    new Interaction(ss,    Interaction::QUADRUPOLAR,spin1)),

        type_HFC             (new Interaction(scalar,Interaction::HFC			   ,spin1,spin2)),
        type_G_TENSER        (new Interaction(scalar,Interaction::G_TENSER		   ,spin1)),			   
        type_ZFS             (new Interaction(scalar,Interaction::ZFS			   ,spin1)),			   
        type_EXCHANGE        (new Interaction(scalar,Interaction::EXCHANGE		   ,spin1,spin2)),		   
        type_SHIELDING       (new Interaction(scalar,Interaction::SHIELDING 	   ,spin1)),	   
        type_SCALAR          (new Interaction(scalar,Interaction::SCALAR		   ,spin1,spin2)),		   
        type_QUADRUPOLAR     (new Interaction(scalar,Interaction::QUADRUPOLAR	   ,spin1)),	   
        type_DIPOLAR         (new Interaction(scalar,Interaction::DIPOLAR		   ,spin1,spin2)),		   
        type_CUSTOM_LINEAR   (new Interaction(scalar,Interaction::CUSTOM_LINEAR    ,spin1)),   
        type_CUSTOM_BILINEAR (new Interaction(scalar,Interaction::CUSTOM_BILINEAR  ,spin1,spin2)), 
        type_CUSTOM_QUADRATIC(new Interaction(scalar,Interaction::CUSTOM_QUADRATIC ,spin1))	{
    }
    ~Setup() {
        SAFE_DELETE(spin1);
        SAFE_DELETE(spin2);

        SAFE_DELETE(storage_scalar);
        SAFE_DELETE(storage_matrix);
        SAFE_DELETE(storage_ev);
        SAFE_DELETE(storage_ar);
        SAFE_DELETE(storage_ss);
		
        SAFE_DELETE(type_HFC);        
        SAFE_DELETE(type_G_TENSER);        
        SAFE_DELETE(type_ZFS);             
        SAFE_DELETE(type_EXCHANGE);        
        SAFE_DELETE(type_SHIELDING);       
        SAFE_DELETE(type_SCALAR);          
        SAFE_DELETE(type_QUADRUPOLAR);     
        SAFE_DELETE(type_DIPOLAR);         
        SAFE_DELETE(type_CUSTOM_LINEAR);   
        SAFE_DELETE(type_CUSTOM_BILINEAR); 
        SAFE_DELETE(type_CUSTOM_QUADRATIC);
    }
    Orientation o;

    energy scalar;

    Matrix3d m;

    energy xx,yy,zz;
    Eigenvalues ev;

    energy ar_iso,ax,rh;
    AxRhom ar;

    energy ss_iso,span;
    double skew;
    SpanSkew ss;

    Spin* spin1;
    Spin* spin2;

    Interaction* storage_scalar;
    Interaction* storage_matrix;
    Interaction* storage_ev;
    Interaction* storage_ar;
    Interaction* storage_ss;

    Interaction* type_HFC;			   
    Interaction* type_G_TENSER;		   
    Interaction* type_ZFS;			   
    Interaction* type_EXCHANGE;		   
    Interaction* type_SHIELDING;	   
    Interaction* type_SCALAR;		   
    Interaction* type_QUADRUPOLAR;	   
    Interaction* type_DIPOLAR;		   
    Interaction* type_CUSTOM_LINEAR;   
    Interaction* type_CUSTOM_BILINEAR; 
    Interaction* type_CUSTOM_QUADRATIC;
};

BOOST_FIXTURE_TEST_CASE( InteractionCtor, Setup) {
    BOOST_CHECK_EQUAL(storage_scalar->GetStorage(),Interaction::STORAGE_SCALAR);
    BOOST_CHECK_EQUAL(storage_matrix->GetStorage(),Interaction::MATRIX);
    BOOST_CHECK_EQUAL(storage_ev->GetStorage()    ,Interaction::EIGENVALUES);
    BOOST_CHECK_EQUAL(storage_ar->GetStorage()    ,Interaction::AXRHOM);
    BOOST_CHECK_EQUAL(storage_ss->GetStorage()   ,Interaction::SPANSKEW);
}


BOOST_FIXTURE_TEST_CASE( InteractionIsType, Setup) {
    energy en = 1.0*Joules;
    BOOST_CHECK(type_HFC			 ->IsType(Interaction::HFC			    ) );
    BOOST_CHECK(type_G_TENSER		 ->IsType(Interaction::G_TENSER		) );
    BOOST_CHECK(type_ZFS			 ->IsType(Interaction::ZFS		    	) );
    BOOST_CHECK(type_EXCHANGE		 ->IsType(Interaction::EXCHANGE		) );
    BOOST_CHECK(type_SHIELDING   	 ->IsType(Interaction::SHIELDING   	) );
    BOOST_CHECK(type_SCALAR		     ->IsType(Interaction::SCALAR		    ) );
    BOOST_CHECK(type_QUADRUPOLAR	 ->IsType(Interaction::QUADRUPOLAR	    ) );
    BOOST_CHECK(type_DIPOLAR		 ->IsType(Interaction::DIPOLAR		    ) );
    BOOST_CHECK(type_CUSTOM_LINEAR   ->IsType(Interaction::CUSTOM_LINEAR   ) );
    BOOST_CHECK(type_CUSTOM_BILINEAR ->IsType(Interaction::CUSTOM_BILINEAR ) );
    BOOST_CHECK(type_CUSTOM_QUADRATIC->IsType(Interaction::CUSTOM_QUADRATIC) );
}									   


//Check that exceptions are thrown for invalid types
BOOST_FIXTURE_TEST_CASE( CorrectFormFromType, Setup) {
    energy en = 1.0*Joules;
    BOOST_CHECK(type_HFC			 ->GetForm()==Interaction::BILINEAR);
    BOOST_CHECK(type_G_TENSER		 ->GetForm()==Interaction::LINEAR);
    BOOST_CHECK(type_ZFS			 ->GetForm()==Interaction::QUADRATIC);
    BOOST_CHECK(type_EXCHANGE		 ->GetForm()==Interaction::BILINEAR);
    BOOST_CHECK(type_SHIELDING   	 ->GetForm()==Interaction::LINEAR);
    BOOST_CHECK(type_SCALAR		     ->GetForm()==Interaction::BILINEAR);
    BOOST_CHECK(type_QUADRUPOLAR	 ->GetForm()==Interaction::QUADRATIC);
    BOOST_CHECK(type_DIPOLAR		 ->GetForm()==Interaction::BILINEAR);
    BOOST_CHECK(type_CUSTOM_LINEAR   ->GetForm()==Interaction::LINEAR);
    BOOST_CHECK(type_CUSTOM_BILINEAR ->GetForm()==Interaction::BILINEAR);
    BOOST_CHECK(type_CUSTOM_QUADRATIC->GetForm()==Interaction::QUADRATIC);
}									   


//Check that exceptions are thrown for invalid types
BOOST_FIXTURE_TEST_CASE( InvalidType, Setup) {
    energy en = 1.0*Joules;
    BOOST_CHECK_THROW(new Interaction(en,Interaction::HFC,		      	spin1      ),runtime_error);
    BOOST_CHECK_THROW(new Interaction(en,Interaction::G_TENSER,		    spin1,spin2),runtime_error);
    BOOST_CHECK_THROW(new Interaction(en,Interaction::ZFS,			  	spin1,spin2),runtime_error);
    BOOST_CHECK_THROW(new Interaction(en,Interaction::EXCHANGE,		  	spin1      ),runtime_error);
    BOOST_CHECK_THROW(new Interaction(en,Interaction::SHIELDING,	  	spin1,spin2),runtime_error);
    BOOST_CHECK_THROW(new Interaction(en,Interaction::SCALAR,   	  	spin1      ),runtime_error);
    BOOST_CHECK_THROW(new Interaction(en,Interaction::QUADRUPOLAR,	    spin1,spin2),runtime_error);
    BOOST_CHECK_THROW(new Interaction(en,Interaction::DIPOLAR,		    spin1      ),runtime_error);
    BOOST_CHECK_THROW(new Interaction(en,Interaction::CUSTOM_LINEAR,  	spin1,spin2),runtime_error);
    BOOST_CHECK_THROW(new Interaction(en,Interaction::CUSTOM_BILINEAR,  spin1      ),runtime_error);
    BOOST_CHECK_THROW(new Interaction(en,Interaction::CUSTOM_QUADRATIC, spin1,spin2),runtime_error);
}

BOOST_FIXTURE_TEST_CASE( SetAndGetMatrix, Setup) {
    type_HFC->SetMatrix(m);
    BOOST_CHECK_EQUAL(type_HFC->GetStorage(),Interaction::MATRIX);
    Matrix3d m2;
    type_HFC->GetMatrix(&m2);
    BOOST_CHECK_EQUAL(m,m2);
};

BOOST_FIXTURE_TEST_CASE( SetAndGetEV, Setup) {
    type_HFC->SetEigenvalues(xx,yy,zz,o);
    BOOST_CHECK_EQUAL(type_HFC->GetStorage(),Interaction::EIGENVALUES);
    energy xx2,yy2,zz2; Orientation o2(EulerAngles(0,0,0));
    type_HFC->GetEigenvalues(&xx2,&yy2,&zz2,&o2);
    BOOST_CHECK_EQUAL(xx.si,xx2.si);
    BOOST_CHECK_EQUAL(yy.si,yy2.si);
    BOOST_CHECK_EQUAL(zz.si,zz2.si);
    BOOST_CHECK(o==o2);
};

BOOST_FIXTURE_TEST_CASE( SetAndGetAX, Setup) {
    type_HFC->SetAxRhom(ar_iso,ax,rh,o);
    BOOST_CHECK_EQUAL(type_HFC->GetStorage(),Interaction::AXRHOM);
    energy ar_iso2,ax2,rh2; Orientation o3(EulerAngles(0,0,0));
    type_HFC->GetAxRhom(&ar_iso2,&ax2,&rh2,&o3);
    BOOST_CHECK_EQUAL(ar_iso.si,ar_iso2.si);
    BOOST_CHECK_EQUAL(ax.si,ax2.si);
    BOOST_CHECK_EQUAL(rh.si,rh2.si);
    BOOST_CHECK(o==o3);
};

BOOST_FIXTURE_TEST_CASE( SetAndGetSS, Setup) {
    type_HFC->SetSpanSkew(ss_iso,span,skew,o);
    BOOST_CHECK_EQUAL(type_HFC->GetStorage(),Interaction::SPANSKEW);
    energy ss_iso2,span2; double skew2; Orientation o4(EulerAngles(0,0,0));
    type_HFC->GetSpanSkew(&ss_iso2,&span,&skew,&o4);
    BOOST_CHECK_EQUAL(ss_iso.si,ss_iso2.si);
    BOOST_CHECK_EQUAL(span.si,span2.si);
    BOOST_CHECK_EQUAL(skew,skew2);
    BOOST_CHECK(o==o4);
};

BOOST_FIXTURE_TEST_CASE( SetAndGetScalar, Setup) {
    type_HFC->SetMatrix(m); //Already set to scalar, so we should change that before testing.
    type_HFC->SetScalar(scalar);
    BOOST_CHECK_EQUAL(type_HFC->GetStorage(),Interaction::STORAGE_SCALAR);
    energy scalar2;
    type_HFC->GetScalar(&scalar2);
    BOOST_CHECK_EQUAL(scalar.si,scalar2.si);
};


struct inter_sig_counter {
    inter_sig_counter(Interaction* inter) : mInter(inter), died(false) {
        this->mDyingConnect=inter->sigDying.connect(mem_fun(this,&inter_sig_counter::onDie));
    }
    ~inter_sig_counter() {
        mDyingConnect.disconnect();
    }
    void onDie(Interaction* inter) {
        died=true;
        mDyingConnect.disconnect();
    }
    Interaction* mInter;
    bool died;
    sigc::connection mDyingConnect;
};

BOOST_FIXTURE_TEST_CASE( DeleteSpin, Setup ) {
    //Deleting a spin should also cause the any attached interactions
    //to delete themselves
    inter_sig_counter watcher(type_HFC);
    delete spin1;
    spin1=NULL;
    BOOST_CHECK(watcher.died);
}


