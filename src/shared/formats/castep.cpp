
#include <shared/formats/castep.hpp>
#include <shared/nuclear_data.hpp>

#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_symbols.hpp>

#include <fstream>
#include <iostream>

using namespace BOOST_SPIRIT_CLASSIC_NS;

using namespace std;
using namespace SpinXML;


struct element_parser_type : symbols<unsigned> {
    /* 
       Just for fun, try renaming this class to element. No need to
       change the element_p bit at the end. Go on, try it. When you
       run the program with the renamed class it segfaults.

       I have no idea what is going on (maybe something is being
       overwritten?).

       And no, I am not making this up.
    */
    element_parser_type()
    {
        add("H",1);
        add("C",6);
        add("F",9);
    }

} element_p;

void on_match(const __gnu_cxx::__normal_iterator<char*, string >,
              const __gnu_cxx::__normal_iterator<char*, string >) {
    cout << "Something was matched" << endl;
}

struct castep : grammar<castep> {
    template <typename ScannerT>
    struct definition {
        definition(castep const& self) {
            header = str_p("============")[&on_match] >>
                str_p("Atom:") >> element_p >> int_p >>
                str_p("============");

            coord_block = element_p >> int_p >> str_p("Coordinates") >> real_p >> real_p >> real_p >> str_p("A");

            shielding_block=str_p("TOTAL") >> "Shielding" >> str_p("Tensor") >>
                real_p >> real_p >> real_p >>
                real_p >> real_p >> real_p >>
                real_p >> real_p >> real_p;

            eigen_block = element_p >> int_p >> str_p("Eigenvalue") >> element_p >> real_p >> str_p("(ppm)") >>
                element_p >> int_p >> str_p("Eigenvector") >> element_p >> real_p >> real_p >> real_p;

            eigenvalues_block = eigen_block >> eigen_block >> eigen_block;

            anisotropy_block = element_p >> int_p >> str_p("Isotropic:") >> real_p >> str_p("(ppm)") >>
                element_p >> int_p >> str_p("Anisotropy:") >> real_p >> str_p("(ppm)") >>
                element_p >> int_p >> str_p("Asymmetry:") >> real_p;

            atom = header >> coord_block;// >> shielding_block >> eigenvalues_block >> anisotropy_block;

            file=*atom;
        }
        rule<ScannerT> const& start() {return file;}
        rule<ScannerT> header;
        rule<ScannerT> coord_block;
        rule<ScannerT> shielding_block;
        rule<ScannerT> eigenvalues_block;
        rule<ScannerT> anisotropy_block;

        rule<ScannerT> eigen_block;

        rule<ScannerT> atom;
        
        rule<ScannerT> file;
    };
} castep_p;


void CASTEPLoader::LoadFile(SpinSystem* ss,const char* filename) const {

    ss->Clear();
    ifstream fin(filename);
    if(!fin.is_open()) {
        throw runtime_error("Couldn't open file");
    }

    // create reader objects
    string str;
    istream_iterator<string> it(fin);
    istream_iterator<string> it_end;

    while( it != it_end ) {
        str += *it;
        it++;
    }
    str="============\n"
        "Atom: C    1\n"
        "============\n"
        "C    1 Coordinates     -0.484    0.440    0.226   A\n";


    if(parse(str.begin(),str.end(),castep_p,space_p).full) {
        cout << "Parse Okay" << endl;
    } else {
        cout << "Parse error" << endl;
    }

    ss->sigReloaded();
}

