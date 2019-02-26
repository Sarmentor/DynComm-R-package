/*
 * R adaptation interface for Dynamic Communities algorithms.
 *
 *
 */

#include "defines.h"

#ifdef FLAG_RCPP

// #include <Rcpp.h>
#include "DynCommBase.h"

/**
 * Dummy class used to implement Enumeration like variables and function parameters in R from C++.
 * May be removed in the future when R starts supporting C++ class enumerations.
 */
class DummyEnum {
	int x;
	int get_x() {return x;}
};

class DummyAlgorithm: public DummyEnum{

};

class DummyQuality: public DummyEnum{

};

/**
 * Enumeration with the list of supported Dynamic Communities algorithms.
 * This enumeration must start at 1 since R indexing (for arrays, etc) starts at 1 instead of 0.
 * Otherwise C++ would assign the first algorithm a 0.
 * This may be moved inside the class in the future when R supports enumerations inside classes.
 */
// enum ALGORITHM:int{LOUVAIN=1};

/**
 * Dynamic Communities class that handles all the IO for the base class.
 * This (file/class) is the R version.
 */
class DynComm{
private:
	ProgramParameters prmtrs;
	ReaderFileEdge rFE;
	DynCommBase dcb;//dynamic communities base class where all the logic is

	ProgramParameters convertToParameters(Rcpp::CharacterMatrix algorithmParameters=Rcpp::CharacterMatrix()){
		int nrow = algorithmParameters.nrow();
		int ncol = algorithmParameters.ncol();
		ProgramParameters p;
		for (int i = 0; i < nrow; i++) {
		  if(algorithmParameters(i,0)=="filename"){
		    char *a[]={"DynComm",algorithmParameters(i,1)};
		    parse_args(2,a,p);
		  }
		  else{
			  char *a[]={"DynComm",algorithmParameters(i,0),algorithmParameters(i,1)};
			  parse_args(3,a,p);
		  }
		}
		return p;
	}

public:
	/**
	 * Default constructor not acceptable.
	 * Must be passed at least the chosen algorithm and the graph
	 */
	DynComm()=delete;

	/**
	 * Constructor for loading graph from white character (tab or space) separated values file
	 * Format of file is:
	 *   - one edge per line
	 *   - edge contains two or three values separated by a white space, in this order, source node, destination node and, optionally, a weight
	 *   - if weight is not given, it will default to 1
	 *   - edges with a weight of exactly zero are not added
	 *   - source and destination nodes are integers between 0 and MAX_INTEGER_VALUE-1
	 *   - weight is a double
	 *   - MAX_INTEGER_VALUE depends on the platform being 32bit or 64bit. It is the maximum value of an integer in that platform
	 */
	DynComm(
			Algorithm::ALGORITHM algorithm=Algorithm::ALGORITHM::LOUVAIN
			,const Quality::QUALITY & quality=Quality::QUALITY::MODULARITY
			// , ProgramParameters algorithmParameters=argumentsDefault
			,Rcpp::CharacterMatrix algorithmParameters=Rcpp::CharacterMatrix()
	)
	:
		prmtrs(convertToParameters(algorithmParameters))
	,rFE(prmtrs)
	,dcb(&rFE, algorithm,quality, prmtrs)
	{
		// std::cout << "failled\n";
	}

	/**
	 * Constructor for loading graph from white character (tab or space) separated values string
	 * Format of file is:
	 *   - one edge per line
	 *   - edge contains two or three values separated by a white space, in this order, source node, destination node and, optionally, a weight
	 *   - if weight is not given, it will default to 1
	 *   - edges with a weight of exactly zero are not added
	 *   - source and destination nodes are integers between 0 and MAX_INTEGER_VALUE-1
	 *   - weight is a double
	 *   - MAX_INTEGER_VALUE depends on the platform being 32bit or 64bit. It is the maximum value of an integer in that platform
	 */
	// DynComm(
	//   std::string str
	//   ,Algorithm::ALGORITHM algorithm=Algorithm::ALGORITHM::LOUVAIN
	//   ,const Quality::QUALITY & quality=Quality::QUALITY::MODULARITY
	//   // , ProgramParameters algorithmParameters=argumentsDefault
	//   ,Rcpp::CharacterMatrix algorithmParameters=Rcpp::CharacterMatrix()
	// )
	//   :
	//   prmtrs(convertToParameters(algorithmParameters))
	//   ,rFE(prmtrs)
	//   ,dcb(&rFE, algorithm,quality, prmtrs)
	// {
	// }
	//
	// ReaderStringEdge rd("1 2\n1 3\n2 3\n3 6\n4 6\n4 5\n5 7\n6 7",parameters);

	/**
	 * Constructor for loading graph from R matrix
	 * Format of matrix is:
	 *   - one edge per line
	 *   - edge contains two or three columns, in this order, source node, destination node and, optionally, a weight
	 *   - if weight is not given, it will default to 1
	 *   - edges with a weight of exactly zero are not added
	 *   - source and destination nodes are integers between 0 and MAX_INTEGER_VALUE-1
	 *   - weight is a double
	 *   - MAX_INTEGER_VALUE depends on the platform being 32bit or 64bit. It is the maximum value of an integer in that platform
	 */
	// DynComm(
	//   Rcpp::NumericMatrix graphMatrix, Algorithm::ALGORITHM algorithm=ALGORITHM::LOUVAIN, std::string algorithmParameters=""
	// )
	//   :
	//   // algrthm(algorithm)
	//   // ,prmtrs(algorithmParameters)
	//   dcb(dataReader, algorithm, algorithmParameters)
	// {
	//   //load graph from R Matrix using R matrix reader
	//   // grph=toGraph(graphMatrix);
	// }

	/**
	 * Function to add and remove edges from the graph using a matrix.
	 * After successfully adding/removing, the algorithm will automatically run again.
	 * Any edge with a weight different from zero is inserted.
	 * Any edge with a weight of exactly zero is removed.
	 * The weight column
	 * Format of the matrix is:
	 *   - one edge per line
	 *   - edge contains two or three columns, in this order, source node, destination node and, optionally, a weight
	 *   - if weight is not given, it will default to 1
	 *   - edges with a weight of exactly zero are removed
	 *   - source and destination nodes are integers between 0 and MAX_INTEGER_VALUE-1
	 *   - weight is a double
	 *   - MAX_INTEGER_VALUE depends on the platform being 32bit or 64bit. It is the maximum value of an integer in that platform
	 * @return true if adding/removing succeeded
	 */
	// bool addRemoveEdgesMatrix(Rcpp::NumericMatrix edges) {
	//   //add edge
	//   dcb.addRemoveEdges(dataReader);
	//   //rerun algorithm
	//   dcb.run();
	// }

	/**
	 * Function to add and remove edges from the graph using a file.
	 * After successfully adding/removing, the algorithm will automatically run again.
	 * Any edge with a weight different from zero is inserted.
	 * Any edge with a weight of exactly zero is removed.
	 * The weight column
	 * Format of the file is:
	 *   - one edge per line
	 *   - edge contains two or three columns, in this order, source node, destination node and, optionally, a weight
	 *   - if weight is not given, it will default to 1
	 *   - edges with a weight of exactly zero are removed
	 *   - source and destination nodes are integers between 0 and MAX_INTEGER_VALUE-1
	 *   - weight is a double
	 *   - MAX_INTEGER_VALUE depends on the platform being 32bit or 64bit. It is the maximum value of an integer in that platform
	 * @return true if adding/removing succeeded
	 */
	bool addRemoveEdgesFile(std::string graphAddRemoveFile) {
		ProgramParameters p(prmtrs);
		p.filename=graphAddRemoveFile;
		ReaderFileEdge r(p);
		//add edge
		return dcb.addRemoveEdges(&r);
	}

	/**
	 * @return the current quality measure of the community mapping on the graph
	 */
	double quality(){
		return dcb.quality();
	}

	/**
	 * @return the number of communities
	 */
	int communityCount(){
		return dcb.communityCount();
	}

	/**
	 * @return a list of all communities
	 */
	Rcpp::NumericVector communities(){
		// return dcb.communities();
		typeCommunities c=dcb.communities();
		// COUT << "comm count="<< dcb.communityCount()<<"\n";
		// COUT << "comms="<< set::toString(dcb.communities())<<"\n";
		Rcpp::NumericVector v(c.size());
		int i=0;
		for(typeCommunities::const_iterator it=c.cbegin();it!=c.cend();++it){
			typeCommunity cc=*it;
			// COUT << "comm "<< cc <<" node count="<< dcb.communityNodeCount(cc)<<"\n";
			// COUT << "comm "<< cc <<" nodes="<< set::toString(dcb.nodes(cc))<<"\n";
			v[i]=cc;
			++i;
		}
		return v;
	}

	/**
	 * @return a list with information about the selected community. The information provided is:
	 *  - total weight of inner edges
	 *  - total weight of outer edges
	 *  - total weight of edges
	 *  - number of nodes in community
	 */
	// Rcpp::NumericVector community(int community){
	//   typeCommunities c=dcb.communities();
	//   Rcpp::NumericVector v(c.size());
	//   int i=1;
	//   for(typeCommunities::const_iterator it=c.cbegin();it!=c.cend();++it){
	//     typeCommunity cc=*it;
	//     v[i]=cc;
	//     ++i;
	//   }
	//   return v;
	// }

	typeWeight communityInnerEdgesWeight(int community){
		return dcb.communityInnerEdgesWeight(community);
	}

	//		int communityInnerEdgesCount(int community){return grph.i

	typeWeight communityTotalWeight(int community){
		return dcb.communityTotalWeight(community);
	}

	//		int communityTotalEdgesCount(int community){

	typeWeight communityEdgeWeight(typeCommunity source,typeCommunity destination)const{
		return dcb.communityEdgeWeight(source,destination);
	}

	int communityNodeCount(int community){
		return dcb.communityNodeCount(community);
	}

	typeCommunity community(typeNode node)const{
		return dcb.community(node);
	}

	unsigned int nodesCount()const{
		return dcb.nodesCount();
	}

	Rcpp::NumericVector nodesAll(){
		typeNodeList c=dcb.nodes();
		Rcpp::NumericVector v(c.size());
		int i=0;
		for(typeNodeListIteratorConst it=c.cbegin();it!=c.cend();++it){
			typeNode cc=*it;
			v[i]=cc;
			++i;
		}
		return v;
	}

	/**
	 * @return a list of all nodes belonging to the given community
	 */
	Rcpp::NumericVector nodes(int community){
		typeNodeList c=dcb.nodes(community);
		Rcpp::NumericVector v(c.size());
		int i=0;
		for(typeNodeListIteratorConst it=c.cbegin();it!=c.cend();++it){
			typeNode cc=*it;
			v[i]=cc;
			++i;
		}
		return v;
	}

	/**
	 * Get a snapshot of the current community mapping as a R Matrix
	 * Format of the file is:
	 *   - one node mapping per line
	 *   - mapping contains two columns, in this order, node and community
	 *   - node and community are integers between 0 and MAX_INTEGER_VALUE-1
	 *   - MAX_INTEGER_VALUE depends on the platform being 32bit or 64bit. It is the maximum value of an integer in that platform
	 */
	Rcpp::NumericMatrix communityMapping(bool diferential=true){
		Rcpp::NumericMatrix v(dcb.nodesCount(),2);
		typeNodeList c=dcb.nodes();
		int i=0;
		for(typeNodeListIteratorConst it=c.cbegin();it!=c.cend();++it){
			typeNode cc=*it;
			v(i,0)=cc;
			v(i,1)=dcb.community(cc);
			++i;
		}
		return v;
	}

	uint64 time(){return dcb.time();}

	/**
	 * Get a snapshot of the current community mapping written to file
	 */
	// bool snapshotFile(const std::string snapshotFile,bool diferential=true){
	//
	// }

};


// RCPP_EXPOSED_ENUM_NODECL(DynComm::ALGORITHM)
RCPP_EXPOSED_ENUM_NODECL(Algorithm::ALGORITHM)
RCPP_EXPOSED_ENUM_NODECL(Quality::QUALITY)

RCPP_MODULE(DynComm) {
	using namespace Rcpp;

	class_<DummyAlgorithm>("Algorithm")
    		  .default_constructor()
			  ;

	class_<DummyQuality>("Quality")
    		  .default_constructor()
			  ;

	enum_<Algorithm::ALGORITHM, DummyAlgorithm>("TypeOfAlgorithm")
    		  .value("LOUVAIN", Algorithm::ALGORITHM::LOUVAIN)
			  // .value("Enum1", ENUM1)
			  // .value("Enum2", ENUM2)
			  ;

	enum_<Quality::QUALITY, DummyQuality>("TypeOfQuality")
    		  .value("MODULARITY", Quality::QUALITY::MODULARITY)
			  .value("BALMOD", Quality::QUALITY::BALMOD)
			  // .value("Enum1", ENUM1)
			  // .value("Enum2", ENUM2)
			  ;

	// class_<ProgramParameters>("ProgramParameters")
	//   .constructor<ProgramParameters>()
	//   ;

	class_<DynComm>( "DynComm")
    		  // .constructor<std::string,DynComm::ALGORITHM, std::string>()
    		  // .constructor<int, DynComm::ALGORITHM, std::string>()
        		 .constructor< Algorithm::ALGORITHM, Quality::QUALITY, Rcpp::CharacterMatrix>()
				 // .constructor< int,Algorithm::ALGORITHM,  std::string>()
				 // .method("addRemoveEdgesMatrix", &DynComm::addRemoveEdgesMatrix)
				 .method("addRemoveEdgesFile", &DynComm::addRemoveEdgesFile)
				 .method("quality", &DynComm::quality)
				 .method("communityCount", &DynComm::communityCount)
				 .method("communities", &DynComm::communities)
				 .method("communityInnerEdgesWeight", &DynComm::communityInnerEdgesWeight)
				 .method("communityTotalWeight", &DynComm::communityTotalWeight)
         .method("communityEdgeWeight", &DynComm::communityEdgeWeight)
				 .method("communityNodeCount", &DynComm::communityNodeCount)
     .method("community", &DynComm::community)
     .method("nodesCount", &DynComm::nodesCount)
				 .method("nodesAll", &DynComm::nodesAll)
         .method("nodes", &DynComm::nodes)
         .method("communityMapping", &DynComm::communityMapping)
		 .method("time", &DynComm::time)
				 // .method("snapshotFile", &DynComm::snapshotFile)
				 // .method("snapshotMatrix", &DynComm::snapshotMatrix)
				 ;
}

/*
 * R example run
 *
 * parameters<-matrix(c("filename","test/full/as19971108.txt","-s","test/full/sequences"),2,2,TRUE)
 * dc<-new(DynComm,DynComm::Algorithm.LOUVAIN,DynComm::Quality.MODULARITY,parameters)
 * dc$communityCount()
 * dc$communities()
 * dc$communityNodeCount(1)
 * dc$nodes(1)
 * dc$communityMapping(TRUE)
 * dc$time()
 * dc$addRemoveEdgesFile("test/full/sequences/s0000000000.txt")
 *
 * or in one line
 *
 * parameters<-matrix(c("filename","test/full/as19971108.txt","-s","test/full/sequences"),2,2,TRUE);dc<-new(DynComm,DynComm::Algorithm.LOUVAIN,DynComm::Quality.MODULARITY,parameters);dc$communityCount();dc$communities();dc$communityNodeCount(1);dc$nodes(1);dc$communityMapping(TRUE);dc$time();dc$addRemoveEdgesFile("test/full/sequences/s0000000000.txt");dc$communityMapping(TRUE);dc$time()
 *
 */

#endif //FLAG_RCPP
