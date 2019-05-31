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
#include <math.h>

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


private:

	/**
	 *
	 * @param com
	 * @return the smallest number used as a vertex in the given community
	 */
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
	 * Update the weight of a community
	 *
	 * @param ce is the list of communities
	 * @param com is the community to update
	 * @param weight is the weight to add or update
	 * @param add if true adds the given weight to the existing value. Otherwise, assigns the given weight
	 * @return true if an update occurred. False if it was erased or does not exist
	 */
	bool update(typeCommunityEdges & ce, const typeCommunity & com, const typeWeight & weight, const bool & add=true){
		typeCommunityEdgesIterator it=ce.find(com);
		if(it!=ce.end()){
			typeWeight w;
			if(add) w=(it->second+weight);
			else w=(it->second-weight);
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

	/**
	 * remove an edge
	 *
	 * @param source
	 * @param destination
	 * @return true if the edge existed and was successfully removed
	 */
	bool removeEdge(const typeVertex & source, const typeVertex & destination){
		typeWeight weight=GraphUndirected::weight(source,destination);
		if(isnan(weight)){

		}
		else{
			const typeCommunity & c1=community(source);
			const typeCommunity & c2=community(destination);
			if(c1==c2){
				typeWeight w=weight;
				if(source==destination) w=weight/2;
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
			if(neighboursCount(source)==0) n2c.remove(source);
			if(neighboursCount(destination)==0) n2c.remove(destination);
			return res;
		}
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
	 * @param level
	 */
	void disband(const typeCommunity & c,const unsigned int & level=0){
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
	}

	/**
	 * @return the list of all existing communities
	 */
	const typeCommunities communities()const{
		typeCommunities c(cc.getVertices().begin(),cc.getVertices().end());
		for(typeCommunityEdgesIteratorConst it=inner.cbegin();it!=inner.cend();++it){
			c.insert(it->first);
		}
		return c;
	}

	/**
	 * @return the number of existing communities
	 */
	const typeWeight communityCount()const{return cc.vertexCount();}

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
		typeWeight a1=neighboursCommunityWeight(vertex,com);
		typeWeight a2=neighboursCommunityWeight(vertex,c);
		typeWeight w=neighboursWeight(vertex);
		typeWeight in=weight(vertex,vertex);
		if(isnan(in))in=0;
		update(inner,com,2*a1+in,true);
		update(inner,c,2*a2+in,false);
		update(total,com,w+in,true);
		update(total,c,w+in,false);
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
		if(vertex<com){//vertex is the new minimum of the destination community
			replaceCommunity(com,vertex);
		}
		return b;
	}

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
	 * Get the weight of the edge form by two communities
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
};

#endif /* GRAPHUNDIRECTEDGROUPABLE_H_ */
