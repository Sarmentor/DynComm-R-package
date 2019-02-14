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
	virtual typeQuality quality()=0;

	/**
	 * @return the number of communities
	 */
	virtual int communityCount()=0;

	/**
	 * @return the list of all existing communities without any further information
	 */
	virtual typeCommunities communities()=0;

	/**
	 *
	 * @param community
	 * @return the sum of the weights of all inner edges of the selected community
	 */
	virtual typeWeight communityInnerEdgesWeight(int community)=0;

//	virtual int communityInnerEdgesCount(int community)=0; TODO

	/**
	 *
	 * @param community
	 * @return the sum of the weights of all edges of the selected community
	 */
	virtual typeWeight communityTotalWeight(int community)=0;

//	virtual int communityTotalEdgesCount(int community)=0; TODO

	/**
	 *
	 * @param community
	 * @return the amount of nodes in the selected community
	 */
	virtual int communityNodeCount(int community)=0;


	/**
	 * @return a list of all nodes belonging to the given community
	 */
	virtual typeNodeList nodes(int community)=0;

	/**
	 * Get a snapshot of the current community mapping
	 * The differential parameter will probably be moved inside the writer as a parameter in the next version of the source code
	 * @return true if the operation succeeded
	 */
	virtual bool results(WriterInterface * writer,bool differential=true)=0;

};

#endif /* SRC_DYNCOMMBASEINTERFACE_H_ */
