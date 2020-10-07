/************************************************************************
 ************************* Developer Notice *****************************
 ************************************************************************
 * @details
 *
 * Groupable Undirected Graph implementation for DynComm implemented in
 * C++11.
 *
 *
 * @author poltergeist0
 *
 * @date 2018-08-19
 ************************************************************************
 ************************************************************************
 ************************************************************************/

#ifndef GRAPHUNDIRECTEDGROUPABLE_H_
#define GRAPHUNDIRECTEDGROUPABLE_H_

#include "mapReversable.h"
#include <cmath>

#include "graphUndirected.h"
#include "setUtilities.h"

/**
 * @brief Data type definition for a single community
 * @details The community and vertex data types must be interchangeable but this
 * might change in the future so define as a different data type.
 */
typedef typeVertex typeCommunity;

/**
 * @brief Indexed community list by vertex
 */
typedef MapReversable<typeVertex,typeCommunity> typeCommunityList;

/**
 * @brief Indexed community list constant iterator
 * @details
 * Expanded type:
 * std::map<typeVertex,typeCommunity>::const_iterator
 */
typedef typeCommunityList::const_iterator typeCommunityListIteratorConst;

/**
 * @brief Indexed community list pair as returned by the iterator
 * @details
 * Expanded type:
 * std::pair<typeVertex,typeCommunity>
 */
typedef typeCommunityList::typePair typeCommunityListPair;

/**
 * @brief Indexed community list constant reverse iterator
 * @details
 * Expanded type:
 * std::multimap<typeCommunity,typeVertex>::const_iterator
 */
typedef typeCommunityList::range_iterator_const typeCommunityListRangeIteratorConst;

/**
 * @brief Indexed community list range
 * @details
 * Defines a pair with two constant reverse iterators to the beginning and end
 * of the range.
 * If the iterators are identical, the list is empty.
 * The pair can change between calls if the list is modified.
 * Expanded type:
 * std::pair<std::multimap<typeCommunity,typeVertex>::const_iterator,std::multimap<typeCommunity,typeVertex>::const_iterator>
 */
typedef typeCommunityList::typeRange typeCommunityListRange;

/**
 * @brief Indexed community list range pair as returned by the reverse iterator
 * @details
 * Expanded type:
 * std::pair<typeCommunity,typeVertex>
 */
typedef typeCommunityList::typeRangePair typeCommunityListRangePair;

/**
 * @brief Indexed community weight list
 * @details Optimization data type used to store the sum of weights of a community
 */
typedef std::map<typeCommunity, typeWeight> typeCommunityEdges;

/**
 * @brief Indexed community weight list iterator
 */
typedef std::map<typeCommunity, typeWeight>::iterator typeCommunityEdgesIterator;

/**
 * @brief Indexed community weight list pair as returned by the iterator
 */
typedef std::pair<typeCommunity, typeWeight> typeCommunityEdgesPair;

/**
 * @brief Indexed community weight list constant iterator
 */
typedef std::map<typeCommunity, typeWeight>::const_iterator typeCommunityEdgesIteratorConst;

/**
 * @brief Indexed community weight list range
 * @details
 * Defines a pair with two iterators to the beginning and end of the range.
 * If the iterators are identical, the list is empty.
 * The pair can change between calls if the list is modified.
 */
typedef std::pair<typeCommunityEdgesIterator,typeCommunityEdgesIterator> typeCommunityEdgesRange;

/**
 * @brief Indexed community weight list constant range
 * @details
 * Defines a pair with two constant iterators to the beginning and end of the range.
 * If the iterators are identical, the list is empty.
 * The pair can change between calls if the list is modified.
 */
typedef std::pair<typeCommunityEdgesIteratorConst,typeCommunityEdgesIteratorConst> typeCommunityEdgesRangeConst;

/**
 * @brief List of communities
 */
typedef std::set<typeCommunity> typeCommunities;

/**
 * special community that indicates NO COMMUNITY
 */
const typeCommunity noGroup=std::numeric_limits<typeCommunity>::max();

/**
 * @brief Groupable Undirected Graph.
 *
 * @details
 * Class that implements a groupable undirected graph.
 * When an edge is added/removed/modified, its mirror edge is also added/removed/modified.
 * This means that, if an edge (A,B) is added, the edge (B,A) is also added.
 * Also, vertices can be individually assigned to groups and, those groups, form
 * an higher level view of the graph, being itself a graph with edges and
 * weights where the groups are vertices.
 *
 * @section Example of a graph:
 *   lower level:
 *     vertices: 1,2,3,4,5,6
 *     edges: (1,2),(1,3),(2,3),(3,4),(4,5),(4,6),(5,6)
 *     communities(vertex,community):(1,1),(2,1),(3,1),(4,4),(5,4),(6,4)
 *   higher level:
 *     vertices: 1,4
 *     edges: (1,4)
 *     communities(vertex,community):(1,1),(4,1)
 *
 *
 * @author poltergeist0
 *
 * @date 2018-08-19
 */
class GraphUndirectedGroupable: public GraphUndirected{
private:
	/*
	 * map used to keep the relation between the graph vertices and their communities
	 */
	typeCommunityList n2c; // vertex to community association used on the first pass of the louvain algorithm

	/*
	 * number of edges inside community
	 */
	typeCommunityEdges inner;

	/*
	 * optimization
	 * total number of links of community
	 */
	typeCommunityEdges total;

	/*
	 * community to community mapping
	 */
	GraphUndirected cc;

	/*
	 * Workaround mathematical errors created by adding and removing edges to/from
	 * community to community mapping that prevent mappings to be removed when they
	 * should.
	 * Instead cache a count of the number of edges in each mapping and remove the
	 * edge when the count is zero.
	 * This TEMPORARY solution should be removed in a future version of the code
	 * when a better graph is implemented.
	 */
//	typeLinks ccl;//link count for cc
//	typeCommunityList ic;//count for inner
//	typeCommunityList tc;//count for total


private:

	/**
	 *
	 * @param com is the target community
	 * @param exclude is a vertex to exclude from the search
	 * @return the smallest number used as a vertex in the given community
	 */
	typeVertex minimumNode(const typeCommunityList & comList, const typeCommunity & com, const typeVertex & exclude=noVertex)const {
		typeCommunityListRange p= comList.keys(com);
		typeVertex min=noVertex;
		for(typeCommunityListRangeIteratorConst it=p.first;it!=p.second;++it){
			const typeCommunityListRangePair & a=*it;
			if(a.second!=exclude){//only process if it is not vertex to exclude
				if(min==noVertex){//first time
					min=a.second;
				}
				else{
					if(a.second<min){
						min=a.second;
					}
				}
			}
		}
		return min;
	}

	/**
	 * Update the weight of a community
	 *
	 * @param ce is the list of communities
	 * @param com is the community to update
	 * @param weight is the weight to add or update
	 * @param add if true adds the given weight to the existing value. Otherwise, remove the given weight
	 * @return true if an update occurred. False if it was erased or does not exist
	 */
	bool update(typeCommunityEdges & ce, const typeCommunity & com, const typeWeight & weight, const bool & add=true){
		typeCommunityEdgesIterator it=ce.find(com);
		if(it!=ce.end()){
			typeWeight w;
			if(add) w=(it->second+weight);
			else{
				w=(it->second-weight);
				if(fabsl(w)<std::numeric_limits<typeWeight>::epsilon()*2) w=0;
			}
			if(w==0){
				ce.erase(it);
			}
			else{
				it->second=w;
				return true;
			}
		}
		else{
			if(add){
				if(weight!=0){
					ce.insert(std::make_pair(com,weight));
				}
				return true;
			}
		}
		return false;
	}

	/**
	 * Replace the name of a community
	 *
	 * @param ce is the list of communities
	 * @param old is the old community name
	 * @param com is the new community name
	 * @return true if old community existed and replacement succeeded. False if old community did not exist
	 */
	bool replace(typeCommunityEdges & ce, const typeCommunity & old,const typeCommunity & com){
		typeCommunityEdgesIteratorConst it=ce.find(old);
		if(it!=ce.cend()){
			const typeCommunityEdgesPair & p=*it;
			ce[com]=p.second;
			ce.erase(it);
			return true;
		}
		return false;
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
//	GraphUndirected debugGraphUndirected;//parent class
	typeCommunityList debugN2c; // node to community association used on the first pass of the louvain algorithm
	typeCommunityEdges debugInner;
	typeCommunityEdges debugTotal;
	GraphUndirected debugCc;//community to community mapping
//#endif	//NDEBUG

//	void debugVerifyEdgeCount(const int & offset=0)const{
////#ifndef NDEBUG
//		ASSERT_EQUAL(debugGraphUndirected.edgeCount()+offset,GraphUndirected::edgeCount());
////#endif	//NDEBUG
//	}
//
//	void debugVerifyNodeCount(const int & offset=0)const{
////#ifndef NDEBUG
//		ASSERT_EQUAL(debugGraphUndirected.vertexCount()+offset,GraphUndirected::vertexCount());
////#endif	//NDEBUG
//	}

	void debugVerifyCcEdgeCount(const int & offset=0)const{
//#ifndef NDEBUG
		ASSERT_EQUAL(debugCc.edgeCount()+offset,cc.edgeCount());
//#endif	//NDEBUG
	}

	void debugVerifyCcNodeCount(const int & offset=0)const{
//#ifndef NDEBUG
		ASSERT_EQUAL(debugCc.vertexCount()+offset,cc.vertexCount());
//#endif	//NDEBUG
	}

	void debugVerifyN2cSize(const int & offset=0)const{
//#ifndef NDEBUG
		ASSERT_EQUAL(debugN2c.size()+offset,n2c.size());
//#endif	//NDEBUG
	}

	void debugVerifyInnerSize(const int & offset=0)const{
//#ifndef NDEBUG
		ASSERT_EQUAL(debugInner.size()+offset,inner.size());
//#endif	//NDEBUG
	}

	void debugVerifyTotalSize(const int & offset=0)const{
//#ifndef NDEBUG
		ASSERT_EQUAL(debugTotal.size()+offset,total.size());
//#endif	//NDEBUG
	}

//	void debugVerifyNodesAndEdges(const typeVertex & source=noVertex,const typeVertex & destination=noVertex)const{
////#ifndef NDEBUG
//		const typeVertexList & nn=debugCc.getVertices();
//		const typeVertexList & n=GraphUndirected::getVertices();
//		for(typeVertexListIteratorConst it=nn.cbegin();it!=nn.cend();++it){//for each node in debug
//			const typeVertex & p=*it;
//			if(p==source || p==destination){
//				//skip
//			}
//			else{
//				ASSERT_NOT_EQUAL_ITERATOR(n.find(p),n.cend()); //check if exists in current graph
//			}
//		}
//		//check if source and destination nodes exist
////		ASSERT_NOT_EQUAL_ITERATOR(n.find(source),n.cend()); //check if exists in current graph
////		ASSERT_NOT_EQUAL_ITERATOR(n.find(destination),n.cend()); //check if exists in current graph
//		const typeLinksRangeConst & ee=debugGraphUndirected.edges();
//		for(typeLinksIteratorConst it=ee.first;it!=ee.second;++it){//for each edge
//			const typeLinksPair & p=*it;
//			const typeVertex & src=p.first;
//			const typeVertex & dst=p.second;
//			if((src==source && dst==destination) || (src==destination && dst==source)){
//				//skip
//			}
//			else{
//				const typeWeight & w=p.second.weight();
//				typeWeight ww=weight(src,dst);
////				ASSERT(!std::isnan(ww));
//				ASSERT_NOT_NAN(ww);
//				ASSERT_NOT_APPROXIMATE(ww,0,ASSERT_PRECISION_LIMIT);//check that it is not zero
//				ASSERT_EQUAL(ww,w);
////				ASSERT_EQUAL(source,noVertex);//check that vertex is valid
////				ASSERT_EQUAL(destination,noVertex);//check that vertex is valid
////				ASSERT_NAN(weight);//check that there is a value
////				ASSERT_NOT_APPROXIMATE(weight,0,ASSERT_PRECISION_LIMIT);//check that it is not zero
//			}
//		}
////#endif	//NDEBUG
//	}

	void debugVerifyN2c(const typeVertex & source=noVertex,const typeVertex & destination=noVertex)const{
//#ifndef NDEBUG
		/*
		 * verify n2c. Existing communities must not change when adding/removing
		 * an edge except for the communities of the source and destination nodes
		 * since they could have been disbanded
		 */
//		const typeCommunityListIteratorConst & itt=debugN2c.find(source);
//		const typeCommunity & srcc=(itt==debugN2c.cend())?noGroup:itt->second;
//		const typeCommunityListIteratorConst & itt2=debugN2c.find(destination);
//		const typeCommunity & dstc=(itt2==debugN2c.cend())?noGroup:itt2->second;
		for(typeCommunityListIteratorConst it=debugN2c.cbegin();it!=debugN2c.cend();++it){
			const typeCommunityListPair & p=*it;
			const typeVertex & v=p.first;
			const typeCommunity & c=p.second;
			if(v==source || v==destination){// || c==srcc || c==dstc){
				//skip
			}
			else{
				typeCommunityListIteratorConst itt=n2c.find(v);
				ASSERT_NOT_EQUAL_ITERATOR(itt,n2c.cend());//must exist
				ASSERT_EQUAL(itt->second,c);//and have the same community as before
				ASSERT_NOT_EQUAL(itt->second,noGroup);//verify community is valid
			}
		}
//#endif	//NDEBUG
	}

	void debugVerifyInner(const typeCommunity & source=noGroup,const typeCommunity & destination=noGroup)const{
//#ifndef NDEBUG
		for(typeCommunityEdgesIteratorConst it=debugInner.cbegin();it!=debugInner.cend();++it){
			const typeCommunityEdgesPair & p=*it;
			if(p.first==source || p.first==destination){
				//skip
			}
			else{
				typeCommunityEdgesIteratorConst itt=inner.find(p.first);
				ASSERT_NOT_EQUAL_ITERATOR(itt,inner.cend());//must exist
				ASSERT_EQUAL(itt->second,p.second);//and have the same weight as before
				ASSERT_NOT_NAN(itt->second);
				ASSERT_NOT_APPROXIMATE(itt->second,0,ASSERT_PRECISION_LIMIT);//check that it is not zero
			}
		}
//#endif	//NDEBUG
	}

	void debugVerifyTotal(const typeCommunity & source=noGroup,const typeCommunity & destination=noGroup)const{
//#ifndef NDEBUG
		for(typeCommunityEdgesIteratorConst it=debugTotal.cbegin();it!=debugTotal.cend();++it){
			const typeCommunityEdgesPair & p=*it;
			if(p.first==source || p.first==destination){
				//skip
			}
			else{
				typeCommunityEdgesIteratorConst itt=total.find(p.first);
				ASSERT_NOT_EQUAL_ITERATOR(itt,total.cend());//must exist
				ASSERT_EQUAL(itt->second,p.second);//and have the same weight as before
				ASSERT_NOT_NAN(itt->second);
				ASSERT_NOT_APPROXIMATE(itt->second,0,ASSERT_PRECISION_LIMIT);//check that it is not zero
			}
		}
//#endif	//NDEBUG
	}

	void debugVerifyCc(const typeCommunity & source=noGroup,const typeCommunity & destination=noGroup)const{
//#ifndef NDEBUG
		const typeLinksRangeConst & ee=debugCc.edges();
		for(typeLinksIteratorConst it=ee.first;it!=ee.second;++it){//for each edge
			const typeLinksPair & p=*it;
			const typeCommunity & src=p.first;
			const typeCommunity & dst=p.second;
			if(src==source || src==destination || dst==destination || dst==source){
				//skip
			}
			else{
				const typeWeight & w=p.second.weight();
				typeWeight ww=cc.weight(src,dst);
//				ASSERT(!std::isnan(ww));
				ASSERT_NOT_NAN(ww);
				ASSERT_NOT_APPROXIMATE(ww,0,ASSERT_PRECISION_LIMIT);//check that it is not zero
				ASSERT_EQUAL(ww,w);
			}
		}
//#endif	//NDEBUG
	}

	void debugVerifySaveState()const{
//#ifndef NDEBUG
//		debugVerifyEdgeCount();
//		debugVerifyNodeCount();
		debugVerifyN2cSize();
		debugVerifyInnerSize();
		debugVerifyTotalSize();
		debugVerifyCcEdgeCount();
		debugVerifyCcNodeCount();
		//verify GraphUndirected
//		debugVerifyNodesAndEdges();
		//verify values
		debugVerifyN2c();
		debugVerifyInner();
		debugVerifyTotal();
		//verify cc
		debugVerifyCc();
//#endif	//NDEBUG
	}

	void debugVerify() const{
//#ifndef NDEBUG
		if(dbg.debugLevel()>=DEBUG_LEVEL::VERIFY){
			//verify sizes
			ASSERT_EQUAL(n2c.size(),vertexCount());
			//verify that all nodes have communities
			const typeVertexList & nd=GraphUndirected::getVertices();
			for(typeVertexListIteratorConst it=nd.cbegin();it!=nd.cend();++it){
				const typeVertex & p=*it;
				typeCommunityListIteratorConst x=n2c.find(p);
				ASSERT_NOT_EQUAL_ITERATOR(x,n2c.cend());//verify vertex to community pair exists
				ASSERT_NOT_EQUAL_ITERATOR(nd.find(x->second),nd.cend());//verify community is an existing vertex
				ASSERT_NOT_EQUAL(x->second,noGroup);//verify community is valid
			}
			//verify inner/total
			typeCommunityEdges ii;
			typeCommunityEdges tt;
			GraphUndirected ccc;
			const typeLinksRangeConst & e=GraphUndirected::edges();
			for(typeLinksIteratorConst it=e.first;it!=e.second;++it){//for each edge
				const typeLinksPair & p=*it;
				const typeVertex & src=p.first;
				const typeVertex & dst=p.second;
				const typeWeight & w=p.second.weight();
				const typeCommunity & srcc=n2c[src];
				const typeCommunity & dstc=n2c[dst];
				if(src==dst){
					ii[srcc]+=w;
					tt[srcc]+=w;
				}
				else{//src!=dst
					//NOTE: addEdge also adds the reverse edge so only add the edges where srcc<dstc. Otherwise weights will appear doubled
					if(src<dst){
						if(srcc==dstc){
			//				ii[srcc]+=w;
			//				tt[srcc]+=w;
							ii[srcc]+=2*w;
							tt[srcc]+=2*w;
						}
						else{
							tt[srcc]+=w;
							tt[dstc]+=w;
							typeWeight wc=ccc.weight(srcc,dstc);
							if(std::isnan(wc)) ccc.addEdge(srcc,dstc,w);
							else ccc.addEdge(srcc,dstc,wc+w,true);
						}
					}
				}
			}
			ASSERT_EQUAL(ii.size(),inner.size());
	//		ASSERT_EQUAL(tt.size(),total.size());
			for(typeCommunityEdgesIteratorConst it=tt.cbegin();it!=tt.cend();++it){
				const typeCommunityEdgesPair & p=*it;
				typeCommunityEdgesIteratorConst itt=total.find(p.first);
				ASSERT_NOT_EQUAL_ITERATOR(itt,total.cend());//make sure community entry exists
				ASSERT_APPROXIMATE(p.second,itt->second,ASSERT_PRECISION_LIMIT);//check if weight matches
				//verify inner for same community if it exists
				typeCommunityEdgesIteratorConst iti=ii.find(p.first);
				if(iti!=ii.cend()){
					typeCommunityEdgesIteratorConst itti=inner.find(p.first);
					ASSERT_NOT_EQUAL_ITERATOR(itti,inner.cend());//make sure community entry exists
					ASSERT_APPROXIMATE(iti->second,itti->second,ASSERT_PRECISION_LIMIT);//check if weight matches
					//check inner is smaller or equal than total
					ASSERT_SMALLER_EQUAL(itti->second,itt->second,ASSERT_PRECISION_LIMIT);
				}
			}
			//verify community to community mapping
			ASSERT_EQUAL(ccc.edgeCount(),cc.edgeCount());
			const typeLinksRangeConst & ee=ccc.edges();
			for(typeLinksIteratorConst it=ee.first;it!=ee.second;++it){//for each edge
				const typeLinksPair & p=*it;
				const typeVertex & src=p.first;
				const typeVertex & dst=p.second;
				const typeWeight & w=p.second.weight();
				typeWeight ww=cc.weight(src,dst);
				ASSERT_NOT_NAN(ww);
				ASSERT_NOT_APPROXIMATE(ww,0,ASSERT_PRECISION_LIMIT);//check that it is not zero
				ASSERT_APPROXIMATE(ww,w,ASSERT_PRECISION_LIMIT);
			}
		}
//#endif	//NDEBUG
	}

	void debugSaveState(){
		if(dbg.debugLevel()>=DEBUG_LEVEL::VERIFY){
			debugVerify();
			//copy listings
	//		debugGraphUndirected=GraphUndirected(*this);
			debugN2c=typeCommunityList(n2c);
			debugInner=typeCommunityEdges(inner);
			debugTotal=typeCommunityEdges(total);
			debugCc=GraphUndirected(cc);
			debugVerifySaveState();
		}
	}

	/**
	 * Verifies consistency of internal data and conformity of implementation
	 *
	 * @param added
	 * @param source
	 * @param destination
	 * @param weight
	 * @param replace
	 */
	void debugAddEdge(const bool & added,const typeVertex & source, const typeVertex & destination, const typeWeight & weight=1.0, const typeWeight & weightOld=0,const bool & replace=false) const{
		if(dbg.debugLevel()>=DEBUG_LEVEL::VERIFY){
			debugVerify();
			if(added){
				const typeCommunityListIteratorConst & itt=debugN2c.find(source);//get old community
				const typeCommunity & srcc=(itt==debugN2c.cend())?noGroup:itt->second;//check it existed
				const typeCommunityListIteratorConst & itt2=debugN2c.find(destination);//get old community
				const typeCommunity & dstc=(itt2==debugN2c.cend())?noGroup:itt2->second;//check it existed
				typeCommunity srcn=noGroup;//new community
				typeCommunity dstn=noGroup;//new community
				//verify all other communities. Eveything should be the same except for nodes belonging to added edge
				debugVerifyN2c(source, destination);
				debugVerifyInner(srcc,dstc);
				debugVerifyTotal(srcc,dstc);
				debugVerifyCc(srcc,dstc);
				{
					const typeCommunityListIteratorConst & it=n2c.find(source);//get community mapping
					ASSERT_NOT_EQUAL_ITERATOR(it,n2c.cend());//check it exists now
					srcn=it->second;//get its community
					if(srcc==noGroup){//vertex did not exist
						ASSERT_EQUAL(source,srcn);//verify the community is the vertex itself
					}
					else{//vertex already existed
						ASSERT_EQUAL(srcc,srcn);//verify the community has not changed
					}
				}
				if(source==destination){//same vertex
					const typeCommunityEdgesIteratorConst & it=inner.find(srcn);//get inner
					ASSERT_NOT_EQUAL_ITERATOR(it,inner.cend());//check it exists now
					const typeCommunityEdgesIteratorConst & it1=total.find(srcn);//get total
					ASSERT_NOT_EQUAL_ITERATOR(it1,total.cend());//check it exists now
					if(srcc==noGroup){//vertex did not exist
	//					ASSERT(!replace);//Fatal error. Requested replace an edge that did not exist and replaced it anyway
						ASSERT_APPROXIMATE(it->second,weight, ASSERT_PRECISION_LIMIT);//check inner
						ASSERT_APPROXIMATE(it1->second,weight, ASSERT_PRECISION_LIMIT);//check total
					}
					else{//vertex already existed
						const typeCommunityEdgesIteratorConst & it2=debugInner.find(srcc);//get old inner
	//					ASSERT_NOT_EQUAL_ITERATOR(it2,debugInner.cend());//check it existed. Should never fail here
						typeWeight in=0;
						if(it2!=debugInner.cend()){//inner entry existed
							in=it2->second;
						}
						const typeCommunityEdgesIteratorConst & it3=debugTotal.find(srcc);//get total
						ASSERT_NOT_EQUAL_ITERATOR(it3,debugTotal.cend());//check it existed. Should never fail here
						if(replace){
							ASSERT_APPROXIMATE(it->second,in+weight-weightOld, ASSERT_PRECISION_LIMIT);//check inner
							ASSERT_APPROXIMATE(it1->second,it3->second+weight-weightOld, ASSERT_PRECISION_LIMIT);//check total
						}
						else{
							ASSERT_APPROXIMATE(it->second,in+weight, ASSERT_PRECISION_LIMIT);//check inner
							ASSERT_APPROXIMATE(it1->second,it3->second+weight, ASSERT_PRECISION_LIMIT);//check total
						}
					}
				}
				else{//source!=destination
					{
						const typeCommunityListIteratorConst & it=n2c.find(destination);//get community mapping
						ASSERT_NOT_EQUAL_ITERATOR(it,n2c.cend());//check it exists now
						dstn=it->second;//get its community
						if(dstc==noGroup){//vertex did not exist
							ASSERT_EQUAL(destination,dstn);//verify the community is the vertex itself
						}
						else{//vertex already existed
							ASSERT_EQUAL(dstc,dstn);//verify the community has not changed
						}
					}
					const typeCommunityEdgesIteratorConst & it1=total.find(srcn);//get total
					ASSERT_NOT_EQUAL_ITERATOR(it1,total.cend());//check it exists now
					//check cc graph
					if(srcn!=dstn){
						const typeCommunityEdgesIteratorConst & it2=total.find(dstn);//get total
						ASSERT_NOT_EQUAL_ITERATOR(it2,total.cend());//check it exists now
						const typeCommunityEdgesIteratorConst & it3=debugTotal.find(srcn);//get total
						const typeCommunityEdgesIteratorConst & it4=debugTotal.find(dstn);//get total
						const typeWeight & wo=debugCc.weight(srcc, dstc);
						const typeWeight & wn=cc.weight(srcn, dstn);
						ASSERT_NOT_NAN(wn);//communities edge must exist now
						if(srcc==noGroup || dstc==noGroup){//edge did not exist
		//					ASSERT(!replace);//Fatal error. Requested replace an edge that did not exist and replaced it anyway
//							ASSERT_APPROXIMATE(it1->second,(it3==debugTotal.cend())?2*weight:it3->second+2*weight, ASSERT_PRECISION_LIMIT);//check total
//							ASSERT_APPROXIMATE(it2->second,(it4==debugTotal.cend())?2*weight:it4->second+2*weight, ASSERT_PRECISION_LIMIT);//check total
							ASSERT_APPROXIMATE(it1->second,(it3==debugTotal.cend())?weight:it3->second+weight, ASSERT_PRECISION_LIMIT);//check total
							ASSERT_APPROXIMATE(it2->second,(it4==debugTotal.cend())?weight:it4->second+weight, ASSERT_PRECISION_LIMIT);//check total
							if(std::isnan(wo)){//communities edge did not existed
								ASSERT_APPROXIMATE(wn,weight, ASSERT_PRECISION_LIMIT);//must match given weight
							}
							else{//communities edge existed
								ASSERT_APPROXIMATE(wn,wo+weight, ASSERT_PRECISION_LIMIT);//must match given weight plus old weight
							}
						}
						else{//old communities already existed
							ASSERT_NOT_EQUAL_ITERATOR(it3,debugTotal.cend());//check it existed
							ASSERT_NOT_EQUAL_ITERATOR(it4,debugTotal.cend());//check it existed
//							ASSERT_APPROXIMATE(it1->second,it3->second+2*weight-2*weightOld, ASSERT_PRECISION_LIMIT);//check total
//							ASSERT_APPROXIMATE(it2->second,it4->second+2*weight-2*weightOld, ASSERT_PRECISION_LIMIT);//check total
							ASSERT_APPROXIMATE(it1->second,it3->second+weight-weightOld, ASSERT_PRECISION_LIMIT);//check total
							ASSERT_APPROXIMATE(it2->second,it4->second+weight-weightOld, ASSERT_PRECISION_LIMIT);//check total
							if(std::isnan(wo)){//communities edge did not existed
								ASSERT_APPROXIMATE(wn,weight, ASSERT_PRECISION_LIMIT);//must match given weight
							}
							else{//communities edge existed
								ASSERT_APPROXIMATE(wn,wo+weight-weightOld, ASSERT_PRECISION_LIMIT);//must match given weight
							}
						}
					}
					else{//srcc==dstc
						const typeCommunityEdgesIteratorConst & it2=total.find(dstn);//get total
						ASSERT_NOT_EQUAL_ITERATOR(it2,total.cend());//check it exists now
						const typeCommunityEdgesIteratorConst & it3=debugTotal.find(srcn);//get total
//						const typeCommunityEdgesIteratorConst & it4=debugTotal.find(dstn);//get total
//						const typeWeight & wo=debugCc.weight(srcc, dstc);
//						const typeWeight & wn=cc.weight(srcn, dstn);
//						ASSERT_NOT_NAN(wn);//communities edge must exist now
						if(srcc==noGroup || dstc==noGroup){//edge did not exist
		//					ASSERT(!replace);//Fatal error. Requested replace an edge that did not exist and replaced it anyway
//							ASSERT_APPROXIMATE(it1->second,(it3==debugTotal.cend())?2*weight:it3->second+2*weight, ASSERT_PRECISION_LIMIT);//check total
							if(source==destination){
								ASSERT_APPROXIMATE(it1->second,(it3==debugTotal.cend())?weight:it3->second+weight, ASSERT_PRECISION_LIMIT);//check total
							}
							else{
								ASSERT_APPROXIMATE(it1->second,(it3==debugTotal.cend())?2*weight:it3->second+2*weight, ASSERT_PRECISION_LIMIT);//check total
							}
//							ASSERT_APPROXIMATE(it2->second,(it4==debugTotal.cend())?2*weight:it4->second+2*weight, ASSERT_PRECISION_LIMIT);//check total
//							ASSERT_APPROXIMATE(it1->second,(it3==debugTotal.cend())?weight:it3->second+weight, ASSERT_PRECISION_LIMIT);//check total
//							ASSERT_APPROXIMATE(it2->second,(it4==debugTotal.cend())?weight:it4->second+weight, ASSERT_PRECISION_LIMIT);//check total
//							if(std::isnan(wo)){//communities edge did not existed
//								ASSERT_APPROXIMATE(wn,weight, ASSERT_PRECISION_LIMIT);//must match given weight
//							}
//							else{//communities edge existed
//								ASSERT_APPROXIMATE(wn,wo+weight, ASSERT_PRECISION_LIMIT);//must match given weight plus old weight
//							}
						}
						else{//old communities already existed
							ASSERT_NOT_EQUAL_ITERATOR(it3,debugTotal.cend());//check it existed
//							ASSERT_NOT_EQUAL_ITERATOR(it4,debugTotal.cend());//check it existed
//							ASSERT_APPROXIMATE(it1->second,it3->second+2*weight-2*weightOld, ASSERT_PRECISION_LIMIT);//check total
							if(source==destination){
								ASSERT_APPROXIMATE(it1->second,it3->second+weight-weightOld, ASSERT_PRECISION_LIMIT);//check total
							}
							else{
								ASSERT_APPROXIMATE(it1->second,it3->second+2*weight-2*weightOld, ASSERT_PRECISION_LIMIT);//check total
							}
//							ASSERT_APPROXIMATE(it2->second,it4->second+2*weight-2*weightOld, ASSERT_PRECISION_LIMIT);//check total
//							ASSERT_APPROXIMATE(it1->second,it3->second+weight-weightOld, ASSERT_PRECISION_LIMIT);//check total
//							ASSERT_APPROXIMATE(it2->second,it4->second+weight-weightOld, ASSERT_PRECISION_LIMIT);//check total
//							if(std::isnan(wo)){//communities edge did not existed
//								ASSERT_APPROXIMATE(wn,weight, ASSERT_PRECISION_LIMIT);//must match given weight
//							}
//							else{//communities edge existed
//								ASSERT_APPROXIMATE(wn,wo+weight-weightOld, ASSERT_PRECISION_LIMIT);//must match given weight
//							}
						}
					}
				}
			}
			else{//edge not added
				//check if current mapping is identical to previous
				debugVerifySaveState();
			}
		}
	}

	void debugRemove(const bool & removed,const typeVertex & source, const typeVertex & destination, const typeWeight & weightOldEdge=0) const{
		if(dbg.debugLevel()>=DEBUG_LEVEL::VERIFY){
			debugVerify();
			if(removed){
				typeCommunityListIteratorConst its=debugN2c.find(source);
				typeCommunityListIteratorConst itd=debugN2c.find(destination);
				if(its!=debugN2c.cend() && itd!=debugN2c.cend()){//vertices existed
					const typeCommunity & srcc=its->second;
					const typeCommunity & dstc=itd->second;
					//verify all other communities
					debugVerifyN2c(source, destination);
					debugVerifyInner(srcc,dstc);
					debugVerifyTotal(srcc,dstc);
					debugVerifyCc(srcc,dstc);
					const typeCommunityEdgesIteratorConst & itto=debugTotal.find(srcc);//get total
					ASSERT_NOT_EQUAL_ITERATOR(itto,debugTotal.cend());//check it existed
					const typeCommunityEdgesIteratorConst & ittn=total.find(srcc);
					const typeWeight & wts=(ittn==total.cend())?0:ittn->second;
					if(wts==0){//check community no longer exists
						typeCommunityListRange itsn=n2c.keys(srcc);
						ASSERT_EQUAL_ITERATOR(itsn.first,itsn.second);
						ASSERT_EQUAL_ITERATOR(ittn,total.cend());//check that there is no total entry
					}
					if(srcc==dstc){//inner edge
						//check inner and total were decreased by weightOldEdge
						const typeCommunityEdgesIteratorConst & itio=debugInner.find(srcc);//get inner
						ASSERT_NOT_EQUAL_ITERATOR(itio,debugInner.cend());//check it existed
						const typeCommunityEdgesIteratorConst & itin=inner.find(srcc);
						const typeWeight & wi=(itin==inner.cend())?0:itin->second;
						if(wts==0){
							ASSERT_APPROXIMATE(wi,0, ASSERT_PRECISION_LIMIT);//if total is zero then so must inner
							ASSERT_EQUAL_ITERATOR(itin,inner.cend());//check that there is no inner entry
						}
//						ASSERT_APPROXIMATE(itio->second-wi,weightOldEdge, ASSERT_PRECISION_LIMIT);//check inner
//						ASSERT_APPROXIMATE(itto->second-wts,weightOldEdge, ASSERT_PRECISION_LIMIT);//check total
						if(source==destination){
							ASSERT_APPROXIMATE(itio->second-wi,weightOldEdge, ASSERT_PRECISION_LIMIT);//check inner
							ASSERT_APPROXIMATE(itto->second-wts,weightOldEdge, ASSERT_PRECISION_LIMIT);//check total
						}
						else{
							ASSERT_APPROXIMATE(itio->second-wi,2*weightOldEdge, ASSERT_PRECISION_LIMIT);//check inner
							ASSERT_APPROXIMATE(itto->second-wts,2*weightOldEdge, ASSERT_PRECISION_LIMIT);//check total
						}
						if(wi==0){
							ASSERT_EQUAL_ITERATOR(itin,inner.cend());//check that there is no inner entry
						}
					}
					else{//not an inner edge
						const typeWeight & wo=debugCc.weight(srcc, dstc);//get old community weight
						ASSERT_NOT_NAN(wo);//if it was removed it must have existed
						//check community edge weight was decreased
						const typeWeight & wn=cc.weight(srcc, dstc);//get new community weight
						if(std::isnan(wn)){//check if it still exists
							ASSERT_APPROXIMATE(wo,weightOldEdge, ASSERT_PRECISION_LIMIT);//check community weight
						}
						else{
							ASSERT_APPROXIMATE(wo-wn,weightOldEdge, ASSERT_PRECISION_LIMIT);//check community weight
						}
//						ASSERT_APPROXIMATE(itto->second-wts,2*weightOldEdge, ASSERT_PRECISION_LIMIT);//check total
						ASSERT_APPROXIMATE(itto->second-wts,weightOldEdge, ASSERT_PRECISION_LIMIT);//check total
					}
				}
				else{//at least one of the vertices did not existed
					//can not remove an edge that does not exist
					ASSERT_NOT_EQUAL_ITERATOR(its,debugN2c.cend());//check it existed
					ASSERT_NOT_EQUAL_ITERATOR(itd,debugN2c.cend());//check it existed
				}
			}
			else{// !removed
				debugVerifySaveState();//check if current mapping is identical to previous
			}
		}
	}

	void debugReplace(const bool & replaced,const typeVertex & oldValue, const typeVertex & newValue) const{
		if(dbg.debugLevel()>=DEBUG_LEVEL::VERIFY){
			debugVerify();
			if(replaced){
				//check if oldValue is the community
				typeCommunityListIteratorConst a=debugN2c.value(oldValue);
				ASSERT_NOT_EQUAL_ITERATOR(a,debugN2c.cend());//check it existed
				bool wasCommunity=false;
				if(a->second==oldValue) wasCommunity=true;
				//get the new minimum vertex of the community that will become the community
				typeCommunityListRange r=debugN2c.keys(a->second);
				typeVertex min=newValue;
				for(typeCommunityListRangeIteratorConst it=r.first;it!=r.second;++it){
					if(it->first<min){
						min=it->first;
					}
				}
				//check the found minimum is the new community
				typeCommunityListIteratorConst b=n2c.value(newValue);
				ASSERT_NOT_EQUAL_ITERATOR(b,n2c.cend());//check it exists
				ASSERT_EQUAL(b->second,min);//check it matches
				//check all community mappings
				for(typeCommunityListIteratorConst it=debugN2c.cbegin();it!=debugN2c.cend();++it){
					typeCommunityListIteratorConst c=n2c.value(it->first);
					ASSERT_NOT_EQUAL_ITERATOR(c,n2c.cend());//check it exists
					if(wasCommunity){//community changed
						ASSERT_EQUAL(c->second,min);//check it matches new community
					}
					else{
						ASSERT_EQUAL(c->second,b->second);//check it matches old community
					}
				}
				//if community has changed, check inner and total
				if(wasCommunity){
					typeCommunityEdgesIteratorConst i=inner.find(min);
					ASSERT_NOT_EQUAL_ITERATOR(i,inner.cend());//check it exists
					typeCommunityEdgesIteratorConst io=debugInner.find(oldValue);
					ASSERT_NOT_EQUAL_ITERATOR(io,debugInner.cend());//check it existed
					ASSERT_APPROXIMATE(i->second,io->second,ASSERT_PRECISION_LIMIT);//check it matches
					typeCommunityEdgesIteratorConst t=total.find(min);
					ASSERT_NOT_EQUAL_ITERATOR(t,total.cend());//check it exists
					typeCommunityEdgesIteratorConst to=debugTotal.find(oldValue);
					ASSERT_NOT_EQUAL_ITERATOR(to,debugTotal.cend());//check it existed
					ASSERT_APPROXIMATE(t->second,to->second,ASSERT_PRECISION_LIMIT);//check it matches
				}
			}
			else{
				debugVerifySaveState();
			}
		}
	}

	void debugCommunity(const bool & result,const typeVertex & vertex, const typeCommunity & com){
		if(dbg.debugLevel()>=DEBUG_LEVEL::VERIFY){
			debugVerify();
			const typeCommunityListIteratorConst & itt=debugN2c.find(vertex);
			ASSERT_NOT_EQUAL_ITERATOR(itt,debugN2c.cend());//check it exists
			const typeCommunity & srcc=itt->second;//original community
			const typeCommunity & srcn=minimumNode(debugN2c,srcc, vertex);
			const typeCommunity & dstc=(vertex<com)?vertex:com;//destination community is com unless vertex is the new minimum
			{
				const typeCommunityListIteratorConst & itn=n2c.find(vertex);//get community mapping
				ASSERT_NOT_EQUAL_ITERATOR(itn,n2c.cend());//check it exists
	//			if(com<vertex){
					ASSERT_EQUAL(itn->second,dstc);//check it equals the intended community
	//			}
	//			else{
	//				ASSERT_EQUAL(itn->second,vertex);//check it equals the intended community
	//			}
			}
	//		debugVerifyN2c(vertex);//verify all other communities have not changed
	//		std::set<typeCommunity> ccs=debugN2c.keys();
	//		for(std::set<typeCommunity>::const_iterator itccs=ccs.cbegin();itccs!=ccs.cend();++itccs){
	//
	//		}
			for(typeCommunityListIteratorConst it=debugN2c.cbegin();it!=debugN2c.cend();++it){
				const typeCommunityListPair & p=*it;
				const typeVertex & v=p.first;
				if(v!=vertex){
					const typeCommunity & c=p.second;
					if(c==srcc){//there are more vertices in the original source community
						typeCommunityListIteratorConst itt=n2c.find(v);
						ASSERT_NOT_EQUAL_ITERATOR(itt,n2c.cend());//must exist
						ASSERT_EQUAL(itt->second,srcn);//must match the new minimum
					}
					else if(c==com){//it is intended community
						typeCommunityListIteratorConst itt=n2c.find(v);
						ASSERT_NOT_EQUAL_ITERATOR(itt,n2c.cend());//must exist
						ASSERT_EQUAL(itt->second,dstc);//must match the new destination community
					}
					else{
						typeCommunityListIteratorConst itt=n2c.find(v);
						ASSERT_NOT_EQUAL_ITERATOR(itt,n2c.cend());//must exist
						ASSERT_EQUAL(itt->second,c);//and have the same community as before
						ASSERT_NOT_EQUAL(itt->second,noGroup);//verify community is valid
					}
				}
			}
			//recreate inner, total and cc to see if they match
			typeWeight innerDeltaSource=0;//inner connections in srcc
	//		typeWeight totalDeltaSource=0;
			typeWeight innerDeltaDestination=0;//inner connections in dstc
	//		typeWeight totalDeltaDestination=0;
			std::map<typeCommunity,typeWeight> totalDelta;//added to debugTotal
			std::map<typeCommunity,typeWeight> ccw;//original community-community of vertex and neighbours. Subtracted from srcc
			std::map<typeCommunity,typeWeight> ccwn;//new community-community of vertex and neighbours. Added to dstc
			typeLinksRangeConst nei=neighbours(vertex);
			for(typeLinksIteratorConst it=nei.first;it!=nei.second;++it){//for all neighbours
				const typeCommunityListIteratorConst & itn=debugN2c.find(it->second);//get neighbour entry
				ASSERT_NOT_EQUAL_ITERATOR(itn,debugN2c.cend());//check it exists
				const typeCommunity & neic=itn->second;//community of neighbour
				const typeWeight & w=it->second.weight();//get weight
				if(srcc==neic){//was in same community
					if(vertex==srcc){//srcc is renamed srcn
						totalDelta[srcn]-=w;
					}
					else{//not renamed
						totalDelta[srcc]-=w;
					}
					innerDeltaSource-=2*w;
	//				totalDelta[srcc]-=w;
					totalDelta[dstc]+=2*w;
					ccwn[neic]+=w;
				}
				else{//not in same community
	//				totalDelta[srcc]-=2*w;
					if(vertex==srcc){//srcc is renamed srcn
						totalDelta[srcn]-=2*w;
					}
					else{//not renamed
						totalDelta[srcc]-=2*w;
					}
					totalDelta[neic]-=2*w;
					ccw[neic]-=w;
					if(neic==com){//is going to the same community
						innerDeltaDestination+=2*w;
	//					totalDelta[dstc]-=2*w;
						totalDelta[dstc]+=w;
					}
					else{//going to different communities
						ccwn[neic]+=w;
						totalDelta[neic]+=2*w;
						totalDelta[dstc]+=2*w;
					}
				}
			}
	//		const typeCommunities cs=communities();
			//get all previous communities
			typeCommunities cs(debugCc.getVertices().cbegin(),debugCc.getVertices().cend());
			for(typeCommunityEdgesIteratorConst it=debugInner.cbegin();it!=debugInner.cend();++it){
				cs.insert(it->first);
			}
			for(typeCommunities::const_iterator c=cs.cbegin();c!=cs.cend();++c){//for all communities in debugInner
				//check inner
				typeCommunityEdgesIteratorConst it=debugInner.find(*c);
				typeWeight itv=(it==debugInner.cend())?0:it->second;
	//			typeCommunityEdgesIteratorConst itn=inner.find(*c);
				if(*c==srcc){//is the community the vertex was removed from
					if(vertex==srcc){//was the community
						typeCommunityEdgesIteratorConst itn=inner.find(srcn);//srcc is renamed srcn when vertex is removed
						if(innerDeltaSource!=0){//check it was part of inner connections
							if(itv+innerDeltaSource!=0){//if the outcome is not zero. There are still vertices in community
								ASSERT_NOT_EQUAL_ITERATOR(itn,inner.cend());//check it exists
								ASSERT_EQUAL(itn->second,itv+innerDeltaSource);//check result
							}
							else{//outcome is zero
								ASSERT_EQUAL_ITERATOR(itn,inner.cend());//check it does not exist now
							}
						}
						else{//not part of inner connections. Probably was single vertex
							ASSERT_EQUAL_ITERATOR(itn,inner.cend());//inner entry does not exist anymore
						}
					}
					else{//vertex was not the community
						typeCommunityEdgesIteratorConst itn=inner.find(srcc);
						if(innerDeltaSource!=0){//check it was part of inner connections
							if(itv+innerDeltaSource!=0){//if the outcome is not zero. There are still vertices in community
								ASSERT_NOT_EQUAL_ITERATOR(itn,inner.cend());//check it exists
								ASSERT_EQUAL(itn->second,itv+innerDeltaSource);//check result
							}
							else{//outcome is zero
								ASSERT_EQUAL_ITERATOR(itn,inner.cend());//check it does not exist now
							}
						}
						else{//not part of inner connections. Probably was single vertex
							ASSERT_EQUAL_ITERATOR(itn,inner.cend());//inner entry does not exist anymore
						}
					}
				}
				else if(*c==com){//it is the community the vertex was moved to
					typeCommunityEdgesIteratorConst itn=inner.find(dstc);
					if(vertex==dstc){//became the community so com was renamed to vertex
						typeCommunityEdgesIteratorConst ito=inner.find(com);
						ASSERT_EQUAL_ITERATOR(ito,inner.cend());//check com does not exist
						if(innerDeltaDestination!=0){//check it became part of inner connections
							ASSERT_NOT_EQUAL_ITERATOR(itn,inner.cend());//check it exists
							ASSERT_EQUAL(itn->second,itv+innerDeltaDestination);//check result
						}
						else{//not part of inner connections. Probably is single vertex
							ASSERT_EQUAL_ITERATOR(itn,inner.cend());//inner entry does not exist
						}
					}
					else{//vertex did not became the community
						if(innerDeltaDestination!=0){//check it became part of inner connections
							ASSERT_NOT_EQUAL_ITERATOR(itn,inner.cend());//check it exists
							ASSERT_EQUAL(itn->second,itv+innerDeltaDestination);//check result
						}
						else{//not part of inner connections. Probably is single vertex
							ASSERT_EQUAL_ITERATOR(itn,inner.cend());//inner entry does not exist
						}
					}
				}
				else if(*c==dstc){//ignore. Already processed

				}
				else{//check it has not changed
					typeCommunityEdgesIteratorConst itn=inner.find(*c);
					if(it!=debugInner.cend()){
						ASSERT_NOT_EQUAL_ITERATOR(itn,inner.cend());//check it exists
						ASSERT_EQUAL(itn->second,itv);//and the values are equal
					}
					else{
						ASSERT_EQUAL_ITERATOR(itn,inner.cend());//check it does not exist
					}
				}
				//check total
	//			for(typeCommunityEdgesIteratorConst it=debugTotal.cbegin();it!=debugTotal.cend();++it){
	//			for(typeCommunity c=cs.cbegin();c!=cs.cend();++c){//for all communities
	//				typeCommunityEdgesIteratorConst itt=debugTotal.find(*c);
					if(srcc==*c){//was in same community
						typeCommunityEdgesIteratorConst itt=debugTotal.find(*c);
						if(vertex==srcc){//srcc is renamed srcn
	//						typeCommunityEdgesIteratorConst itt=debugTotal.find(*c);
							typeCommunityEdgesIteratorConst ittn=total.find(srcn);
							if(itt->second+totalDelta[srcn]!=0){
								ASSERT_NOT_EQUAL_ITERATOR(ittn,total.cend());//check it exists
								ASSERT_EQUAL(ittn->second,itt->second+totalDelta[srcn]);
							}
							else{
								ASSERT_EQUAL_ITERATOR(ittn,total.cend());//check it does not exist
							}
						}
						else{//not renamed
							typeCommunityEdgesIteratorConst ittn=total.find(*c);
							if(itt->second+totalDelta[*c]!=0){
								ASSERT_NOT_EQUAL_ITERATOR(ittn,total.cend());//check it exists
								ASSERT_EQUAL(ittn->second,itt->second+totalDelta[*c]);
							}
							else{
								ASSERT_EQUAL_ITERATOR(ittn,total.cend());//check it does not exist
							}
						}
					}
					else if(com==*c){//is in same community
						typeCommunityEdgesIteratorConst itt=debugTotal.find(com);
	//					ASSERT_NOT_EQUAL_ITERATOR(itt,debugTotal.cend());//check it exists
						typeCommunityEdgesIteratorConst ittn=total.find(dstc);
						if(vertex==dstc){//com was renamed vertex
	//						typeCommunityEdgesIteratorConst itt=debugTotal.find(com);
	//						typeCommunityEdgesIteratorConst ittn=total.find(srcn);
							if(itt->second+totalDelta[dstc]!=0){
								ASSERT_NOT_EQUAL_ITERATOR(ittn,total.cend());//check it exists
								ASSERT_EQUAL(ittn->second,(itt==debugTotal.cend())?totalDelta[dstc]:itt->second+totalDelta[dstc]);
							}
							else{
								ASSERT_EQUAL_ITERATOR(ittn,total.cend());//check it does not exist
							}
						}
						else{//not renamed
							ASSERT_NOT_EQUAL_ITERATOR(itt,debugTotal.cend());//check it existed
	//						typeCommunityEdgesIteratorConst ittn=total.find(*c);
							if(itt->second+totalDelta[*c]!=0){
								ASSERT_NOT_EQUAL_ITERATOR(ittn,total.cend());//check it exists
								ASSERT_EQUAL(ittn->second,itt->second+totalDelta[*c]);
							}
							else{
								ASSERT_EQUAL_ITERATOR(ittn,total.cend());//check it does not exist
							}
						}
					}
					else{//not in same community
						typeCommunityEdgesIteratorConst itt=debugTotal.find(*c);
						typeCommunityEdgesIteratorConst ittn=total.find(*c);
						if(itt!=debugTotal.cend()){
							if(itt->second+totalDelta[*c]!=0){
								ASSERT_NOT_EQUAL_ITERATOR(ittn,total.cend());//check it exists
								ASSERT_EQUAL(ittn->second,itt->second+totalDelta[*c]);
							}
							else{
								ASSERT_EQUAL_ITERATOR(ittn,total.cend());//check it does not exist
							}
	//						ASSERT_EQUAL(ittn->second,itt->second);//check values are equal
						}
						else{
							ASSERT_NOT_EQUAL_ITERATOR(ittn,total.cend());//check it does not exist
						}
					}
	//			}
			}
			//check cc
			typeLinksRangeConst ed=debugCc.edges();
			for(typeLinksIteratorConst it=ed.first;it!=ed.second;++it){
				typeWeight ww=it->second.weight();
				const typeWeight & w=cc.weight(it->first, it->second);
				if(it->first==srcc){//one of the vertex is the original community
	//				typeWeight ww=ccw[it->second];//get difference from srcc
	//				if(it->second==com){
	//					if(it->second.weight()+ww!=0){
	//						ASSERT_EQUAL(w,it->second.weight()+ww);
	//					}
	//					else{
	//						ASSERT_NAN(cc.weight(it->first, it->second));
	//					}
	//				}
	//				else{//it->second!=com
	//					typeWeight wwn=ccwn[it->second];//get difference
	//					if(it->second.weight()+ww+wwn!=0){
	//						ASSERT_EQUAL(w,it->second.weight()+ww+wwn);
	//					}
	//					else{
	//						ASSERT_NAN(cc.weight(it->first, it->second));
	//					}
	//				}
					ww+=ccw[it->second];
				}
	//			else{//it->first!=srcc
	//				if(it->second==srcc){//one of the vertex is the original community
	//					typeWeight ww=ccw[it->first];//get difference from srcc
	//					if(it->first==com){
	//						if(it->second.weight()+ww!=0){
	//							ASSERT_EQUAL(w,it->second.weight()+ww);
	//						}
	//						else{
	//							ASSERT_NAN(cc.weight(it->first, it->second));
	//						}
	//					}
	//					else{//it->second!=com
	//						typeWeight wwn=ccwn[srcc];//get difference
	//						if(it->second.weight()+ww+wwn!=0){
	//							ASSERT_EQUAL(w,it->second.weight()+ww+wwn);
	//						}
	//						else{
	//							ASSERT_NAN(cc.weight(it->first, it->second));
	//						}
	//					}
	//				}
	//				else{//it->first!=srcc
	//
	//				}
	//			}
				if(it->second==srcc){//one of the vertex is the original community
					ww+=ccw[it->first];
				}
				if(it->first==dstc){//one of the vertex is the original community
					ww+=ccwn[it->second];
				}
				if(it->second==dstc){//one of the vertex is the original community
					ww+=ccwn[it->first];
				}
				if(ww!=0){
					ASSERT_EQUAL(w,ww);
				}
				else{
					ASSERT_NAN(cc.weight(it->first, it->second));
				}
	//			isnan
	//			typeCommunityEdgesIteratorConst itn=total.find(it->first);
	//			if(it->second+totalDelta[it->first]!=0){
	//				ASSERT_NOT_EQUAL_ITERATOR(itn,total.cend());//check it exists
	//				ASSERT_EQUAL(itn->second,it->second+innerDeltaSource);
	//			}
			}
		}
	}

	void debugDisband(const typeVertex & source, const typeVertex & destination){

	}

	void debugCommunitiesToGraph(const typeVertex & source, const typeVertex & destination){

	}

public:

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
		debugSaveState();
		dbg.pre(DEBUG_LEVEL::MODIFICATIONS,"GGa", debugPrint());
		dbg.msg(DEBUG_LEVEL::CALLS,"e"+std::to_string(source)+"#"+std::to_string(destination)+"="+std::to_string(weight)+";"+std::to_string(replace));
		//get old weight of edge if it exists
		typeWeight w=GraphUndirected::weight(source,destination);
		if(std::isnan(w)) w=0;
		//get existing nodes before adding new edge to check for self loops afterwards. Can not check before adding to graph because nodes might not exist
		bool b=GraphUndirected::addEdge(source,destination,weight,replace);
		if(b){
			const typeCommunity & c1=community(source);
			typeCommunity cc1=c1;
			if(c1==noGroup){
				n2c.add(source,source);
				cc1=source;
			}
			const typeCommunity & c2=community(destination);
			typeCommunity cc2=c2;
			if(c2==noGroup){
				n2c.add(destination,destination);
				cc2=destination;
			}
			typeWeight ww=weight;
			dbg.msg(DEBUG_LEVEL::VERIFY,"c1"+std::to_string(source)+"c2"+std::to_string(destination)+"="+std::to_string(ww));
			if(cc1==cc2){
				if(replace){
					ww-=w;
					if(fabsl(ww)<std::numeric_limits<typeWeight>::epsilon()*2) ww=0;
				}
				bool b=false;
				if(source==destination){// ww=ww/2;
					b=update(total,cc1,ww,true);
					if(b) update(inner,cc1,ww,true);
					else inner.erase(cc1);
				}
				else{
					b=update(total,cc1,2*ww,true);
					if(b) update(inner,cc1,2*ww,true);
//					b=update(total,cc1,ww,true);
//					if(b) update(inner,cc1,ww,true);
					else inner.erase(cc1);
				}
			}
			else{//cc1!=cc2
				if(replace){
					ww-=w;
					if(fabsl(ww)<std::numeric_limits<typeWeight>::epsilon()*2) ww=0;
				}
//				update(total,cc1,2*ww,true);
//				update(total,cc2,2*ww,true);
				update(total,cc1,ww,true);
				update(total,cc2,ww,true);
				typeWeight ccw=cc.weight(cc1,cc2);
				dbg.msg(DEBUG_LEVEL::CALLS,"cw"+std::to_string(ccw)+"ww"+std::to_string(ww)+"w"+std::to_string(weight));
				if(std::isnan(ccw)){
					cc.addEdge(cc1,cc2,weight);
//					ccl.insert(std::make_pair(cc1, HalfEdge(cc2,1)));
//					ccl.insert(std::make_pair(cc2, HalfEdge(cc1,1)));
				}
				else{
					ccw=ccw+ww;
					cc.addEdge(cc1,cc2,ccw,true);
//					if(!replace){
//						typeLinksIterator it=multimap::find(ccl,cc1,cc2);
//						it->second.weight(it->second.weight()+1);
//						it=multimap::find(ccl,cc2,cc1);
//						it->second.weight(it->second.weight()+1);
//					}
				}
			}
		}
		dbg.post(DEBUG_LEVEL::MODIFICATIONS,debugPrint());
		debugAddEdge(b,source,destination,weight,w,replace);
		return b;
	}

	/**
	 * Add an edge
	 *
	 * @param edge
	 * @param replace if true and link exists, it replaces the weight, otherwise fails. Default value is false
	 * @return true if the edge was added. False otherwise
	 */
	bool addEdge(const Edge & edge, const bool & replace=false){return addEdge(edge.source(),edge.destination(),edge.weight(),replace);}

	/**
	 * Replace all vertex occurrences of oldValue by newValue
	 *
	 * @param oldValue
	 * @param newValue
	 * @return true if replacement succeeded
	 */
	bool replace(const typeVertex & oldValue, const typeVertex & newValue){
		typeCommunityListIteratorConst a=n2c.value(oldValue);
		if(a==n2c.cend())return false;//old node does not exist
		debugSaveState();
//		typeVertex debugOldValue=oldValue;
//		typeVertex debugNewValue=newValue;
		dbg.pre(DEBUG_LEVEL::MODIFICATIONS,"GGrp", debugPrint());
//		dbg.msg(DEBUG_LEVEL::CALLS,"e"+std::to_string(source)+"#"+std::to_string(destination)+"="+std::to_string(weight)+";"+std::to_string(replace));
		if(a->second==oldValue){//node is also the community
			n2c.replace(oldValue,newValue);
			typeCommunityEdgesIteratorConst it=inner.find(oldValue);
			if(it!=inner.cend()){
				inner[newValue]=it->second;
				inner.erase(it);
			}
			total[newValue]=total[oldValue];
			total.erase(oldValue);
			cc.replace(oldValue,newValue);
//			ccl
		}
		n2c.replaceKey(oldValue,newValue);
//		return GraphUndirected::replace(oldValue,newValue);
		bool result=GraphUndirected::replace(oldValue,newValue);
		dbg.post(DEBUG_LEVEL::MODIFICATIONS,debugPrint());
		debugReplace(result,oldValue,newValue);
		return result;
	}

	/**
	 * remove an edge
	 *
	 * @param source
	 * @param destination
	 * @return true if the edge existed and was successfully removed
	 */
	bool removeEdge(const typeVertex & source, const typeVertex & destination){
		const typeWeight & weight=GraphUndirected::weight(source,destination);
		dbg.pre(DEBUG_LEVEL::MODIFICATIONS,"GGrm", debugPrint());
		if(std::isnan(weight)){

		}
		else{
			debugSaveState();
//			typeVertex debugSource=source;
//			typeVertex debugDestination=destination;
//			dbg.msg(DEBUG_LEVEL::CALLS,"e"+std::to_string(source)+"#"+std::to_string(destination)+"="+std::to_string(weight)+";"+std::to_string(replace));
			const typeCommunity & c1=community(source);
			const typeCommunity & c2=community(destination);
			if(c1==c2){
//				typeWeight w=weight;
				if(source==destination){
//					w=weight/2;
					bool b=update(total,c1,weight,false);
					if(b) update(inner,c1,weight,false);
					else inner.erase(c1);
				}
				else{
					bool b=update(total,c1,2*weight,false);
					if(b) update(inner,c1,2*weight,false);
					else inner.erase(c1);
				}
			}
			else{
//				update(total,c1,2*weight,false);
//				update(total,c2,2*weight,false);
				update(total,c1,weight,false);
				update(total,c2,weight,false);
				typeWeight cw=cc.weight(c1,c2);
//				if(fabsl(cw-weight)<std::numeric_limits<typeWeight>::epsilon()*2) cw=weight;//temporary correction
//				if(cw-weight!=0) cc.addEdge(c1,c2,cw-weight,true);//update weight in cc
				if(fabsl(cw-weight)>=std::numeric_limits<typeWeight>::epsilon()*2) cc.addEdge(c1,c2,cw-weight,true);//update weight in cc
				else cc.removeEdge(c1,c2);
//				typeLinksIterator it=multimap::find(ccl,c1,c2);
//				if(it->second.weight()<1){//no more edges
//					cc.removeEdge(c1,c2);
//					ccl.erase(it);
//					it=multimap::find(ccl,c2,c1);
//					ccl.erase(it);
//				}
//				else{//update
//					typeWeight cw=cc.weight(c1,c2);
//					cc.addEdge(c1,c2,cw-weight,true);//update weight in cc
//				}
			}
			bool res=GraphUndirected::removeEdge(source,destination);
			if(neighboursCount(source)==0) n2c.remove(source);
			if(neighboursCount(destination)==0) n2c.remove(destination);
			debugRemove(res,source,destination,weight);
			dbg.post(DEBUG_LEVEL::MODIFICATIONS,debugPrint());
			return res;
		}
		dbg.post(DEBUG_LEVEL::MODIFICATIONS,debugPrint());
		return false;
	}

	/**
	 * @see{removeEdge(const typeVertex & source, const typeVertex & destination)}
	 * @param edge
	 * @return
	 */
	bool removeEdge(const Edge & edge){return removeEdge(edge.source(),edge.destination());}

	/**
	 *
	 * @param c
	 * @return the list of vertices of the given community
	 */
	typeCommunityListRange vertices(const typeCommunity & c)const{
		return n2c.keys(c);
	}

	/**
	 * @brief Disband the given community
	 * @details Disbanding takes all vertices of a community and turns them into
	 * individual communities.
	 * Since the name of a community will match the name of a vertex, the given
	 * community becomes a community of a single vertex.
	 *
	 * @param c
	 * @param level indicates the community neighbour level to disband (example: level three will disband the neighbours, the neighbours of the neighbours and the neighbours of the neighbours of the neighbours). Level zero disbands only given community. Parameter currently ignored.
	 */
	void disband(const typeCommunity & c,const unsigned int & level=0){
		dbg.pre(DEBUG_LEVEL::MODIFICATIONS,"GGd", debugPrint());
//		dbg.msg(DEBUG_LEVEL::CALLS,"e"+std::to_string(source)+"#"+std::to_string(destination)+"="+std::to_string(weight)+";"+std::to_string(replace));
		typeCommunityListRange n=n2c.keys(c);
		std::set<typeVertex> k;//temporary to store nodes. Needed since the iterators returned in the keys range are invalidated on first call to community(node,community)
		for(typeCommunityListRangeIteratorConst it=n.first;it!=n.second;++it){
			const typeCommunityListRangePair & p=*it;
			const typeVertex & node=p.second;
			k.insert(node);
		}
		for(std::set<typeVertex>::const_iterator it=k.cbegin();it!=k.cend();++it){
			const typeVertex & node=*it;
			community(node,node);
		}
		dbg.post(DEBUG_LEVEL::MODIFICATIONS,debugPrint());
	}

	/**
	 * @return the list of all existing communities
	 */
	const typeCommunities communities()const{
		typeCommunities c(cc.getVertices().cbegin(),cc.getVertices().cend());
		for(typeCommunityEdgesIteratorConst it=inner.cbegin();it!=inner.cend();++it){
			c.insert(it->first);
		}
		return c;
	}

	/**
	 * @return the number of existing communities
	 */
	const typeWeight communityCount()const{return total.size();}

	/**
	 *
	 * @param node
	 * @return the community of the given vertex or a special community of noGRoup if the vertex does not exist
	 */
	const typeCommunity & community(const typeVertex & vertex)const{
		typeCommunityListIteratorConst it=n2c.find(vertex);
		if(it!=n2c.cend()){
			return (*it).second;
		}
		return noGroup;//if node does not exist return special group
	}

	/**
	 * replace old community by new community on all vertices
	 *
	 * @param old is the old community
	 * @param comm is the new community
	 * @return true if the replacement succeeded
	 */
	bool replaceCommunity(const typeCommunity & old, const typeCommunity & com){
		if(old==noGroup || com==noGroup) return false;
		if(old==com)return true;
		dbg.pre(DEBUG_LEVEL::MODIFICATIONS,"GGrC", debugPrint());
//		dbg.msg(DEBUG_LEVEL::CALLS,"e"+std::to_string(source)+"#"+std::to_string(destination)+"="+std::to_string(weight)+";"+std::to_string(replace));
		//first check if com is empty, otherwise fail
		typeCommunityEdgesIteratorConst itt=total.find(com);
		if(itt!=total.cend()) if(itt->second!=0) return false;
		n2c.replace(old,com);
		replace(inner,old,com);
		replace(total,old,com);
		cc.replace(old,com);
//		ccl
		dbg.post(DEBUG_LEVEL::MODIFICATIONS,debugPrint());
		return true;
	}

	/**
	 * Get the number of vertices in the given community
	 *
	 * @param community
	 * @return the number of vertices
	 */
	int communityVertexCount(typeCommunity community)const {
		unsigned int cnt=0;
		typeCommunityListRange r=vertices(community);
		for(typeCommunityListRangeIteratorConst it=r.first; it!=r.second; ++it){
			++cnt;
		}
		return cnt;
	}

	/**
	 * set the community of a single vertex
	 * @param vertex is the vertex to be assigned a new community
	 * @param community is the new community
	 * @return true if the node was already in the requested community or, the node exists and insertion succeeded
	 */
	bool community(const typeVertex & vertex, const typeCommunity & com){
		if(vertex==noVertex || com==noGroup) return false;
		const typeCommunity c=community(vertex);
		if(c==noGroup) return false;
		if(c==com)return true;
		debugSaveState();
		dbg.pre(DEBUG_LEVEL::MODIFICATIONS,"GGc", debugPrint());
//		dbg.msg(DEBUG_LEVEL::CALLS,"e"+std::to_string(source)+"#"+std::to_string(destination)+"="+std::to_string(weight)+";"+std::to_string(replace));
		typeWeight a1=neighboursCommunityWeight(vertex,com);//connections with new community
		typeWeight a2=neighboursCommunityWeight(vertex,c);//connections inside old community
		typeWeight w=neighboursWeight(vertex);
		typeWeight in=weight(vertex,vertex);
		if(std::isnan(in))in=0;
		update(inner,c,2*a2+in,false);
		update(inner,com,2*a1+in,true);
//		update(total,com,w+in,true);
//		update(total,c,w+in,false);
//		update(total,com,2*a2+in,true);
//		update(total,c,2*a1+in,false);
//		update(total,c,2*(w-a2)+in,false);
//		update(total,com,2*(w-a1)+in,true);
		update(total,c,a2+in+(w-a2),false);//(w-a1-a2) is the weight of connections to all other communities that are not either c or com
		update(total,com,a1+in+(w-a1),true);
		const typeLinksRangeConst & nei=neighbours(vertex);
		typeLinksIteratorConst it=nei.first;
		while(it!=nei.second){
			const typeLinksPair & p=*it;
			++it;
			if(p.first!=vertex)break;
			const HalfEdge & he=p.second;
			const typeVertex & dest=he.destination();
			if(dest!=vertex){
				const typeWeight & wei=he.weight();
				const typeCommunity & co=community(dest);
				if(communityVertexCount(c)<=1){
					//this is the only vertex and is about to change community so delete edge
					cc.removeEdge(c,co);
				}
				else{
					if(c!=co){
						typeWeight cw=cc.weight(c,co);
						if(std::isnan(cw)){
						}
						else{
//							ccl
							cw-=wei;
							if(fabsl(cw)<std::numeric_limits<typeWeight>::epsilon()*2) cw=0;
							if(cw!=0){
								cc.addEdge(c,co,cw,true);
							}
							else{//remove edge since there will be no more connection between the two communities
								cc.removeEdge(c,co);
							}
						}
					}
				}
				if(co!=com){
					typeWeight cw=cc.weight(co,com);
					if(std::isnan(cw)){
						cc.addEdge(co,com,wei);
//						ccl
					}
					else{
						cc.addEdge(co,com,cw+wei,true);
//						ccl
					}
				}
			}
		}
		bool b=n2c.add(vertex,com,true);
		if(vertex==c){//node was the minimum of the community
			typeVertex min=minimumNode(n2c,c);//determine new minimum
			if(min!=noVertex){//if community is not empty
				replaceCommunity(c,min);
			}
			else{
				inner.erase(c);
				total.erase(c);
			}
		}
		typeCommunityListRange x=n2c.keys(c);
		if(x.first==x.second){//no more nodes in community
			inner.erase(c);
			total.erase(c);
		}
		if(vertex<com){//vertex is the new minimum of the destination community
			replaceCommunity(com,vertex);
		}
		dbg.post(DEBUG_LEVEL::MODIFICATIONS,debugPrint());
//		debugCommunity(b, vertex, com);
		debugVerify();
		return b;
	}
	/**
	 * Get the edge count of all community to community edges
	 *
	 * @return the edge count
	 */
	const typeWeight communitiesEdgeCount()const{return cc.edgeCount()+inner.size();}

	/**
	 * Get the sum of the weights of all edges of the given community where both
	 * vertices of an edge belong to the given community.
	 *
	 * @param c
	 * @return the sum of the weights of all inner edges
	 */
	typeWeight innerEdges(const typeCommunity & c)const {
		typeCommunityEdgesIteratorConst it=inner.find(c);
		if(it!=inner.cend()){
			const typeCommunityEdgesPair & p=*it;
			return p.second;
		}
		return 0;
	}

	/**
	 * Get the sum of the weights of all edges of the given community.
	 *
	 * @param c
	 * @return the sum of the weights of all edges
	 */
	typeWeight totalEdges(const typeCommunity & c)const {
		typeCommunityEdgesIteratorConst it=total.find(c);
		if(it!=total.cend()){
			const typeCommunityEdgesPair & p=*it;
			return p.second;
		}
		return 0;
	}

	/**
	 * Get the communities that are neighbours of the given community
	 *
	 * @param com
	 * @return the neighbouring communities
	 */
	typeLinksRangeConst neighbouringCommunities(const typeCommunity & com)const {
		return cc.neighbours(com);
	}

	/**
	 * Get the sum of the weights of the communities that are neighbours of the given community
	 *
	 * @param com
	 * @return the sum of the weights of neighbouring communities
	 */
	typeWeight neighbouringCommunitiesWeight(const typeCommunity & com)const{
		return cc.neighboursWeight(com);
	}

	/**
	 * Get the number of communities that are neighbours of the given community
	 *
	 * @param com
	 * @return the number of neighbouring communities
	 */
	unsigned int neighbouringCommunitiesCount(const typeCommunity & com)const {
		return cc.neighboursCount(com);
	}

	/**
	 * Get the weight of the edge formed by two communities
	 *
	 * @param source community
	 * @param destination community
	 * @return the weight of the edge
	 */
	typeWeight weightCommunity(const typeCommunity & source, const typeCommunity & destination)const{
		if(source==destination){
			typeCommunityEdgesIteratorConst it=inner.find(source);
			if(it==inner.cend()) return std::numeric_limits<typeWeight>::quiet_NaN();
			return it->second;
		}
		return cc.weight(source,destination);
	}

	/**
	 * Get the number of neighbours of the given vertex that belong to the same
	 * community as the vertex
	 *
	 * @param vertex
	 * @return the number of neighbours
	 */
	unsigned int neighboursCommunityCount(const typeVertex & vertex)const{
		const typeCommunity & com=community(vertex);
		unsigned int cnt=0;
		typeLinksRangeConst a=neighbours(vertex);
		for(typeLinksIteratorConst it=a.first;it!=a.second;++it){
			const typeLinksPair & b=*it;
			const HalfEdge & c=b.second;
			if(community(c.destination())==com)cnt++;
		}
		return cnt;
	}

	/**
	 * Get the sum of the weight of the neighbours of the given vertex that belong
	 * to the given community
	 *
	 * @param vertex
	 * @param com is the target community
	 * @return the sum of the weight of the neighbours
	 */
	typeWeight neighboursCommunityWeight(const typeVertex & vertex, const typeCommunity & com)const{
		typeWeight cnt=0;
		typeLinksRangeConst a=neighbours(vertex);
		for(typeLinksIteratorConst it=a.first;it!=a.second;++it){
			const typeLinksPair & b=*it;
			const HalfEdge & c=b.second;
			const typeVertex & d=c.destination();
			if(d!=vertex){
				if(community(d)==com){
					cnt+=c.weight();
				}
			}
		}
		return cnt;
	}

	/**
	 * Get the sum of the weight of the neighbours of the given vertex that belong
	 * to the same community as the vertex
	 *
	 * @param vertex
	 * @return the sum of the weight of the neighbours
	 */
	typeWeight neighboursCommunityWeight(const typeVertex & vertex)const{
		const typeCommunity & com=community(vertex);
		return neighboursCommunityWeight(vertex,com);
	}

	/**
	 * @brief Populate the graph with the current community to community mapping.
	 *
	 * @details
	 * When vertices (lower level view) are merged into communities (higher level
	 * view) and there is an interest to save only the higher level view, this
	 * function can be called to perform that action.
	 *
	 * @section Example:
	 * graph before operation:
	 *   lower level:
	 *     vertices: 1,2,3,4,5,6
	 *     edges: (1,2),(1,3),(2,3),(3,4),(4,5),(4,6),(5,6)
	 *     communities(vertex,community):(1,1),(2,1),(3,1),(4,4),(5,4),(6,4)
	 *   higher level:
	 *     vertices: 1,4
	 *     edges: (1,4)
	 *     communities(vertex,community):(1,1),(4,1)
	 *
	 * graph after operation:
	 *   lower level:
	 *     vertices: 1,4
	 *     edges: (1,4)
	 *     communities(vertex,community):(1,1),(4,1)
	 *   higher level:
	 *     vertices: 1,4
	 *     edges: (1,4)
	 *     communities(vertex,community):(1,1),(4,1)
	 *
	 * @return true if operation succeeded. False, otherwise
	 */
	bool communitiesToGraph(){
//	  CERR << "sync other runs communitiesToGraph\n";
		dbg.pre(DEBUG_LEVEL::MODIFICATIONS,"Gctg", debugPrint());
	  typeVertexList nd;
		//remove edge if community!=node else update weight
//		dbg.msg(DEBUG_LEVEL::ACTIONS, "r");
		typeLinksRangeConst e=edges();
		typeLinks edgsRem;
		typeLinks edgsUpd;
		typeLinksIteratorConst it=e.first;
//		CERR << "communitiesToGraph remove or update edge\n";
		while(it!=e.second){
			const typeLinksPair & p=*it;
			const typeVertex & s=p.first;
			const typeVertex & d=p.second.destination();
			if(s!=community(s) || d!=community(d)){
				++it;
//				dbg.msg(DEBUG_LEVEL::ACTIONS, "r"+std::to_string(s)+"#"+std::to_string(d)+"="+std::to_string(GraphUndirected::weight(s, d)));
//				GraphUndirected::removeEdge(s,d);
//				dbg.msg(DEBUG_LEVEL::ACTIONS, debugPrint());
				edgsRem.insert(p);
			}
			else{
				nd.insert(s);
				nd.insert(d);
				const typeWeight & w=weightCommunity(s,d);
				++it;
//				dbg.msg(DEBUG_LEVEL::ACTIONS, "u"+std::to_string(s)+"#"+std::to_string(d)+"="+std::to_string(GraphUndirected::weight(s, d))+">"+std::to_string(w));
//				GraphUndirected::addEdge(s,d,w,true);//update weight
//				dbg.msg(DEBUG_LEVEL::ACTIONS, debugPrint());
				edgsUpd.insert(std::make_pair(s, HalfEdge(d,w)));
			}
		}
		for(it=edgsRem.cbegin();it!=edgsRem.cend();++it){
			const typeLinksPair & p=*it;
			const typeVertex & s=p.first;
			const typeVertex & d=p.second.destination();
			dbg.msg(DEBUG_LEVEL::ACTIONS, "r"+std::to_string(s)+"#"+std::to_string(d)+"="+std::to_string(GraphUndirected::weight(s, d)));
			GraphUndirected::removeEdge(s,d);
			dbg.msg(DEBUG_LEVEL::ACTIONS, debugPrint());
		}
		for(it=edgsUpd.cbegin();it!=edgsUpd.cend();++it){
			const typeLinksPair & p=*it;
			const typeVertex & s=p.first;
			const typeVertex & d=p.second.destination();
			const typeWeight & w=p.second.weight();
			dbg.msg(DEBUG_LEVEL::ACTIONS, "u"+std::to_string(s)+"#"+std::to_string(d)+"="+std::to_string(GraphUndirected::weight(s, d))+">"+std::to_string(w));
			GraphUndirected::addEdge(s,d,w,true);//update weight
			dbg.msg(DEBUG_LEVEL::ACTIONS, debugPrint());
		}
		dbg.msg(DEBUG_LEVEL::ACTIONS, "m"+debugPrint());
		//add any links from cc missing
//		CERR << "communitiesToGraph add missing cc\n";
		typeLinksRangeConst ee=cc.edges();
		typeLinksIteratorConst it2=ee.first;
		while(it2!=ee.second){
			const typeLinksPair & p=*it2;
			const typeVertex & s=p.first;
			const typeVertex & d=p.second.destination();
			const typeWeight & w=cc.weight(s,d);
			if(!std::isnan(w)){
				nd.insert(s);
				nd.insert(d);
				GraphUndirected::addEdge(s,d,p.second.weight());//add missing link
			}
			++it2;
		}
		//add inner edges
		dbg.msg(DEBUG_LEVEL::ACTIONS, "i"+debugPrint());
//		CERR << "communitiesToGraph add inner edge\n";
		typeCommunityEdgesIteratorConst iti=inner.cbegin();
		while(iti!=inner.cend()){
			const typeCommunityEdgesPair & p=*iti;
			const typeCommunity & c=p.first;
			const typeWeight & w=p.second;
			nd.insert(c);
			GraphUndirected::addEdge(c,c,w,true);
			++iti;
		}
		//remove non existing nodes from n2c
		dbg.msg(DEBUG_LEVEL::ACTIONS, "n"+debugPrint());
//		CERR << "communitiesToGraph remove non existing n2c\n";
		typeCommunityListIteratorConst itc=n2c.cbegin();
		while(itc!=n2c.cend()){
			const typeCommunityListPair & p=*itc;
			const typeVertex & n=p.first;
			const typeCommunity & c=p.second;
			if(nd.find(n)==nd.end()){
				++itc;
				n2c.remove(n,c);
			}
			else{
				++itc;
			}
		}
		dbg.post(DEBUG_LEVEL::MODIFICATIONS,debugPrint());
		return true;
	}

	/**
	 * @return a string representation of this graph
	 */
	const std::string toString(const StringFormatter & sf=defaultStringFormatter)const{
		StringFormatter f=sf;
		std::stringstream ss;
		if(!sf.isDefault()){
			f.build(ss,"");
		}
		f.header("Graph=");
		ss << GraphUndirected::toString(f);
		f.header("Node to Group:");
		ss << n2c.toString(f);
		f.header("in=");
		ss << map::toString(inner,f(1));
		f.header("tot=");
		ss << map::toString(total,f(1));
		f.header("cc=");
		ss << cc.toString(f);
		return ss.str();
	}

	const std::string debugPrint()const{
		std::stringstream ss;
		ss << "g" << GraphUndirected::debugPrint() << "\n";
		ss << n2c.debugPrint() << "\n";
		ss << "i" << map::debugPrint(inner) << "\n";
		ss << "t" << map::debugPrint(total) << "\n";
		ss << "c"<< cc.debugPrint();
		return ss.str();
	}
};

#endif /* GRAPHUNDIRECTEDGROUPABLE_H_ */
