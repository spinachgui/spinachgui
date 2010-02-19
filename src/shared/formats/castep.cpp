
#include <shared/formats/castep.hpp>
#include <shared/nuclear_data.hpp>

#include <boost/spirit/include/classic_core.hpp>

#include <fstream>
#include <iostream>

using namespace BOOST_SPIRIT_CLASSIC_NS;

using namespace std;
using namespace SpinXML;



/*struct elements : symbols<char,long> {
    elements() {
        add("H",1);
        add("C",6);
        add("F",9);
    }

    } elements_;*/


template <typename Iterator>
bool parse_castep(Iterator first, Iterator last) {

    /*struct atom : grammar<Iterator> {
        atom() : atom::base_type(r) {
            r=*lit("atom");
        }
        rule<Iterator> r;
        };*/
    /*   struct castep : grammar<Iterator> {
        castep() : castep::base_type(atom) {
            

            //file=*atom;
                
            // atom = header;// >> coord_block >> shielding_block >> eigenvalues_block >> anisotropy_block;
            
            //header = lit("============") >>
            //     lit("Atom:") >> elements_ >> int_ >>
            //     lit("============");

            /*coord_block = elements >> int_ >> lit("Coordinates") >> float_ >> float_ >> float_ >> lit("A");

            shielding_block=lit("TOTAL") >> "Shielding" >> lit("Tensor") >>
                float_ >> float_ >> float_ >>
                float_ >> float_ >> float_ >>
                float_ >> float_ >> float_;

            eigen_block = elements >> int_ >> lit("Eigenvalue") >> elements >> float_ >> lit("(ppm)") >>
                elements >> int_ >> lit("Eigenvector") >> elements >> float_ >> float_ >> float_;

            eigenvalues_block = eigen_block >> eigen_block >> eigen_block;

            anisotropy_block = elements >> int_ >> lit("Isotropic:") >> float_ >> lit("(ppm)") >>
                elements >> int_ >> lit("Anisotropy:") >> float_ >> lit("(ppm)") >>
                elements >> int_ >> lit("Asymmetry:") >> float_;

        }
        rule<Iterator> header;
        //rule<Iterator> coord_block;
        //rule<Iterator> shielding_block;
        //rule<Iterator> eigenvalues_block;
        //rule<Iterator> anisotropy_block;

        //rule<Iterator> eigen_block;

        rule<Iterator> atom;
        
        //rule<Iterator> file;
    };

    */
    //unused_type result;
    //castep c;
    //bool ret = parse(first,last,c.start);
    if(first != last) {
        return false;
    }
    return false;
}

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

    if(parse_castep(str.begin(),str.end())) {
        cout << "Parse Okay" << endl;
    } else {
        cout << "Parse error" << endl;
    }

    ss->sigReloaded();
}

