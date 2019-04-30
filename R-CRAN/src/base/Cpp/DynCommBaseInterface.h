/*
 * algorithmInterface.h
 *
 *  Created on: 02/02/2019
 *      Author: poltergeist0
 */

#ifndef SRC_DYNCOMMBASEINTERFACE_H_
#define SRC_DYNCOMMBASEINTERFACE_H_

#include "qualityInterface.h"
#include "reader.h"
#include "writer.h"
#include "systemDefines.h"

typedef ReaderInterface<Edge> ReaderEdgeBase;

/**
 * Dynamic Communities class API.
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
	 * @param reader is a Reader object that is responsible for converting the required data from its original format to Edges
	 * @return true if adding/removing succeeded
	 */
	virtual bool addRemoveEdges(ReaderEdgeBase * reader)=0;


	/**
	 * @return the current quality measure of the community mapping on the graph
	 */
	virtual typeQuality quality()const=0;

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
	 * @return the neighboring communities of the given community
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
	 * Get a snapshot of the current community mapping
	 * The differential parameter will probably be moved inside the writer as a parameter in the next version of the source code
	 * @return true if the operation succeeded
	 */
	virtual bool communityMapping(WriterInterface * writer,bool differential=true)const=0;

	virtual typeLinksRangeConst neighbours(typeVertex vertex)const=0;

	virtual typeWeight weight(const typeVertex & source, const typeVertex & destination) const =0;

	virtual uint64 time(bool accumulated=true)const=0;

};

#endif /* SRC_DYNCOMMBASEINTERFACE_H_ */
