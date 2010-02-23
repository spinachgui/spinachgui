
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

typedef file_iterator<char>                                               iter_t;
typedef tree_match<iter_t>                                                parse_tree_match_t;
typedef parse_tree_match_t::const_tree_iterator                           tree_iter_t;

//typedef ast_match_policy<iter_t>                                          match_policy_t;
//typedef scanner_policies<iteration_policy, match_policy_t, action_policy> scanner_policy_t;
//typedef scanner<iter_t, scanner_policy_t>                                 scanner_t;
typedef rule<>                                                            rule_t;


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

struct castep : grammar<castep> {
    ///These IDs label all the places on interest in the parse tree
    enum {
        total_shielding_tensorID,
        total_tensor
    };
    void process_tree(tree_iter_t tree,string indent="") {
        cout << indent << "process_tree(" 
             << string(tree->value.begin(), tree->value.end()) << ")"<< endl;
        for(tree_iter_t j=tree->children.begin();j!=tree->children.end();++j) {
            process_tree(j,indent+"   ");
        }
    }

    template <typename ScannerT>
    struct definition {
        rule<ScannerT> const& start() {return file;}
#define DECLARE_RULE(name,id) rule<ScannerT, parser_context<>,parser_tag<name##id> > name;
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
        rule<ScannerT> total_tensor;
        rule<ScannerT> quadrupole_block;
        
        rule<ScannerT> file;
#undef DECLARE_RULE


        definition(castep const& self) {

            element_index =
                element_p >> int_p;

            header = 
                lexeme_d[+str_p("=")] >>
                str_p("Atom:") >> element_index >>
                lexeme_d[+str_p("=")];

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

            anisotropy_block =
                element_index >> str_p("Isotropic:") >> real_p >> str_p("(ppm)") >>
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
                lexeme_d[+str_p("=")] >>
                str_p("Using") >> str_p("the") >> str_p("following") >> str_p("values") >>
                str_p("of") >> str_p("the") >> str_p("electric") >> str_p("quadrupole") >>
                str_p("moment") >>
                element_p >> str_p("User") >> str_p("defined.") >> str_p("Q") >> str_p("=") >> real_p >> str_p("Barn") >>
                element_p >> str_p("User") >> str_p("defined.") >> str_p("Q") >> str_p("=") >> real_p >> str_p("Barn") >>
                lexeme_d[+str_p("=")];
                

            file=*(
                   quadrupole_block |
                   total_shielding_tensor | 
                   total_tensor 
                   ) >> end_p;
        }
    };
}  castep_p;

void CASTEPLoader::LoadFile(SpinSystem* ss,const char* filename) const {

    ss->Clear();

    iter_t fin(filename);
    iter_t end=fin.make_end();
    if(!fin) {
        throw runtime_error("Couldn't open file");
    }

    tree_parse_info<iter_t> result=ast_parse(fin,end,castep_p,space_p);
    if(result.full) {
        castep_p.process_tree(result.trees.begin());
    } else {
        string badtext(result.stop,end);
        if(badtext.length() > 200) {
            badtext=badtext.substr(0,200);
        }
        string error="Syntax error near: \"" + badtext + "\"";
        cout << error << endl;
        throw runtime_error(error);
    }

    ss->sigReloaded();
}

