/*
 * algorithmInterface.h
 *
 *  Created on: 02/02/2019
 *      Author: poltergeist0
 */

#ifndef SRC_DYNCOMMBASEINTERFACE_H_
#define SRC_DYNCOMMBASEINTERFACE_H_

#include <list>

#include "reader.h"
#include "writer.h"

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
	 * @return true if adding/removing succeeded
	 */
	template<typename T , typename std::enable_if<std::is_base_of<ReaderInterface, T>::value>::type* = nullptr>
	virtual bool addRemoveEdges(T & reader)=0;


	/**
	 * @return the current quality measure of the community mapping on the graph
	 */
	virtual double quality()=0;

	/**
	 * @return the number of communities
	 */
	virtual int communityCount()=0;

	/**
	 * @return a list of all communities
	 */
	virtual std::list<long> communities()=0;

	/**
	 * @return a list with information about the selected community. The information provided is:
	 *  - total weight of inner edges
	 *  - total weight of outer edges
	 *  - total weight of edges
	 *  - number of nodes in community
	 */
	virtual std::list<long> community(int community)=0;

	/**
	 * @return a list of all nodes belonging to the given community
	 */
	virtual std::list<long> nodes(int community)=0;

	/**
	 * Get a snapshot of the current community mapping
	 * The differential parameter will probably be moved inside the writer as a parameter
	 * @return true if the operation succeeded
	 */
	template<typename T , typename std::enable_if<std::is_base_of<WriterInterface, T>::value>::type* = nullptr>
	virtual bool results(T & writer,bool differential=true)=0;

};

#endif /* SRC_DYNCOMMBASEINTERFACE_H_ */
