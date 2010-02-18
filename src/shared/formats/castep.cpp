
#include <shared/formats/castep.hpp>
#include <shared/nuclear_data.hpp>
#include <boost/spirit/include/qi.hpp>
#include <fstream>
#include <iostream>

using namespace boost::spirit::qi;
using namespace boost::spirit;
using namespace boost::spirit::ascii;
using namespace boost::spirit::arg_names;


using namespace std;
using namespace SpinXML;


template <typename Iterator>
bool parse_castep(Iterator first, Iterator last) {

    /*struct atom : grammar<Iterator> {
        atom() : atom::base_type(r) {
            r=*lit("atom");
        }
        rule<Iterator> r;
        };*/
    struct castep : grammar<Iterator> {
        castep() : castep::base_type(r) {
            r=*lit("atom,");
        }
        rule<Iterator> r;
    };


    unused_type result;
    castep c;
    bool ret = parse(first,last,c.start);
    if(first != last) {
        return false;
    }
    return ret;
}

void CASTEPLoader::LoadFile(SpinSystem* ss,const char* filename) const {

    ss->Clear();
    ifstream fin(filename);
    if(!fin.is_open()) {
        throw runtime_error("Couldn't open file");
    }
  
    string s="atom,atom,atom,";
    if(parse_castep(s.begin(),s.end())) {
        cout << "Parse Okay" << endl;
    } else {
        cout << "Parse error" << endl;
    }

    ss->sigReloaded();
}

