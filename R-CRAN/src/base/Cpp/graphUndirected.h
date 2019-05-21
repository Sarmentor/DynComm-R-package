/************************************************************************
 ************************* Developer Notice *****************************
 ************************************************************************
 * @details
 *
 * Undirected Graph implementation for DynComm implemented in C++11.
 *
 *
 * @author poltergeist0
 *
 * @date 2018-08-19
 ************************************************************************
 ************************************************************************
 ************************************************************************/

#ifndef GRAPHUNDIRECTED_H_
#define GRAPHUNDIRECTED_H_

#include "graphInterface.h"
#include "multimapUtilities.h"
#include <sstream>
#include <limits>

/**
 * @brief Graph.
 *
 * @details
 * Class that implements an undirected graph.
 * When an edge is added/removed/modified, its mirror edge is also added/removed/modified.
 * This means that, if an edge (A,B) is added, the edge (B,A) is also added.
 *
 *
 * @author poltergeist0
 *
 * @date 2018-08-19
 */
class GraphUndirected: public GraphInterface {
private:
	typeVertexList vertices;
	typeLinks links;

	typeWeight total_weight;
	typeWeight max_weight;

	/*
	 * update max weight if a given weight is larger
	 * @param weight
	 */
	void maxWeight(const typeWeight & weight){
		if(max_weight<weight)max_weight=weight;
	}

	/*
	 * go through all edges and update max weight
	 */
	void updateMaxWeight(){
		max_weight=std::numeric_limits<typeWeight>::min();
		for(typeLinksIteratorConst it=links.cbegin();it!=links.cend();it++){
			const HalfEdge & h=it->second;
			maxWeight(h.weight());
		}
	}

	/*
	 * update weight
	 */
	void updateWeight(HalfEdge & he,const typeWeight & newWeight){
		total_weight -= he.weight();//remove old value
		he.weight(newWeight);//set new value
		total_weight += newWeight;//add new value
	}

public:

	/**
	 * default constructor
	 */
	GraphUndirected():total_weight(0.0),max_weight(std::numeric_limits<typeWeight>::min()){}

	/**
	 * Copy constructor
	 */
	GraphUndirected(const GraphUndirected & original):vertices(typeVertexList(original.vertices)),links(typeLinks(original.links)),total_weight(original.total_weight),max_weight(original.max_weight){}

	typeLinksRangeConst edges()const {return std::make_pair(links.cbegin(),links.cend());}

	/**
	 * Add an edge
	 *
	 * @param source
	 * @param destination
	 * @param weight Default value is one
	 * @param replace if true and link exists, it replaces the weight, otherwise fails. Default value is false
	 * @return true if the edge was added. False otherwise
	 */
	bool addEdge(const typeVertex & source, const typeVertex & destination, const typeWeight & weight=1.0, const bool & replace=false){
		if(source==noVertex || destination==noVertex) return false;
		if(replace && weight==0) return false;
		bool result=false;
		if (vertices.find(source)==vertices.end()) {//node does not exist
			vertices.insert(source);//create
		}
		if (vertices.find(destination)==vertices.end()) {
			vertices.insert(destination);
		}
		//check if link already exists
		typeLinksIterator it=multimap::find(links,source,destination);
		if(it!=links.end()){
			if(replace){
				HalfEdge & h=it->second;
				typeWeight wei=h.weight();//can not be const & since old value is required later to update max weight
				updateWeight(h,weight);
				maxWeight(weight);//update max weight
				//do the same for the mirror link if it exists (it has to since it was automatically added)
				if(source!=destination){
					typeLinksIterator itm=multimap::find(links,destination,source);
					if(itm!=links.end()){
						HalfEdge & hm=itm->second;
						updateWeight(hm,weight);
					}
				}
				if(max_weight==wei){//old weight was equal to max weight
					if(weight<wei) updateMaxWeight();
				}
				result=true;
			}
			else{
				result=false;
			}
		}
		else{//link does not exist
			links.insert(std::make_pair(source,HalfEdge(destination,weight)));
			total_weight += weight;
			maxWeight(weight);
			if (source!=destination){
				links.insert(std::make_pair(destination,HalfEdge(source,weight)));
				total_weight += weight;
			}
			result=true;
		}
		return result;
	}

	/**
	 * @see{addEdge(const typeVertex & source, const typeVertex & destination, const typeWeight & weight=1.0, const bool & replace=false)}
	 * @param edge
	 * @param replace
	 * @return
	 */
	bool addEdge(const Edge & edge, const bool & replace=false){return addEdge(edge.source(),edge.destination(),edge.weight());}

	/**
	 * remove an edge
	 *
	 * @param source
	 * @param destination
	 * @return true if the edge existed and was successfully removed
	 */
	bool removeEdge(const typeVertex & source, const typeVertex & destination){
		bool result=false;
		typeLinksIterator it=multimap::find(links,source,destination);
		if(it!=links.end()){
			typeWeight we=it->second.weight();
			total_weight-=we;
			links.erase(it);
			if(source!=destination){
				//remove mirror
				it=multimap::find(links,destination,source);
				if(it!=links.end()){
					total_weight-=we;
					links.erase(it);
				}
			}
			typeLinksRange a=links.equal_range(source);
			if(a.first==a.second){//no nodes connected to it
				vertices.erase(source);//remove node
			}
			a=links.equal_range(destination);
			if(a.first==a.second){//no nodes connected to it
				vertices.erase(destination);//remove node
			}
			if(max_weight==we) updateMaxWeight();
			result=true;
		}
		return result;
	}

	/**
	 * @see{removeEdge(const typeVertex & source, const typeVertex & destination)}
	 * @param edge
	 * @return
	 */
	bool removeEdge(const Edge & edge){return removeEdge(edge.source(),edge.destination());}

	/**
	 * @return a constant set with all vertices
	 */
	const typeVertexList & getVertices()const {return vertices;}

	/**
	 * @brief Get the neighbors of a vertex
	 * @details The pair can change between calls if the Indexed edge list is modified.
	 * @param vertex
	 * @return pointers to the first and last neighbor of the vertex
	 */
	typeLinksRangeConst neighbors(const typeVertex & vertex)const {
		if(vertex==noVertex) return std::make_pair(links.cbegin(),links.cend());
		return links.equal_range(vertex);
	}

	/**
	 * @brief Get the sum of the weights of the neighbors of a vertex
	 * @param vertex
	 * @return the sum of the weights of the neighbors of the given vertex
	 */
	typeWeight neighborsWeight(const typeVertex & vertex)const{
		typeWeight w=0;
		typeLinksRangeConst r=neighbors(vertex);
		for(typeLinksIteratorConst it=r.first;it!=r.second;++it){
			const typeLinksPair & p=*it;
			if(vertex!=p.second.destination()) w+=p.second.weight();
		}
		return w;
	}

	/**
	 * @brief Get the number of neighbors of a vertex
	 * @param vertex
	 * @return the number of neighbors of the given vertex
	 */
	unsigned int neighborsCount(const typeVertex & vertex)const {
		if (vertex==noVertex) return links.size();
		return links.count(vertex);
	}

	/**
	 * @return the weight of the edge
	 */
	typeWeight weight(const typeVertex & source, const typeVertex & destination) const {
		typeLinksIteratorConst it=multimap::find(links,source,destination);
		if(it!=links.end()){
			HalfEdge e=(*it).second;
			return e.weight();
		}
		return std::numeric_limits<typeWeight>::quiet_NaN();
	}

	/**
	 * @return the largest weight of all edges in the graph
	 */
	const typeWeight & maxWeight()const {
		return max_weight;
	}

	/**
	 * @return the sum of the weight of all edges in the graph
	 */
	const typeWeight totalWeight()const {
		return total_weight;
	}

	/**
	 * @return the number of vertices in the graph
	 */
	const typeWeight vertexCount()const{return vertices.size();}

	/**
	 * @return the number of edges in the graph
	 */
	const typeWeight edgeCount()const{return links.size();}

	/**
	 * @param vertex
	 * @return the weighted degree (sum of weights of the neighbors) of the given vertex
	 */
	typeWeight weighted_degree(const typeVertex & vertex)const {
		typeLinksRangeConst p = neighbors(vertex);
		typeWeight res = 0.0L;
		for (typeLinksIteratorConst it=p.first ; it!=p.second ; ++it) {
			const typeLinksPair & a=*it;
			const HalfEdge & b=a.second;
				res += b.weight();
		}
		return res;
	}

	/**
	 * Replace all vertex occurrences of oldValue by newValue
	 *
	 * @param oldValue
	 * @param newValue
	 * @return true if replacement succeeded
	 */
	bool replace(const typeVertex & oldValue, const typeVertex & newValue){
		if(oldValue==noVertex || newValue==noVertex) return false;
		//check if oldValue exists
		typeVertexListIteratorConst itn=vertices.find(oldValue);
		if(itn==vertices.cend()) return false;//node does not exist so it can not be replaced
		if(oldValue==newValue)return true;//ignore if values are the same
		bool result=false;
		typeLinksRange v=links.equal_range(newValue);//find range with new value
		if(v.first!=v.second){//there are already edges with newValue
//			return false;
			result=false;
		}
		else{
			//check if newValue exists and add it if not
			itn=vertices.find(newValue);
			if(itn==vertices.cend()){
				vertices.insert(newValue);
			}
			typeLinksIterator it=links.find(oldValue);
			while(it!=links.end()){
				const typeVertex & n=it->second.destination();
				const typeWeight & w=it->second.weight();
				if(oldValue==n){//same source and destination
					links.insert(std::make_pair(newValue,HalfEdge(newValue,w)));
					typeLinksIterator itmp=it;//make copy of iterator to use with erase since erasing invalidates the iterator
					++it;
					links.erase(itmp);//remove edge
				}
				else{
					links.insert(std::make_pair(newValue,HalfEdge(n,w)));
					links.insert(std::make_pair(n,HalfEdge(newValue,w)));
					typeLinksIterator itt=multimap::find(links,n,oldValue);
					links.erase(itt);//remove reverse edge
					typeLinksIterator itmp=it;//make copy of iterator to use with erase since erasing invalidates the iterator
					++it;
					links.erase(itmp);//remove edge
				}
				if(it->first!=oldValue) it=links.find(oldValue);//renew search since first iterator returned is not guaranteed by the standard to be the first element with given key
			}
			vertices.erase(oldValue);
			result=true;
		}
		return result;
	}

	/**
	 * @return a string representation of this graph
	 */
	const std::string toString(const StringFormatter & sf=defaultStringFormatter)const {
		std::stringstream ss;
		StringFormatter f=sf;
		if(sf.isDefault()){
			f.header("Graph={(source ; destination ; weight)(...)...}(total weight ; max weight)=");
		}
		f.start(ss,true);
		for(typeVertexListIterator itn=vertices.begin();itn!=vertices.end();++itn){
			typeVertex vertex=*itn;
			typeLinksRangeConst p=neighbors(vertex);
			for (typeLinksIteratorConst it=p.first ; it!=p.second ; ++it){
				const typeLinksPair & a=*it;
				const HalfEdge & e=a.second;
				ss << f.tupleOpen() << vertex << f.valueSeparator() << e.destination() << f.valueSeparator() << e.weight() << f.tupleClose();
			}
		}
		ss << f.setClose() << f.setOpen() << f.tupleOpen()<< total_weight << f.valueSeparator()<< max_weight << f.tupleClose();
		f.end(ss,true);
		return ss.str();
	}

};

#endif /* GRAPHUNDIRECTED_H_ */
