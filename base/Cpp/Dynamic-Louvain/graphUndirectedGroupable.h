/*
 * GraphGroupable.h
 *
 *  Created on: 19/08/2018
 *      Author: poltergeist0
 */

#ifndef GRAPHUNDIRECTEDGROUPABLE_H_
#define GRAPHUNDIRECTEDGROUPABLE_H_

#include "mapReversable.h"
//#include "delta.h"
#include <math.h>

#include "graphUndirected.h"
#include "setUtilities.h"

typedef typeNode typeCommunity;

typedef MapReversable<typeNode,typeCommunity> typeCommunityList;

/**
 * std::map<typeNode,typeCommunity>::const_iterator
 */
typedef typeCommunityList::const_iterator typeCommunityListIteratorConst;

/**
 * std::pair<typeNode,typeCommunity>
 */
typedef typeCommunityList::typePair typeCommunityListPair;

/**
 * std::multimap<typeCommunity,typeNode>::const_iterator
 */
typedef typeCommunityList::const_range_iterator typeCommunityListRangeIteratorConst;

/**
 * std::pair<std::multimap<typeCommunity,typeNode>::const_iterator,std::multimap<typeCommunity,typeNode>::const_iterator>
 */
typedef typeCommunityList::typeRange typeCommunityListRange;

/**
 * std::pair<typeCommunity,typeNode>
 */
typedef typeCommunityList::typeRangePair typeCommunityListRangePair;

typedef std::map<typeCommunity, typeWeight> typeCommunityEdges;
typedef std::map<typeCommunity, typeWeight>::iterator typeCommunityEdgesIterator;
typedef std::pair<typeCommunity, typeWeight> typeCommunityEdgesPair;
typedef std::map<typeCommunity, typeWeight>::const_iterator typeCommunityEdgesIteratorConst;
typedef std::pair<typeCommunityEdgesIterator,typeCommunityEdgesIterator> typeCommunityEdgesRange;
typedef std::pair<typeCommunityEdgesIteratorConst,typeCommunityEdgesIteratorConst> typeCommunityEdgesRangeConst;
typedef std::set<typeCommunity> typeCommunities;

const typeCommunity noGroup=0;

class GraphUndirectedGroupable: public GraphUndirected{
private:
	/**
	 * map used to keep the relation between the graph nodes and their communities
	 */
	typeCommunityList n2c; // node to community association used on the first pass of the louvain algorithm

	/**
	 * number of links inside community
	 */
	typeCommunityEdges inner;

	/**
	 * optimization
	 * total number of links of community
	 */
	typeCommunityEdges total;

	GraphUndirected cc;//community to community mapping

//#ifdef TEST
	/******************************************************************************
	 ******************************************************************************
	 ******************************************************************************
	 * Functions used for testing ONLY
	 * Allows direct access to private members of the class to verify correct
	 * implementation
	 ******************************************************************************
	 ******************************************************************************
	 ******************************************************************************/
//public:


//#endif	//TEST

private:
	/******************************************************************************
	 ******************************************************************************
	 ******************************************************************************
	 * Functions used for debugging ONLY
	 * Verify consistency of the class member as a set
	 ******************************************************************************
	 ******************************************************************************
	 ******************************************************************************/
#ifndef NDEBUG
	GraphUndirected debugGraphUndirected;//parent class
	typeCommunityList debugN2c; // node to community association used on the first pass of the louvain algorithm
	typeCommunityEdges debugInner;
	typeCommunityEdges debugTotal;
	GraphUndirected debugCc;//community to community mapping
#endif	//NDEBUG

	void debugVerifyEdgeCount(const int & offset=0)const{
#ifndef NDEBUG
		ASSERT_EQUAL(debugGraphUndirected.edgeCount()+offset,GraphUndirected::edgeCount());
#endif	//NDEBUG
	}

	void debugVerifyNodeCount(const int & offset=0)const{
#ifndef NDEBUG
		ASSERT_EQUAL(debugGraphUndirected.nodeCount()+offset,GraphUndirected::nodeCount());
#endif	//NDEBUG
	}

	void debugVerifyCcEdgeCount(const int & offset=0)const{
#ifndef NDEBUG
		ASSERT_EQUAL(debugCc.edgeCount()+offset,cc.edgeCount());
#endif	//NDEBUG
	}

	void debugVerifyCcNodeCount(const int & offset=0)const{
#ifndef NDEBUG
		ASSERT_EQUAL(debugCc.nodeCount()+offset,cc.nodeCount());
#endif	//NDEBUG
	}

	void debugVerifyN2cSize(const int & offset=0)const{
#ifndef NDEBUG
		ASSERT_EQUAL(debugN2c.size()+offset,n2c.size());
#endif	//NDEBUG
	}

	void debugVerifyInnerSize(const int & offset=0)const{
#ifndef NDEBUG
		ASSERT_EQUAL(debugInner.size()+offset,inner.size());
#endif	//NDEBUG
	}

	void debugVerifyTotalSize(const int & offset=0)const{
#ifndef NDEBUG
		ASSERT_EQUAL(debugTotal.size()+offset,total.size());
#endif	//NDEBUG
	}

	void debugVerifyNodesAndEdges(const typeNode & source=noNode,const typeNode & destination=noNode)const{
#ifndef NDEBUG
		const typeNodeList & nn=debugCc.getNodes();
		const typeNodeList & n=GraphUndirected::getNodes();
		for(typeNodeListIteratorConst it=nn.cbegin();it!=nn.cend();++it){//for each node
			const typeNode & p=*it;
			if(p==source || p==destination){
				//skip
			}
			else{
				ASSERT_NOT_EQUAL_ITERATOR(n.find(p),n.cend());
			}
		}
		const typeLinksRangeConst & ee=debugGraphUndirected.edges();
		for(typeLinksIteratorConst it=ee.first;it!=ee.second;++it){//for each edge
			const typeLinksPair & p=*it;
			const typeNode & src=p.first;
			const typeNode & dst=p.second;
			if((src==source && dst==destination) || (src==destination && dst==source)){
				//skip
			}
			else{
				const typeWeight & w=p.second.weight();
				typeWeight ww=weight(src,dst);
				ASSERT(!isnan(ww));
				ASSERT_EQUAL(ww,w);
			}
		}
#endif	//NDEBUG
	}

	void debugVerifyN2c(const typeNode & source=noNode,const typeNode & destination=noNode)const{
#ifndef NDEBUG
		//verify n2c. Existing communities must not change when adding/removing an edge
		for(typeCommunityListIteratorConst it=debugN2c.cbegin();it!=debugN2c.cend();++it){
			const typeCommunityListPair & p=*it;
			if(p.first==source || p.first==destination){
				//skip
			}
			else{
				typeCommunityListIteratorConst itt=n2c.find(p.first);
				ASSERT_NOT_EQUAL_ITERATOR(itt,n2c.cend());//must exist
				ASSERT_EQUAL(itt->second,p.second);//and have the same community as before
			}
		}
#endif	//NDEBUG
	}

	void debugVerifyInner(const typeCommunity & source=noGroup,const typeCommunity & destination=noGroup)const{
#ifndef NDEBUG
		for(typeCommunityEdgesIteratorConst it=debugInner.cbegin();it!=debugInner.cend();++it){
			const typeCommunityEdgesPair & p=*it;
			if(p.first==source || p.first==destination){
				//skip
			}
			else{
				typeCommunityEdgesIteratorConst itt=inner.find(p.first);
				ASSERT_NOT_EQUAL_ITERATOR(itt,inner.cend());
				ASSERT_EQUAL(itt->second,p.second);
			}
		}
#endif	//NDEBUG
	}

	void debugVerifyTotal(const typeCommunity & source=noGroup,const typeCommunity & destination=noGroup)const{
#ifndef NDEBUG
		for(typeCommunityEdgesIteratorConst it=debugTotal.cbegin();it!=debugTotal.cend();++it){
			const typeCommunityEdgesPair & p=*it;
			if(p.first==source || p.first==destination){
				//skip
			}
			else{
				typeCommunityEdgesIteratorConst itt=total.find(p.first);
				ASSERT_NOT_EQUAL_ITERATOR(itt,total.cend());
				ASSERT_EQUAL(itt->second,p.second);
			}
		}
#endif	//NDEBUG
	}

	void debugVerifyCc(const typeNode & source=noNode,const typeNode & destination=noNode)const{
#ifndef NDEBUG
		const typeLinksRangeConst & ee=debugCc.edges();
		for(typeLinksIteratorConst it=ee.first;it!=ee.second;++it){//for each edge
			const typeLinksPair & p=*it;
			const typeNode & src=p.first;
			const typeNode & dst=p.second;
			if((src==source && dst==destination) || (src==destination && dst==source)){
				//skip
			}
			else{
				const typeWeight & w=p.second.weight();
				typeWeight ww=cc.weight(src,dst);
				ASSERT(!isnan(ww));
				ASSERT_EQUAL(ww,w);
			}
		}
#endif	//NDEBUG
	}

	void debugVerifySaveState()const{
#ifndef NDEBUG
		//verify sizes
//		ASSERT_EQUAL(debugGraphUndirected.edgeCount(),GraphUndirected::edgeCount());
//		ASSERT_EQUAL(debugGraphUndirected.nodeCount(),GraphUndirected::nodeCount());
//		ASSERT_EQUAL(debugN2c.size(),n2c.size());
//		ASSERT_EQUAL(debugInner.size(),inner.size());
//		ASSERT_EQUAL(debugTotal.size(),total.size());
//		ASSERT_EQUAL(debugCc.edgeCount(),cc.edgeCount());
//		ASSERT_EQUAL(debugCc.nodeCount(),cc.nodeCount());
		debugVerifyCcEdgeCount();
		debugVerifyNodeCount();
		debugVerifyN2cSize();
		debugVerifyInnerSize();
		debugVerifyTotalSize();
		debugVerifyCcEdgeCount();
		debugVerifyCcNodeCount();
		//verify GraphUndirected
//		const typeNodeList & nn=debugCc.getNodes();
//		const typeNodeList & n=GraphUndirected::getNodes();
//		for(typeNodeListIteratorConst it=nn.cbegin();it!=nn.cend();++it){//for each node
//			const typeNode & p=*it;
//			ASSERT_NOT_EQUAL_ITERATOR(n.find(p),n.cend());
//		}
//		{
//			const typeLinksRangeConst & ee=debugGraphUndirected.edges();
//			for(typeLinksIteratorConst it=ee.first;it!=ee.second;++it){//for each edge
//				const typeLinksPair & p=*it;
//				const typeNode & src=p.first;
//				const typeNode & dst=p.second;
//				const typeWeight & w=p.second.weight();
//				typeWeight ww=weight(src,dst);
//				ASSERT(!isnan(ww));
//				ASSERT_EQUAL(ww,w);
//			}
//		}
		debugVerifyNodesAndEdges();
		//verify values
//		for(typeCommunityListIteratorConst it=debugN2c.cbegin();it!=debugN2c.cend();++it){
//			const typeCommunityListPair & p=*it;
//			typeCommunityListIteratorConst itt=n2c.find(p.first);
//			ASSERT_NOT_EQUAL_ITERATOR(itt,n2c.cend());
//			ASSERT_EQUAL(itt->second,p.second);
//		}
		debugVerifyN2c();
//		for(typeCommunityEdgesIteratorConst it=debugInner.cbegin();it!=debugInner.cend();++it){
//			const typeCommunityEdgesPair & p=*it;
//			typeCommunityEdgesIteratorConst itt=inner.find(p.first);
//			ASSERT_NOT_EQUAL_ITERATOR(itt,inner.cend());
//			ASSERT_EQUAL(itt->second,p.second);
//		}
//		for(typeCommunityEdgesIteratorConst it=debugTotal.cbegin();it!=debugTotal.cend();++it){
//			const typeCommunityEdgesPair & p=*it;
//			typeCommunityEdgesIteratorConst itt=total.find(p.first);
//			ASSERT_NOT_EQUAL_ITERATOR(itt,total.cend());
//			ASSERT_EQUAL(itt->second,p.second);
//		}
		debugVerifyInner();
		debugVerifyTotal();
		//verify cc
//		{
//			const typeLinksRangeConst & ee=debugCc.edges();
//			for(typeLinksIteratorConst it=ee.first;it!=ee.second;++it){//for each edge
//				const typeLinksPair & p=*it;
//				const typeNode & src=p.first;
//				const typeNode & dst=p.second;
//				const typeWeight & w=p.second.weight();
//				typeWeight ww=cc.weight(src,dst);
//				ASSERT(!isnan(ww));
//				ASSERT_EQUAL(ww,w);
//			}
//		}
		debugVerifyCc();
#endif	//NDEBUG
	}

	void debugVerify() const{
#ifndef NDEBUG
		//verify sizes
		ASSERT_EQUAL(n2c.size(),nodeCount());
		//verify that all nodes have communities
		const typeNodeList & nd=GraphUndirected::getNodes();
		for(typeNodeListIteratorConst it=nd.cbegin();it!=nd.cend();++it){
			const typeNode & p=*it;
			ASSERT_NOT_EQUAL_ITERATOR(n2c.find(p),n2c.cend());
		}
		//verify inner/total
		typeCommunityEdges ii;
		typeCommunityEdges tt;
		GraphUndirected ccc;
		const typeLinksRangeConst & e=GraphUndirected::edges();
		for(typeLinksIteratorConst it=e.first;it!=e.second;++it){//for each edge
			const typeLinksPair & p=*it;
			const typeNode & src=p.first;
			const typeNode & dst=p.second;
			const typeWeight & w=p.second.weight();
			const typeCommunity & srcc=n2c[src];
			const typeCommunity & dstc=n2c[dst];
			if(srcc==dstc){
				ii[srcc]+=w;
				tt[srcc]+=w;
			}
			else{
				tt[srcc]+=w;
				tt[dstc]+=w;
				typeWeight wc=ccc.weight(srcc,dstc);
				//NOTE: addEdge also adds the reverse edge so only add the edges where srcc<dstc. Otherwise weights will appear doubled
				if(srcc<dstc){
					if(isnan(wc)) ccc.addEdge(srcc,dstc,w);
					else ccc.addEdge(srcc,dstc,wc+w,true);
				}
			}
		}
		ASSERT_EQUAL(ii.size(),inner.size());
		for(typeCommunityEdgesIteratorConst it=ii.cbegin();it!=ii.cend();++it){
			const typeCommunityEdgesPair & p=*it;
			typeCommunityEdgesIteratorConst itt=inner.find(p.first);
			ASSERT_NOT_EQUAL_ITERATOR(itt,inner.cend());//make sure community entry exists
			ASSERT_EQUAL(p.second,itt->second);//check if weight matches
		}
		ASSERT_EQUAL(tt.size(),total.size());
		for(typeCommunityEdgesIteratorConst it=tt.cbegin();it!=tt.cend();++it){
			const typeCommunityEdgesPair & p=*it;
			typeCommunityEdgesIteratorConst itt=total.find(p.first);
			ASSERT_NOT_EQUAL_ITERATOR(itt,total.cend());//make sure community entry exists
			ASSERT_EQUAL(p.second,itt->second);//check if weight matches
		}
		//verify community to community mapping
		ASSERT_EQUAL(ccc.edgeCount(),cc.edgeCount());
		const typeLinksRangeConst & ee=ccc.edges();
		for(typeLinksIteratorConst it=ee.first;it!=ee.second;++it){//for each edge
			const typeLinksPair & p=*it;
			const typeNode & src=p.first;
			const typeNode & dst=p.second;
			const typeWeight & w=p.second.weight();
			typeWeight ww=cc.weight(src,dst);
			ASSERT(!isnan(ww));
			ASSERT_EQUAL(ww,w);
		}
#endif	//NDEBUG
	}

	void debugSaveState(){
		debugVerify();
#ifndef NDEBUG
		//copy listings
		debugGraphUndirected=GraphUndirected(*this);
		debugN2c=typeCommunityList(n2c);
		debugInner=typeCommunityEdges(inner);
		debugTotal=typeCommunityEdges(total);
		debugCc=GraphUndirected(cc);
		debugVerifySaveState();
#endif	//NDEBUG
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
	void debugAddEdge(const bool & added,const typeNode & source, const typeNode & destination, const typeWeight & weight=1.0, const bool & replace=false) const{
		debugVerify();
#ifndef NDEBUG
		int incNodes=0;
		//verify the node was added
		typeCommunity srcc;
		typeCommunity dstc;
		typeCommunityListIteratorConst it;
		const typeNodeList & n=getNodes();
		const typeNodeList & nn=debugGraphUndirected.getNodes();
		if(added){// || nn.find(source)!=nn.cend()){//was added or already existed
			//verify nodes exist
			ASSERT_NOT_EQUAL_ITERATOR(n.find(source),n.cend());
			if(source!=destination)	ASSERT_NOT_EQUAL_ITERATOR(n.find(destination),n.cend());
			it=n2c.find(source);
			ASSERT_NOT_EQUAL_ITERATOR(it,n2c.cend());
			if(source!=destination)	ASSERT_NOT_EQUAL_ITERATOR(n2c.find(destination),n2c.cend());
			//verify if node existed or was added
			if(nn.find(source)==nn.cend()){//did not exist
				++incNodes;
				ASSERT_EQUAL(it->second,source);//verify that the attributed community matches itself when added
				srcc=source;
			}
			else{
				srcc=it->second;//if node existed get its community
			}
			if(source!=destination){
				//same for destination
				it=n2c.find(destination);
				ASSERT_NOT_EQUAL_ITERATOR(it,n2c.cend());
				if(nn.find(destination)==nn.cend()){//did not exist
					++incNodes;
					ASSERT_EQUAL(it->second,destination);//verify that the attributed community matches itself
					dstc=destination;
				}
				else{
					dstc=it->second;//if node existed get its community
				}
			}
			else{//if source==destination
				dstc=srcc;
			}
			//verify nodes sizes
			ASSERT_EQUAL(n.size(),nn.size()+incNodes);
//			ASSERT_EQUAL(n2c.size(),debugN2c.size()+incNodes);
//			ASSERT_EQUAL(total.size(),debugTotal.size()+incNodes);
			debugVerifyN2cSize(incNodes);
			debugVerifyTotalSize(incNodes);
			//verify n2c. Existing communities must not change when adding an edge
//			for(typeCommunityListIteratorConst it=debugN2c.cbegin();it!=debugN2c.cend();++it){
//				const typeCommunityListPair & p=*it;
//				typeCommunityListIteratorConst itt=n2c.find(p.first);
//				ASSERT_NOT_EQUAL_ITERATOR(itt,n2c.cend());
//				ASSERT_EQUAL(itt->second,p.second);
//			}
			debugVerifyN2c();//including source and destination
			//verify total and inner
			typeWeight wold=debugGraphUndirected.weight(source,destination);
			if(isnan(wold))wold=0;
			if(srcc==dstc){
				typeCommunityEdgesIteratorConst it=debugInner.find(srcc);
				typeWeight w=(added)?weight:0;
				if(source!=destination) w*=2;
				if(it!=debugInner.cend()){//inner already had a value
					w+=it->second;
					if(replace){
						w-=wold;
					}
				}
				ASSERT_EQUAL(inner.find(srcc)->second,w);
				it=debugTotal.find(srcc);
				w=(added)?weight:0;
				if(source!=destination) w*=2;
				if(it!=debugTotal.cend()){//total already had a value
					w+=it->second;
					if(replace){
						w-=wold;
					}
				}
				ASSERT_EQUAL(total.find(srcc)->second,w);
			}
			else{// ! srcc==dstc
				typeCommunityEdgesIteratorConst it=debugTotal.find(srcc);
				typeWeight w=(added)?2*weight:0;
				if(it!=debugTotal.cend()){//total already had a value
					w+=it->second;
					if(replace){
						w-=2*wold;
					}
				}
				ASSERT_EQUAL(total.find(srcc)->second,w);
				it=debugTotal.find(dstc);
				w=(added)?2*weight:0;
				if(it!=debugTotal.cend()){//total already had a value
					w+=it->second;
					if(replace){
						w-=2*wold;
					}
				}
				ASSERT_EQUAL(total.find(dstc)->second,w);
			}
			//verify all other inner and total
			debugVerifyInner(srcc,dstc);
			debugVerifyTotal(srcc,dstc);
			//verify cc
			if(added && srcc!=dstc){
				typeWeight w=debugCc.weight(srcc,dstc);
				if(isnan(w)){//did not exist
					w=cc.weight(srcc,dstc);
					ASSERT(!isnan(w));//must exist now
					ASSERT_EQUAL(w,weight);//and match weight
				}
				else{//existed
					typeWeight ww=cc.weight(srcc,dstc);
					ASSERT(!isnan(ww));//must exist now
					ASSERT_EQUAL(ww,((replace)?w-wold+weight:w+weight));
				}
				debugVerifyCc(srcc,dstc);//verify all other cc
			}
		}
		else{//not added
			//must be identical to saved state
			debugVerifySaveState();
		}
#endif	//NDEBUG
	}

//	void debugRemove(const bool & removed,const typeNode & source, const typeNode & destination) const{
//		debugVerify();
//#ifndef NDEBUG
//		int decNodes=0;
//		//verify the node was removed
//		typeCommunity srcc;
//		typeCommunity dstc;
//		typeCommunityListIteratorConst it;
//		const typeNodeList & n=getNodes();
//		const typeNodeList & nn=debugGraphUndirected.getNodes();
//		if(removed){// && nn.find(source)!=nn.cend()){//existed and was removed
//			//verify nodes existed
//			ASSERT_NOT_EQUAL_ITERATOR(nn.find(source),nn.cend());
//			if(source!=destination)	ASSERT_NOT_EQUAL_ITERATOR(nn.find(destination),nn.cend());
//			//verify community existed
//			it=n2c.find(source);
//			ASSERT_NOT_EQUAL_ITERATOR(it,n2c.cend());
//			if(source!=destination)	ASSERT_NOT_EQUAL_ITERATOR(n2c.find(destination),n2c.cend());
//			//verify if node no longer exists
//			if(n.find(source)==n.cend()){//does not exist
//				++decNodes;
//				ASSERT_EQUAL(it->second,noGroup);//verify that the community matches noGroup
//				srcc=source;
//			}
//			else{//still exists
//				ASSERT_EQUAL(it->second,debugN2c.find(source)->second);//verify that the community still the same
//				srcc=it->second;
//			}
//			if(source!=destination){
//				//same for destination
//				it=n2c.find(destination);
//				ASSERT_NOT_EQUAL_ITERATOR(it,n2c.cend());
//				if(nn.find(destination)==nn.cend()){//did not exist
//					++decNodes;
//					ASSERT_EQUAL(it->second,destination);//verify that the attributed community matches itself
//					dstc=destination;
//				}
//				else{
//					ASSERT_EQUAL(it->second,debugN2c.find(destination)->second);//verify that the community still the same
//					dstc=it->second;//if node existed get its community
//				}
//			}
//			else{//if source==destination
//				dstc=srcc;
//			}
//			//verify nodes sizes
//			ASSERT_EQUAL(n.size(),nn.size()-decNodes);
//			ASSERT_EQUAL(n2c.size(),debugN2c.size()-decNodes);
//			ASSERT_EQUAL(total.size(),debugTotal.size()-decNodes);
//			//verify n2c. Existing communities must not change when adding an edge
//			for(typeCommunityListIteratorConst it=debugN2c.cbegin();it!=debugN2c.cend();++it){
//				const typeCommunityListPair & p=*it;
//				typeCommunityListIteratorConst itt=n2c.find(p.first);
//				ASSERT_NOT_EQUAL_ITERATOR(itt,n2c.cend());
//				ASSERT_EQUAL(itt->second,p.second);
//			}
//			//verify total and inner
//			typeWeight wold=debugGraphUndirected.weight(source,destination);
//			if(isnan(wold))wold=0;
//			if(srcc==dstc){
//				typeCommunityEdgesIteratorConst it=debugInner.find(srcc);
//				typeWeight w=((removed)?weight:0);
//				if(source!=destination) w*=2;
//				if(it!=debugInner.cend()){//inner already had a value
//					w+=it->second;
//					if(replace){
//						w-=wold;
//					}
//				}
//				ASSERT_EQUAL(inner.find(srcc)->second,w);
//				it=debugTotal.find(srcc);
//				w=((removed)?weight:0);
//				if(source!=destination) w*=2;
//				if(it!=debugTotal.cend()){//total already had a value
//					w+=it->second;
//					if(replace){
//						w-=wold;
//					}
//				}
//				ASSERT_EQUAL(total.find(srcc)->second,w);
//			}
//			else{// ! srcc==dstc
//				typeCommunityEdgesIteratorConst it=debugTotal.find(srcc);
//				typeWeight w=((removed)?2*weight:0);
//				if(it!=debugTotal.cend()){//total already had a value
//					w+=it->second;
//					if(replace){
//						w-=2*wold;
//					}
//				}
//				ASSERT_EQUAL(total.find(srcc)->second,w);
//				it=debugTotal.find(dstc);
//				w=((removed)?2*weight:0);
//				if(it!=debugTotal.cend()){//total already had a value
//					w+=it->second;
//					if(replace){
//						w-=2*wold;
//					}
//				}
//				ASSERT_EQUAL(total.find(dstc)->second,w);
//			}
//			//verify cc
//			if(removed && srcc!=dstc){
//				typeWeight w=debugCc.weight(srcc,dstc);
//				if(isnan(w)){//did not exist
//					w=cc.weight(srcc,dstc);
//					ASSERT(!isnan(w));//must exist now
//					ASSERT_EQUAL(w,weight);//and match weight
//				}
//				else{//existed
//					typeWeight ww=cc.weight(srcc,dstc);
//					ASSERT(!isnan(ww));//must exist now
//					ASSERT_EQUAL(ww,((replace)?w-wold+weight:w+weight));
//				}
//			}
//		}
//		else{//not removed
//			//must be identical to saved state
//			debugVerifySaveState();
//		}
//#endif	//NDEBUG
//	}

//	void debugReplace(const bool & replaced,const typeNode & oldValue, const typeNode & newValue) const{
//		debugVerify();
//#ifndef NDEBUG
//		//verify sizes
//		ASSERT_EQUAL(debugNodes.size()==nodes.size());
//		ASSERT_EQUAL(debugLinks.size()==links.size());
//		ASSERT_EQUAL(debugTotal_weight==total_weight);
//		ASSERT_EQUAL(debugMax_weight==max_weight);
//		//verify values and weights
//		typeWeight w=0;
//		typeWeight t=0;
//		typeWeight m=std::numeric_limits<typeWeight>::min();
//		for(typeLinksIteratorConst it=debugLinks.cbegin();it!=debugLinks.cend();++it){
//			const typeLinksPair & p=*it;
//			if(replaced){
//				if(p.first==oldValue && p.second==oldValue){//both were old value
//					ASSERT_EQUAL(multimap::find(links,newValue,newValue)!=links.cend());
//					w=(multimap::find(links,newValue,newValue))->second.weight();
//				}
//				else{//only one or none were old value
//					if(p.first==oldValue){//only the source was the old value
//						ASSERT_EQUAL(multimap::find(links,newValue,p.second)!=links.cend());
//						w=(multimap::find(links,newValue,p.second))->second.weight();
//					}
//					else{//the source was not old value
//						if(p.second==oldValue){//only the destination was the old value
//							ASSERT_EQUAL(multimap::find(links,p.first,newValue)!=links.cend());
//							w=(multimap::find(links,p.first,newValue))->second.weight();
//						}
//						else{//none was old value
//							ASSERT_EQUAL(multimap::find(links,p.first,p.second)!=links.cend());
//							w=(multimap::find(links,p.first,p.second))->second.weight();
//						}
//					}
//				}
//			}
//			else{// if it was not replaced
//				ASSERT_EQUAL(multimap::find(links,p.first,p.second)!=links.cend());//edge still exists
//				w=(multimap::find(links,p.first,p.second))->second.weight();
//			}
//			t+=w;
//			if(m<w) m=w;
//		}
//		ASSERT_EQUAL(total_weight==t);
//		ASSERT_EQUAL(max_weight==m);
//#endif	//NDEBUG
//	}

private:

	typeNode minimumNode(const typeCommunity & com){
		typeCommunityListRange p= n2c.keys(com);
		typeNode min=noNode;
		for(typeCommunityListRangeIteratorConst it=p.first;it!=p.second;++it){
			const typeCommunityListRangePair & a=*it;
			if(min==noNode){//first time
				min=a.second;
			}
			else{
				if(a.second<min){
					min=a.second;
				}
			}
		}
		return min;
	}

	/**
	 *
	 * @param ce
	 * @param com
	 * @param weight
	 * @param add
	 * @return true if an update occurred. False if it was erased or does not exist
	 */
	bool update(typeCommunityEdges & ce, const typeCommunity & com, const typeWeight & weight, const bool & add=true){
//		typeWeight ww=2*weight;
		typeCommunityEdgesIterator it=ce.find(com);
		if(it!=ce.end()){
//			const typeCommunityEdgesPair & p=*it;
			typeWeight w;
			if(add) w=2*(it->second/2+weight);
			else w=2*(it->second/2-weight);
//			if(innerEdge) w-=2*weight;
//			else w-=*weight;
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
					ce.insert(std::make_pair(com,2*weight));
				}
				return true;
			}
		}
		return false;
	}

	/**
	 *
	 * @param ce
	 * @param old
	 * @param com
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

public:

	bool addEdge(const typeNode & source, const typeNode & destination, const typeWeight & weight=1.0, const bool & replace=false){
		debugSaveState();
		//get old weight of edge if it exists
		typeWeight w=GraphUndirected::weight(source,destination);
		if(isnan(w)) w=0;
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
			if(cc1==cc2){
				if(replace){
					ww-=w;
				}
				if(source==destination) ww=ww/2;
				bool b=update(total,cc1,ww,true);
				if(b) update(inner,cc1,ww,true);
				else inner.erase(cc1);
			}
			else{
				if(replace){
					ww-=w;
				}
				update(total,cc1,ww,true);
				update(total,cc2,ww,true);
				typeWeight ccw=cc.weight(cc1,cc2);
				if(isnan(ccw))cc.addEdge(cc1,cc2,weight);
				else{
//					if(replace) ccw+=weight;
//					else
					ccw=ccw+ww;
					cc.addEdge(cc1,cc2,ccw,true);
				}
			}
		}
		debugAddEdge(b,source,destination,weight,replace);
		return b;
	}

	bool addEdge(const Edge & edge, const bool & replace=false){return addEdge(edge.source(),edge.destination(),edge.weight(),replace);}

	/**
	 * Replace all occurrences of oldValue by newValue
	 * @param oldValue
	 * @param newValue
	 */
	bool replace(const typeNode & oldValue, const typeNode & newValue){
		typeCommunityListIteratorConst a=n2c.value(oldValue);
		if(a==n2c.cend())return false;//old node does not exist
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
		}
		n2c.replaceKey(oldValue,newValue);
		return GraphUndirected::replace(oldValue,newValue);
	}

	bool removeEdge(const typeNode & source, const typeNode & destination){
		typeWeight weight=GraphUndirected::weight(source,destination);
		if(isnan(weight)){

		}
		else{
			const typeCommunity & c1=community(source);
			const typeCommunity & c2=community(destination);
			if(c1==c2){
//				typeCommunityEdgesIteratorConst it=total.find(c1);
////				if(it!=total.cend()){
//					const typeCommunityEdgesPair & p=*it;
//					typeWeight w=p.second-2*weight;
//					if(w==0){
//						inner.erase(c1);
//						total.erase(it);
//					}
//					else{
//						inner[c1]-=2*weight;
//						total[c1]=w;
//					}
////				}
////				else{
////					inner[c1]-=weight;
////					total[c1]-=weight;
////				}
				typeWeight w=weight;
				if(source==destination) w=weight/2;
				bool b=update(total,c1,w,false);
				if(b) update(inner,c1,w,false);
				else inner.erase(c1);
			}
			else{
//				{
//					typeCommunityEdgesIteratorConst it=total.find(c1);
////					if(it!=total.cend()){
//						const typeCommunityEdgesPair & p=*it;
//						typeWeight w=p.second-2*weight;
//						if(w==0){
//							total.erase(it);
//						}
//						else{
//							total[c1]=w;
//						}
////					}
////					else{
////						total[c1]-=2*weight;
////					}
//				}
//				{
//					typeCommunityEdgesIteratorConst it=total.find(c2);
////					if(it!=total.cend()){
//						const typeCommunityEdgesPair & p=*it;
//						typeWeight w=p.second-2*weight;
//						if(w==0){
//							total.erase(it);
//						}
//						else{
//							total[c2]=w;
//						}
////					}
////					else{
////						total[c2]-=2*weight;
////					}
//				}
				update(total,c1,weight,false);
				update(total,c2,weight,false);
				typeWeight cw=cc.weight(c1,c2);
//				if(isnan(cw))cc.addEdge(c1,c2,weight);
//				else
				if(cw-weight!=0) cc.addEdge(c1,c2,cw-weight,true);//update weight in cc
				else cc.removeEdge(c1,c2);
			}
			bool res=GraphUndirected::removeEdge(source,destination);
			if(neighborsCount(source)==0) n2c.remove(source);
			if(neighborsCount(destination)==0) n2c.remove(destination);
			return res;
		}
		return false;
	}

	bool removeEdge(const Edge & edge){return removeEdge(edge.source(),edge.destination());}

	typeCommunityListRange nodes(const typeCommunity & c){
		return n2c.keys(c);
	}

	void disband(const typeCommunity & c,const unsigned int & level=0){
//		if(level==0){
			typeCommunityListRange n=n2c.keys(c);
			std::set<typeNode> k;//temporary to store nodes. Needed since the iterators returned in the keys range are invalidated on first call to community(node,community)
			for(typeCommunityListRangeIteratorConst it=n.first;it!=n.second;++it){
				const typeCommunityListRangePair & p=*it;
				const typeNode & node=p.second;
				k.insert(node);
			}
			for(std::set<typeNode>::const_iterator it=k.cbegin();it!=k.cend();++it){
				const typeNode & node=*it;
				community(node,node);
			}
//			typeCommunityListRangeIteratorConst it=n2c.key(c);
//			while(it!=n2c.cendr()){
//				const typeCommunityListRangePair & p=*it;
//				const typeNode & node=p.second;
//				++it;
//				community(node,node);
//				if(it->first!=c) it=n2c.key(c);
//			}
//		}
//		else
	}

	const typeCommunities communities(){
//		return cc.getNodes();
		typeNodeList c(cc.getNodes());
		for(typeCommunityEdgesIteratorConst it=inner.cbegin();it!=inner.cend();++it){
			c.insert(it->first);
		}
		return c;
	}

	const typeWeight communityCount()const{return cc.nodeCount();}

	/**
	 *
	 * @param node
	 * @return the community of the given node or a special community of zero if the node does not exist
	 */
	const typeCommunity & community(const typeNode & node)const{
		typeCommunityListIteratorConst it=n2c.find(node);
		if(it!=n2c.cend()){
			return (*it).second;
		}
		return noGroup;//if node does not exist return special community zero
	}

	/**
	 * replace old community by new community on all nodes
	 * @param old is the old community
	 * @param comm is the new community
	 * @return true if the replacement succeeded
	 */
	bool replaceCommunity(const typeCommunity & old, const typeCommunity & com){
		if(old==noGroup || com==noGroup) return false;
		if(old==com)return true;
		//first check if com is empty, otherwise fail
		typeCommunityEdgesIteratorConst itt=total.find(com);
		if(itt!=total.cend()) if(itt->second!=0) return false;
		n2c.replace(old,com);
//		typeCommunityEdgesIteratorConst it=inner.find(old);
//		if(it!=inner.cend()){
//			const typeCommunityEdgesPair & p=*it;
//	//		inner[com]=inner[old];
//			inner[com]=p.second;
//	//		inner[old]=0;
//			inner.erase(it);
//		}
//		itt=total.find(old);
//		if(itt!=total.cend()){
//			const typeCommunityEdgesPair & p1=*itt;
//	//		total[com]=total[old];
//			total[com]=p1.second;
//	//		total[old]=0;
//			total.erase(itt);
//		}
		replace(inner,old,com);
		replace(total,old,com);
		cc.replace(old,com);
		return true;
	}

	/**
	 * set the community of a single node
	 * @param node is the node to be assigned a new community
	 * @param community is the new community
	 * @return true if the node was already in the requested community or, the node exists and insertion succeeded
	 */
	bool community(const typeNode & node, const typeCommunity & com){
		if(node==noNode || com==noGroup) return false;
		const typeCommunity c=community(node);
		if(c==noGroup) return false;
		if(c==com)return true;
		typeWeight a1=neighborsCommunityWeight(node,com);
		typeWeight a2=neighborsCommunityWeight(node,c);
		typeWeight w=neighborsWeight(node);
		update(inner,com,a1/2.0,true);
		update(inner,c,a2/2.0,false);
		update(total,com,(w-a1)/2.0,true);
		update(total,c,(w-a2)/2.0,false);
		const typeLinksRangeConst & nei=neighbors(node);
//		for(typeLinksIteratorConst it=nei.first;it!=nei.second;++it){
		typeLinksIteratorConst it=nei.first;
		while(it!=nei.second){
			const typeLinksPair & p=*it;
			++it;
			if(p.first!=node)break;
			const HalfEdge & he=p.second;
			const typeNode & dest=he.destination();
			const typeWeight & wei=he.weight();
			const typeCommunity & co=community(dest);
//			typeWeight ww=weight(node,dest);
//			if(isnan(ww)){//edge does not exist
////				cc.addEdge(c,)
//			}
//			else{
				if(c!=co){
					typeWeight cw=cc.weight(c,co);
					if(isnan(cw)){
//						cc.addEdge(c,co,ww);
//						cc.addEdge(co,c,ww);
					}
					else{
						cw-=wei;
						if(cw!=0){
							cc.addEdge(c,co,cw,true);
						}
						else{//remove edge since there will be no more connection between the two communities
							cc.removeEdge(c,co);
						}
					}
				}
//			}
			if(co!=com){
				typeWeight cw=cc.weight(co,com);
				if(isnan(cw)){
					cc.addEdge(co,com,wei);
//					cc.addEdge(co,c,ww);
				}
				else{
					cc.addEdge(co,com,cw+wei,true);
//					cc.addEdge(com,co,cw+wei,true);
				}
			}
		}
		bool b=n2c.add(node,com,true);
		if(node==c){//node was the minimum of the community
			typeNode min=minimumNode(c);//determine new minimum
			if(min!=noNode){//if community is not empty
				replaceCommunity(c,min);
			}
			else{
//				cm.erase(c);
				inner.erase(c);
				total.erase(c);
			}
//			min=minimumNode(com);
		}
		typeCommunityListRange x=n2c.keys(c);
		if(x.first==x.second){//no more nodes in community
//			cm.erase(c);
			inner.erase(c);
			total.erase(c);
		}
		if(node<com){//node is the new minimum of the destination community
			replaceCommunity(com,node);
		}
		return b;
	}

	typeWeight innerEdges(const typeCommunity & c)const {
		typeCommunityEdgesIteratorConst it=inner.find(c);
		if(it!=inner.cend()){
			const typeCommunityEdgesPair & p=*it;
			return p.second;
		}
		return 0;
	}

	typeWeight totalEdges(const typeCommunity & c)const {
		typeCommunityEdgesIteratorConst it=total.find(c);
		if(it!=total.cend()){
			const typeCommunityEdgesPair & p=*it;
			return p.second;
		}
		return 0;
	}

	typeLinksRangeConst neighboringCommunities(const typeCommunity & com)const {
		return cc.neighbors(com);
	}

	typeWeight neighboringCommunitiesWeight(const typeCommunity & com)const{
		return cc.neighborsWeight(com);
	}

	unsigned int neighboringCommunitiesCount(const typeCommunity & com)const {
		return cc.neighborsCount(com);
	}

	typeWeight weightCommunity(const typeCommunity & source, const typeCommunity & destination){
		if(source==destination){
			typeCommunityEdgesIteratorConst it=inner.find(source);
			if(it==inner.cend()) return std::numeric_limits<typeWeight>::quiet_NaN();
			return it->second;
		}
		return cc.weight(source,destination);
	}

//	std::map<typeCommunity, typeWeight> neigh_comm(const typeNode & node)const {
//		std::map<typeCommunity, typeWeight> a;
//
//		a[community(node)]=0;
//
//		//get neighbors of node
//		typeLinksRangeConst p = neighbors(node);
//
//		//for all neighbors of node
//		for (typeLinksIteratorConst it=p.first ; it!=p.second ; it++){
//			//get neighbor, community and weight
//			const typeLinksPair & b=*it;
//			const HalfEdge & c=b.second;
//			const typeNode & neigh  = c.destination();
//			const typeCommunity & neigh_comm = community(neigh);
//			const typeWeight & neigh_w = c.weight();
//
//			//if neighbor is not the given node
//			if (neigh!=node) {
////				if (neigh_weight[neigh_comm]==-1) {
////					neigh_weight[neigh_comm] = 0.0L;
////					neigh_pos[neigh_last++] = neigh_comm;
////				}
//				//increment weight
////				neigh_weight[neigh_comm] += neigh_w;
//				a[neigh_comm]+=neigh_w;
//			}
//		}
//		return a;
//	}

	unsigned int neighborsCommunityCount(const typeNode & node)const{
		const typeCommunity & com=community(node);
		unsigned int cnt=0;
		typeLinksRangeConst a=neighbors(node);
		for(typeLinksIteratorConst it=a.first;it!=a.second;++it){
			const typeLinksPair & b=*it;
			const HalfEdge & c=b.second;
			if(community(c.destination())==com)cnt++;
		}
//		if(!firstRun){
//			typeCommunityEdgesIteratorConst it=inner.find(com);
//			if(it!=inner.cend()){
//				const std::pair<typeCommunity,typeWeight> & p=*it;
//				cnt+=p.second;
//			}
//		}
		return cnt;
	}

	typeWeight neighborsCommunityWeight(const typeNode & node, const typeCommunity & com)const{
//		const typeCommunity & com=community(node);
		typeWeight cnt=0;
		typeLinksRangeConst a=neighbors(node);
		for(typeLinksIteratorConst it=a.first;it!=a.second;++it){
			const typeLinksPair & b=*it;
			const HalfEdge & c=b.second;
			const typeNode & d=c.destination();
			if(community(d)==com){
				cnt+=2*c.weight();
				//get reverse edge
//				typeWeight w=weight(d,node);
//				if(!isnan(w))cnt+=w;
			}
		}
//		if(!firstRun){
//			typeCommunityInnerEdgesIteratorConst it=inner.find(com);
//			if(it!=inner.cend()){
//				const std::pair<typeCommunity,typeWeight> & p=*it;
//				cnt+=p.second;
//			}
//		}
		return cnt;
	}

	typeWeight neighborsCommunityWeight(const typeNode & node)const{
		const typeCommunity & com=community(node);
		return neighborsCommunityWeight(node,com);
	}

//	const std::string groupsToString()const{
//		std::stringstream ss;
////		ss << "cm:";
////		ss << set::toString(cm);
//		ss << "in:";
//		ss << map::toString(inner);
//		ss << "tot:";
//		ss << map::toString(total);
//		ss << "cc:";
//		ss << cc.toString();
//		return ss.str();
//	}

	/**
	 * populate the graph with the current community to community mapping
	 * @return
	 */
	bool communitiesToGraph(){
		typeNodeList nd;
		//remove edge if community!=node else update weight
		typeLinksRangeConst e=edges();
		typeLinksIteratorConst it=e.first;
		while(it!=e.second){
			const typeLinksPair & p=*it;
			const typeNode & s=p.first;
			const typeNode & d=p.second.destination();
			if(s!=community(s) || d!=community(d)){
				++it;
				GraphUndirected::removeEdge(s,d);
			}
			else{
				nd.insert(s);
				nd.insert(d);
				const typeWeight & w=weightCommunity(s,d);
				++it;
				GraphUndirected::addEdge(s,d,w,true);//update weight
			}
		}
		//add any links from cc missing
		if(edgeCount()<cc.edgeCount()){
			typeLinksRangeConst e=cc.edges();
			typeLinksIteratorConst it=e.first;
			while(it!=e.second){
				const typeLinksPair & p=*it;
				const typeNode & s=p.first;
				const typeNode & d=p.second.destination();
				const typeWeight & w=weight(s,d);
				if(isnan(w)){
					nd.insert(s);
					nd.insert(d);
					GraphUndirected::addEdge(s,d,p.second.weight());//add missing link
				}
				++it;
			}
		}
		//add inner edges
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
		typeCommunityListIteratorConst itc=n2c.cbegin();
		while(itc!=n2c.cend()){
			const typeCommunityListPair & p=*itc;
			const typeNode & n=p.first;
			const typeCommunity & c=p.second;
			if(nd.find(n)==nd.end()){
				++itc;
				n2c.remove(n,c);
			}
			else{
				++itc;
			}
		}
		return true;
	}

	const std::string toString(const StringFormater & sf=defaultStringFormater)const{
		StringFormater f=sf;
		std::stringstream ss;
		if(!sf.isDefault()){
			f.build(ss,"");
//			++f;
		}
//		if(sf.isDefault())
		f.header("Graph=");
		ss << GraphUndirected::toString(f);
//		ss << "Groups:\n";
//		if(sf.isDefault())
		f.header("Node to Group:");
//		f.build(ss,"Groups:");
		ss << n2c.toString(f);
//		ss << groupsToString();
//		ss << "in:";
		f.header("in=");
		ss << map::toString(inner,f(1));
//		ss << "tot:";
		f.header("tot=");
		ss << map::toString(total,f(1));
//		ss << "cc:";
		f.header("cc=");
		ss << cc.toString(f);
		return ss.str();
	}
};

#endif /* GRAPHUNDIRECTEDGROUPABLE_H_ */
