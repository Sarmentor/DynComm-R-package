/*
 * R adaptation interface for Dynamic Communities algorithms.
 *
 *
 */

#include "base/Cpp/defines.h"

#ifdef FLAG_RCPP

// #include <Rcpp.h>
#include "base/Cpp/DynCommBase.h"

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
class DynCommR{
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
	DynCommR()=delete;

	/**
	 * Constructor for loading graph from white character (tab or space) separated values file
	 * Format of file is:
	 *   - one edge per line
	 *   - edge contains two or three values separated by a white space, in this order, source vertex, destination vertex and, optionally, a weight
	 *   - if weight is not given, it will default to 1
	 *   - edges with a weight of exactly zero are not added
	 *   - source and destination vertices are integers between 0 and MAX_INTEGER_VALUE-1
	 *   - weight is a double
	 *   - MAX_INTEGER_VALUE depends on the platform being 32bit or 64bit. It is the maximum value of an integer in that platform
	 */
	DynCommR(
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
	 *   - edge contains two or three values separated by a white space, in this order, source vertex, destination vertex and, optionally, a weight
	 *   - if weight is not given, it will default to 1
	 *   - edges with a weight of exactly zero are not added
	 *   - source and destination vertices are integers between 0 and MAX_INTEGER_VALUE-1
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
	 *   - edge contains two or three columns, in this order, source vertex, destination vertex and, optionally, a weight
	 *   - if weight is not given, it will default to 1
	 *   - edges with a weight of exactly zero are not added
	 *   - source and destination vertices are integers between 0 and MAX_INTEGER_VALUE-1
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
	 *   - edge contains two or three columns, in this order, source vertex, destination vertex and, optionally, a weight
	 *   - if weight is not given, it will default to 1
	 *   - edges with a weight of exactly zero are removed
	 *   - source and destination vertices are integers between 0 and MAX_INTEGER_VALUE-1
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
	 *   - edge contains two or three columns, in this order, source vertex, destination vertex and, optionally, a weight
	 *   - if weight is not given, it will default to 1
	 *   - edges with a weight of exactly zero are removed
	 *   - source and destination vertices are integers between 0 and MAX_INTEGER_VALUE-1
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
			// COUT << "comm "<< cc <<" vertex count="<< dcb.communityVertexCount(cc)<<"\n";
			// COUT << "comm "<< cc <<" vertices="<< set::toString(dcb.vertices(cc))<<"\n";
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
	 *  - number of vertices in community
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

	int communityVertexCount(int community){
		return dcb.communityVertexCount(community);
	}

	typeCommunity community(typeVertex vertex)const{
		return dcb.community(vertex);
	}

	unsigned int verticesCount()const{
		return dcb.verticesCount();
	}

	Rcpp::NumericVector verticesAll(){
		typeVertexList c=dcb.vertices();
		Rcpp::NumericVector v(c.size());
		int i=0;
		for(typeVertexListIteratorConst it=c.cbegin();it!=c.cend();++it){
			typeVertex cc=*it;
			v[i]=cc;
			++i;
		}
		return v;
	}

	/**
	 * @return a list of all vertices belonging to the given community
	 */
	Rcpp::NumericVector vertices(int community){
		typeVertexList c=dcb.vertices(community);
		Rcpp::NumericVector v(c.size());
		int i=0;
		for(typeVertexListIteratorConst it=c.cbegin();it!=c.cend();++it){
			typeVertex cc=*it;
			v[i]=cc;
			++i;
		}
		return v;
	}

	/**
	 * Get a snapshot of the current community mapping as a R Matrix
	 * Format of the file is:
	 *   - one vertex mapping per line
	 *   - mapping contains two columns, in this order, vertex and community
	 *   - vertex and community are integers between 0 and MAX_INTEGER_VALUE-1
	 *   - MAX_INTEGER_VALUE depends on the platform being 32bit or 64bit. It is the maximum value of an integer in that platform
	 */
	Rcpp::NumericMatrix communityMapping(bool diferential=true){
		Rcpp::NumericMatrix v(dcb.verticesCount(),2);
		typeVertexList c=dcb.vertices();
		int i=0;
		for(typeVertexListIteratorConst it=c.cbegin();it!=c.cend();++it){
			typeVertex cc=*it;
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

	class_<DynCommR>( "DynCommR")
    		  // .constructor<std::string,DynComm::ALGORITHM, std::string>()
    		  // .constructor<int, DynComm::ALGORITHM, std::string>()
        		 .constructor< Algorithm::ALGORITHM, Quality::QUALITY, Rcpp::CharacterMatrix>()
				 // .constructor< int,Algorithm::ALGORITHM,  std::string>()
				 // .method("addRemoveEdgesMatrix", &DynComm::addRemoveEdgesMatrix)
				 .method("addRemoveEdgesFile", &DynCommR::addRemoveEdgesFile)
				 .method("quality", &DynCommR::quality)
				 .method("communityCount", &DynCommR::communityCount)
				 .method("communities", &DynCommR::communities)
				 .method("communityInnerEdgesWeight", &DynCommR::communityInnerEdgesWeight)
				 .method("communityTotalWeight", &DynCommR::communityTotalWeight)
         .method("communityEdgeWeight", &DynCommR::communityEdgeWeight)
				 .method("communityVertexCount", &DynCommR::communityVertexCount)
     .method("community", &DynCommR::community)
     .method("verticesCount", &DynCommR::verticesCount)
				 .method("verticesAll", &DynCommR::verticesAll)
         .method("vertices", &DynCommR::vertices)
         .method("communityMapping", &DynCommR::communityMapping)
		 .method("time", &DynCommR::time)
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
 * dc$communityVertexCount(1)
 * dc$vertices(1)
 * dc$communityMapping(TRUE)
 * dc$time()
 *dc$addRemoveEdgesFile("test/full/sequences/s0000000000.txt")
 *
 * or in one line
 *
 * parameters<-matrix(c("filename","test/full/as19971108.txt","-s","test/full/sequences"),2,2,TRUE);dc<-new(DynComm,DynComm::Algorithm.LOUVAIN,DynComm::Quality.MODULARITY,parameters);dc$communityCount();dc$communities();dc$communityVertexCount(1);dc$vertices(1);dc$communityMapping(TRUE);dc$time()
 *
 */

#endif //FLAG_RCPP
