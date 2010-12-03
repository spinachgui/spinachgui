
#include <fstream>

#include <shared/formats/pdb.hpp>

#define BOOST_SYSTEM_NO_DEPRECATED

#include <boost/spirit/include/support_istream_iterator.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/version.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

#include <shared/nuclear_data.hpp>
#include <shared/spinsys.hpp>

#include <shared/basic_math.hpp>

using namespace boost::spirit::qi;

using namespace std;
using namespace SpinXML;

template <typename Expr, typename Iterator = spirit::unused_type>
struct attribute_of_parser
{
    typedef typename spirit::result_of::compile<
        spirit::qi::domain, Expr
    >::type parser_expression_type;
 
    typedef typename spirit::traits::attribute_of<
        parser_expression_type, spirit::unused_type, Iterator
    >::type type;
};

template <typename T>
void display_attribute_of_parser(T const&)
{
    typedef typename attribute_of_parser<T>::type attribute_type;
    std::cout << typeid(attribute_type).name() << std::endl;
}
 


/*
struct element_parser_type : symbols<unsigned> {
  element_parser_type() {
      for(long i=0;i<getElementCount();i++) {
          add(getElementSymbol(i),i);
      }
    }

} element_p;
*/


///Important note: the global pdb_p parser is not thread safe (in
///case it ever becomes relevent)

void P(std::string str) {
	cout << str << endl;
}

template<typename Iterator>
struct pdb : grammar<Iterator> {
	pdb() : pdb::base_type(file) {
		//Grammer definition here
		validChar %=
			alnum      | char_('`') | char_('-') | char_('=') |
			char_('[') | char_(']') | char_('\\')| char_(';') |
			char_('\'')| char_(',') | char_('.') | char_('/') |
			char_('~') | char_('!') | char_('@') | char_('#') |
			char_('$') | char_('%') | char_('^') | char_('&') |
			char_('*') | char_('(') | char_(')') | char_('_') | 
			char_('+') | char_('{') | char_('}') | char_('|') |
			char_(':') | char_('"') | char_('<') | char_('>') |
			char_('?');
		validCharS %= validChar | char_(" ");
		element = validChar | (validChar >> validChar);
		residue = validChar >> validChar >> validChar;
		vector = *char_(" ") >> double_ >> *char_(" ") >> double_ >> *char_(" ") >> double_ >> *char_(" ");
		atomName = validCharS >> validCharS >> validCharS >> validCharS;

		uninterestingRecordName = (validCharS >> validCharS >> validCharS >> validCharS >> validCharS >> validCharS) - lit("ATOM  ");
		otherRecord %= uninterestingRecordName >> *validCharS >> eol;
	
		atomRecord =
			lit("ATOM  ") >> *char_(" ") >>
			int_ >> char_(" ") >>
			atomName >> //Atom Name
			omit[validCharS >> //Alternate location indicator (can be blank)
			residue >> char_(" ") >> //Residue name
			validChar >> *char_(" ") >> //Chain identifier
			int_ >>      //Residue sequence number.
			validCharS] >> //Code for insertion of residues
			vector >>   //XYZ coordinates
			double_ >> *char_(" ") >> double_ >> *char_(" ") >> //occupancy and Temperature factor
			element >>
			*validCharS >>
			eol;
		//file = *(atomRecord[cout << "End of Record " << _1 << endl] | otherRecord) >> eoi;
		file = *(otherRecord | atomRecord) >> eoi;
	}

	//List of rules
	rule<Iterator> file;
	rule<Iterator,std::string()> uninterestingRecordName; /*Consumes the first 6 characters
															of input after checking it's not
															a recond type we are interesting
															in */
	rule<Iterator> atomRecord;
	rule<Iterator,std::string()> otherRecord;
	rule<Iterator,char()> validChar;
	rule<Iterator,char()> validCharS;
	rule<Iterator> element;
	rule<Iterator> residue;
	rule<Iterator> vector;
	rule<Iterator> atomName;

	void print_tree() const {
		
	}
};

void PDBLoader::LoadFile(SpinSystem* ss,const char* filename) const {

    ss->Clear();

	ifstream fin(filename);
	if(!fin.is_open()) {
		throw runtime_error("Couldn't open file");
	}
	fin.unsetf(std::ios::skipws);

	spirit::istream_iterator begin(fin);
	spirit::istream_iterator end;

    if(!fin) {
        throw runtime_error("Couldn't open file");
    }

	pdb<spirit::istream_iterator> pdb_parser;

    if(parse(begin,end,pdb_parser)) {
		cout << "Parsing suceeded" << endl;
    } else {
        throw runtime_error("Parsing failed.");
    }

    ss->sigReloaded();
}


