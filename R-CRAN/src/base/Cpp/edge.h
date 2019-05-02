/*
 * edge.h
 *
 *  Created on: 19/08/2018
 *      Author: poltergeist0
 */

#ifndef EDGE_H_
#define EDGE_H_

#include "stringFormater.h"
#include <limits>

typedef unsigned int typeVertex;
typedef long double typeWeight;

/**
 * Class that encapsulates the destination and weight portion of an edge (link between two nodes)
 */
class HalfEdge{
private:
	typeVertex dst;//destination node
	typeWeight wght;//weight of edge

public:
	/**
	 *
	 * @return the destination
	 */
	const typeVertex& destination() const {return dst;}
	/**
	 * Modify the destination
	 * @param destination
	 * @return true if the value was modified successfully
	 */
	const bool destination(const typeVertex & destination){
		dst=destination;
		return true;
	}
	/**
	 *
	 * @return the weight of the edge
	 */
	const typeWeight& weight() const {return wght;}
	/**
	 * Modify the weight of the edge
	 * @param weight
	 * @return true if the value was modified successfully
	 */
	const bool weight(const typeWeight& weight){
		wght=weight;
		return true;
	}

	/**
	 * Constructor with a weight of zero as default
	 * @param destination
	 * @param weight
	 */
	HalfEdge(const typeVertex & destination, const typeWeight & weight=0) : dst (destination), wght (weight) {}

	/**
	 * Weak comparator for ordering. Weight is ignored
	 * @param rhs is another half edge to compare to
	 * @return true if this half edge (destination) is smaller than the given (rhs) half edge (destination)
	 */
	const bool operator< (const HalfEdge & rhs) const {return dst < rhs.dst;}

	/**
	 * Comparison operator. Weight is ignored
	 * @param rhs
	 * @return true if this half edge (destination) is equal to the given (rhs) half edge (destination)
	 */
	const bool operator== (const HalfEdge & rhs) const {
		if(dst== rhs.dst) return true;
		return false;
	}

	/**
	 * Comparison operator against node only. Weight is ignored
	 * @param rhs is the node used to compare to
	 * @return true if the destination node of this half edge is equal to the given node
	 */
	const bool operator== (const typeVertex & rhs) const {
		return dst==rhs;
	}

	/**
	 * Assignment operator for node only. Weight is ignored
	 * @return the destination node of this half edge
	 */
	operator const typeVertex &() const {return dst;}

	/**
	 *
	 * @return a string representation of this half edge
	 */
	std::string toString(const StringFormater & sf=defaultStringFormater) const{
		std::string s;
		s.append(std::to_string(dst));
		s.append(sf.valueSeparator());
		s.append(std::to_string(wght));
		return s;
	}
};


/**
 * Class that implements an edge (link between two nodes)
 */
class Edge: private HalfEdge{
private:
	typeVertex src;//source node

public:
	/**
	 * Constructor with a weight of zero as default
	 * @param source
	 * @param destination
	 * @param weight
	 */
	Edge (const typeVertex & source, const typeVertex & destination, const typeWeight & weight=0) : HalfEdge(destination, weight),src (source){}

	/**
	 *
	 * @return the source
	 */
	const typeVertex& source() const {return src;}
	/**
	 * Modify the source
	 * @param source
	 * @return true if the value was modified successfully
	 */
	const bool source(const typeVertex& source){
		src=source;
		return true;
	}
	/**
	 *
	 * @return the destination
	 */
	const typeVertex& destination() const {return HalfEdge::destination();}
	/**
	 * Modify the destination
	 * @param destination
	 * @return true if the value was modified successfully
	 */
	const bool destination(const typeVertex & destination){
		HalfEdge::destination(destination);
		return true;
	}
	/**
	 *
	 * @return the weight of the edge
	 */
	const typeWeight& weight() const {return HalfEdge::weight();}
	/**
	 * Modify the weight of the edge
	 * @param weight
	 * @return true if the value was modified successfully
	 */
	const bool weight(const typeWeight& weight){
		HalfEdge::weight(weight);
		return true;
	}

	/**
	 * Weak comparator for ordering. Weight is ignored
	 * @param rhs is another edge to compare to
	 * @return true if this edge (first source then destination) is smaller than the given (rhs) edge (first source then destination)
	 */
	const bool operator< (const Edge & rhs) const {
		if (src == rhs.src){
			return HalfEdge::operator <(rhs);
		}
		else{
			return src < rhs.src;
		}
	}

	/**
	 * Comparison operator. Weight is ignored
	 * @param rhs
	 * @return true if this edge (first source then destination) is equal to the given (rhs) edge (first source then destination)
	 */
	const bool operator== (const Edge & rhs) const {
		if (src == rhs.src)return HalfEdge::operator ==(rhs);
		return false;
	}

	/**
	 * Comparison operator against source node only. Weight is ignored
	 * @param rhs is the node used to compare to
	 * @return true if the destination node of this half edge is equal to the given node
	 */
	const bool operator== (const typeVertex & rhs) const {
		return src==rhs;
	}

	/**
	 * Assignment operator for source node only. Weight is ignored
	 * @return the source node of this edge
	 */
	operator const typeVertex &() const {return src;}

	/**
	 *
	 * @return a string representation of this half edge
	 */
	std::string toString(const StringFormater & sf=defaultStringFormater) const{
		std::string s;
		s.append(sf.tupleOpen());
		s.append(std::to_string(src));
		s.append(sf.valueSeparator());
		s.append(HalfEdge::toString());
		s.append(sf.tupleClose());
		return s;
	}
};

/**
 * special node that indicates NO VERTEX
 */
const typeVertex noVertex=std::numeric_limits<typeVertex>::max();

/**
 * special edge that indicates NO EDGE
 */
const Edge noEdge(noVertex,noVertex);


#endif /* EDGE_H_ */
