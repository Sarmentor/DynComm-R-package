//#include "defines.h"
#include "DynCommBase.h"
#include "quality.h"
#include "reader.h"
#include "writer.h"
#include "program.h"

int main(int argc, char *argv[]) {
	ProgramParameters parameters;
//	parse_args(argc, argv,parameters);
	ReaderStringEdge rd("1 2\n1 3\n2 3\n3 6\n4 6\n4 5\n5 7\n6 7",parameters);

	bool improvement=true;

	DynCommBase c(&rd,Algorithm::ALGORITHM::LOUVAIN,Quality::QUALITY::MODULARITY,parameters);//call algorithm constructor
//	do {
//		improvement=c.addRemoveEdges(c,index,has_add_file,has_rem_file);//call addRemoveEdges if there are edges to add or remove
//
//	} while(improvement);
	std::cout << c.quality()<<"\n";
	return 0;
}
