#include "../../R-CRAN/src/base/Cpp/defines.h"

#ifndef FLAG_RCPP

#include "../../R-CRAN/src/base/Cpp/DynCommBase.h"
//#include "quality.h"
//#include "reader.h"
//#include "writer.h"
//#include "program.h"
//#include "timeFunctions.h"
#include "testFileList.h"

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
//	std::string origin="1 2\n#comment\n\n1 3\n2 3\n3 6\n4 6\n4 5\n5 7\n6 7";
//	unsigned int size=0;
//	std::string sequences[size];//={"3 6 0\n1 4 1","2 3 0\n5 6 1","1 4 0\n8 9 1","5 6 0\n8 9 0\n2 3 1\n3 6 1"};
//	unsigned int size=4;
//	std::string sequences[size]={"3 6 0\n1 4 1","2 3 0\n5 6 1","1 4 0\n8 9 1","5 6 0\n8 9 0\n2 3 1\n3 6 1"};
//	std::string origin="149 181 0.249683\n149 181 0.240531\n149 181 0.125624\n149 181 0.127816\n253 271 0.151593\n149 181 0.12727\n271 299 0.161972\n149 299 0.144484";
//	std::string origin="149 181 0.249683";
//	std::string origin="";
	std::string origin="2019-10-27 15:03:01.224514-partial.sims.txt";
	parameters.type=LINK_WEIGHT::WEIGHTED;
//	parameters.debugLevel=DEBUG_LEVEL::ALL;
	parameters.debugDepth=5;
	parameters.debugFilename="";
//	unsigned int size=3;
//	std::string sequences[size]={"","149 181 0.249683\n149 181 0.240531\n149 181 0.125624\n149 181 0.127816\n253 271 0.151593\n149 181 0.12727\n271 299 0.161972\n149 299 0.144484\n149 299 0.217468\n271 317 0.161972\n149 317 0.120904\n149 181 0.12727\n149 181 0.12727\n","52 337 0.159483\n149 181 0.12727\n271 364 0.129333\n253 364 0.142884\n253 364 0.166993\n52 337 0.14911\n52 337 0.14911\n337 364 0.122834\n253 364 0.181235\n337 364 0.133213\n91 380 0.296198\n91 380 0.315637\n91 380 0.162473\n91 380 0.17525\n91 380 0.168561\n52 337 0.14911\n337 364 0.133213\n91 380 0.191722\n52 337 0.14911\n337 364 0.133213\n337 380 0.14609"};
	const unsigned int & size=sizeFileList;
	const std::string (& sequences)[size]=sequencesFileList;

//	bool improvement=true;
	bool hasAddRemove = false;
	unsigned int index=0;

	DynCommBase c(ALGORITHM::LOUVAIN,Criterion::CRITERION::MODULARITY,parameters);//call algorithm constructor

	ProgramParameters p=parameters;//WARNING: workaround. All addRemoveEdges are weighted
	p.type=LINK_WEIGHT::WEIGHTED;
	p.directory="/home/poltergeist0/Desktop/debugCpp/Edges/";

//	ReaderStringEdge rd(origin,parameters);
	parameters.directory="/home/poltergeist0/Desktop/debugCpp/Edges/";
	parameters.filename=origin;
	ReaderFileEdge rd(parameters);
	c.addRemoveEdges(&rd);

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
			COUT << c.debugPrint()<<"\n";
			COUT << "comm count="<< c.communityCount()<<"\n";
			COUT << "comms="<< set::toString(c.communities())<<"\n";
			COUT << "comm 1 node count="<< c.communityVertexCount(1)<<"\n";
			COUT << "comm 1 nodes="<< set::toString(c.vertices(1))<<"\n";
			WriterStream wr(std::cout,parameters);
			wr.write("snapshot:",WriterInterface::WRITETYPE::LINE);
			c.communityMapping(&wr);
			COUT << "\n";
			COUT << c.quality()<<"\n";
			COUT << c.time()<<"ns\n";
//			ReaderStringEdge rd2(sequences[index],p);
			p.filename=sequences[index];
			ReaderFileEdge rd2(p);
			c.addRemoveEdges(&rd2);
		}
		++index;

	} while(hasAddRemove);
	COUT << c.debugPrint()<<"\n";
	COUT << "comm count="<< c.communityCount()<<"\n";
	COUT << "comms="<< set::toString(c.communities())<<"\n";
	COUT << "comm 1 node count="<< c.communityVertexCount(1)<<"\n";
	COUT << "comm 1 nodes="<< set::toString(c.vertices(1))<<"\n";
	std::stringstream ss;
	WriterStream wr(ss,parameters);
	wr.write("snapshot:",WriterInterface::WRITETYPE::LINE);
	c.communityMapping(&wr,false);
	COUT << ss.str();
	COUT << "\n";
	COUT << c.quality()<<"\n";
	COUT << c.time()<<"ns\n";
	return 0;
}
#endif //FLAG_RCPP
