/*
 * graph.h
 *
 *  Created on: 19/08/2018
 *      Author: poltergeist0
 */

#ifndef GRAPHUNDIRECTED_H_
#define GRAPHUNDIRECTED_H_

#include "graphInterface.h"
#include "multimapUtilities.h"
//#include "program.h"
#include <sstream>
#include <limits>

class GraphUndirected: public GraphInterface {
private:
	typeNodeList nodes;
	typeLinks links;

	typeWeight total_weight;
	typeWeight max_weight;

	/******************************************************************************
	 ******************************************************************************
	 ******************************************************************************
	 * Functions used for debugging ONLY
	 * Verify consistency of the class member as a set
	 ******************************************************************************
	 ******************************************************************************
	 ******************************************************************************/
#ifndef NDEBUG
	typeNodeList debugNodes;
	typeLinks debugLinks;

	typeWeight debugTotal_weight;
	typeWeight debugMax_weight;
#endif	//NDEBUG

	void debugVerify() const{
#ifndef NDEBUG
		//verify that all nodes have edges
		for(typeNodeListIteratorConst it=nodes.cbegin();it!=nodes.cend();++it){
			const typeNode & p=*it;
			typeLinksRangeConst r=links.equal_range(p);
			ASSERT_NOT_EQUAL(r.first,r.second);
		}
		//verify weights
		typeWeight t=0;
		typeWeight m=std::numeric_limits<typeWeight>::min();
		for(typeLinksIteratorConst it=links.cbegin();it!=links.cend();++it){
			const typeLinksPair & p=*it;
			const HalfEdge & e=p.second;
			const typeWeight & w=e.weight();
			t+=w;
			if(m<w)m=w;
		}
		ASSERT_EQUAL(t,total_weight);
		ASSERT_EQUAL(m,max_weight);
#endif	//NDEBUG
	}

//	static std::vector<typeLinksIteratorConst> debugVerify(
//			const typeLinks & l1,const typeLinks & l2,const typeNode & source, const typeNode & destination
//			, const typeWeight & weight=1.0, const bool & excludeSource, const bool & excludeDestination
//			, const bool & excludeWeight
//			) const{
//		std::vector<typeLinksIteratorConst> res;
//#ifndef NDEBUG
//		//verify if old values are still present except for the replaced value, if one was replaced
//		for(typeLinksIteratorConst it=l1.cbegin();it!=l1.cend();++it){
//			const typeLinksPair & p=*it;
//			ASSERT_EQUAL(multimap::find(links,p.first,p.second)!=links.cend());
//			if(replace && ((p.first==source && p.second==destination)||(p.first==destination && p.second==source))){//if it was a replaced value
//				ASSERT_EQUAL((multimap::find(links,p.first,p.second))->second.weight()==weight);
//			}
//			else{// if it was not a replaced value, the weight must be the old value
//				ASSERT_EQUAL((multimap::find(links,p.first,p.second))->second.weight()==p.second.weight());
//			}
////			typeWeight w=(multimap::find(links,p.first,p.second))->second.weight();
////			t+=w;
////			if(m<w) m=w;
//		}
////		ASSERT_EQUAL(total_weight==t);
////		ASSERT_EQUAL(max_weight==m);
//#endif	//NDEBUG
//		return res;
//	}

	void debugSaveState(){
		debugVerify();
#ifndef NDEBUG
		//copy listings
		debugNodes=std::set<typeNode>(nodes);
		debugLinks=typeLinks(links);
		debugTotal_weight=total_weight;
		debugMax_weight=max_weight;
		//verify sizes
		ASSERT_EQUAL(debugNodes.size(),nodes.size());
		ASSERT_EQUAL(debugLinks.size(),links.size());
		//verify values in nodes
		for(typeNodeListIteratorConst it=nodes.cbegin();it!=nodes.cend();++it){
			const typeNode & p=*it;
			ASSERT_NOT_EQUAL_ITERATOR(debugNodes.find(p),debugNodes.cend());
		}
		//verify values in links
		for(typeLinksIteratorConst it=links.cbegin();it!=links.cend();++it){
			const typeLinksPair & p=*it;
			ASSERT_NOT_EQUAL_ITERATOR(multimap::find(debugLinks,p.first,p.second),debugLinks.cend());
		}
#endif	//NDEBUG
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
	void debugAddEdge(const bool & added,const typeNode & source, const typeNode & destination, const typeWeight & weight=1.0, const bool & replace=false) const{
		debugVerify();
#ifndef NDEBUG
		int incNodes=0;
		//verify if node existed or was added
		ASSERT_NOT_EQUAL_ITERATOR(nodes.find(source),nodes.cend());
		ASSERT_NOT_EQUAL_ITERATOR(nodes.find(destination),nodes.cend());
		if(debugNodes.find(source)==debugNodes.cend()){//did not exist
			++incNodes;
		}
		if(source!=destination && debugNodes.find(destination)==debugNodes.cend()){//did not exist
			++incNodes;
		}
		//verify nodes sizes
		ASSERT_EQUAL(nodes.size(),debugNodes.size()+incNodes);
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
		ASSERT_EQUAL(total_weight,t);
		ASSERT_EQUAL(max_weight,m);
#endif	//NDEBUG
	}

	void debugRemove(const bool & removed,const typeNode & source, const typeNode & destination) const{
		debugVerify();
#ifndef NDEBUG
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
					ASSERT_EQUAL_ITERATOR(nodes.find(source),nodes.cend());
					++decNodes;
				}
			}
			else{//there were more entries
				//check if node was not removed
				ASSERT_NOT_EQUAL_ITERATOR(nodes.find(source),nodes.cend());
			}
		}
		if(source!=destination){
			r=debugLinks.equal_range(destination);
			if(r.first!=r.second){//edge existed
				if(std::distance(r.first,r.second)==1){//there was only one entry
					if((r.first)->first==destination && (r.first)->second==source){//it was the entry requested to remove
						//check if the node was removed
						ASSERT_EQUAL_ITERATOR(nodes.find(destination),nodes.cend());
						++decNodes;
					}
				}
				else{//there were more entries
					//check if node was not removed
					ASSERT_NOT_EQUAL_ITERATOR(nodes.find(destination),nodes.cend());
				}
			}
		}
		//verify nodes sizes
		ASSERT_EQUAL(nodes.size(),debugNodes.size()-decNodes);
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
		ASSERT_EQUAL(total_weight,t);
		ASSERT_EQUAL(max_weight,m);
#endif	//NDEBUG
	}

	void debugReplace(const bool & replaced,const typeNode & oldValue, const typeNode & newValue) const{
		debugVerify();
#ifndef NDEBUG
		//verify sizes
		ASSERT_EQUAL(debugNodes.size(),nodes.size());
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
		ASSERT_EQUAL(total_weight,t);
		ASSERT_EQUAL(max_weight,m);
#endif	//NDEBUG
	}

private:

	/**
	 * find edge with given source and destination nodes
	 * @param source
	 * @param destination
	 * @return the edge or end() if not found
	 */
//	typeLinksIterator findLink(const typeNode & source, const typeNode & destination){
//		typeLinksRange a=links.equal_range(source);
////		if(a.first==a.second){//no nodes connected to it
////			nodes.erase(source);//remove node
////			//link does not exist
////			return links.end();
////		}
//		for(typeLinksIterator it=a.first;it!=a.second;++it){
//			HalfEdge & h=it->second;
//			const typeNode & dest=h.destination();
//			if(dest==destination){
//				return it;
//			}
//		}
//		//link does not exist
//		return links.end();
//	}

	/**
	 * update max weight if a given weight is larger
	 * @param weight
	 */
	void maxWeight(const typeWeight & weight){
		if(max_weight<weight)max_weight=weight;
	}

	/**
	 * go through all edges and update max weight
	 */
	void updateMaxWeight(){
		max_weight=std::numeric_limits<typeWeight>::min();
		for(typeLinksIteratorConst it=links.cbegin();it!=links.cend();it++){
			const HalfEdge & h=it->second;
			maxWeight(h.weight());
		}
	}

	/**
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
	GraphUndirected():total_weight(0.0),max_weight(std::numeric_limits<typeWeight>::min())
#ifndef NDEBUG
					,debugTotal_weight(0)
					,debugMax_weight(std::numeric_limits<typeWeight>::min())
#endif	//NDEBUG
	{}

	/**
	 * Copy constructor
	 */
	GraphUndirected(const GraphUndirected & original):nodes(typeNodeList(original.nodes)),links(typeLinks(original.links)),total_weight(original.total_weight),max_weight(original.max_weight)
#ifndef NDEBUG
					,debugTotal_weight(0)
					,debugMax_weight(std::numeric_limits<typeWeight>::min())
#endif	//NDEBUG
	{}

	typeLinksRangeConst edges()const {return std::make_pair(links.cbegin(),links.cend());}

	/**
	 * add a new edge or update the weight of an existing one
	 * @param source
	 * @param destination
	 * @param weight
	 * @param replace if true and the edge exists, replaces the weight
	 * @return true if either the edge was successfully added or replaced. False if the edge exists and either replace parameter was false or the given weight is zero ( since a weight of zero would cause the removal of the edge)
	 */
	bool addEdge(const typeNode & source, const typeNode & destination, const typeWeight & weight=1.0, const bool & replace=false){
		if(source==noNode || destination==noNode) return false;
		if(replace && weight==0) return false;
		debugSaveState();
		typeNode debugSource=source;
		typeNode debugDestination=destination;
		typeWeight debugWeight=weight;
		bool debugReplace=replace;
		bool result=false;
		if (nodes.find(source)==nodes.end()) {//node does not exist
			nodes.insert(source);//create
		}
		if (nodes.find(destination)==nodes.end()) {
			nodes.insert(destination);
		}
		//check if link already exists
//		typeLinksIterator it=findLink(source,destination);
		typeLinksIterator it=multimap::find(links,source,destination);
		if(it!=links.end()){
			if(replace){
				HalfEdge & h=it->second;
				typeWeight wei=h.weight();//can not be const & since old value is required later to update max weight
				updateWeight(h,weight);
				maxWeight(weight);//update max weight
				//do the same for the mirror link if it exists (it has to since it was automatically added)
				if(source!=destination){
//					typeLinksIterator itm=findLink(destination,source);
					typeLinksIterator itm=multimap::find(links,destination,source);
					if(itm!=links.end()){
						HalfEdge & hm=itm->second;
						updateWeight(hm,weight);
					}
				}
				if(max_weight==wei){//old weight was equal to max weight
//					if(weight>wei) maxWeight(weight);//already done
					if(weight<wei) updateMaxWeight();
				}
//				return true;
				result=true;
			}
			else{
//				return false;
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
//			return true;
			result=true;
		}
		debugAddEdge(result,debugSource,debugDestination,debugWeight,debugReplace);
		return result;
	}

	/**
	 * @see{addEdge(const typeNode & source, const typeNode & destination, const typeWeight & weight=1.0, const bool & replace=false)}
	 * @param edge
	 * @param replace
	 * @return
	 */
	bool addEdge(const Edge & edge, const bool & replace=false){return addEdge(edge.source(),edge.destination(),edge.weight());}

	/**
	 * remove edge
	 * @param source
	 * @param destination
	 * @return true if the edge existed and was successfully removed
	 */
	bool removeEdge(const typeNode & source, const typeNode & destination){
		debugSaveState();
		bool result=false;
		typeNode debugSource=source;
		typeNode debugDestination=destination;
//		typeLinksIterator it=findLink(source,destination);
		typeLinksIterator it=multimap::find(links,source,destination);
		if(it!=links.end()){
			typeWeight we=it->second.weight();
			total_weight-=we;
			links.erase(it);
			if(source!=destination){
				//remove mirror
//				it=findLink(destination,source);
				it=multimap::find(links,destination,source);
				if(it!=links.end()){
					total_weight-=we;
					links.erase(it);
				}
			}
			typeLinksRange a=links.equal_range(source);
			if(a.first==a.second){//no nodes connected to it
				nodes.erase(source);//remove node
			}
			a=links.equal_range(destination);
			if(a.first==a.second){//no nodes connected to it
				nodes.erase(destination);//remove node
			}
			if(max_weight==we) updateMaxWeight();
//			return true;
			result=true;
		}
//		return false;
		debugRemove(result,debugSource,debugDestination);
		return result;
	}

	bool removeEdge(const Edge & edge){return removeEdge(edge.source(),edge.destination());}

	/**
	 *
	 * @return a constant set with all nodes
	 */
	const typeNodeList & getNodes()const {return nodes;}

	/**
	 * Get the neighbors of a node
	 * @param node
	 * @return a pair of iterators pointing to the beginning and the end of the edges that start with the given node
	 */
	typeLinksRangeConst neighbors(const typeNode & node)const {
		if(node==noNode) return std::make_pair(links.cbegin(),links.cend());
		return links.equal_range(node);
	}

	typeWeight neighborsWeight(const typeNode & node)const{
		typeWeight w=0;
		typeLinksRangeConst r=neighbors(node);
		for(typeLinksIteratorConst it=r.first;it!=r.second;++it){
			const typeLinksPair & p=*it;
			if(node!=p.second.destination()) w+=p.second.weight();
		}
		return w;
	}

	unsigned int neighborsCount(const typeNode & node)const {
		if (node==noNode) return links.size();
		return links.count(node);
	}

	typeWeight weight(const typeNode & source, const typeNode & destination) const {
//		typeLinksIterator it=findLink(source,destination);
		typeLinksIteratorConst it=multimap::find(links,source,destination);
		if(it!=links.end()){
			HalfEdge e=(*it).second;
			return e.weight();
		}
		return std::numeric_limits<typeWeight>::quiet_NaN();
	}

	const typeWeight & maxWeight()const {
		return max_weight;
	}

	const typeWeight totalWeight()const {
		return total_weight;
	}

	const typeWeight nodeCount()const{return nodes.size();}

	const typeWeight edgeCount()const{return links.size();}

	typeWeight weighted_degree(const typeNode & node)const {
		typeLinksRangeConst p = neighbors(node);
		typeWeight res = 0.0L;
		for (typeLinksIteratorConst it=p.first ; it!=p.second ; ++it) {
			const typeLinksPair & a=*it;
			const HalfEdge & b=a.second;
//			if(node!=b.destination())
				res += b.weight();
		}
		return res;
	}

	/**
	 * Replace all node occurrences of oldValue by newValue
	 * @param oldValue
	 * @param newValue
	 * @return
	 */
	bool replace(const typeNode & oldValue, const typeNode & newValue){
		if(oldValue==noNode || newValue==noNode) return false;
		//check if oldValue exists
		typeNodeListIteratorConst itn=nodes.find(oldValue);
		if(itn==nodes.cend()) return false;//node does not exist so it can not be replaced
		if(oldValue==newValue)return true;//ignore if values are the same
		debugSaveState();
		bool result=false;
		typeNode debugOldValue=oldValue;
		typeNode debugNewValue=newValue;
		typeLinksRange v=links.equal_range(newValue);//find range with new value
		if(v.first!=v.second){//there are already edges with newValue
//			return false;
			result=false;
		}
		else{
			//check if newValue exists and add it if not
			itn=nodes.find(newValue);
			if(itn==nodes.cend()){
				nodes.insert(newValue);
			}
			typeLinksIterator it=links.find(oldValue);
			while(it!=links.end()){
				const typeNode & n=it->second.destination();
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
//					typeLinksIterator itt=findLink(n,oldValue);
					typeLinksIterator itt=multimap::find(links,n,oldValue);
					links.erase(itt);//remove reverse edge
					typeLinksIterator itmp=it;//make copy of iterator to use with erase since erasing invalidates the iterator
					++it;
					links.erase(itmp);//remove edge
				}
				if(it->first!=oldValue) it=links.find(oldValue);//renew search since first iterator returned is not guaranteed by the standard to be the first element with given key
			}
			nodes.erase(oldValue);
//			return true;
			result=true;
		}
		debugReplace(result,debugOldValue,debugNewValue);
		return result;
	}

	const std::string toString(const StringFormater & sf=defaultStringFormater)const {
		std::stringstream ss;
		StringFormater f=sf;
		if(sf.isDefault()){
			f.header("Graph={(source ; destination ; weight)(...)...}(total weight ; max weight)=");
		}
		f.start(ss,true);
		for(typeNodeListIterator itn=nodes.begin();itn!=nodes.end();++itn){
			typeNode node=*itn;
			typeLinksRangeConst p=neighbors(node);
			for (typeLinksIteratorConst it=p.first ; it!=p.second ; ++it){
				const typeLinksPair & a=*it;
				const HalfEdge & e=a.second;
				ss << f.tupleOpen() << node << f.valueSeparator() << e.destination() << f.valueSeparator() << e.weight() << f.tupleClose();
			}
		}
		ss << f.setClose() << f.setOpen() << f.tupleOpen()<< total_weight << f.valueSeparator()<< max_weight << f.tupleClose();
		f.end(ss,true);
		return ss.str();
	}

};

#endif /* GRAPHUNDIRECTED_H_ */
