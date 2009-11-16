
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
  po::options_description desc("Options");
  desc.add_options()
    ("help,h", "produce this help message")
    ("output-file,o", po::value<int>(), "Output File")
    ("command,c",po::value<int>(),"command")
    ;

  try {
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
  } catch(exception& e) {
    cerr << e.what() << endl;
    exit(1);
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

