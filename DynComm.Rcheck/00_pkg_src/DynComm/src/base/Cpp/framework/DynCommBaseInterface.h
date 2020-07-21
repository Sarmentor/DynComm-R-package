/************************************************************************
 ************************* Developer Notice *****************************
 ************************************************************************
 * @details
 *
 * Interface for DynComm implemented in C++11.
 *
 *
 * @author poltergeist0
 *
 * @date 2019-01-01
 ************************************************************************
 ************************************************************************
 ************************************************************************/

#ifndef SRC_DYNCOMMBASEINTERFACE_H_
#define SRC_DYNCOMMBASEINTERFACE_H_

#include "criterion/criterionInterface.h"
#include "io/reader.h"
#include "io/writer.h"
#include "systemDefines.h"

typedef ReaderInterface<Edge> ReaderEdgeBase;

/**
 * @brief Interface for DynComm.
 *
 * @details
 * Class that defines the interface for DynComm as seen by the user.
 *
 * It has as base the interface of classes graph, criterion and algorithm.
 *
 *
 * @author poltergeist0
 *
 * @date 2019-01-01
 */
class DynCommBaseInterface {
public:
	/**
	 * Destructor
	 */
	virtual ~DynCommBaseInterface(){}

	/**
	 * Function to add and remove edges from the graph.
	 * Any edge with a weight different from zero is inserted.
	 * Any edge with a weight of exactly zero is removed.
	 *
	 * @param reader is a Reader object that is responsible for converting the required data from its original format to Edges
	 * @return true if adding/removing succeeded
	 */
	virtual bool addRemoveEdges(ReaderEdgeBase * reader)=0;


	/**
	 * compute the quality of the current partition scheme
	 *
	 * @return the quality value
	 */
	virtual typeCriterion quality()const=0;

	/**
	 * @return the number of communities
	 */
	virtual int communityCount()const=0;

	/**
	 * @return the list of all existing communities without any further information
	 */
	virtual typeCommunities communities()const=0;

	/**
	 *
	 * @param community
	 * @return the neighbouring communities of the given community
	 */
	virtual typeLinksRangeConst communityNeighbours(typeCommunity community)const=0;

	/**
	 *
	 * @param community
	 * @return the sum of the weights of all inner edges of the selected community
	 */
	virtual typeWeight communityInnerEdgesWeight(typeCommunity community)const=0;

//	virtual int communityInnerEdgesCount(int community)=0; TODO

	/**
	 *
	 * @param community
	 * @return the sum of the weights of all edges of the selected community
	 */
	virtual typeWeight communityTotalWeight(typeCommunity community)const=0;

//	virtual int communityTotalEdgesCount(int community)=0; TODO

	/**
	 *
	 * @param source
	 * @param destination
	 * @return the weight of the edge that connects two given communities
	 */
	virtual typeWeight communityEdgeWeight(typeCommunity source,typeCommunity destination)const=0;

	/**
	 *
	 * @param community
	 * @return the amount of vertices in the selected community
	 */
	virtual int communityVertexCount(typeCommunity community)const=0;


	/**
	 *
	 * @param vertex
	 * @return the community of a given vertex
	 */
	virtual typeCommunity community(typeVertex vertex)const=0;

	/**
	 * @return the number of existing vertices
	 */
	virtual unsigned int vertexCount()const=0;

	/**
	 * @return the list of all existing vertices
	 */
	virtual typeVertexList vertices()const=0;

	/**
	 * @return a list of all vertices belonging to the given community
	 */
	virtual typeVertexList vertices(typeCommunity community)const=0;

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
	virtual bool communityMapping(WriterInterface * writer,bool communityFirst=true,bool differential=true)const=0;

	/**
	 * @return pointers to the first and last neighbour of the vertex
	 */
	virtual typeLinksRangeConst neighbours(typeVertex vertex)const=0;

	/**
	 * @return the weight of an edge
	 */
	virtual typeWeight weight(const typeVertex & source, const typeVertex & destination) const =0;

	/**
	 * @return the time spent on processing
	 */
	virtual uint64 time(bool accumulated=true)const=0;

};

#endif /* SRC_DYNCOMMBASEINTERFACE_H_ */
