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
 * @date 2018-08-19
 ************************************************************************
 ************************************************************************
 ************************************************************************/

#ifndef GRAPHINTERFACE_H_
#define GRAPHINTERFACE_H_

#include "edge.h"

/**
 * @brief Indexed edge list by source vertex
 * @details
 * Expanded type:
 * std::multimap<unsigned int, HalfEdge>
 */
typedef std::multimap<typeVertex, HalfEdge> typeLinks;

/**
 * @brief Indexed edge list iterator
 * @details
 * Expanded type:
 * std::multimap<unsigned int, HalfEdge>::iterator
 */
typedef typeLinks::iterator typeLinksIterator;

/**
 * @brief Indexed edge list constant iterator
 * @details
 * Expanded type:
 * std::multimap<unsigned int, HalfEdge>::const_iterator
 */
typedef typeLinks::const_iterator typeLinksIteratorConst;

/**
 * @brief Indexed edge list pair as returned by the iterator
 */
typedef std::pair<typeVertex, HalfEdge> typeLinksPair;

/**
 * @brief Indexed edge list range.
 * @details
 * Defines a pair with two iterators to the beginning and end of the range.
 * If the iterators are identical, the list is empty.
 * The pair can change between calls if the Indexed edge list is modified.
 * Expanded type:
 * std::pair<std::multimap<unsigned int, HalfEdge>::iterator, std::multimap<unsigned int, HalfEdge>::iterator >
 */
typedef std::pair<typeLinksIterator, typeLinksIterator > typeLinksRange;

/**
 * @brief Indexed edge list const range.
 * @details
 * Defines a pair with two constant iterators to the beginning and end of the
 * range.
 * If the iterators are identical, the list is empty.
 * The pair can change between calls if the Indexed edge list is modified.
 * Expanded type:
 * std::pair<std::multimap<unsigned int, HalfEdge>::const_iterator, std::multimap<unsigned int, HalfEdge>::const_iterator >
 */
typedef std::pair<typeLinksIteratorConst, typeLinksIteratorConst > typeLinksRangeConst;

/**
 * @brief Vertex list
 */
typedef std::set<typeVertex> typeVertexList;

/**
 * @brief Constant Vertex List
 */
typedef const std::set<typeVertex> typeVertexListConst;

/**
 * @brief Vertex list iterator
 * @details
 * Expanded type:
 * std::set<typeVertex>::iterator
 */
typedef std::set<typeVertex>::iterator typeVertexListIterator;

/**
 * @brief Vertex list constant iterator
 * @details
 * Expanded type:
 * std::set<typeVertex>::const_iterator
 */
typedef std::set<typeVertex>::const_iterator typeVertexListIteratorConst;

//typedef std::pair<typeVertexListIteratorConst, typeVertexListIteratorConst > typeVertexListRangeConst;

/**
 * @brief Interface for graph.
 *
 * @details
 * Class that defines the interface for the graph used in DynComm.
 *
 *
 * @author poltergeist0
 *
 * @date 2018-08-19
 */
class GraphInterface {
public:
	/**
	 * Destructor
	 */
	virtual ~GraphInterface(){}

	/**
	 * @return a pair with iterators to the beginning and end of the edge list.
	 */
	virtual typeLinksRangeConst edges()const =0;

	/**
	 * Add an edge
	 *
	 * @param source
	 * @param destination
	 * @param weight Default value is one
	 * @param replace if true and link exists, it replaces the weight, otherwise fails. Default value is false
	 * @return true if the edge was added. False otherwise
	 */
	virtual bool addEdge(const typeVertex & source, const typeVertex & destination, const typeWeight & weight=1.0, const bool & replace=false)=0;

	/**
	 * Add an edge
	 *
	 * @param edge
	 * @param replace if true and link exists, it replaces the weight, otherwise fails. Default value is false
	 * @return true if the edge was added. False otherwise
	 */
	virtual bool addEdge(const Edge & edge, const bool & replace=false)=0;

	/**
	 * remove an edge
	 *
	 * @param source
	 * @param destination
	 * @return true if the edge existed and was successfully removed
	 */
	virtual bool removeEdge(const typeVertex & source, const typeVertex & destination)=0;

	/**
	 * @see{removeEdge(const typeVertex & source, const typeVertex & destination)}
	 * @param edge
	 * @return
	 */
	virtual bool removeEdge(const Edge & edge)=0;

	/**
	 * @return all unique vertices in the graph
	 */
	virtual const typeVertexList & getVertices()const =0;

	/**
	 * @brief Get the neighbours of a vertex
	 * @details The pair can change between calls if the Indexed edge list is modified.
	 * @param vertex
	 * @return pointers to the first and last neighbour of the vertex
	 */
	virtual typeLinksRangeConst neighbours(const typeVertex & vertex)const =0;

	/**
	 * @brief Get the sum of the weights of the neighbours of a vertex
	 * @param vertex
	 * @return the sum of the weights of the neighbours of the given vertex
	 */
	virtual typeWeight neighboursWeight(const typeVertex & vertex)const=0;

	/**
	 * @brief Get the number of neighbours of a vertex
	 * @param vertex
	 * @return the number of neighbours of the given vertex
	 */
	virtual unsigned int neighboursCount(const typeVertex & vertex)const=0;

	/**
	 * @return the weight of the edge
	 */
	virtual typeWeight weight(const typeVertex & source, const typeVertex & destination) const =0;

	/**
	 * @return the largest weight of all edges in the graph
	 */
	virtual const typeWeight & maxWeight()const =0;

	/**
	 * @return the sum of the weight of all edges in the graph
	 */
	virtual const typeWeight totalWeight()const =0;

	/**
	 * @return the number of vertices in the graph
	 */
	virtual const typeWeight vertexCount()const=0;

	/**
	 * @return the number of edges in the graph
	 */
	virtual const typeWeight edgeCount()const=0;

	/**
	 * @param vertex
	 * @return the weighted degree (sum of weights of the neighbours) of the given vertex
	 */
	virtual typeWeight weighted_degree(const typeVertex & node)const=0;

	/**
	 * Replace all vertex occurrences of oldValue by newValue
	 *
	 * @param oldValue
	 * @param newValue
	 * @return true if replacement succeeded
	 */
	virtual bool replace(const typeVertex & oldValue, const typeVertex & newValue)=0;

	/**
	 * @return a string representation of this graph
	 */
	virtual const std::string toString(const StringFormatter & sf=defaultStringFormatter)const=0;

};

#endif /* GRAPHINTERFACE_H_ */
