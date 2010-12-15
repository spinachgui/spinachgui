
#include <fstream>

#include <shared/formats/pdb.hpp>
#include <shared/nuclear_data.hpp>
#include <shared/spinsys.hpp>

#include <shared/basic_math.hpp>

#include <shared/formats/spirit_common.hpp>
#include <boost/spirit/home/phoenix/object/new.hpp>
#include <boost/bind.hpp>

using namespace std;
using namespace SpinXML;

template<typename Iterator>
struct pdb : qi::grammar<Iterator> {
	pdb(SpinSystem* spinsys) : pdb::base_type(file), ss(spinsys) {
		using qi::blank;
		using qi::alnum;
		using qi::char_;
		using qi::int_;
		using qi::double_;
		using qi::lit;
		using qi::eol;
		using qi::eoi;
		using qi::omit;

		//Grammer definition here

		//There appears to be a bug in charset in the version of boost 1.42 to this:
		//alnum | char_("`=-[]\\;',./~!@#$%^&*()_+{}|:\"<>?");
		//Doesn't work.
		validChar =  	
			alnum      | char_('`') | char_('-') | char_('=') |
			char_('[') | char_(']') | char_('\\')| char_(';') |
			char_('\'')| char_(',') | char_('.') | char_('/') |
			char_('~') | char_('!') | char_('@') | char_('#') |
			char_('$') | char_('%') | char_('^') | char_('&') |
			char_('*') | char_('(') | char_(')') | char_('_') | 
			char_('+') | char_('{') | char_('}') | char_('|') |
			char_(':') | char_('"') | char_('<') | char_('>') |
			char_('?');

		validCharS = validChar | blank;
		element  = (validChar >> -validChar);
		vector   = omit[*blank] >> double_ >> omit[*blank] >> double_ >> omit[*blank] >>  double_ >> omit[*blank];
		atomName = validCharS >> validCharS >> validCharS >> validCharS;

		uninterestingRecordName = (validCharS >> validCharS >> validCharS >> validCharS >> validCharS >> validCharS) - lit("ATOM  ");
		otherRecord = uninterestingRecordName >> *validCharS >> eol;
	
		atomRecord =
			(omit[lit("ATOM  ") >> *blank >> int_ >> blank] >>
			 atomName >> //Atom Name
			 omit[validCharS >> //Alternate location indicator (can be blank)
				 validChar >> validChar >> validChar >> blank >> //Residue name
				 validChar >> *blank >> //Chain identifier
				 int_ >>      //Residue sequence number.
				 validCharS] >> //Code for insertion of residues
			 vector >>   //XYZ coordinates
			 omit[double_ >> *blank >> double_ >> *blank] >> //occupancy and Temperature factor
			 element >>
			 omit[*validCharS >> eol]);
		file =
			*(otherRecord | atomRecord) >> eoi;
	}

	//List of rules
	qi::rule<Iterator> file;
	qi::rule<Iterator,std::string()> uninterestingRecordName; /*Consumes the first 6 characters
															of input after checking it's not
															a recond type we are interesting
															in */
	qi::rule<Iterator> atomRecord;
	qi::rule<Iterator> otherRecord;
	qi::rule<Iterator,char()> validChar;
	qi::rule<Iterator,char()> validCharS;
	qi::rule<Iterator> element;
	qi::rule<Iterator,fusion::vector<double,double,double>() > vector;
	qi::rule<Iterator,std::string()> atomName;

	SpinSystem* ss;
};

void PDBLoader::LoadFile(SpinSystem* ss,const char* filename) const {
	boost::bind(&SpinSystem::InsertSpin,new Spin(Vector3d(0,0,0),"",1,0));

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

	pdb<spirit::istream_iterator> pdb_parser(ss);

    if(qi::phrase_parse(begin,end,pdb_parser,qi::blank)) {
		cout << "Parsing suceeded" << endl;
    } else {
        throw runtime_error("Parsing failed.");
    }

    ss->sigReloaded();
}


