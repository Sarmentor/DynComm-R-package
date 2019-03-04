/*
 * program.h
 *
 *  Created on: 19/08/2018
 *      Author: poltergeist0
 */

#ifndef PROGRAM_H_
#define PROGRAM_H_

#include "defines.h"
#include <string>
#include <iostream>
#include <sstream>

enum class LINK_WEIGHT:unsigned int {WEIGHTED=1, UNWEIGHTED};

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

//#define WEIGHTED   0
//#define UNWEIGHTED 1
struct ProgramParameters{
	std::string filename = "";
	std::string outfilename = "";
	std::string filename_w = "";
	std::string filename_part = "";
	LINK_WEIGHT type = LINK_WEIGHT::UNWEIGHTED;
//	CRITERION criterion=CRITERION::MODULARITY;

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
}argumentsDefault;

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
			default:
				usage(argv[0], "Unknown option\n");
			}
		} else {
			if (par.filename=="")
				par.filename = std::string(argv[i]);
			else
				usage(std::string(argv[0]), "More than one filename\n");
		}
	}
	if (par.filename == "")
		usage(std::string(argv[0]), "No input file has been provided\n");
}

//void display_time(const std::string & str) {
//	time_t rawtime;
//	time ( &rawtime );
//	std::stringstream ss;
//	ss << str << ": " << ctime (&rawtime);
//	COUT << ss.str();
//}

#endif /* PROGRAM_H_ */
