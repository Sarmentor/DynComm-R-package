/*
 * R adaptation interface for Dynamic Communities algorithms.
 *
 *
 */


#include <Rcpp.h>
#include "DynComm.h"

/**
 * Dummy class used to implement Enumeration like variables and function parameters in R from C++.
 * May be removed in the future when R starts suporting C++ class enumerations.
 */
class Dummy {
  int x;
  int get_x() {return x;}
};

/**
 * Enumeration with the list of supported Dynamic Communities algorithms.
 * This enumeration must start at 1 since R indexing (for arrays, etc) starts at 1 instead of 0.
 * Otherwise C++ would assign the first algorithm a 0.
 * This may be moved inside the class in the future when R supports enumerations inside classes.
 */
enum ALGORITHM:int{LOUVAIN=1};

/**
 * Dynamic Communities class that handles all the IO for the base class.
 * This (file/class) is the R version.
 */
class DynComm {
public:
  // enum ALGORITHM:int{LOUVAIN=1};

private:
  // ALGORITHM algrthm=ALGORITHM::LOUVAIN;//algorithm selection
  // std::string grph;//graph with edges
  // std::string prmtrs;//algorithm parameters
  DynCommBase dcb;//dynamic communities base class where all the logic is
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
  *   - source and destination nodes are integers between 0 and MAX_INTEGER_VALUE
  *   - weight is a double
  *   - MAX_INTEGER_VALUE depends on the platform being 32bit or 64bit. It is the maximum value of an integer in that platform
  */
  DynComm(
    std::string graphFile, ALGORITHM algorithm=ALGORITHM::LOUVAIN, std::string algorithmParameters=""
  )
    :
    // algrthm(algorithm)
    // ,prmtrs(algorithmParameters)
    dcb(dataReader, algorithm, algorithmParameters)
  {
    //load graph from file using file reader
    // grph=toGraph(graphFile);
  }

  /**
  * Constructor for loading graph from R matrix
  * Format of matrix is:
  *   - one edge per line
  *   - edge contains two or three columns, in this order, source node, destination node and, optionally, a weight
  *   - if weight is not given, it will default to 1
  *   - edges with a weight of exactly zero are not added
  *   - source and destination nodes are integers between 0 and MAX_INTEGER_VALUE
  *   - weight is a double
  *   - MAX_INTEGER_VALUE depends on the platform being 32bit or 64bit. It is the maximum value of an integer in that platform
  */
  DynComm(
    Rcpp::NumericMatrix graphMatrix, ALGORITHM algorithm=ALGORITHM::LOUVAIN, std::string algorithmParameters=""
  )
    :
    // algrthm(algorithm)
    // ,prmtrs(algorithmParameters)
    dcb(dataReader, algorithm, algorithmParameters)
  {
    //load graph from R Matrix using R matrix reader
    // grph=toGraph(graphMatrix);
  }

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
  *   - source and destination nodes are integers between 0 and MAX_INTEGER_VALUE
  *   - weight is a double
  *   - MAX_INTEGER_VALUE depends on the platform being 32bit or 64bit. It is the maximum value of an integer in that platform
  * @return true if adding/removing succeeded
  */
  bool addRemoveEdgesMatrix(Rcpp::NumericMatrix edges) {
    //add edge
    dcb.addRemoveEdges(dataReader);
  }

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
   *   - source and destination nodes are integers between 0 and MAX_INTEGER_VALUE
   *   - weight is a double
   *   - MAX_INTEGER_VALUE depends on the platform being 32bit or 64bit. It is the maximum value of an integer in that platform
   * @return true if adding/removing succeeded
   */
  bool addRemoveEdgesFile(std::string graphAddRemoveFile) {
    //add edge
    dcb.addRemoveEdges(dataReader);
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
  Rcpp::List communities(){

  }

  /**
  * @return a list with information about the selected community. The information provided is:
  *  - total weight of inner edges
  *  - total weight of outer edges
  *  - total weight of edges
  *  - number of nodes in community
  */
  Rcpp::List community(int community){

  }

  /**
  * @return a list of all nodes belonging to the given community
  */
  Rcpp::List nodes(int community){

  }

  /**
  * Get a snapshot of the current community mapping as a R Matrix
  * Format of the file is:
  *   - one node mapping per line
  *   - edge contains two or three columns, in this order, source node, destination node and, optionally, a weight
  *   - if weight is not given, it will default to 1
  *   - edges with a weight of exactly zero are removed
  *   - source and destination nodes are integers between 0 and MAX_INTEGER_VALUE
  *   - weight is a double
  *   - MAX_INTEGER_VALUE depends on the platform being 32bit or 64bit. It is the maximum value of an integer in that platform
  */
  Rcpp::NumericMatrix snapshotMatrix(bool diferential=true){

  }

  /**
  * Get a snapshot of the current community mapping written to file
  */
  bool snapshotFile(const std::string snapshotFile,bool diferential=true){

  }

};


// RCPP_EXPOSED_ENUM_NODECL(DynComm::ALGORITHM)
RCPP_EXPOSED_ENUM_NODECL(ALGORITHM)

  RCPP_MODULE(DynComm) {
    using namespace Rcpp;

    class_<Dummy>("ALGORITHM")
      .default_constructor()
    ;
    enum_<ALGORITHM, Dummy>("EnumType")
      .value("LOUVAIN", LOUVAIN)
    // .value("Enum1", ENUM1)
    // .value("Enum2", ENUM2)
    ;

    class_<DynComm>( "DynComm")
      // .constructor<std::string,DynComm::ALGORITHM, std::string>()
      // .constructor<int, DynComm::ALGORITHM, std::string>()
         .constructor< std::string,ALGORITHM,  std::string>()
         .constructor< int,ALGORITHM,  std::string>()
         .method("addRemoveEdgesMatrix", &DynComm::addRemoveEdgesMatrix)
         .method("addRemoveEdgesFile", &DynComm::addRemoveEdgesFile)
         .method("quality", &DynComm::quality)
         .method("communityCount", &DynComm::communityCount)
         .method("communities", &DynComm::communities)
         .method("community", &DynComm::community)
         .method("nodes", &DynComm::nodes)
         .method("snapshotFile", &DynComm::snapshotFile)
         .method("snapshotMatrix", &DynComm::snapshotMatrix)
      ;
  }
