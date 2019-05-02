/*
 * GraphGroupable.h
 *
 *  Created on: 19/08/2018
 *      Author: poltergeist0
 */

#ifndef GRAPHUNDIRECTEDGROUPABLE_H_
#define GRAPHUNDIRECTEDGROUPABLE_H_

#include "mapReversable.h"
#include <math.h>

#include "graphUndirected.h"
#include "setUtilities.h"

typedef typeVertex typeCommunity;
//typedef long typeCommunity;

typedef MapReversable<typeVertex,typeCommunity> typeCommunityList;

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
typedef typeCommunityList::range_iterator_const typeCommunityListRangeIteratorConst;

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

const typeCommunity noGroup=std::numeric_limits<typeCommunity>::max();

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


private:

	typeVertex minimumNode(const typeCommunity & com){
		typeCommunityListRange p= n2c.keys(com);
		typeVertex min=noVertex;
		for(typeCommunityListRangeIteratorConst it=p.first;it!=p.second;++it){
			const typeCommunityListRangePair & a=*it;
			if(min==noVertex){//first time
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
//			if(add) w=2*(it->second/2+weight);
//			else w=2*(it->second/2-weight);
			if(add) w=(it->second+weight);
			else w=(it->second-weight);
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
//					ce.insert(std::make_pair(com,2*weight));
					ce.insert(std::make_pair(com,weight));
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

	bool addEdge(const typeVertex & source, const typeVertex & destination, const typeWeight & weight=1.0, const bool & replace=false){
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
//				CERR << "graph add edge("<<source<<","<< destination<<"): com("<<cc1<<","<< cc2<<") ; wei="<<2*ww<< "\n";
				bool b=update(total,cc1,2*ww,true);
				if(b) update(inner,cc1,2*ww,true);
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
					ccw=ccw+ww;
					cc.addEdge(cc1,cc2,ccw,true);
				}
			}
		}
//		COUT << "graph post add edge: "<<toString();
		return b;
	}

	bool addEdge(const Edge & edge, const bool & replace=false){return addEdge(edge.source(),edge.destination(),edge.weight(),replace);}

	/**
	 * Replace all occurrences of oldValue by newValue
	 * @param oldValue
	 * @param newValue
	 */
	bool replace(const typeVertex & oldValue, const typeVertex & newValue){
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

	bool removeEdge(const typeVertex & source, const typeVertex & destination){
//		COUT << "graph pre remove edge: "<<toString();
		typeWeight weight=GraphUndirected::weight(source,destination);
		if(isnan(weight)){

		}
		else{
			const typeCommunity & c1=community(source);
			const typeCommunity & c2=community(destination);
			if(c1==c2){
				typeWeight w=weight;
				if(source==destination) w=weight/2;
//				CERR << "graph remove edge("<<source<<","<< destination<<"): com("<<c1<<","<< c2<<") ; wei="<<2*w<< "\n";
				bool b=update(total,c1,2*w,false);
				if(b) update(inner,c1,2*w,false);
				else inner.erase(c1);
			}
			else{
				update(total,c1,weight,false);
				update(total,c2,weight,false);
				typeWeight cw=cc.weight(c1,c2);
				if(cw-weight!=0) cc.addEdge(c1,c2,cw-weight,true);//update weight in cc
				else cc.removeEdge(c1,c2);
			}
			bool res=GraphUndirected::removeEdge(source,destination);
			if(neighborsCount(source)==0) n2c.remove(source);
			if(neighborsCount(destination)==0) n2c.remove(destination);
//			COUT << "graph post remove edge: "<<toString();
			return res;
		}
//		COUT << "graph post remove edge: "<<toString();
		return false;
	}

	bool removeEdge(const Edge & edge){return removeEdge(edge.source(),edge.destination());}

	typeCommunityListRange vertices(const typeCommunity & c)const{
		return n2c.keys(c);
	}

	void disband(const typeCommunity & c,const unsigned int & level=0){
//		if(level==0){
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
//		}
//		else
	}

	const typeCommunities communities()const{
		typeCommunities c(cc.getVertices().begin(),cc.getVertices().end());
		for(typeCommunityEdgesIteratorConst it=inner.cbegin();it!=inner.cend();++it){
			c.insert(it->first);
		}
		return c;
	}

	const typeWeight communityCount()const{return cc.vertexCount();}

	/**
	 *
	 * @param node
	 * @return the community of the given node or a special community of zero if the node does not exist
	 */
	const typeCommunity & community(const typeVertex & vertex)const{
		typeCommunityListIteratorConst it=n2c.find(vertex);
		if(it!=n2c.cend()){
			return (*it).second;
		}
		return noGroup;//if node does not exist return special community zero
	}

	/**
	 * replace old community by new community on all vertices
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
		replace(inner,old,com);
		replace(total,old,com);
		cc.replace(old,com);
		return true;
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
//		COUT << "graph pre community("<<node<<","<< com<<"): "<<toString();
		typeWeight a1=neighborsCommunityWeight(vertex,com);
		typeWeight a2=neighborsCommunityWeight(vertex,c);
		typeWeight w=neighborsWeight(vertex);
		typeWeight in=weight(vertex,vertex);
		if(isnan(in))in=0;
		update(inner,com,2*a1+in,true);
		update(inner,c,2*a2+in,false);
		update(total,com,w+in,true);
		update(total,c,w+in,false);
//		COUT << "graph community update("<<node<<","<< com<<"): "<<toString();
		const typeLinksRangeConst & nei=neighbors(vertex);
		typeLinksIteratorConst it=nei.first;
		while(it!=nei.second){
			const typeLinksPair & p=*it;
			++it;
//			COUT << "graph community("<<vertex<<","<< com<<"): nei="<<p.first<<"\n";
			if(p.first!=vertex)break;
			const HalfEdge & he=p.second;
			const typeVertex & dest=he.destination();
//			COUT << "graph community nei("<<vertex<<","<< com<<"): nei("<<p.first<<","<< dest<<"\n";
			if(dest!=vertex){
				const typeWeight & wei=he.weight();
				const typeCommunity & co=community(dest);
					if(c!=co){
						typeWeight cw=cc.weight(c,co);
						if(isnan(cw)){
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
				if(co!=com){
					typeWeight cw=cc.weight(co,com);
					if(isnan(cw)){
						cc.addEdge(co,com,wei);
					}
					else{
						cc.addEdge(co,com,cw+wei,true);
					}
				}
			}
		}
//		COUT << "graph community n2c\n";
		bool b=n2c.add(vertex,com,true);
		if(vertex==c){//node was the minimum of the community
			typeVertex min=minimumNode(c);//determine new minimum
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
//		COUT << "graph community min\n";
		if(vertex<com){//vertex is the new minimum of the destination community
			replaceCommunity(com,vertex);
		}
//	COUT << "graph post community: "<<toString();
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

	typeWeight weightCommunity(const typeCommunity & source, const typeCommunity & destination)const{
		if(source==destination){
			typeCommunityEdgesIteratorConst it=inner.find(source);
			if(it==inner.cend()) return std::numeric_limits<typeWeight>::quiet_NaN();
			return it->second;
		}
		return cc.weight(source,destination);
	}

	unsigned int neighborsCommunityCount(const typeVertex & vertex)const{
		const typeCommunity & com=community(vertex);
		unsigned int cnt=0;
		typeLinksRangeConst a=neighbors(vertex);
		for(typeLinksIteratorConst it=a.first;it!=a.second;++it){
			const typeLinksPair & b=*it;
			const HalfEdge & c=b.second;
			if(community(c.destination())==com)cnt++;
		}
		return cnt;
	}

	typeWeight neighborsCommunityWeight(const typeVertex & vertex, const typeCommunity & com)const{
		typeWeight cnt=0;
		typeLinksRangeConst a=neighbors(vertex);
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

	typeWeight neighborsCommunityWeight(const typeVertex & vertex)const{
		const typeCommunity & com=community(vertex);
		return neighborsCommunityWeight(vertex,com);
	}

	/**
	 * populate the graph with the current community to community mapping
	 * @return
	 */
	bool communitiesToGraph(){
		typeVertexList nd;
		//remove edge if community!=node else update weight
		typeLinksRangeConst e=edges();
		typeLinksIteratorConst it=e.first;
		while(it!=e.second){
			const typeLinksPair & p=*it;
			const typeVertex & s=p.first;
			const typeVertex & d=p.second.destination();
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
			typeLinksRangeConst ee=cc.edges();
			typeLinksIteratorConst it2=ee.first;
			while(it2!=ee.second){
				const typeLinksPair & p=*it2;
				const typeVertex & s=p.first;
				const typeVertex & d=p.second.destination();
				const typeWeight & w=cc.weight(s,d);
				if(!isnan(w)){
					nd.insert(s);
					nd.insert(d);
					GraphUndirected::addEdge(s,d,p.second.weight());//add missing link
				}
				++it2;
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
		return true;
	}

	const std::string toString(const StringFormater & sf=defaultStringFormater)const{
		StringFormater f=sf;
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
};

#endif /* GRAPHUNDIRECTEDGROUPABLE_H_ */
