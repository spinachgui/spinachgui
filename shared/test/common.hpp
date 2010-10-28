#ifndef TEST_COMMON_H_
#define TEST_COMMON_H_

#include <boost/test/unit_test.hpp>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <shared/spin.hpp>
#include <shared/interaction.hpp>

using namespace std;
using namespace SpinXML;

boost::test_tools::predicate_result
boundedCompaire(double n1,double n2,double range,double tolarance)
{
    if(fabs((n1-n2)/range) > (tolarance/100)) {
        boost::test_tools::predicate_result res(false);
        res.message() << n1 << " !~= " << n2;
        return res;
    }
    return true;
}

#define SAFE_DELETE(obj)                        \
    if((obj)!=NULL) {                           \
        delete obj;                             \
    }

#define CHECK_MATRIX_CLOSE(obj)                                 \
    BOOST_CHECK(boundedCompaire(m(0,0),(obj)(0,0),2,0.01));	\
    BOOST_CHECK(boundedCompaire(m(0,1),(obj)(0,1),2,0.01));	\
    BOOST_CHECK(boundedCompaire(m(0,2),(obj)(0,2),2,0.01));	\
    BOOST_CHECK(boundedCompaire(m(1,0),(obj)(1,0),2,0.01));	\
    BOOST_CHECK(boundedCompaire(m(1,1),(obj)(1,1),2,0.01));	\
    BOOST_CHECK(boundedCompaire(m(1,2),(obj)(1,2),2,0.01));	\
    BOOST_CHECK(boundedCompaire(m(2,0),(obj)(2,0),2,0.01));	\
    BOOST_CHECK(boundedCompaire(m(2,1),(obj)(2,1),2,0.01));	\
    BOOST_CHECK(boundedCompaire(m(2,2),(obj)(2,2),2,0.01));			


double getRandomDouble() {
    return double(rand())/double(RAND_MAX);
}

struct spin_sig_counter {
    spin_sig_counter(Spin* spin)
        : mSpin(spin),
          changes(0),
          died(false) {
        this->mDyingConnect= mSpin->sigDying.connect(sigc::mem_fun(this, &spin_sig_counter::onDie));
        this->mChangeConnect=mSpin->sigChange.connect(sigc::mem_fun(this,&spin_sig_counter::onChange));
    }
    ~spin_sig_counter() {
        mDyingConnect.disconnect();
        mChangeConnect.disconnect();
    }
    void onDie(Spin* inter) {
        died=true;
        mDyingConnect.disconnect();
        mChangeConnect.disconnect();
    }
    void onChange() {
        changes++;
    }
    Spin* mSpin;
    long changes;
    bool died;
    sigc::connection mChangeConnect;
    sigc::connection mDyingConnect;
};

struct inter_sig_counter {
    inter_sig_counter(Interaction* inter) :
        mInter(inter),
        changes(0),
        RemovedSpins(0),
        died(false) {
        this->mDyingConnect=inter->sigDying.connect(sigc::mem_fun(this,&inter_sig_counter::onDie));
        this->mChangeConnect=inter->sigChange.connect(sigc::mem_fun(this,&inter_sig_counter::onChange));
        this->mRemoveSpinConnect=inter->sigRemoveSpin.connect(sigc::mem_fun(this,&inter_sig_counter::onRemoveSpin));
    }
    ~inter_sig_counter() {
        mRemoveSpinConnect.disconnect();
        mChangeConnect.disconnect();
        mDyingConnect.disconnect();
    }
    void onDie(Interaction* inter) {
        died=true;
        mRemoveSpinConnect.disconnect();
        mChangeConnect.disconnect();
        mDyingConnect.disconnect();
    }
    void onChange() {
        changes++;
    }
    void onRemoveSpin(Interaction*,Spin*) {
        RemovedSpins++;
    }
    Interaction* mInter;
    long changes;
    long RemovedSpins;
    bool died;
    sigc::connection mRemoveSpinConnect;
    sigc::connection mChangeConnect;
    sigc::connection mDyingConnect;
};



#endif
