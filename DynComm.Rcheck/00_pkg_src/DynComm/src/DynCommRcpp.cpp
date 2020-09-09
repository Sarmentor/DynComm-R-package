/************************************************************************
 ************************* Developer Notice *****************************
 ************************************************************************
 * Description:
 * 
 * R adaptation interface for Dynamic Communities algorithms implemented 
 * in C++11.
 * 
 * There should never be any reason to change it unless the API or the 
 * user interface changes.
 * 
 * Add your C++ algorithms in the "base/Cpp/DynCommBase.h" file.
 * 
 * Author: poltergeist0
 * 
 * Date: 2019-01-01
 ************************************************************************
 ************************************************************************
 ************************************************************************/

#include "base/Cpp/framework/defines.h"

#ifdef FLAG_RCPP

#include "base/Cpp/DynCommBase.h"

/**
 * Dummy class used to implement Enumeration like variables and function parameters in R from C++.
 * May be removed in the future when R starts supporting C++ class enumerations.
 */
// class DummyEnum {
// 	int x;
// 	int get_x() {return x;}
// };
// 
// class DummyAlgorithm: public DummyEnum{
// 
// };
// 
// class DummyQuality: public DummyEnum{
// 
// };

/**
 * Dynamic Communities class that handles all the IO for the base class.
 * This (file/class) is the R version.
 */
class DynCommRcpp{
private:
	ProgramParameters prmtrs;
	DynCommBase dcb;//dynamic communities base class where all the logic is

	ProgramParameters convertToParameters(Rcpp::CharacterMatrix algorithmParameters=Rcpp::CharacterMatrix()){
		int nrow = algorithmParameters.nrow();
		// int ncol = algorithmParameters.ncol();
		ProgramParameters p;
		// COUT << nrow << "\n";
		for (int i = 0; i < nrow; ++i) {
		  // COUT << i << " c1="<< algorithmParameters(i,0) << " c2="<< algorithmParameters(i,1)<< " \n";
		  if(algorithmParameters(i,0)=="filename"){
		    // COUT << i << " filename\n";
		    // char *a[]={"DynCommRcpp",algorithmParameters(i,1)};
		    // parse_args(2,a,p);
		    // const char a='f';//algorithmParameters(i,1);
		    parse_arg("f",std::string(algorithmParameters(i,1)),p);
		  }
		  else{
			  // char *a[]={"DynCommRcpp",algorithmParameters(i,0),algorithmParameters(i,1)};
			  // parse_args(3,a,p);
			  // char *a[]={"DynCommRcpp",,};
			  parse_arg(std::string(algorithmParameters(i,0)),std::string(algorithmParameters(i,1)),p);
		  }
		}
		return p;
	}

public:
	/**
	 * Default constructor not acceptable.
	 * Must be passed at least the chosen algorithm and the graph
	 */
	DynCommRcpp()=delete;

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
	DynCommRcpp(
	    ALGORITHM algorithm=ALGORITHM::LOUVAIN
			,const Criterion::CRITERION & quality=Criterion::CRITERION::MODULARITY
			,Rcpp::CharacterMatrix algorithmParameters=Rcpp::CharacterMatrix()
	)
	:
		prmtrs(convertToParameters(algorithmParameters))
	,dcb(algorithm,quality, prmtrs)
	{
	  // COUT << "prmtrs\n" << prmtrs.toString();
	}

	/**
	 * Function to add and remove edges from the graph using a file.
	 * After successfully adding/removing, the algorithm will automatically run again.
	 * Any edge with a weight different from zero is inserted.
	 * Any edge with a weight of exactly zero is removed.
	 * The weight column is optional
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
	bool addRemoveEdgesFile(std::string graphAddRemove) {
		ProgramParameters p(prmtrs);
		p.filename=graphAddRemove;
		ReaderFileEdge r(p);
		//add edge
		return dcb.addRemoveEdges(&r);
	}

	/**
	 * Function to add and remove edges from the graph using a matrix.
	 * After successfully adding/removing, the algorithm will automatically run again.
	 * Any edge with a weight different from zero is inserted.
	 * Any edge with a weight of exactly zero is removed.
	 * The weight column is optional
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
	bool addRemoveEdgesMatrix(Rcpp::NumericMatrix graphAddRemove=Rcpp::NumericMatrix()) {
	  ProgramParameters p(prmtrs);
	  ReaderMatrixEdge r(graphAddRemove,p);
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
		typeCommunities c=dcb.communities();
		Rcpp::NumericVector v(c.size());
		int i=0;
		for(typeCommunities::const_iterator it=c.cbegin();it!=c.cend();++it){
			typeCommunity cc=*it;
			v[i]=cc;
			++i;
		}
		return v;
	}

	/**
	 * Get the number of community to community edges in the graph
	 *
	 * @return the number of edges
	 */
	typeWeight communitiesEdgeCount()const {
		return dcb.communitiesEdgeCount();
	}

	/**
	 * @return a matrix of neighbouring communities of the given community and the weight of their edge
	 */
	Rcpp::NumericMatrix communityNeighbours(typeCommunity community)const{
		typeLinksRangeConst c=dcb.communityNeighbours(community);
		int i=0;
		for(typeLinksIteratorConst it=c.first;it!=c.second;++it){
			++i;
		}
		Rcpp::NumericMatrix v(i,2);
		Rcpp::colnames(v) = Rcpp::CharacterVector::create("neighbour","weight");
		i=0;
		for(typeLinksIteratorConst it=c.first;it!=c.second;++it){
			typeLinksPair cc=*it;
			v(i,0)=cc.second.destination();
			v(i,1)=cc.second.weight();
			++i;
		}
		return v;
	}

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

	unsigned int vertexCount()const{
		return dcb.vertexCount();
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
	 * Get the number of vertex to vertex edges in the graph
	 *
	 * @return the number of edges
	 */
	typeWeight edgeCount()const {
	  return dcb.edgeCount();
	}
	
	/**
	 * Get a snapshot of the current community mapping as a R Matrix
	 * Format of the file is:
	 *   - one vertex mapping per line
	 *   - mapping contains two columns, in this order, vertex and community
	 *   - vertex and community are integers between 0 and MAX_INTEGER_VALUE-1
	 *   - MAX_INTEGER_VALUE depends on the platform being 32bit or 64bit. It is the maximum value of an integer in that platform
	 */
	Rcpp::NumericMatrix communityMappingMatrix(bool differential=true){
		Rcpp::NumericMatrix v(dcb.vertexCount(),2);
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

	/**
	 * Get a snapshot of the current community mapping as a R Matrix
	 * Format of the file is:
	 *   - one vertex mapping per line
	 *   - mapping contains two columns, in this order, vertex and community
	 *   - vertex and community are integers between 0 and MAX_INTEGER_VALUE-1
	 *   - MAX_INTEGER_VALUE depends on the platform being 32bit or 64bit. It is the maximum value of an integer in that platform
	 */
	Rcpp::NumericMatrix communityMappingFile(bool communityFirst=true,bool differential=true,const std::string & file="communityMapping.txt"){
	  Rcpp::NumericMatrix v(1,1);
	  ProgramParameters p(prmtrs);
	  p.outfilename=file;
	  WriterFile w(p);
	  bool b=dcb.communityMapping(&w,communityFirst);
	  if(b) v(1,1)=true;
	  else v(1,1)=false;
	  return v;
	}
	
	Rcpp::CharacterMatrix results(bool differential=true){
		Rcpp::CharacterMatrix v(1,2);
		v(0,0)="time delta";
		v(0,1)=std::to_string(dcb.time(true));
    return v;
	}

	/**
	 * @return a list of neighbouring communities of the given community
	 */
	Rcpp::NumericMatrix neighbours(typeVertex vertex)const{
		typeLinksRangeConst c=dcb.neighbours(vertex);
		int i=0;
		for(typeLinksIteratorConst it=c.first;it!=c.second;++it){
			++i;
		}
		Rcpp::NumericMatrix v(i,2);
		Rcpp::colnames(v) = Rcpp::CharacterVector::create("neighbour","weight");
		i=0;
		for(typeLinksIteratorConst it=c.first;it!=c.second;++it){
			typeLinksPair cc=*it;
			v(i,0)=cc.second.destination();
			v(i,1)=cc.second.weight();
			++i;
		}
		return v;
	}

	typeWeight edgeWeight(typeVertex source,typeVertex destination)const{
		return dcb.weight(source,destination);
	}

	uint64 time(bool differential=false){return dcb.time(differential);}

	uint64 version(){return dcb.version();}

};

// // [[Rcpp::export]]
double currentTime() {
  return Time::currentTime();
}

RCPP_EXPOSED_ENUM_NODECL(ALGORITHM)
RCPP_EXPOSED_ENUM_NODECL(Criterion::CRITERION)

RCPP_MODULE(DynCommRcppModule) {
	using namespace Rcpp;

  function("currentTime"   , &currentTime );
  
// 	class_<DummyAlgorithm>("Algorithm")
//     		  .default_constructor()
// 			  ;
// 
// 	class_<DummyQuality>("Quality")
//     		  .default_constructor()
// 			  ;
// 
// 	enum_<ALGORITHM, DummyAlgorithm>("TypeOfAlgorithm")
//     		  .value("LOUVAIN", ALGORITHM::LOUVAIN)
// 			  ;
// 
// 	enum_<Criterion::CRITERION, DummyQuality>("TypeOfQuality")
//     		  .value("MODULARITY", Criterion::CRITERION::MODULARITY)
//   			  .value("BALMOD", Criterion::CRITERION::BALMOD)
// 			  ;

	class_<DynCommRcpp>( "DynCommRcpp")
    		 .constructor< ALGORITHM, Criterion::CRITERION, Rcpp::CharacterMatrix>()
				 .method("addRemoveEdgesMatrix", &DynCommRcpp::addRemoveEdgesMatrix)
				 .method("addRemoveEdgesFile", &DynCommRcpp::addRemoveEdgesFile)
				 .method("quality", &DynCommRcpp::quality)
				 .method("results", &DynCommRcpp::results)
				 .method("communityCount", &DynCommRcpp::communityCount)
				 .method("communities", &DynCommRcpp::communities)
				 .method("communitiesEdgeCount", &DynCommRcpp::communitiesEdgeCount)
				 .method("communityInnerEdgesWeight", &DynCommRcpp::communityInnerEdgesWeight)
				 .method("communityTotalWeight", &DynCommRcpp::communityTotalWeight)
         .method("communityEdgeWeight", &DynCommRcpp::communityEdgeWeight)
				 .method("communityVertexCount", &DynCommRcpp::communityVertexCount)
         .method("community", &DynCommRcpp::community)
         .method("communityNeighbours", &DynCommRcpp::communityNeighbours)
	       .method("vertexCount", &DynCommRcpp::vertexCount)
				 .method("verticesAll", &DynCommRcpp::verticesAll)
         .method("vertices", &DynCommRcpp::vertices)
         .method("edgeCount", &DynCommRcpp::edgeCount)
         .method("communityMappingFile", &DynCommRcpp::communityMappingFile)
         .method("communityMappingMatrix", &DynCommRcpp::communityMappingMatrix)
         .method("neighbours", &DynCommRcpp::neighbours)
         .method("edgeWeight", &DynCommRcpp::edgeWeight)
		     .method("time", &DynCommRcpp::time)
		     .method("version", &DynCommRcpp::version)
				 ;
}

/*
 * R example run
 *
 * parameters<-matrix(c("filename","test/full/as19971108.txt","-s","test/full/sequences"),2,2,TRUE)
 * dc<-new(DynCommRcpp,DynCommRcpp::Algorithm.LOUVAIN,DynCommRcpp::Quality.MODULARITY,parameters)
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
 * parameters<-matrix(c("filename","test/full/as19971108.txt","-s","test/full/sequences"),2,2,TRUE);dc<-new(DynCommRcpp,DynCommRcpp::Algorithm.LOUVAIN,DynCommRcpp::Quality.MODULARITY,parameters);dc$communityCount();dc$communities();dc$communityVertexCount(1);dc$vertices(1);dc$communityMapping(TRUE);dc$time()
 *
 */

#endif //FLAG_RCPP
