/************************************************************************
 ************************* Developer Notice *****************************
 ************************************************************************
 * @details
 *
 * This file defines the base class for DynComm. It implements the base
 * of the user interface dispatching function calls accordingly.
 *
 * There should never be any reason to change it unless the API or user
 * interface changes.
 *
 *
 * @author poltergeist0
 *
 * @date 2019-02-02
 ************************************************************************
 ************************************************************************
 ************************************************************************/

#ifndef SRC_DYNCOMMSINGLE_H_
#define SRC_DYNCOMMSINGLE_H_

#include "defines.h"
#include "DynCommBaseInterface.h"
// #include "algorithm.h"
#include "criterion/criterion.h"
#include "time/timeFunctions.h"
// #include "mapReversable.h"
#include "../algorithms/algorithmList.h"

/**
 * @brief Dynamic Communities base class.
 *
 * @details
 *
 * Simple class that only converts between data types and dispatches function
 * calls to the correct object (graph, criterion or algorithm).
 *
 * @author poltergeist0
 *
 * @date 2019-02-02
 */
class DynCommSingle:private DynCommBaseInterface{
private:
	
	const ALGORITHM & algrthm;//algorithm
  const Criterion::CRITERION & qlt;//criterion
  ProgramParameters prmtrs;//algorithm parameters
  
	/*
	 * map used to keep the old relation between the graph vertices and their communities
	 * It is used to keep track of vertices that change communities between iterations
	 * of the algorithm to reply to differential=true communityMapping calls.
	 */
	typeCommunityList oldCommunities;

public:
	/**
	 * Default constructor not acceptable.
	 * Must be passed at least the chosen algorithm and the graph
	 */
	DynCommSingle()=delete;

	/**
	 * Constructor
	 */
	DynCommSingle(
			 ALGORITHM algorithm=ALGORITHM::LOUVAIN
			,const Criterion::CRITERION & quality=Criterion::CRITERION::MODULARITY
			, ProgramParameters & algorithmParameters=argumentsDefault
	)
	:
		algrthm(algorithm)
	,qlt(quality)
	,prmtrs(algorithmParameters)
	{
	}

	/**
	 * Function to add and remove edges from the graph.
	 * Any edge with a weight different from zero is inserted.
	 * Any edge with a weight of exactly zero is removed.
	 * @return true if adding/removing succeeded
	 */
	bool addRemoveEdges(ReaderInterface<Edge> * reader){
		return false;
	}

	/**
	 * @return the current quality measure of the community mapping on the graph
	 */
	typeCriterion quality()const {
		return 0;
	}

	/**
	 * @return the number of existing communities
	 */
	int communityCount()const {
		return 0;
	}

	/**
	 * @return the list of all existing communities
	 */
	typeCommunities communities()const {
		return typeCommunities();
	}

	/**
	 * Get the number of community to community edges in the graph
	 *
	 * @return the number of edges
	 */
	typeWeight communitiesEdgeCount()const {
		return 0;
	}

	/**
	 * Get the communities that are neighbours of the given community
	 *
	 * @param community
	 * @return the neighbouring communities
	 */
	typeLinksRangeConst communityNeighbours(typeCommunity community)const {
		return typeLinksRangeConst();
	}

	/**
	 * Get the sum of the weights of all edges of the given community where both
	 * vertices of an edge belong to the given community.
	 *
	 * @param community
	 * @return the sum of the weights of all inner edges
	 */
	typeWeight communityInnerEdgesWeight(typeCommunity community)const {
	  return 0;
	}

	//		int communityInnerEdgesCount(int community){return grph.i

	/**
	 * Get the sum of the weights of all edges of the given community.
	 *
	 * @param community
	 * @return the sum of the weights of all edges
	 */
	typeWeight communityTotalWeight(typeCommunity community)const {
	  return 0;
	}

	//		int communityTotalEdgesCount(int community){

	/**
	 * Get the weight of the edge form by two communities
	 *
	 * @param source community
	 * @param destination community
	 * @return the weight of the edge
	 */
	typeWeight communityEdgeWeight(typeCommunity source,typeCommunity destination)const{
		return 0;
	}

	/**
	 * Get the number of vertices in the given community
	 *
	 * @param community
	 * @return the number of vertices
	 */
	int communityVertexCount(typeCommunity community)const {
		return 0;
	}

	/**
	 *
	 * @param node
	 * @return the community of the given vertex or a special community of noGRoup if the vertex does not exist
	 */
	typeCommunity community(typeVertex vertex)const{
		return noGroup;
	}

	/**
	 * @return the number of vertices in the graph
	 */
	unsigned int vertexCount()const{
		return 0;
	}

	/**
	 * @return a constant set with all vertices
	 */
	typeVertexList vertices()const{
		return typeVertexList();
	}

	/**
	 * @return a list of all vertices belonging to the given community
	 */
	typeVertexList vertices(typeCommunity community)const {
		return typeVertexList();
	}

	/**
	 * Get the number of vertex to vertex edges in the graph
	 *
	 * @return the number of edges
	 */
	typeWeight edgeCount()const {
		return 0;
	}

	/**
	 * Get a snapshot of the current community mapping.
	 * If communityFirst is true the result will be one community per line with
	 * each line being a community followed by a list of vertices. If false, the
	 * result will be a vertex per line with each line being a single vertex and
	 * a single community.
	 * The differential parameter will probably be moved inside the writer as a parameter
	 *
	 * @param writer
	 * @param communityFirst if true returns a community followed by a list of vertices, otherwise a vertex and its community
	 * @param differential return only what changed in the last iteration of the algorithm
	 * @return true if the operation succeeded
	 */
	bool communityMapping(WriterInterface * writer,bool communityFirst=true,bool differential=true) const{
		return false;
	}

	/**
	 * @brief Get the neighbours of a vertex
	 * @details The pair can change between calls if the Indexed edge list is modified.
	 * @param vertex
	 * @return pointers to the first and last neighbour of the vertex
	 */
	typeLinksRangeConst neighbours(typeVertex vertex)const {
		return typeLinksRangeConst();
	}

	/**
	 * @return the weight of the edge
	 */
	typeWeight weight(const typeVertex & source, const typeVertex & destination) const {
		return 0;
	}

	/**
	 *
	 * @return the total processing time in nanoseconds
	 */
	// uint64 time(bool accumulated=true)const{
	//   if(!accumulated) return timeProcessing;
	//   return timeTotal;
	// }

};

#endif /* SRC_DYNCOMMSINGLE_H_ */
