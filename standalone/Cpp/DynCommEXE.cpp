#include "../../base/Cpp/defines.h"

#ifndef FLAG_RCPP

#include "../../base/Cpp/DynCommBase.h"
//#include "quality.h"
//#include "reader.h"
//#include "writer.h"
//#include "program.h"
//#include "timeFunctions.h"


int main(int argc, char *argv[]) {
//	Time::TimeKeeper tm;
//	COUT << "temporal experiment 1\n";
//	uint64 t1=Time::currentTime();
//	COUT << tm.get() << " ; " << t1 << "\n";
//	COUT << tm.set() << " ; " << t1 << "\n";
//	COUT << tm.get(t1) << " ; " << t1 << "\n";
//	COUT << tm.set(t1) << " ; " << t1 << "\n";
//
//	Time::TimeKeeper tm2(t1);
//	COUT << "temporal experiment 2\n";
//	uint64 t2=Time::currentTime();
//	COUT << tm2.get() << " ; " << t2 << "\n";
//	COUT << tm2.set() << " ; " << t2 << "\n";
//	COUT << tm2.get(t2) << " ; " << t2 << "\n";
//	COUT << tm2.set(t2) << " ; " << t2 << "\n";


	ProgramParameters parameters;
//	parse_args(argc, argv,parameters);
	std::string origin="1 2\n#comment\n\n1 3\n2 3\n3 6\n4 6\n4 5\n5 7\n6 7";
//	unsigned int size=0;
//	std::string sequences[size];//={"3 6 0\n1 4 1","2 3 0\n5 6 1","1 4 0\n8 9 1","5 6 0\n8 9 0\n2 3 1\n3 6 1"};
	unsigned int size=4;
	std::string sequences[size]={"3 6 0\n1 4 1","2 3 0\n5 6 1","1 4 0\n8 9 1","5 6 0\n8 9 0\n2 3 1\n3 6 1"};
	ReaderStringEdge rd(origin,parameters);

//	bool improvement=true;
	bool hasAddRemove = false;
	unsigned int index=0;

	DynCommBase c(&rd,Algorithm::ALGORITHM::LOUVAIN,Quality::QUALITY::MODULARITY,parameters);//call algorithm constructor

	ProgramParameters p=parameters;//WARNING: workaround. All addRemoveEdges are weighted
	p.type=LINK_WEIGHT::WEIGHTED;

	do {
//		c.addRemoveEdges(c,index,has_add_file,has_rem_file);//call addRemoveEdges if there are edges to add or remove
		hasAddRemove = false;
//		std::stringstream ss;
//		ss.str("");
//		ss << louvain.parameters().directory.c_str();
//		if(louvain.parameters().directory[louvain.parameters().directory.length()-1]!=PATH_SEPARATOR) ss << PATH_SEPARATOR;
//		ss << "s" << std::setw(10) << std::setfill('0') << index << ".txt";
//		std::set<Edge> edges = read_from_file (ss.str().c_str(), has_rem_file, louvain.parameters().type);


		// check if sequence exists
		if(index<size) hasAddRemove=true;

		if(hasAddRemove){
			COUT << "comm count="<< c.communityCount()<<"\n";
			COUT << "comms="<< set::toString(c.communities())<<"\n";
			COUT << "comm 1 node count="<< c.communityNodeCount(1)<<"\n";
			COUT << "comm 1 nodes="<< set::toString(c.nodes(1))<<"\n";
			WriterString wr(parameters);
			wr.write("snapshot:",WriterInterface::WRITETYPE::LINE);
			c.results(&wr);
			COUT << c.quality()<<"\n";
			COUT << c.time()<<"us\n";
			ReaderStringEdge rd2(sequences[index],p);
			c.addRemoveEdges(&rd2);
		}
		++index;

	} while(hasAddRemove);
	COUT << "comm count="<< c.communityCount()<<"\n";
	COUT << "comms="<< set::toString(c.communities())<<"\n";
	COUT << "comm 1 node count="<< c.communityNodeCount(1)<<"\n";
	COUT << "comm 1 nodes="<< set::toString(c.nodes(1))<<"\n";
	WriterString wr(parameters);
	wr.write("snapshot:",WriterInterface::WRITETYPE::LINE);
	c.results(&wr);
	COUT << c.quality()<<"\n";
	COUT << c.time()<<"ms\n";
	return 0;
}
#endif //FLAG_RCPP
