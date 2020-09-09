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
#include "../debug/DebugLog.h"
#include "../debug/debugging.h"

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

	/******************************************************************************
	 ******************************************************************************
	 ******************************************************************************
	 * Functions used for debugging ONLY
	 * Verify consistency of the class member as a set
	 ******************************************************************************
	 ******************************************************************************
	 ******************************************************************************/
//#ifndef NDEBUG
	typeVertexList debugNodes;
	typeLinks debugLinks;

	typeWeight debugTotal_weight;
	typeWeight debugMax_weight;
//#endif	//NDEBUG

	void debugVerify() const{
//#ifndef NDEBUG
		//verify that all nodes have edges
		for(typeVertexListIteratorConst it=vertices.cbegin();it!=vertices.cend();++it){
			const typeVertex & p=*it;
			ASSERT_NOT_EQUAL(p,noVertex);//check that vertex is valid
			typeLinksRangeConst r=links.equal_range(p);
			ASSERT_NOT_EQUAL_ITERATOR(r.first,r.second);
		}
		//verify edges (weights and vertices)
		typeWeight t=0;
		typeWeight m=std::numeric_limits<typeWeight>::min();
		for(typeLinksIteratorConst it=links.cbegin();it!=links.cend();++it){
			const typeLinksPair & p=*it;
			ASSERT_NOT_EQUAL(p.first,noVertex);//check that vertex is valid
			const HalfEdge & e=p.second;
			ASSERT_NOT_EQUAL(e.destination(),noVertex);//check that vertex is valid
			const typeWeight & w=e.weight();
			ASSERT_NOT_NAN(w);//check that there is a value
			ASSERT_NOT_APPROXIMATE(w,0,ASSERT_PRECISION_LIMIT);//check that it is not zero
			t+=w;
			if(m<w)m=w;
		}
		ASSERT_APPROXIMATE(t,total_weight,ASSERT_PRECISION_LIMIT);
		ASSERT_APPROXIMATE(m,max_weight,ASSERT_PRECISION_LIMIT);
//#endif	//NDEBUG
	}

	void debugSaveState(){
		if(dbg.debugLevel()>=DEBUG_LEVEL::VERIFY){
			debugVerify();
			//copy listings
			debugNodes=std::set<typeVertex>(vertices);
			debugLinks=typeLinks(links);
			debugTotal_weight=total_weight;
			debugMax_weight=max_weight;
			//verify sizes
			ASSERT_EQUAL(debugNodes.size(),vertices.size());
			ASSERT_EQUAL(debugLinks.size(),links.size());
			//verify values in nodes
			for(typeVertexListIteratorConst it=vertices.cbegin();it!=vertices.cend();++it){
				const typeVertex & p=*it;
				ASSERT_NOT_EQUAL_ITERATOR(debugNodes.find(p),debugNodes.cend());
			}
			//verify values in links
			for(typeLinksIteratorConst it=links.cbegin();it!=links.cend();++it){
				const typeLinksPair & p=*it;
				ASSERT_NOT_EQUAL_ITERATOR(multimap::find(debugLinks,p.first,p.second),debugLinks.cend());
			}
		}
	}

	/**
	 * Verifies consistency of internal data and conformity of implementation
	 *
	 * AddEdge returns true if either the edge was successfully added or replaced. False if the edge
	 * exists and either replace parameter was false or the given weight is zero ( since a weight of
	 * zero would cause the removal of the edge)
	 *
	 * The following use cases are verified:
	 * 	- source/destination node did not exist (before the operation) and was created (after the operation)
	 * 	- size of node list after operation is equal to the size before the operation plus the number of created nodes
	 * 	- if requested edge did not existed, add it
	 * 		= if source and destination are the same
	 * 			+ size of link list is increased by 1
	 * 			+ total weight is increased by weight
	 * 		= if source and destination are not the same
	 * 			+ size of link list is increased by 2
	 * 			+ total weight is increased by 2*weight
	 * 	- if requested edge existed and replace was true, replace its weight
	 * 	- if requested edge existed and replace was false, do nothing
	 *
	 * @param added
	 * @param source
	 * @param destination
	 * @param weight
	 * @param replace
	 */
	void debugAddEdge(const bool & added,const typeVertex & source, const typeVertex & destination, const typeWeight & weight=1.0, const bool & replace=false) const{
		if(dbg.debugLevel()>=DEBUG_LEVEL::VERIFY){
			debugVerify();
			ASSERT_NOT_EQUAL(source,noVertex);//check that vertex is valid
			ASSERT_NOT_EQUAL(destination,noVertex);//check that vertex is valid
			ASSERT_NOT_NAN(weight);//check that there is a value
			if(added){//edge was added
				ASSERT_NOT_APPROXIMATE(weight,0,ASSERT_PRECISION_LIMIT);//very bad case. Can not request to add edge with weight equal to zero and add it anyway
			}
	//		ASSERT_NOT_APPROXIMATE(weight,0,ASSERT_PRECISION_LIMIT);//can not request to add edge with weight equal to zero
			int incNodes=0;
			//verify if node existed or was added
			ASSERT_NOT_EQUAL_ITERATOR(vertices.find(source),vertices.cend());
			ASSERT_NOT_EQUAL_ITERATOR(vertices.find(destination),vertices.cend());
			if(debugNodes.find(source)==debugNodes.cend()){//did not exist
				++incNodes;
			}
	//		else{//existed
	//			if(added) ASSERT(replace);//very bad case. Node existed and was added despite replace being false
	//		}
			if(source!=destination){
				if(debugNodes.find(destination)==debugNodes.cend()){//did not exist
					++incNodes;
				}
	//			else{//existed
	//				if(added) ASSERT(replace);//very bad case. Node existed and was added despite replace being false
	//			}
			}
			//verify nodes sizes
			ASSERT_EQUAL(vertices.size(),debugNodes.size()+incNodes);
			typeWeight t=0;
			typeWeight m=std::numeric_limits<typeWeight>::min();
			//verify requested edge
			if(added){
				if(debugLinks.size()>0 && multimap::find(debugLinks,source,destination)!=debugLinks.cend()){//edge existed before the operation
					ASSERT_EQUAL(links.size(),debugLinks.size());
					//verify edge existed before
					if(source!=destination){
						ASSERT_NOT_EQUAL_ITERATOR(multimap::find(debugLinks,destination,source),debugLinks.cend());
					}
					//verify it still exists
					ASSERT_NOT_EQUAL_ITERATOR(multimap::find(links,source,destination),links.cend());
					ASSERT_NOT_EQUAL_ITERATOR(multimap::find(links,destination,source),links.cend());
				}
				else{//edge did not exist before the operation
					if(source==destination){
						ASSERT_EQUAL(links.size(),debugLinks.size()+1);
					}
					else{
						ASSERT_EQUAL(links.size(),debugLinks.size()+2);
					}
					//verify if new edge was added (did not existed)
					ASSERT_EQUAL_ITERATOR(multimap::find(debugLinks,destination,source),debugLinks.cend());
					//verify it exists now
					ASSERT_NOT_EQUAL_ITERATOR(multimap::find(links,source,destination),links.cend());
					ASSERT_NOT_EQUAL_ITERATOR(multimap::find(links,destination,source),links.cend());
					typeWeight w=(multimap::find(links,source,destination))->second.weight();
					t+=w;
					if(source!=destination){
						w=(multimap::find(links,destination,source))->second.weight();
						t+=w;
					}
					if(m<w) m=w;
				}
				//verify new weight was recorded
				ASSERT_EQUAL((multimap::find(links,source,destination))->second.weight(),weight);
			}
			else{//not added because already existed and was not requested to be replaced
				//verify sizes have not changed
				ASSERT_EQUAL(links.size(),debugLinks.size());

				ASSERT_NOT_EQUAL_ITERATOR(multimap::find(debugLinks,source,destination),debugLinks.cend());
				ASSERT_NOT_EQUAL_ITERATOR(multimap::find(debugLinks,destination,source),debugLinks.cend());
				//verify the new edge does exists
				ASSERT_NOT_EQUAL_ITERATOR(multimap::find(links,source,destination),links.cend());
				ASSERT_NOT_EQUAL_ITERATOR(multimap::find(links,destination,source),links.cend());
			}
			//verify if old values are still present except for the replaced value, if one was replaced
			for(typeLinksIteratorConst it=debugLinks.cbegin();it!=debugLinks.cend();++it){
				const typeLinksPair & p=*it;
				ASSERT_NOT_EQUAL_ITERATOR(multimap::find(links,p.first,p.second),links.cend());
				if(replace && ((p.first==source && p.second==destination)||(p.first==destination && p.second==source))){//if it was a replaced value
					ASSERT_EQUAL((multimap::find(links,p.first,p.second))->second.weight(),weight);
				}
				else{// if it was not a replaced value, the weight must be the old value
					ASSERT_EQUAL((multimap::find(links,p.first,p.second))->second.weight(),p.second.weight());
				}
				typeWeight w=(multimap::find(links,p.first,p.second))->second.weight();
				t+=w;
				if(m<w) m=w;
			}
			ASSERT_APPROXIMATE(total_weight,t,ASSERT_PRECISION_LIMIT);
			ASSERT_APPROXIMATE(max_weight,m,ASSERT_PRECISION_LIMIT);
		}
	}

	void debugRemove(const bool & removed,const typeVertex & source, const typeVertex & destination) const{
		if(dbg.debugLevel()>=DEBUG_LEVEL::VERIFY){
			debugVerify();
			ASSERT_NOT_EQUAL(source,noVertex);//check that vertex is valid
			ASSERT_NOT_EQUAL(destination,noVertex);//check that vertex is valid
			int decNodes=0;
			//check if the edge no longer exists even if it never did
			ASSERT_EQUAL_ITERATOR(multimap::find(links,source,destination),links.cend());
			ASSERT_EQUAL_ITERATOR(multimap::find(links,destination,source),links.cend());
			//verify if the nodes were removed
			typeLinksRangeConst r=debugLinks.equal_range(source);
			if(r.first!=r.second){//edge existed
				if(std::distance(r.first,r.second)==1){//there was only one entry
					if((r.first)->first==source && (r.first)->second==destination){//it was the entry requested to remove
						//check if the node was removed
						ASSERT_EQUAL_ITERATOR(vertices.find(source),vertices.cend());
						++decNodes;
					}
				}
				else{//there were more entries
					//check if node was not removed
					ASSERT_NOT_EQUAL_ITERATOR(vertices.find(source),vertices.cend());
				}
			}
			else{//edge did not existed
				ASSERT(!removed);//bad case. Edge did not existed and was still removed
			}
			if(source!=destination){
				r=debugLinks.equal_range(destination);
				if(r.first!=r.second){//edge existed
					if(std::distance(r.first,r.second)==1){//there was only one entry
						if((r.first)->first==destination && (r.first)->second==source){//it was the entry requested to remove
							//check if the node was removed
							ASSERT_EQUAL_ITERATOR(vertices.find(destination),vertices.cend());
							++decNodes;
						}
					}
					else{//there were more entries
						//check if node was not removed
						ASSERT_NOT_EQUAL_ITERATOR(vertices.find(destination),vertices.cend());
					}
				}
				else{//edge did not existed
					ASSERT(!removed);//bad case. Edge did not existed and was still removed
				}
			}
			//verify nodes sizes
			ASSERT_EQUAL(vertices.size(),debugNodes.size()-decNodes);
			//verify requested edge
			if(debugLinks.size()>0 && multimap::find(debugLinks,source,destination)!=debugLinks.cend()){//existed
				if(removed){//was removed
					if(source==destination){
						ASSERT_EQUAL(links.size(),debugLinks.size()-1);
					}
					else{
						ASSERT_EQUAL(links.size(),debugLinks.size()-2);
					}
				}
				else{//was not removed
					ASSERT_EQUAL(links.size(),debugLinks.size());
				}
			}
			else{//did not exist
				//verify sizes have not changed
				ASSERT_EQUAL(links.size(),debugLinks.size());
			}
			//verify if old values are still present except for the removed value, if one was removed
			typeWeight t=0;
			typeWeight m=std::numeric_limits<typeWeight>::min();
			for(typeLinksIteratorConst it=debugLinks.cbegin();it!=debugLinks.cend();++it){
				const typeLinksPair & p=*it;
	//			ASSERT_EQUAL(multimap::find(links,p.first,p.second)!=links.cend());
				if(removed && ((p.first==source && p.second==destination)||(p.first==destination && p.second==source))){//if it was the removed value
				}
				else{// if it was not the removed value
					ASSERT_NOT_EQUAL_ITERATOR(multimap::find(links,p.first,p.second),links.cend());//edge still exists
					typeWeight w=(multimap::find(links,p.first,p.second))->second.weight();
					t+=w;
					if(m<w) m=w;
				}
			}
	//		ASSERT_EQUAL(total_weight,t);
	//		ASSERT_EQUAL(max_weight,m);
			ASSERT_APPROXIMATE(t,total_weight,ASSERT_PRECISION_LIMIT);
			ASSERT_APPROXIMATE(m,max_weight,ASSERT_PRECISION_LIMIT);
		}
	}

	void debugReplace(const bool & replaced,const typeVertex & oldValue, const typeVertex & newValue) const{
		if(dbg.debugLevel()>=DEBUG_LEVEL::VERIFY){
			debugVerify();
			//verify sizes
			ASSERT_EQUAL(debugNodes.size(),vertices.size());
			ASSERT_EQUAL(debugLinks.size(),links.size());
			ASSERT_EQUAL(debugTotal_weight,total_weight);
			ASSERT_EQUAL(debugMax_weight,max_weight);
			//verify values and weights
			typeWeight w=0;
			typeWeight t=0;
			typeWeight m=std::numeric_limits<typeWeight>::min();
			for(typeLinksIteratorConst it=debugLinks.cbegin();it!=debugLinks.cend();++it){
				const typeLinksPair & p=*it;
				if(replaced){
					if(p.first==oldValue && p.second==oldValue){//both were old value
						ASSERT_NOT_EQUAL_ITERATOR(multimap::find(links,newValue,newValue),links.cend());
						w=(multimap::find(links,newValue,newValue))->second.weight();
					}
					else{//only one or none were old value
						if(p.first==oldValue){//only the source was the old value
							ASSERT_NOT_EQUAL_ITERATOR(multimap::find(links,newValue,p.second),links.cend());
							w=(multimap::find(links,newValue,p.second))->second.weight();
						}
						else{//the source was not old value
							if(p.second==oldValue){//only the destination was the old value
								ASSERT_NOT_EQUAL_ITERATOR(multimap::find(links,p.first,newValue),links.cend());
								w=(multimap::find(links,p.first,newValue))->second.weight();
							}
							else{//none was old value
								ASSERT_NOT_EQUAL_ITERATOR(multimap::find(links,p.first,p.second),links.cend());
								w=(multimap::find(links,p.first,p.second))->second.weight();
							}
						}
					}
				}
				else{// if it was not replaced
					ASSERT_NOT_EQUAL_ITERATOR(multimap::find(links,p.first,p.second),links.cend());//edge still exists
					w=(multimap::find(links,p.first,p.second))->second.weight();
				}
				t+=w;
				if(m<w) m=w;
			}
			ASSERT_APPROXIMATE(total_weight,t, ASSERT_PRECISION_LIMIT);
			ASSERT_APPROXIMATE(max_weight,m, ASSERT_PRECISION_LIMIT);
		}
	}

public:

	/**
	 * default constructor
	 */
	GraphUndirected():total_weight(0.0),max_weight(std::numeric_limits<typeWeight>::min()),debugTotal_weight(0),debugMax_weight(0){}

	/**
	 * Copy constructor
	 */
	GraphUndirected(const GraphUndirected & original):vertices(typeVertexList(original.vertices)),links(typeLinks(original.links)),total_weight(original.total_weight),max_weight(original.max_weight),debugTotal_weight(0),debugMax_weight(0){}

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
		debugSaveState();
		typeVertex debugSource=source;
		typeVertex debugDestination=destination;
		typeWeight debugWeight=weight;
		bool debugReplace=replace;
		dbg.pre(DEBUG_LEVEL::MODIFICATIONS,"Ga", debugPrint());
		dbg.msg(DEBUG_LEVEL::CALLS,"e"+std::to_string(source)+"#"+std::to_string(destination)+"="+std::to_string(weight));
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
		dbg.post(DEBUG_LEVEL::MODIFICATIONS,debugPrint());
		debugAddEdge(result,debugSource,debugDestination,debugWeight,debugReplace);
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
		debugSaveState();
		typeVertex debugSource=source;
		typeVertex debugDestination=destination;
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
		debugRemove(result,debugSource,debugDestination);
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
	 * @brief Get the neighbours of a vertex
	 * @details The pair can change between calls if the Indexed edge list is modified.
	 * @param vertex
	 * @return pointers to the first and last neighbour of the vertex
	 */
	typeLinksRangeConst neighbours(const typeVertex & vertex)const {
		if(vertex==noVertex) return std::make_pair(links.cbegin(),links.cend());
		return links.equal_range(vertex);
	}

	/**
	 * @brief Get the sum of the weights of the neighbours of a vertex
	 * @param vertex
	 * @return the sum of the weights of the neighbours of the given vertex
	 */
	typeWeight neighboursWeight(const typeVertex & vertex)const{
		typeWeight w=0;
		typeLinksRangeConst r=neighbours(vertex);
		for(typeLinksIteratorConst it=r.first;it!=r.second;++it){
			const typeLinksPair & p=*it;
			if(vertex!=p.second.destination()) w+=p.second.weight();
		}
		return w;
	}

	/**
	 * @brief Get the number of neighbours of a vertex
	 * @param vertex
	 * @return the number of neighbours of the given vertex
	 */
	unsigned int neighboursCount(const typeVertex & vertex)const {
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
	 * @return the weighted degree (sum of weights of the neighbours) of the given vertex
	 */
	typeWeight weighted_degree(const typeVertex & vertex)const {
		typeLinksRangeConst p = neighbours(vertex);
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
		debugSaveState();
		typeVertex debugOldValue=oldValue;
		typeVertex debugNewValue=newValue;
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
		debugReplace(result,debugOldValue,debugNewValue);
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
			typeLinksRangeConst p=neighbours(vertex);
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

	const std::string debugPrint()const {
		std::stringstream ss;
		for(typeVertexListIteratorConst itn=vertices.cbegin();itn!=vertices.cend();++itn){
			const typeVertex & vertex=*itn;
			typeLinksRangeConst p=neighbours(vertex);
			for (typeLinksIteratorConst it=p.first ; it!=p.second ; ++it){
				const typeLinksPair & a=*it;
				const HalfEdge & e=a.second;
				ss << vertex << "+" << e.destination() << "=" << e.weight() << ";";
			}
		}
		ss << total_weight << ";" << max_weight;
		return ss.str();
	}

};

#endif /* GRAPHUNDIRECTED_H_ */
