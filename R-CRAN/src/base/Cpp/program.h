/************************************************************************
 ************************* Developer Notice *****************************
 ************************************************************************
 * @details
 *
 * This file has the structure used to store program parameters, its
 * default values and functions used to parse those parameters.
 *
 *
 * @author poltergeist0
 *
 * @date 2018-08-19
 ************************************************************************
 ************************************************************************
 ************************************************************************/

#ifndef PROGRAM_H_
#define PROGRAM_H_

#include "defines.h"
#include <string>
#include <iostream>
#include <sstream>
#include <cstring> //strlen
#include "debugUtilities.h"

//#include "DebugLog.h"

/**
 * enumeration used to indicate if the edges being added to a graph are
 * considered as weighted (must have a weight or a default value is
 * attributed) or unweighted (if a weight is given it is ignored).
 */
enum class LINK_WEIGHT:unsigned int {WEIGHTED=1, UNWEIGHTED};

/**
 * Usage print for standalone program
 * @param prog_name is the name of the executable
 * @param more is a header
 */
void usage(const std::string & prog_name, const std::string & more) {
	std::stringstream ss;
	ss << more;
	ss << "usage: " << prog_name << " input_file [-q id_qual] [-c alpha] [-k min] [-w weight_file] [-p part_file] [-e epsilon] [-l display_level] [-v] [-h] [-s sequence_directory]" << "\n" << "\n";
	ss << "input_file: file containing the graph to decompose in communities" << "\n";

	ss << "-q id\tthe quality function used to compute partition of the graph (modularity is chosen by default):" << "\n" << "\n";

	ss << "\tid = 0\t -> the classical Newman-Girvan criterion (also called \"Modularity\")" << "\n";
	ss << "\tid = 1\t -> the Zahn-Condorcet criterion" << "\n";
	ss << "\tid = 2\t -> the Owsinski-Zadrozny criterion (you should specify the value of the parameter with option -c)" << "\n";
	ss << "\tid = 3\t -> the Goldberg Density criterion" << "\n";
	ss << "\tid = 4\t -> the A-weighted Condorcet criterion" << "\n";
	ss << "\tid = 5\t -> the Deviation to Indetermination criterion" << "\n";
	ss << "\tid = 6\t -> the Deviation to Uniformity criterion" << "\n";
	ss << "\tid = 7\t -> the Profile Difference criterion" << "\n";
	ss << "\tid = 8\t -> the Shi-Malik criterion (you should specify the value of kappa_min with option -k)" << "\n";
	ss << "\tid = 9\t -> the Balanced Modularity criterion" << "\n";

	ss << "\n";

	ss << "-c al\tthe parameter for the Owsinski-Zadrozny quality function (between 0.0 and 1.0: 0.5 is chosen by default)" << "\n";
	ss << "-k min\tthe kappa_min value (for Shi-Malik quality function) (it must be > 0: 1 is chosen by default)" << "\n";

	ss << "\n";

	ss << "-w file\tread the graph as a weighted one (weights are set to 1 otherwise)" << "\n";
	ss << "-p file\tstart the computation with a given partition instead of the trivial partition" << "\n";
	ss << "\tfile must contain lines \"node community\"" << "\n";
	ss << "-e eps\ta given pass stops when the quality is increased by less than epsilon" << "\n";
	ss << "-l k\tdisplays the graph of level k rather than the hierachical structure" << "\n";
	ss << "\tif k=-1 then displays the hierarchical structure rather than the graph at a given level" << "\n";
	ss << "-v\tverbose mode: gives computation time, information about the hierarchy and quality" << "\n";
	ss << "-h\tshow this usage message" << "\n";
	ss << "-s\tsequence directory: indicates the directory where the sequence files are placed." << "\n";

	COUT << ss.str();
	exit(0);
}

/**
 * Program parameters structure with default parameters
 */
struct ProgramParameters{
	std::string filename = "";
	std::string outfilename = "";
	std::string filename_w = "";
	std::string filename_part = "";
	LINK_WEIGHT type = LINK_WEIGHT::UNWEIGHTED;

	int nb_pass = 0;
	long double precision = 0.000001L;
	int display_level = -2;

	unsigned short id_qual = 0;

	long double alpha = 0.5L;
	int kmin = 1;

	long double sum_se = 0.0L;
	long double sum_sq = 0.0L;

	long double max_w = 1.0L;
	bool verbose = false;

	std::string directory=".";

	DEBUG_LEVEL debugLevel=DEBUG_LEVEL::NONE;
	unsigned int debugDepth=4;
	std::string debugFilename="debugCpp.log";

}argumentsDefault;//variable with default program parameters

/**
 * Parsing function for parameters passed to R
 * @param name
 * @param value
 * @param par
 */
void parse_arg(const std::string & name, const std::string & value, ProgramParameters & par) {
  if(name.compare("o")){
    par.outfilename = std::string(value);
  }
  else if(name.compare("w")){
    par.type = LINK_WEIGHT::WEIGHTED;
    par.filename_w = value;
  }
  else if(name.compare("q")){
    par.id_qual = std::stoi(value.c_str());
  }
  else if(name.compare("c")){
    par.alpha = std::stof(value);
  }
  else if(name.compare("k")){
    par.kmin = std::stoi(value);
  }
  else if(name.compare("p")){
    par.filename_part = value;
  }
  else if(name.compare("e")){
    par.precision = std::stof(value);
  }
  else if(name.compare("l")){
    par.display_level = std::stoi(value);
  }
  else if(name.compare("s")){
    par.directory = value;
  }
  else if(name.compare("v")){
    par.verbose = true;
  }
  else if(name.compare("f")){
    if (par.filename=="") par.filename = value;
  }
  else if(name.compare("dl")){
	  par.debugLevel=fromInt(std::stoi(value));
  }
  else if(name.compare("dd")){
	  par.debugDepth=std::stoi(value);
  }
  else if(name.compare("df")){
    if (value!="") par.debugFilename = value;
  }
}

/**
 * Parsing function for program parameters passed to the standalone program
 * @param argc
 * @param argv
 * @param par
 */
void parse_args(int argc, char *argv[], ProgramParameters & par) {
	if (argc<2)
		usage(argv[0], "Bad arguments number\n");
	for (int i = 1; i < argc; i++) {
		if(argv[i][0] == '-') {
			switch(argv[i][1]) {
			case 'o':
				par.outfilename = std::string(argv[i+1]);
				i++;
				break;
			case 'w':
				par.type = LINK_WEIGHT::WEIGHTED;
				par.filename_w = std::string(argv[i+1]);
				i++;
				break;
			case 'q':
				par.id_qual = (unsigned short)atoi(argv[i+1]);
				i++;
				break;
			case 'c':
				par.alpha = atof(argv[i+1]);
				i++;
				break;
			case 'k':
				par.kmin = atoi(argv[i+1]);
				i++;
				break;
			case 'p':
				par.filename_part = std::string(argv[i+1]);
				i++;
				break;
			case 'e':
				par.precision = atof(argv[i+1]);
				i++;
				break;
			case 'l':
				par.display_level = atoi(argv[i+1]);
				i++;
				break;
				//#ifdef	//MODIFIED
			case 's':
				par.directory = std::string(argv[i+1]);
//				if(par.directory[par.directory.length()-1]!=PATH_SEPARATOR) par.directory.append(PATH_SEPARATOR_STRING);
				i++;
				break;
				//#endif	//MODIFIED
			case 'v':
				par.verbose = true;
				break;
			case 'h':
				usage(argv[0], "");
				break;
			case 'd':
				if(strlen(argv[i])>=2){
					switch(argv[i][2]) {
						case 'l':
							par.debugLevel=fromInt(atoi(argv[i+1]));
							++i;
							break;
						case 'd':
							par.debugDepth=atoi(argv[i+1]);
							++i;
							break;
						case 'f':
							par.debugFilename=argv[i+1];
							++i;
							break;
					}
				}
				break;
			default:
				usage(argv[0], "Unknown option\n");
			}
		} else {
			if (par.filename=="")
				par.filename = std::string(argv[i]);
			// else
			// 	usage(std::string(argv[0]), "More than one filename\n");
		}
	}
}

#endif /* PROGRAM_H_ */
