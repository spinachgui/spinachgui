
#include <iostream>
#include <fstream>
#include <string>

#include <shared/nuclear_data.hpp>
#include <shared/spinsys.hpp>
#include <boost/program_options.hpp>

using namespace SpinXML;
using namespace std;
namespace po = boost::program_options;


int main(int argc,char** argv) {
  po::options_description visible_opt("Options");
  visible_opt.add_options()
    ("help,h", "produce this help message")
    ("output-file,o", po::value<string>(), "Output File")
    ;
  po::options_description hidden_opt("Hidden Options");
  hidden_opt.add_options()
    ("command",po::value<string>(),"command")
    ;

  po::options_description cmd_opt("options");
  cmd_opt.add(visible_opt).add(hidden_opt);

  po::positional_options_description pos_desc;
  pos_desc.add("command",1);
  pos_desc.add("input-file",1);

  po::variables_map vm;

  try {
    po::store(po::command_line_parser(argc, argv).options(cmd_opt).positional(pos_desc).run(), vm);
    po::notify(vm);
  } catch(exception& e) {
    cerr << e.what() << endl;
    cerr << endl;
    cerr << visible_opt << endl;
    exit(1);
  }
  if(vm.count("help")) {
    cout << visible_opt << endl;
  }
  
  if(vm["command"].empty()) {
    cerr << "No Command Specified." << endl;
    cerr << visible_opt << endl;
    return 1;
  }
  cout << "Command=" << vm["command"].as<string>() << endl;
  if(vm["command"].as<string>()=="convert") {
    cout << "Input File" << vm["input-file"].as<string>() << endl;
    cout << "Output File" << vm["output-file"].as<string>() << endl;
  } else {
    cerr << "Unknown Command" << endl;
    cerr << visible_opt << endl;
    return 1;
  }

  try {
    LoadIsotopes();
  } catch(runtime_error e) {
    cerr << "Error loading data/isotopes.dat. Is the file present and not corrupt?" << endl << 
      "Message was:" << e.what() << endl;

    return 1;
  }

  return 0;
}

