/*
 * algorithmInterface.h
 *
 *  Created on: 02/02/2019
 *      Author: poltergeist0
 */

#ifndef SRC_ALGORITHMINTERFACE_H_
#define SRC_ALGORITHMINTERFACE_H_

#include "DynCommBaseInterface.h"

/**
 * Algorithms API.
 */
class AlgorithmInterface{
protected:
	/**
	 * Function to execute before adding or removing a single edge from the graph.
	 * It is used internally by the constructor and the addRemoveEdges function.
	 * Any edge with a weight different from zero is inserted.
	 * Any edge with a weight of exactly zero is removed.
	 * @param source
	 * @param destination
	 * @param weight
	 * @return true if adding/removing succeeded
	 */
	virtual bool addRemoveEdgePre(const typeNode & source, const typeNode & destination, const typeWeight & weight=1.0)=0;

	/**
	 * Function execute after adding or removing a single edge from the graph.
	 * It is used internally by the constructor and the addRemoveEdges function.
	 * Any edge with a weight different from zero is inserted.
	 * Any edge with a weight of exactly zero is removed.
	 * @param source
	 * @param destination
	 * @param weight
	 * @return true if adding/removing succeeded
	 */
	virtual bool addRemoveEdgePost(const typeNode & source, const typeNode & destination, const typeWeight & weight=1.0)=0;

	/**
	 * Function where the actual algorithm is implemented
	 * It is called at the end of the constructor and the addRemoveEdges function.
	 * @return false if an error occurred. Otherwise, true
	 */
	virtual bool run()=0;

public:
	/**
	 * Destructor
	 */
	virtual ~AlgorithmInterface(){}

};

#endif /* SRC_ALGORITHMINTERFACE_H_ */
