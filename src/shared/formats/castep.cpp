
#include <shared/formats/castep.hpp>
#include <shared/nuclear_data.hpp>

#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_ast.hpp>
#include <boost/spirit/include/classic_symbols.hpp>
#include <boost/spirit/include/classic_file_iterator.hpp>

#include <fstream>
#include <iostream>

using namespace BOOST_SPIRIT_CLASSIC_NS;

using namespace std;
using namespace SpinXML;

typedef file_iterator<char> itor_t;
typedef tree_match<itor_t>::tree_iterator  tree_itor_t;

struct element_parser_type : symbols<unsigned> {
    /* 
       Just for fun, try renaming this class to element. No need to
       change the element_p bit at the end. Go on, try it. When you
       run the program with the renamed class it segfaults.

       I have no idea what is going on (maybe something is being
       overwritten?).

       And no, I am not making this up.
    */
    element_parser_type() {
        add("H",1);
        add("C",6);
        add("F",9);
    }

} element_p;


struct sigma_parser_type : symbols<unsigned> {
    sigma_parser_type() {
        add("sigma_xx",1);
        add("sigma_yy",2);
        add("sigma_zz",3);
    }

} sigma_p;

struct v_parser_type : symbols<unsigned> {
    v_parser_type() {
        add("V_xx",1);
        add("V_yy",2);
        add("V_zz",3);
    }

} v_xyz_p;


void on_shielding_match(file_iterator<char>,file_iterator<char>) {
    cout << "Sheilding Tensor Block Matched" << endl;
}
void on_total_match(file_iterator<char>,file_iterator<char>) {
    cout << "Total tensor block matched" << endl;
}
void on_quad_match(file_iterator<char>,file_iterator<char>) {
    cout << "quadrupole block matched" << endl;
}
void on_match(file_iterator<char>,file_iterator<char>) {
    cout << "something_was_matched" << endl;
}



struct castep : grammar<castep> {
    template <typename ScannerT>
    struct definition {
        definition(castep const& self) {
            element_index =
                element_p >> int_p;

            header = 
                token_node_d[+str_p("=")] >>
                str_p("Atom:") >> element_index >>
                token_node_d[+str_p("=")];

            coord_block =
                element_index >> str_p("Coordinates") >> real_p >> real_p >> real_p >> str_p("A");

            total_shielding_matrix=str_p("TOTAL") >> "Shielding" >> str_p("Tensor") >>
                real_p >> real_p >> real_p >>
                real_p >> real_p >> real_p >>
                real_p >> real_p >> real_p;

            total_matrix=str_p("TOTAL") >> str_p("tensor") >>
                real_p >> real_p >> real_p >>
                real_p >> real_p >> real_p >>
                real_p >> real_p >> real_p;

            eigen_block_total_shielding =
                element_index >> str_p("Eigenvalue")  >> sigma_p >> real_p >> str_p("(ppm)") >>
                element_index >> str_p("Eigenvector") >> sigma_p >> real_p >> real_p >> real_p;

            eigen_block_total =
                element_index >> str_p("Eigenvalue")  >> v_xyz_p >> real_p >>
                element_index >> str_p("Eigenvector") >> v_xyz_p >> real_p >> real_p >> real_p;


            eigensystem_block_total_shielding = eigen_block_total_shielding >> eigen_block_total_shielding >> eigen_block_total_shielding;
            eigensystem_block_total           = eigen_block_total           >> eigen_block_total           >> eigen_block_total;

            anisotropy_block = element_p >> int_p >> str_p("Isotropic:") >> real_p >> str_p("(ppm)") >>
                element_index >> str_p("Anisotropy:") >> real_p >> str_p("(ppm)") >>
                element_index >> str_p("Asymmetry:") >> real_p;

            cq_eta_block = 
                element_index >> str_p("Cq:") >> real_p >> str_p("(MHz)") >>
                element_index >> str_p("Eta:") >> real_p;

            total_shielding_tensor = 
                header >>
                coord_block >>
                total_shielding_matrix >>
                eigensystem_block_total_shielding >>
                anisotropy_block;

            total_tensor = 
                header >>
                coord_block >>
                total_matrix >>
                eigensystem_block_total >>
                cq_eta_block;

            quadrupole_block = 
                token_node_d[+str_p("=")] >>
                str_p("Using") >> str_p("the") >> str_p("following") >> str_p("values") >>
                str_p("of") >> str_p("the") >> str_p("electric") >> str_p("quadrupole") >>
                str_p("moment") >>
                element_p >> str_p("User") >> str_p("defined.") >> str_p("Q") >> str_p("=") >> real_p >> str_p("Barn") >>
                element_p >> str_p("User") >> str_p("defined.") >> str_p("Q") >> str_p("=") >> real_p >> str_p("Barn") >>
                token_node_d[+str_p("=")];
                

            file=*(
                   total_shielding_tensor | 
                   total_tensor | 
                   quadrupole_block
                   ) >> end_p;
        }
        rule<ScannerT> const& start() {return file;}
        rule<ScannerT> element_index;

        rule<ScannerT> header;
        rule<ScannerT> coord_block;
        rule<ScannerT> total_shielding_matrix;
        rule<ScannerT> total_matrix;
        rule<ScannerT> eigensystem_block_total_shielding;
        rule<ScannerT> eigensystem_block_total;
        rule<ScannerT> anisotropy_block;
        rule<ScannerT> cq_eta_block;


        rule<ScannerT> eigen_block_total_shielding;
        rule<ScannerT> eigen_block_total;

        rule<ScannerT> total_shielding_tensor;
        rule<ScannerT> quadrupole_block;
        rule<ScannerT> total_tensor;
        
        rule<ScannerT> file;
    };
} castep_p;


void process_tree(tree_itor_t tree,string indent="") {
    cout << indent << "process_tree(" 
         << string(tree->value.begin(), tree->value.end()) << ")"<< endl;
    for(tree_itor_t j=tree->children.begin();j!=tree->children.end();++j) {
        process_tree(j,indent+"   ");
    }
}

void CASTEPLoader::LoadFile(SpinSystem* ss,const char* filename) const {

    ss->Clear();
    //ifstream fin(filename);

    itor_t fin(filename);
    itor_t end=fin.make_end();
    if(!fin) {
        throw runtime_error("Couldn't open file");
    }

    tree_parse_info<itor_t> result=ast_parse(fin,end,castep_p,space_p);
    if(result.full) {
        process_tree(result.trees.begin());
    } else if(result.match) {
        string badtext(result.stop,end);
        if(badtext.length() > 40) {
            badtext=badtext.substr(0,40);
        }
        string error="Syntax error near " + badtext;
        throw runtime_error(error);
    } else {
        cout << "Parse Failed" << endl;
        throw runtime_error("Parse Failed, file is corrupt");
    }

    ss->sigReloaded();
}

