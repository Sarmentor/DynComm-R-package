/*
 * program.h
 *
 *  Created on: 19/08/2018
 *      Author: poltergeist0
 */

#ifndef PROGRAM_H_
#define PROGRAM_H_

#include "defines.h"

void usage(const std::string & prog_name, const std::string & more) {
	std::cout << more;
	std::cout << "usage: " << prog_name << " input_file [-q id_qual] [-c alpha] [-k min] [-w weight_file] [-p part_file] [-e epsilon] [-l display_level] [-v] [-h] [-s sequence_directory]" << endl << endl;
	std::cout << "input_file: file containing the graph to decompose in communities" << endl;

	std::cout << "-q id\tthe quality function used to compute partition of the graph (modularity is chosen by default):" << endl << endl;

	std::cout << "\tid = 0\t -> the classical Newman-Girvan criterion (also called \"Modularity\")" << endl;
	std::cout << "\tid = 1\t -> the Zahn-Condorcet criterion" << endl;
	std::cout << "\tid = 2\t -> the Owsinski-Zadrozny criterion (you should specify the value of the parameter with option -c)" << endl;
	std::cout << "\tid = 3\t -> the Goldberg Density criterion" << endl;
	std::cout << "\tid = 4\t -> the A-weighted Condorcet criterion" << endl;
	std::cout << "\tid = 5\t -> the Deviation to Indetermination criterion" << endl;
	std::cout << "\tid = 6\t -> the Deviation to Uniformity criterion" << endl;
	std::cout << "\tid = 7\t -> the Profile Difference criterion" << endl;
	std::cout << "\tid = 8\t -> the Shi-Malik criterion (you should specify the value of kappa_min with option -k)" << endl;
	std::cout << "\tid = 9\t -> the Balanced Modularity criterion" << endl;

	std::cout << endl;

	std::cout << "-c al\tthe parameter for the Owsinski-Zadrozny quality function (between 0.0 and 1.0: 0.5 is chosen by default)" << endl;
	std::cout << "-k min\tthe kappa_min value (for Shi-Malik quality function) (it must be > 0: 1 is chosen by default)" << endl;

	std::cout << endl;

	std::cout << "-w file\tread the graph as a weighted one (weights are set to 1 otherwise)" << endl;
	std::cout << "-p file\tstart the computation with a given partition instead of the trivial partition" << endl;
	std::cout << "\tfile must contain lines \"node community\"" << endl;
	std::cout << "-e eps\ta given pass stops when the quality is increased by less than epsilon" << endl;
	std::cout << "-l k\tdisplays the graph of level k rather than the hierachical structure" << endl;
	std::cout << "\tif k=-1 then displays the hierarchical structure rather than the graph at a given level" << endl;
	std::cout << "-v\tverbose mode: gives computation time, information about the hierarchy and quality" << endl;
	std::cout << "-h\tshow this usage message" << endl;
	std::cout << "-s\tsequence directory: indicates the directory where the sequence files are placed." << endl;

	exit(0);
}

//#define WEIGHTED   0
//#define UNWEIGHTED 1
struct ProgramParameters{
	std::string filename = "";
	std::string filename_w = "";
	std::string filename_part = "";
	LINK_WEIGHT type = UNWEIGHTED;
	CRITERION criterion=CRITERION::MODULARITY;

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
			case 'w':
				par.type = WEIGHTED;
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

void display_time(const std::string & str) {
	time_t rawtime;
	time ( &rawtime );
	std::cout << str << ": " << ctime (&rawtime);
}

#endif /* PROGRAM_H_ */
