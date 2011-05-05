
#include <shared/formats/g03.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <boost/algorithm/string/trim.hpp>
#include <shared/nuclear_data.hpp>

#include <shared/panic.hpp>
#include <shared/basic_math.hpp>
#include <shared/foreach.hpp>
#include <boost/optional.hpp>

#include <shared/formats/spirit_common.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

#include <vector>
#include <map>

// Seems to fix compile error in some versions of boost.
// http://lists.boost.org/boost-users/2009/09/52337.php
#undef BOOST_SYSTEM_NO_DEPRECATED

#include <boost/filesystem.hpp>
#include <shared/formats/proto.hpp>

using namespace std;
using namespace boost;
using namespace boost::filesystem;
using namespace SpinXML;

namespace qi=boost::spirit::qi;

using boost::spirit::qi::char_;
using boost::spirit::qi::int_;
using boost::spirit::qi::alpha;
using boost::spirit::qi::double_;
using boost::spirit::qi::lit;
using boost::spirit::qi::phrase_parse;
using boost::spirit::qi::rule;
using boost::spirit::qi::space;
using boost::spirit::qi::_1;

typedef string::iterator stritor;

typedef vector<string> Lines;

//Define this to put the filter into debuging mode. Intermediate bits
//will be writen out for inspection
#define G03_DEBUG

#define readDbl(x) double_[boost::phoenix::ref((x))=_1]
#define readInt(x) int_[boost::phoenix::ref((x))=_1]

//================================================================================//
//                             Utilities                                          //
//================================================================================//

template <typename Expr>
void guardParse(string str, Expr const& expr,string error) {
	stritor begin = str.begin();
	if(!phrase_parse(begin,str.end(),expr,qi::space)) {
		throw runtime_error(error + "\n Failed line was:\n" + str);
	}
}


string fortranToC(const string& str) {
	string out = str;
	//Quick hack, but it works just fine here. Convert all "D"
	//characters to "e"
	for(unsigned long i = 0;i < out.length();i++) {
		if(out[i] == 'D') {
			out[i] = 'e';
		}
	}
	return out;
}

struct ProtoIsoHFC {
	double scalar;
	int massNumber;
};

struct ProtoAnisoHFC {
	optional<Eigenvalues> ev;
	int massNumber;
};


struct G03File {
	/*
	  Structure that stores the various interesting parts of a g03
	  file
	 */

	optional<Lines> stdOrientation;
	optional<Lines> gTensor;
	optional<Lines> shielding;
	optional<Lines> jCoupling;
	optional<Lines> isoHFC;
	optional<Lines> anisoHFC;
	optional<int>    multiplicity;
};
#ifdef G03_DEBUG
void dumpLinesToFile(optional<Lines> lines,string filename) {
	if(lines) {
		cout << filename << endl;
		Lines lines_ = lines.get();
		ofstream fout(filename.c_str());
		if(!fout.is_open()) {
			cerr << "Warning, couldn't open " << filename << endl;
			return;
		}
		foreach(string line,lines_) {
			fout << line << endl;
		}
	}
}
#endif

string safeGetLine(istream& fin) {
	string line;
	if(!getline(fin, line)) {
		throw runtime_error("Unexpected End of File");
	}
	return line;
}

G03File g03Recognise(const char* filename) {
	ifstream fin(filename);
	cout << "Opening a g03 file:" << filename << endl;
	if(!fin.is_open()) {
		//Throw some sort of error here
		cerr << "Couldn't open file" << endl;
		throw runtime_error("Couldn't Open File");
	}

	G03File g03File;

	string line;
	if(!getline(fin, line)) {
		throw runtime_error("file is empty");
	}
	while(true) {
		/*
		  We're going to use goto (yes, I know) at the end of some of
		  the regognisers to jump back up to the top of the loop. This
		  is because if a regognisers consumes some input and then
		  decides that a particular line is not it's resposibility
		  (that is, the second has ended) we need to test the line to
		  see if it's the begining of any other section we might be
		  interested it, otherwise we would miss that section. If we
		  don't skip back to the top we would end up only testing the
		  line against regognisers further down the loop
		*/
		if(!getline(fin, line)) {
			break;
		}
	loopStart:
		boost::algorithm::trim(line); //Remove whitespace
		int mult;
		stritor begin = line.begin();
		if(phrase_parse(begin,line.end(),
						lit("Charge =") >> int_ >> lit("Multiplicity =") >> readInt(mult),qi::space)) {
			g03File.multiplicity = mult;
			continue;
		} else if(line == "Standard orientation:" || line == "Input orientation:") {
			g03File.stdOrientation = Lines();
			//We need to skip 4 lines here
			safeGetLine(fin); //  ---------------------------------------------------------------------
			safeGetLine(fin); //  Center     Atomic     Atomic              Coordinates (Angstroms)
			safeGetLine(fin); //  Number     Number      Type              X           Y           Z
			safeGetLine(fin); // ---------------------------------------------------------------------

			//The std orientation second ends with line of hyphans
			while((line = safeGetLine(fin)).find("----") == string::npos) {
				g03File.stdOrientation.get().push_back(line);
			}
		} else if(line == "g tensor [g = g_e + g_RMC + g_DC + g_OZ/SOC]:"){
			g03File.gTensor = Lines();
			//Really easy, it's 4 lines long
			line = safeGetLine(fin);
			g03File.gTensor.get().push_back(line);
			line = safeGetLine(fin);
			g03File.gTensor.get().push_back(line);
			line = safeGetLine(fin);
			g03File.gTensor.get().push_back(line);
		} else if(line == "SCF GIAO Magnetic shielding tensor (ppm):"){
			/*
			  This section always looks like this:

			  SCF GIAO Magnetic shielding tensor (ppm):
			  1  C    Isotropic =   153.0648   Anisotropy =    35.1132
			  XX=   153.5147   YX=     1.4099   ZX=   -15.8194
			  XY=     6.4190   YY=   141.7114   ZY=   -10.9938
			  XZ=    -8.3913   YZ=   -13.7749   ZZ=   163.9681
			  Eigenvalues:     9.9934    13.9913    99.2020

			  With the last for lines repeated many times. If we
			  recognise the "1 C Isotropic = 153.0648 Anisotropy =
			  35.1132" line we can assume the next three lines and
			  then check again
			*/
			g03File.shielding = Lines();
			while((line = safeGetLine(fin)).find("Isotropic") != string::npos) {
				line = safeGetLine(fin); //We don't need the top line anymore
				g03File.shielding.get().push_back(line);
				line = safeGetLine(fin);
				g03File.shielding.get().push_back(line);
				line = safeGetLine(fin);
				g03File.shielding.get().push_back(line);
				line = safeGetLine(fin);
				g03File.shielding.get().push_back(line);
			}
			goto loopStart;
		} else if(line == "Total nuclear spin-spin coupling J (Hz):"){
			g03File.jCoupling = Lines();
			
			/*
			  Lines look like either 1 to 5 integers or an integer and
			  between 1 and 5 FORTRAN doubles
			*/
			line = fortranToC(line);
			while(true) {
				line = safeGetLine(fin);
				// The - operator is interpreted by boost::spirit as
				// "match zero or one times"
				stritor lbegin = line.begin();
				if(phrase_parse(lbegin,line.end(),
								(int_ >> -(int_ >> -(int_ >> -(int_ >> -int_)))) |
								(int_ >> double_ >> -(double_ >> -(double_ >> -(double_ >> -double_)))),
								qi::space)) {
					g03File.jCoupling.get().push_back(line);
				} else {
					goto loopStart;
				}
			}
		} else if(line == "Isotropic Fermi Contact Couplings") {
			//We need to skip 4 lines here
			safeGetLine(fin); // Atom                 a.u.       MegaHertz       Gauss      10(-4) cm-1
			/*
			  This section is the following type of line repeated with no end marker
			  1  C(13)              0.07610      85.54716      30.52535      28.53546
			*/
			g03File.isoHFC = Lines();
			while(true) {
				line = safeGetLine(fin);//1  C(13)              0.07610      85.54716      30.52535      28.53546
				stritor lbegin = line.begin();
				if(phrase_parse(lbegin,line.end(),
								int_ >> (alpha >> -alpha) >> char_('(') >> int_ >> char_(')') >>
								double_ >> double_ >> double_,
								qi::space)) {
					g03File.isoHFC.get().push_back(line);
				} else {
					goto loopStart;
				}
			}
		} else if(line == "Anisotropic Spin Dipole Couplings in Principal Axis System") {
			g03File.anisoHFC = Lines();
			//We need to skip 4 lines here
			safeGetLine(fin); // ------
			safeGetLine(fin); // <<blank>>
			safeGetLine(fin); // Atom             a.u.   MegaHertz   Gauss  10(-4) cm-1        Axes
			safeGetLine(fin); // <<blank>>
			line = safeGetLine(fin); // Baa    -0.5540   -74.335   -26.525   -24.796 -0.2607  0.9654  0.0000

			/*
			  This section of the file looks like thse four lines repeated:

			  Baa    -0.5540   -74.335   -26.525   -24.796 -0.2607  0.9654  0.0000
			  1 C(13)  Bbb    -0.5540   -74.335   -26.525   -24.796  0.9654  0.2607  0.0000
			  Bcc     1.1079   148.671    53.049    49.591  0.0000  0.0000  1.0000
			  <<blank>>

			  And ends with a double blank line
			*/

			while(line != "") {
				g03File.anisoHFC.get().push_back(line);
				line = safeGetLine(fin);
				g03File.anisoHFC.get().push_back(line);
				line = safeGetLine(fin);
				g03File.anisoHFC.get().push_back(line);
				safeGetLine(fin);        //expected blank line, don't do anything with it
				line = safeGetLine(fin); //Might be blank
				boost::algorithm::trim(line); //Remove whitespace so we can test the next line against ""
			}
		}
	}
	return g03File;
}

void G03Loader::LoadFile(SpinSystem* ss,const char* filename) const {
	ss->Clear();
	G03File g03File = g03Recognise(filename);

	//============================================================//
	// Debug Stuff
#ifdef G03_DEBUG
	path filepath(filename);
	path dirname(filepath.root_directory() /path("parts") / path(path(filename).filename()));
	remove_all(dirname);
	create_directory(dirname);

	path newpath = dirname / path(filename).filename();

	dumpLinesToFile(g03File.stdOrientation	,newpath.string() + "stdOrientation.part");
	dumpLinesToFile(g03File.gTensor		    ,newpath.string() + "gTensor.part");
	dumpLinesToFile(g03File.shielding		,newpath.string() + "shielding.part");
	dumpLinesToFile(g03File.jCoupling		,newpath.string() + "jCoupling.part");
	dumpLinesToFile(g03File.isoHFC			,newpath.string() + "isoHFC.part");
	dumpLinesToFile(g03File.anisoHFC        ,newpath.string() + "anisoHFC.part");

	cout << endl;
#endif

	//============================================================//
	// Now parse each section of the file seperately if it's present

	vector<ProtoSpin> spins;
	vector<ProtoInteraction> interactions;


	map<int,ProtoIsoHFC> number2isoHFC;
	map<int,ProtoAnisoHFC> number2anisoHFC;

	//============================================================//
	// Free electron

	int nElectrons = 0; 	//If we didn't find a multiplcity, assume it was 1
	if(g03File.multiplicity) {
		//Invert the 2S+1 rule
		nElectrons = g03File.multiplicity.get() - 1;
		if(nElectrons < 0) {
			cout << "g03File.multiplicity = " << g03File.multiplicity.get() << endl;
			throw runtime_error("Negative multiplicity");
		}
		nElectrons = nElectrons;
	} 
	for(long i = 0; i < nElectrons; i++) {
		ProtoSpin e;
		e.x = 0;  e.y = 0;  e.z = 0;
		e.label = "Unpaired electron";
		e.number = - i - 1;
		e.isotope = 0;
		e.element = 0;
		e.frame = 0;
		spins.push_back(e);
	}
	//============================================================//
	// Standard orientation

	//If we don't have at least one spin, we might as well give up
	if(!g03File.stdOrientation) {
		throw runtime_error("Couldn't find any nuceli in the file!");
	}

	foreach(string line,g03File.stdOrientation.get()) {
		ProtoSpin spin;
		guardParse(line,readInt(spin.number) >> readInt(spin.element) >> int_ >> readDbl(spin.x) >> readDbl(spin.y) >> readDbl(spin.z),
				   "Failed to parse standard orientation section");
		spin.x = spin.x * Angstroms;
		spin.y = spin.y * Angstroms;
		spin.z = spin.z * Angstroms;
		spin.isotope = getCommonIsotope(spin.element) + spin.element;
		spin.label = getElementSymbol(spin.element);
		spin.frame = 0;
		spins.push_back(spin);
	}

	//============================================================//
	// gTensor
	if(g03File.gTensor) {
		ProtoInteraction gT;
		double
			xx,yx,zx,
			xy,yy,zy,
			xz,yz,zz;			
		if(g03File.gTensor.get().size() != 3) {
			throw runtime_error("Unexpected end of g tensor section");
		}
		string gTensorError = "Failed to parse g-Tensor section";
		string line0 = fortranToC(g03File.gTensor.get()[0]);
		string line1 = fortranToC(g03File.gTensor.get()[1]);
		string line2 = fortranToC(g03File.gTensor.get()[2]);
		guardParse(line0,lit("XX=") >> readDbl(xx) >> lit("YX=") >> readDbl(yx) >> lit("ZX=") >> readDbl(zx) ,gTensorError);
		guardParse(line1,lit("XY=") >> readDbl(xy) >> lit("YY=") >> readDbl(yy) >> lit("ZY=") >> readDbl(zy) ,gTensorError);
		guardParse(line2,lit("XZ=") >> readDbl(xz) >> lit("YZ=") >> readDbl(yz) >> lit("ZZ=") >> readDbl(zz) ,gTensorError);
		gT.payload = MakeMatrix3d(xx,yx,zx,
								  xy,yy,zy,
								  xz,yz,zz);
		gT.type = Interaction::G_TENSER;
		gT.frame = 0;
		gT.label = "";
		gT.spin1 = -1; //Attached to the first unpaired electron
					   //(which hopefully exists).
		interactions.push_back(gT);
	}
	//============================================================//
	// shielding
	string shieldingError = "Failed to parse shielding section";
	if(g03File.shielding) {
		int counter = 1;
		Lines shieldLines = g03File.shielding.get();
		for(unsigned long i = 0;i < shieldLines.size();i+=4) {
			ProtoInteraction inter;
			double
				xx,yx,zx,
				xy,yy,zy,
				xz,yz,zz;
			double XX,YY,ZZ;
			guardParse(shieldLines[i  ],lit("XX=") >> readDbl(xx) >> lit("YX=") >> readDbl(yx) >> lit("ZX=") >> readDbl(zx),shieldingError);
			guardParse(shieldLines[i+1],lit("XY=") >> readDbl(xy) >> lit("YY=") >> readDbl(yy) >> lit("ZY=") >> readDbl(zy),shieldingError);
			guardParse(shieldLines[i+2],lit("XZ=") >> readDbl(xz) >> lit("YZ=") >> readDbl(yz) >> lit("ZZ=") >> readDbl(zz),shieldingError);
			guardParse(shieldLines[i+3],lit("Eigenvalues:") >> readDbl(XX) >> readDbl(YY) >> readDbl(ZZ) ,shieldingError);
			/*Orientation o(MakeMatrix3d(xx,yx,zx,  ***MEMORY CORRUPTION***
									   xy,yy,zy,
									   xz,yz,zz));*/
			inter.payload = 0.0;//Eigenvalues(XX,YY,ZZ,o);
			inter.spin1 = counter;
			inter.type = Interaction::SHIELDING;
			inter.label = "";
			inter.frame = 0;
			counter++;
			interactions.push_back(inter);
		}
	}

	//============================================================//
	// isoHFC
	string isoHFCError = "Failed to parse Fermi Contact section";
	if(g03File.isoHFC) {
		foreach(string line,g03File.isoHFC.get()) {
			ProtoIsoHFC isoHFC;
			int number;
			guardParse(line, readInt(number) >> (alpha >> - alpha) >> char_('(') >> readInt(isoHFC.massNumber) >> char_(')')
					   >> double_ >> readDbl(isoHFC.scalar) >> double_ >> double_,isoHFCError);
			number2isoHFC[number] = isoHFC;
		}
	}

	//============================================================//
	// anisoHFC
	string anisoError = "Failed to parse anisotropic HFC";
	if(g03File.anisoHFC) {
		Lines ansioLines = g03File.anisoHFC.get();
		for(unsigned long i = 0;i < ansioLines.size();i+=3) {
			ProtoAnisoHFC anisoHFC;
			int number;
			double
				xx,yx,zx,
				xy,yy,zy,
				xz,yz,zz;
			double XX,YY,ZZ;
			rule<string::iterator,qi::space_type> elementRule = (alpha >> - alpha) >> char_('(') >> readInt(anisoHFC.massNumber) >> char_(')');
			guardParse(ansioLines[i  ],lit("Baa") 
					   >> double_ >> readDbl(XX) >> double_ >> double_ >> readDbl(xx) >> readDbl(yx) >> readDbl(zx),anisoError);
			guardParse(ansioLines[i+1], readInt(number) >> elementRule >> lit("Bbb")
					   >> double_ >> readDbl(YY) >> double_ >> double_ >> readDbl(xy) >> readDbl(yy) >> readDbl(zy),anisoError);
			guardParse(ansioLines[i+2],lit("Bcc")
					   >> double_ >> readDbl(ZZ) >> double_ >> double_ >> readDbl(xz) >> readDbl(yz) >> readDbl(zz),anisoError);
			Orientation o;/* = MakeMatrix3d(xx,yx,zx,// ***MEMORY CORRUPTION***
										 xy,yy,zy,
										 xz,yz,zz);*/
			anisoHFC.ev = Eigenvalues(XX,YY,ZZ,o);
			number2anisoHFC[number] = anisoHFC;

		}
	}

	//============================================================//
	// Knit the anisotroic and isotropic HFC coupings together into a
	// single interaction
	typedef pair<int,ProtoIsoHFC> int_isoFHC_pair;
	foreach(int_isoFHC_pair p,number2isoHFC) {
		ProtoInteraction inter;
		int number = p.first;
		map<int,ProtoAnisoHFC>::iterator it;
		if((it = number2anisoHFC.find(number)) == number2anisoHFC.end()) {
			throw runtime_error("Isotropic HFC missing it's anisotropic part");
		}
		double      iso       = p.second.scalar;
		if(!it->second.ev) {
			throw logic_error("Anisotropic HFC eigenvalue structure not formally set");
		}
		Eigenvalues toCompose = it->second.ev.get();
		toCompose.xx += iso;
		toCompose.yy += iso;
		toCompose.zz += iso;
		
		inter.payload = toCompose;
		inter.spin1 = number;
		inter.spin2 = -1;
		inter.type = Interaction::HFC;
		inter.label = "";
		inter.frame = 0;
		interactions.push_back(inter);
		
		//We also get the mass number from reading the HFC
		//information. We can use this replace the mass number we guessed easiler
		foreach(ProtoSpin spin,spins) {
			if(spin.number == number) {
				spin.isotope = p.second.massNumber;
			}
		}
	}

	
	//============================================================//
	// Call Assemble to put the spin system together
	Assemble(ss,spins,interactions,vector<ProtoFrame>());

	return;
}

