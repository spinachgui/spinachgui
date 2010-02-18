
#include <shared/formats/castep.hpp>
#include <shared/nuclear_data.hpp>
#include <boost/spirit/include/qi.hpp>
#include <fstream>

using namespace boost::spirit::qi;
using namespace boost::spirit;
using namespace boost::spirit::ascii;
using namespace boost::spirit::arg_names;


using namespace std;
using namespace SpinXML;

template <typename Iterator>
bool parse_castep(Iterator first, Iterator last) {
    bool ret = phrase_parse(
                            first,
                            last,
                            double_ >> *(',' >> double_),
                            space
                            );
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
  
    string s="1,2.0,3,4";
    parse_castep(s.begin(),s.end());

    ss->sigReloaded();
}

