#include "../../R-CRAN/src/base/Cpp/framework/defines.h"

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

	{//test 1 with paper graph, 1 pass (static louvain)
		COUT << "********* test 1 with paper graph, 1 pass (static louvain) *********\n";
		dbg.pre(DEBUG_LEVEL::MODIFICATIONS,"DC","test 1 with paper graph, 1 pass (static louvain)");
		ProgramParameters parameters;
		std::string origin="1 2\n#comment\n\n1 3\n2 3\n3 6\n4 6\n4 5\n5 7\n6 7";
		unsigned int size=0;
		std::string sequences[size];//={"3 6 0\n1 4 1","2 3 0\n5 6 1","1 4 0\n8 9 1","5 6 0\n8 9 0\n2 3 1\n3 6 1"};
		parameters.type=LINK_WEIGHT::WEIGHTED;
		parameters.debugLevel=DEBUG_LEVEL::ACTIONS;
		parameters.debugDepth=50;

		DynCommBase c(ALGORITHM::LOUVAIN,Criterion::CRITERION::MODULARITY,parameters);//call algorithm constructor

		ProgramParameters p=parameters;//WARNING: workaround. All addRemoveEdges are weighted
		p.type=LINK_WEIGHT::WEIGHTED;

		ReaderStringEdge rd(origin,parameters);

		c.addRemoveEdges(&rd);

		COUT << "final:\n";
		COUT << c.debugPrint()<<"\n";
		COUT << "comm count="<< c.communityCount()<<"\n";
		COUT << "comms="<< set::toString(c.communities())<<"\n";
		COUT << "comm 1 node count="<< c.communityVertexCount(*(c.communities().begin()))<<"\n";
		COUT << "comm 1 nodes="<< set::toString(c.vertices(*(c.communities().begin())))<<"\n";
		std::stringstream ss;
		WriterStream wr(ss,parameters);
		wr.write("snapshot:",WriterInterface::WRITETYPE::LINE);
		c.communityMapping(&wr,false);
		COUT << ss.str();
		COUT << "\n";
		COUT << c.quality()<<"\n";
		COUT << c.time(false)<<"ns\n";
		//assert quality=0.367188
		dbg.msg(DEBUG_LEVEL::CALLS, "Quality="+std::to_string(c.quality()));
		dbg.post(DEBUG_LEVEL::MODIFICATIONS,"end");
		ASSERT_APPROXIMATE(c.quality(),0.367188,ASSERT_PRECISION_DEFAULT);
		COUT << "\n\n\n";
	}//test 1 end
	{//test 2 with paper graph 1 edge at a time
		COUT << "********* test 2 with paper graph 1 edge at a time *********\n";
		dbg.pre(DEBUG_LEVEL::MODIFICATIONS,"DC","test 2 with paper graph 1 edge at a time");
		ProgramParameters parameters;
		std::string origin="1 2\n#comment\n";
		unsigned int size=7;
		std::string sequences[size]={"1 3","2 3","3 6","4 6","4 5","5 7","7 6"};
//		parameters.type=LINK_WEIGHT::WEIGHTED;
//		parameters.debugLevel=DEBUG_LEVEL::ALL;
//		parameters.debugDepth=5;

		bool hasAddRemove = false;
		unsigned int index=0;

		DynCommBase c(ALGORITHM::LOUVAIN,Criterion::CRITERION::MODULARITY,parameters);//call algorithm constructor

		ProgramParameters p=parameters;//WARNING: workaround. All addRemoveEdges are weighted
		p.type=LINK_WEIGHT::WEIGHTED;

		ReaderStringEdge rd(origin,parameters);

		c.addRemoveEdges(&rd);

		do {
			hasAddRemove = false;
			// check if sequence exists
			if(index<size) hasAddRemove=true;

			if(hasAddRemove){
				COUT << "\nindex="<< index<<"\n";
				COUT << c.debugPrint()<<"\n";
				COUT << "comm count="<< c.communityCount()<<"\n";
				COUT << "comms="<< set::toString(c.communities())<<"\n";
				COUT << "comm 1 node count="<< c.communityVertexCount(*(c.communities().begin()))<<"\n";
				COUT << "comm 1 nodes="<< set::toString(c.vertices(*(c.communities().begin())))<<"\n";
				WriterStream wr(std::cout,parameters);
				wr.write("snapshot:",WriterInterface::WRITETYPE::LINE);
				c.communityMapping(&wr);
				COUT << "\n";
				COUT << c.quality()<<"\n";
				COUT << c.time()<<"ns\n";
				ReaderStringEdge rd2(sequences[index],p);
				c.addRemoveEdges(&rd2);
			}
			++index;

		} while(hasAddRemove);
		COUT << "final:\n";
		COUT << c.debugPrint()<<"\n";
		COUT << "comm count="<< c.communityCount()<<"\n";
		COUT << "comms="<< set::toString(c.communities())<<"\n";
		COUT << "comm 1 node count="<< c.communityVertexCount(*(c.communities().begin()))<<"\n";
		COUT << "comm 1 nodes="<< set::toString(c.vertices(*(c.communities().begin())))<<"\n";
		std::stringstream ss;
		WriterStream wr(ss,parameters);
		wr.write("snapshot:",WriterInterface::WRITETYPE::LINE);
		c.communityMapping(&wr,false);
		COUT << ss.str();
		COUT << "\n";
		COUT << c.quality()<<"\n";
		COUT << c.time(false)<<"ns\n";
		//assert quality=0.367188
		dbg.msg(DEBUG_LEVEL::CALLS, "Quality="+std::to_string(c.quality()));
		dbg.post(DEBUG_LEVEL::MODIFICATIONS,"end");
		ASSERT_APPROXIMATE(c.quality(),0.367188,ASSERT_PRECISION_DEFAULT);
		COUT << "\n\n\n";
	}//test 2 end
	{//test 3 with paper graph with several add/remove
		COUT << "********* test 3 with paper graph with several add/remove *********\n";
		dbg.pre(DEBUG_LEVEL::MODIFICATIONS,"DC","test 3 with paper graph with several add/remove");
		ProgramParameters parameters;
		std::string origin="1 2\n#comment\n\n1 3\n2 3\n3 6\n4 6\n4 5\n5 7\n6 7";
		unsigned int size=4;
		std::string sequences[size]={"3 6 0\n1 4 1","2 3 0\n5 6 1","1 4 0\n8 9 1","5 6 0\n8 9 0\n2 3 1\n3 6 1"};
//		parameters.type=LINK_WEIGHT::WEIGHTED;
//		parameters.debugLevel=DEBUG_LEVEL::ALL;
//		parameters.debugDepth=5;

		bool hasAddRemove = false;
		unsigned int index=0;

		DynCommBase c(ALGORITHM::LOUVAIN,Criterion::CRITERION::MODULARITY,parameters);//call algorithm constructor

		ProgramParameters p=parameters;//WARNING: workaround. All addRemoveEdges are weighted
		p.type=LINK_WEIGHT::WEIGHTED;

		ReaderStringEdge rd(origin,parameters);

		c.addRemoveEdges(&rd);

		do {
			hasAddRemove = false;
			// check if sequence exists
			if(index<size) hasAddRemove=true;

			if(hasAddRemove){
				COUT << "\nindex="<< index<<"\n";
				COUT << c.debugPrint()<<"\n";
				COUT << "comm count="<< c.communityCount()<<"\n";
				COUT << "comms="<< set::toString(c.communities())<<"\n";
				COUT << "comm 1 node count="<< c.communityVertexCount(*(c.communities().begin()))<<"\n";
				COUT << "comm 1 nodes="<< set::toString(c.vertices(*(c.communities().begin())))<<"\n";
				WriterStream wr(std::cout,parameters);
				wr.write("snapshot:",WriterInterface::WRITETYPE::LINE);
				c.communityMapping(&wr);
				COUT << "\n";
				COUT << c.quality()<<"\n";
				COUT << c.time()<<"ns\n";
				ReaderStringEdge rd2(sequences[index],p);
				c.addRemoveEdges(&rd2);
			}
			++index;

		} while(hasAddRemove);
		COUT << "final:\n";
		COUT << c.debugPrint()<<"\n";
		COUT << "comm count="<< c.communityCount()<<"\n";
		COUT << "comms="<< set::toString(c.communities())<<"\n";
		COUT << "comm 1 node count="<< c.communityVertexCount(*(c.communities().begin()))<<"\n";
		COUT << "comm 1 nodes="<< set::toString(c.vertices(*(c.communities().begin())))<<"\n";
		std::stringstream ss;
		WriterStream wr(ss,parameters);
		wr.write("snapshot:",WriterInterface::WRITETYPE::LINE);
		c.communityMapping(&wr,false);
		COUT << ss.str();
		COUT << "\n";
		COUT << c.quality()<<"\n";
		COUT << c.time(false)<<"ns\n";
		//assert quality=0.367188
		dbg.msg(DEBUG_LEVEL::CALLS, "Quality="+std::to_string(c.quality()));
		dbg.post(DEBUG_LEVEL::MODIFICATIONS,"end");
		ASSERT_APPROXIMATE(c.quality(),0.367188,ASSERT_PRECISION_DEFAULT);
		COUT << "\n\n\n";
	}//test 3 end
	{//test 4 with paper graph following the example on DynComm help
		COUT << "********* test 4 with paper graph following the example on DynComm help *********\n";
		dbg.pre(DEBUG_LEVEL::MODIFICATIONS,"DC","test 4 with paper graph following the example on DynComm help");
		ProgramParameters parameters;
		std::string origin="1 2\n#comment\n\n1 3\n2 3\n3 6\n4 6\n4 5\n5 7\n6 7";
		unsigned int size=2;
		std::string sequences[size]={"3 6 0\n4 6 0.23\n1 8 2342\n8 9 3.1415\n","3 6 1\n4 6 1\n1 8 0\n8 9 0\n"};
//		parameters.type=LINK_WEIGHT::WEIGHTED;
//		parameters.debugLevel=DEBUG_LEVEL::ACTIONS;
//		parameters.debugDepth=5;

		bool hasAddRemove = false;
		unsigned int index=0;

		DynCommBase c(ALGORITHM::LOUVAIN,Criterion::CRITERION::MODULARITY,parameters);//call algorithm constructor

		ProgramParameters p=parameters;//WARNING: workaround. All addRemoveEdges are weighted
		p.type=LINK_WEIGHT::WEIGHTED;

		ReaderStringEdge rd(origin,parameters);

		c.addRemoveEdges(&rd);

		do {
			hasAddRemove = false;
			// check if sequence exists
			if(index<size) hasAddRemove=true;

			if(hasAddRemove){
				COUT << "\nindex="<< index<<"\n";
				COUT << c.debugPrint()<<"\n";
				COUT << "comm count="<< c.communityCount()<<"\n";
				COUT << "comms="<< set::toString(c.communities())<<"\n";
				COUT << "comm 1 node count="<< c.communityVertexCount(*(c.communities().begin()))<<"\n";
				COUT << "comm 1 nodes="<< set::toString(c.vertices(*(c.communities().begin())))<<"\n";
				WriterStream wr(std::cout,parameters);
				wr.write("snapshot:",WriterInterface::WRITETYPE::LINE);
				c.communityMapping(&wr);
				COUT << "\n";
				COUT << c.quality()<<"\n";
				COUT << c.time()<<"ns\n";
				ReaderStringEdge rd2(sequences[index],p);
				c.addRemoveEdges(&rd2);
			}
			++index;

		} while(hasAddRemove);
		COUT << "final:\n";
		COUT << c.debugPrint()<<"\n";
		COUT << "comm count="<< c.communityCount()<<"\n";
		COUT << "comms="<< set::toString(c.communities())<<"\n";
		COUT << "comm 1 node count="<< c.communityVertexCount(*(c.communities().begin()))<<"\n";
		COUT << "comm 1 nodes="<< set::toString(c.vertices(*(c.communities().begin())))<<"\n";
		std::stringstream ss;
		WriterStream wr(ss,parameters);
		wr.write("snapshot:",WriterInterface::WRITETYPE::LINE);
		c.communityMapping(&wr,false);
		COUT << ss.str();
		COUT << "\n";
		COUT << c.quality()<<"\n";
		COUT << c.time(false)<<"ns\n";
		//assert quality=0.367188
		dbg.msg(DEBUG_LEVEL::CALLS, "Quality="+std::to_string(c.quality()));
		dbg.post(DEBUG_LEVEL::MODIFICATIONS,"end");
		ASSERT_APPROXIMATE(c.quality(),0.367188,ASSERT_PRECISION_DEFAULT);
		COUT << "\n\n\n";
	}//test 4 end
//	{
//		ProgramParameters parameters;
//	//	parse_args(argc, argv,parameters);
//		std::string origin="1 2\n#comment\n\n1 3\n2 3\n3 6\n4 6\n4 5\n5 7\n6 7";
//		unsigned int size=0;
//		std::string sequences[size];//={"3 6 0\n1 4 1","2 3 0\n5 6 1","1 4 0\n8 9 1","5 6 0\n8 9 0\n2 3 1\n3 6 1"};
//	//	unsigned int size=4;
//	//	std::string sequences[size]={"3 6 0\n1 4 1","2 3 0\n5 6 1","1 4 0\n8 9 1","5 6 0\n8 9 0\n2 3 1\n3 6 1"};
//	//	std::string origin="149 181 0.249683\n149 181 0.240531\n149 181 0.125624\n149 181 0.127816\n253 271 0.151593\n149 181 0.12727\n271 299 0.161972\n149 299 0.144484";
//	//	std::string origin="149 181 0.249683";
//	//	std::string origin="";
//	//	std::string origin="2019-10-27 15:03:01.224514-partial.sims.txt";
//	//	std::string origin="2020-01-19 19:11:39.746-partial.sims.txt";
//		parameters.type=LINK_WEIGHT::WEIGHTED;
//		parameters.debugLevel=DEBUG_LEVEL::ALL;
//		parameters.debugDepth=5;
//	//	parameters.debugFilename="";
//	//	unsigned int size=3;
//	//	std::string sequences[size]={"","149 181 0.249683\n149 181 0.240531\n149 181 0.125624\n149 181 0.127816\n253 271 0.151593\n149 181 0.12727\n271 299 0.161972\n149 299 0.144484\n149 299 0.217468\n271 317 0.161972\n149 317 0.120904\n149 181 0.12727\n149 181 0.12727\n","52 337 0.159483\n149 181 0.12727\n271 364 0.129333\n253 364 0.142884\n253 364 0.166993\n52 337 0.14911\n52 337 0.14911\n337 364 0.122834\n253 364 0.181235\n337 364 0.133213\n91 380 0.296198\n91 380 0.315637\n91 380 0.162473\n91 380 0.17525\n91 380 0.168561\n52 337 0.14911\n337 364 0.133213\n91 380 0.191722\n52 337 0.14911\n337 364 0.133213\n337 380 0.14609"};
//	//	const unsigned int & size=sizeFileList;
//	//	const std::string (& sequences)[size]=sequencesFileList;
//
//	//	bool improvement=true;
//		bool hasAddRemove = false;
//		unsigned int index=0;
//
//		DynCommBase c(ALGORITHM::LOUVAIN,Criterion::CRITERION::MODULARITY,parameters);//call algorithm constructor
//
//		ProgramParameters p=parameters;//WARNING: workaround. All addRemoveEdges are weighted
//		p.type=LINK_WEIGHT::WEIGHTED;
//	//	p.directory="/home/poltergeist0/Desktop/debugCpp/Edges/";
//
//		ReaderStringEdge rd(origin,parameters);
//	//	parameters.directory="/home/poltergeist0/Desktop/debugCpp/Edges/";
//	//	parameters.directory="/home/poltergeist0/Desktop/r_projects/2020-01-19-v3-results/";
//	//	parameters.filename=origin;
//	//	ReaderFileEdge rd(parameters);
//
//		c.addRemoveEdges(&rd);
//
//		do {
//	//		c.addRemoveEdges(c,index,has_add_file,has_rem_file);//call addRemoveEdges if there are edges to add or remove
//			hasAddRemove = false;
//	//		std::stringstream ss;
//	//		ss.str("");
//	//		ss << louvain.parameters().directory.c_str();
//	//		if(louvain.parameters().directory[louvain.parameters().directory.length()-1]!=PATH_SEPARATOR) ss << PATH_SEPARATOR;
//	//		ss << "s" << std::setw(10) << std::setfill('0') << index << ".txt";
//	//		std::set<Edge> edges = read_from_file (ss.str().c_str(), has_rem_file, louvain.parameters().type);
//
//
//			// check if sequence exists
//	//		if(index<size) hasAddRemove=true;
//
//			if(hasAddRemove){
//				COUT << c.debugPrint()<<"\n";
//				COUT << "comm count="<< c.communityCount()<<"\n";
//				COUT << "comms="<< set::toString(c.communities())<<"\n";
//				COUT << "comm 1 node count="<< c.communityVertexCount(*(c.communities().begin()))<<"\n";
//				COUT << "comm 1 nodes="<< set::toString(c.vertices(*(c.communities().begin())))<<"\n";
//				WriterStream wr(std::cout,parameters);
//				wr.write("snapshot:",WriterInterface::WRITETYPE::LINE);
//				c.communityMapping(&wr);
//				COUT << "\n";
//				COUT << c.quality()<<"\n";
//				COUT << c.time()<<"ns\n";
//	//			ReaderStringEdge rd2(sequences[index],p);
//				p.filename=sequences[index];
//				ReaderFileEdge rd2(p);
//				c.addRemoveEdges(&rd2);
//			}
//			++index;
//
//		} while(hasAddRemove);
//		COUT << c.debugPrint()<<"\n";
//		COUT << "comm count="<< c.communityCount()<<"\n";
//		COUT << "comms="<< set::toString(c.communities())<<"\n";
//		COUT << "comm 1 node count="<< c.communityVertexCount(*(c.communities().begin()))<<"\n";
//		COUT << "comm 1 nodes="<< set::toString(c.vertices(*(c.communities().begin())))<<"\n";
//		std::stringstream ss;
//		WriterStream wr(ss,parameters);
//		wr.write("snapshot:",WriterInterface::WRITETYPE::LINE);
//		c.communityMapping(&wr,false);
//		COUT << ss.str();
//		COUT << "\n";
//		COUT << c.quality()<<"\n";
//		COUT << c.time(false)<<"ns\n";
//	}

	return 0;
}
#endif //FLAG_RCPP
