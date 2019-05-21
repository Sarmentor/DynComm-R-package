/************************************************************************
 ************************* Developer Notice *****************************
 ************************************************************************
 * @details
 *
 * Dynamic Louvain main algorithm implemented in C++11.
 *
 * @cite cordeiro2016dynamic
 *
 * @author poltergeist0
 *
 * @date 2019-01-01
 ************************************************************************
 ************************************************************************
 ************************************************************************/

#ifndef SRC_ALGORITHMLOUVAIN_H_
#define SRC_ALGORITHMLOUVAIN_H_

#include "algorithmBase.h"

/**
 * @brief Class that implements the Dynamic Louvain algorithm
 *
 * @details
 * The Dynamic Louvain algorithm is a greedy optimization method to extract
 * communities from large networks by optimizing the density of edges inside
 * communities to edges outside communities.
 *
 * @author poltergeist0
 *
 * @date 2019-01-01
 */
class AlgorithmLouvain: private AlgorithmBase{
private:
	/*
	 * Community to community graph to use after the first run.
	 * Required due to the algorithm performing disband of communities
	 */
	GraphUndirectedGroupable cg;

	/*
	 * Quality object used after the first run
	 * Required due to the algorithm performing disband of communities and
	 * because objects of class Quality receive the graph reference used for
	 * computations on initialization.
	 */
	Criterion qltc;

	/*
	 * On first run use the graph on algorithmBase (reference graph) to calculate
	 * node grouping into communities.
	 * On subsequent runs, use the cg having the nodes being the communities found
	 * in the reference.
	 */
	bool firstRun=true;

	/* **********************************************************************
	 ************************************************************************
	 * NOTE: Defined all function from the graph interface so it is easier and to
	 * avoid code duplication
	 ************************************************************************
	 ************************************************************************/

	const typeCommunity & community(const typeVertex & node)const{
		if(!firstRun){
			const typeCommunity & c=cg.community(node);
			if(c!=noGroup) return c;
		}
		return grph.community(node);
	}

	bool community(const typeVertex & node, const typeCommunity & com){
		if(firstRun){
			return grph.community(node,com);
		}
		else{
			return cg.community(node,com);
		}
	}

	typeWeight innerEdges(const typeCommunity & c)const {
		if(firstRun)return grph.innerEdges(c);
		return cg.innerEdges(c);
	}

	typeWeight totalEdges(const typeCommunity & c)const {
		if(firstRun)return grph.totalEdges(c);
		return cg.totalEdges(c);
	}

	typeLinksRangeConst edges()const {
		if(firstRun) return grph.edges();
		return cg.edges();
	}

	const typeVertexList & getVertices()const{
		if(firstRun) return grph.getVertices();
		return cg.getVertices();
	}

	typeCommunityListRange vertices(const typeCommunity & c){
		if(firstRun) return grph.vertices(c);
		return cg.vertices(c);
	}

	typeLinksRangeConst neighbors(const typeVertex & node)const {
		if(firstRun) return grph.neighbors(node);
		return cg.neighbors(node);
	}

	unsigned int neighborsCount(const typeVertex & node)const{
		if(firstRun) return grph.neighborsCount(node);
		return cg.neighborsCount(node);
	}

	unsigned int neighborsCommunityCount(const typeVertex & node)const{
		if(firstRun)return grph.neighborsCommunityCount(node);
		return cg.neighborsCommunityCount(node);
	}

	typeWeight neighborsCommunityWeight(const typeVertex & node, const typeCommunity & com){
		if(firstRun)return grph.neighborsCommunityWeight(node,com);
		return cg.neighborsCommunityWeight(node,com);
	}

	typeWeight neighborsCommunityWeight(const typeVertex & node){
		if(firstRun)return grph.neighborsCommunityWeight(node);
		return cg.neighborsCommunityWeight(node);
	}

	const typeWeight & maxWeight()const{
		if(firstRun) return grph.maxWeight();
		return cg.maxWeight();
	}

	const typeWeight totalWeight()const{
		if(firstRun)return grph.totalWeight();
		return cg.totalWeight();
	}

	const typeWeight verticesCount()const{
		if(firstRun)return grph.vertexCount();
		return cg.vertexCount();
	}

	const typeWeight edgeCount()const{
		if(firstRun)return grph.edgeCount();
		return cg.edgeCount();
	}

	const typeWeight communityCount()const{
		if(firstRun)return grph.communityCount();
		return cg.communityCount();
	}

	typeWeight weighted_degree(const typeVertex & vertex)const{
		if(firstRun) return grph.weighted_degree(vertex);
		return cg.weighted_degree(vertex);
	}

	/* **********************************************************************
	 ************************************************************************
	 * NOTE: Defined all function from the quality interface so it is easier
	 * and to avoid code duplication
	 ************************************************************************
	 ************************************************************************/

	typeCriterion gain(const typeVertex & vertex,const typeCommunity & comm)const{
		if(firstRun) return qlt.gain(vertex,comm);
		return qltc.gain(vertex,comm);
	}

	typeCriterion quality()const{
		if(firstRun) return qlt.quality();
		return qltc.quality();
	}

	/* **********************************************************************
	 ************************************************************************
	 * NOTE: Algorithm functions
	 ************************************************************************
	 ************************************************************************/

	/**
	 * Disband communities belonging to the given edge.
	 * Source and destination communities must be different.
	 * It is an algorithm requirement that, when adding edges, the communities
	 * involved get removed and replaced by their individual vertices, so that
	 * calculation are only performed over those vertices, making the Louvain
	 * algorithm dynamic.
	 * @param c1 the source community of the edge
	 * @param c2 the destination community of the edge
	 */
	void disband(const typeCommunity c1,const typeCommunity c2){
		//remove affected communities from cg by removing all edges to their respective neighbors
		typeLinksRangeConst nc1=cg.neighboringCommunities(c1);
		{
			typeLinksIteratorConst it=nc1.first;
			while(it!=nc1.second){
				const typeLinksPair & p=*it;
				if(p.first!=c1) break;
				const HalfEdge & h=p.second;
				++it;
				cg.removeEdge(c1,h.destination());
			}
		}
		{
			typeLinksRangeConst nc2=cg.neighboringCommunities(c2);
			typeLinksIteratorConst it=nc2.first;
			while(it!=nc2.second){
				const typeLinksPair & p=*it;
				if(p.first!=c2) break;
				const HalfEdge & h=p.second;
				++it;
				cg.removeEdge(c2,h.destination());
			}
		}
		//remove inner edges
		cg.removeEdge(c1,c1);
		cg.removeEdge(c2,c2);
		//take nodes of affected communities from g and add them to cg disbanded by adding edges to their neighbors
		std::set<typeVertex> ns;
		typeCommunityListRange rc1=grph.vertices(c1);
		for(typeCommunityListRangeIteratorConst it=rc1.first;it!=rc1.second;++it){
			const typeCommunityListRangePair & p=*it;
			ns.insert(p.second);
			cg.community(p.second,p.second);//set community of vertex to vertex
		}
		typeCommunityListRange rc2=grph.vertices(c2);
		for(typeCommunityListRangeIteratorConst it=rc2.first;it!=rc2.second;++it){
			const typeCommunityListRangePair & p=*it;
			ns.insert(p.second);
			cg.community(p.second,p.second);//set community of node to node
		}
		for(std::set<typeVertex>::const_iterator it=ns.cbegin();it!=ns.cend();++it){
			const typeVertex & n=*it;
			typeLinksRangeConst r=grph.neighbors(n);
			typeLinksIteratorConst itn=r.first;
			while(itn!=r.second){
				const typeLinksPair & p=*itn;
				++itn;
				if(p.first!=n) break;
				const HalfEdge & h=p.second;
				const typeVertex & nei=h.destination();
				const typeCommunity & cn=grph.community(nei);
				if(cn!=c1 && cn!=c2){
					cg.addEdge(n,cn,h.weight());//add edge between community of neighbor and node
				}
				else{
					cg.addEdge(n,nei,h.weight());//add edge between neighbor and node
				}
			}
		}
		//disband communities on the reference graph
		grph.disband(c1);
		grph.disband(c2);
	}

	/**
	 * Get the neighboring communities of the given vertex with edge weight
	 *
	 * @param vertex
	 * @return the neighboring communities of the given vertex
	 */
	std::map<typeCommunity, typeWeight> neigh_comm(const typeVertex & vertex)const {
		std::map<typeCommunity, typeWeight> a;
		if(vertex==noVertex) return a;
		a[community(vertex)]=0;
		//get neighbors of vertex
		typeLinksRangeConst p = neighbors(vertex);
		//for all neighbors of vertex
		for (typeLinksIteratorConst it=p.first ; it!=p.second ; it++){
			//get neighbor, community and weight
			const typeLinksPair & b=*it;
			const HalfEdge & c=b.second;
			const typeVertex & neigh  = c.destination();
			const typeCommunity & neigh_comm = community(neigh);
			const typeWeight & neigh_w = c.weight();
			//if neighbor is not the given vertex
			if (neigh!=vertex) {
				//increment weight
				a[neigh_comm]+=neigh_w;
			}
		}
		return a;
	}

	/**
	 * Function where the actual algorithm is implemented
	 *
	 * @return true if there was an improvement in quality. False, otherwise
	 */
    bool one_level(){
		bool improvement=false ;
		int nb_moves;
		long double new_qual = quality();
		long double cur_qual = new_qual;

		const typeVertexList nodes=getVertices();
		// repeat while
		//   there is an improvement of quality
		//   or there is an improvement of quality greater than a given epsilon
		//   or a predefined number of passes have been done
		do {
			cur_qual = new_qual;
			nb_moves = 0;

			// for each node: remove the node from its community and insert it in the best community
			for (typeVertexListIteratorConst node_tmp = nodes.cbegin() ; node_tmp != nodes.cend() ; node_tmp++) {
				const typeVertex & vertex = *node_tmp;
				typeCommunity node_comm = community(vertex);

				// computation of all neighbor node communities of current node
				std::map<typeCommunity, typeWeight> nc=neigh_comm(vertex);

				// compute the nearest community for node
				// default choice for future insertion is the former community
				typeCommunity best_comm = node_comm;
				typeWeight best_increase = 0.0L;
				//for all neighbors
				for (std::map<typeCommunity, typeWeight>::iterator it=nc.begin() ; it!=nc.end() ; it++){
					const std::pair<typeCommunity, typeWeight> & p=*it;
					//calculate gain in quality by inserting the given node in the community of the neighbor
					typeWeight increase=gain(vertex,p.first);
					if (increase>best_increase) {
						best_comm = p.first;
						best_increase = increase;
					}
				}
				// insert node in the nearest community
				community(vertex,best_comm);
				if (best_comm!=node_comm){
					nb_moves++;
				}
			}

			new_qual = quality();

			if (nb_moves>0)
				improvement=true;

		} while (nb_moves>0 && new_qual-cur_qual > prmtrs.precision);

		//sync changed communities back to reference graph
		if(firstRun){
			typeCommunities coms=grph.communities();//get all found communities
			for(typeCommunities::const_iterator itc=coms.cbegin();itc!=coms.cend();++itc){
				const typeCommunity & srcc=*itc;
				//handle inner edges
				const typeWeight & in=grph.innerEdges(srcc);
				if(in!=0){
					cg.addEdge(srcc,srcc,in);
				}
				//handle outer edges
				typeLinksRangeConst neighbors=grph.neighboringCommunities(srcc);
				for(typeLinksIteratorConst itn=neighbors.first;itn!=neighbors.second;++itn){
					const typeLinksPair & p=*itn;
					const HalfEdge & he=p.second;
					const typeVertex & destc=he.destination();
					const typeWeight & weight=he.weight();
					cg.addEdge(srcc,destc,weight);
				}
			}
			firstRun=false;
		}
		else{//not the first run
			typeVertexListConst coms=cg.getVertices();
			for(typeVertexListIteratorConst itc=coms.cbegin();itc!=coms.cend();++itc){
				const typeVertex & n=*itc;
				const typeCommunity & c=cg.community(n);
				if(n!=c){//community has changed
					typeCommunityListRange r=grph.vertices(n);
					for(typeCommunityListRangeIteratorConst itr=r.first;itr!=r.second;){
						const typeCommunityListRangePair & p=*itr;
						const typeVertex & nd=p.second;
						++itr;
						grph.community(nd,c);
					}
				}
			}
			cg.communitiesToGraph();
		}
		return improvement;
  }

public:
	/**
	 * Function to execute before adding or removing a single edge from the graph.
	 * It is used internally by the addRemoveEdges function.
	 * Useful if pre-processing is required, for example, to update some of
	 * variables before the edge can be added or removed.
	 * Any edge with a weight different from zero is inserted.
	 * Any edge with a weight of exactly zero is removed.
	 *
	 * @param source source vertex
	 * @param destination destination vertex
	 * @param weight optional if adding an edge. Must be exactly zero to remove an edge.
	 * @return true if all operations succeeded. False, otherwise.
	 */
	bool addRemoveEdgePre(const typeVertex & source, const typeVertex & destination, const typeWeight & weight=1.0){
		if(weight!=0.0){//add edge

		}
		else{//remove edge
			if(!firstRun){
				const typeCommunity & c1=grph.community(source);
				const typeCommunity & c2=grph.community(destination);
				typeWeight w=cg.weight(c1,c2);//get weight of link if it exists
				if(isnan(w)){//edge does not exist
				}
				else{//edge already exists
					typeWeight weight=grph.weight(source,destination);
					if(isnan(weight)) return false;
					if(c1==c2) w-=2*weight;
					else w-=weight;
					if(w!=0) cg.addEdge(c1,c2,w,true);//replace
					else cg.removeEdge(c1,c2);
				}
			}
		}
		return true;
	}

	/**
	 * Function execute after adding or removing a single edge from the graph.
	 * It is used internally by the addRemoveEdges function.
	 * Useful if post-processing is required, for example, to update some of
	 * variables after the edge has be added or removed.
	 * Any edge with a weight different from zero is inserted.
	 * Any edge with a weight of exactly zero is removed.
	 *
	 * @param source source vertex
	 * @param destination destination vertex
	 * @param weight optional if adding an edge. Must be exactly zero to remove an edge.
	 * @return true if all operations succeeded. False, otherwise.
	 */
	bool addRemoveEdgePost(const typeVertex & source, const typeVertex & destination, const typeWeight & weight=1.0){
		if(weight!=0.0){//add edge
			if(!firstRun){
				const typeCommunity & c1=grph.community(source);
				const typeCommunity & c2=grph.community(destination);
				typeWeight w=cg.weight(c1,c2);//get weight of link if it exists
				if(isnan(w)){//edge does not exist
					if(c1==c2) w=2*weight;
					cg.addEdge(c1,c2,weight);
				}
				else{//edge already exists
					if(c1==c2) w+=2*weight;
					else w+=weight;
					cg.addEdge(c1,c2,w,true);
				}
			}
			if(!firstRun){
				const typeCommunity & c1=grph.community(source);
				const typeCommunity & c2=grph.community(destination);
				if(c1!=c2){//disband both communities
					disband(c1,c2);
				}
			}
		}
		else{//remove edge

		}
		return true;
	}

	/**
	 * Function where the actual algorithms are implemented.
	 * It is called at the end of the addRemoveEdges function.
	 *
	 * @return true if all operations succeeded. False, otherwise.
	 */
	bool run(){
			bool improvement = true;
			int level = 0;
			//main cycle
			do {
				//group nodes into communities
				improvement = one_level();
				//get quality of the new grouping
				++level;
			} while(improvement);
			return true;
	  }

public:
	/**
	 * Default constructor not acceptable.
	 * Must be passed at least the chosen algorithm and the graph
	 */
	AlgorithmLouvain()=delete;

	/**
	 * Destructor
	 */
	~AlgorithmLouvain(){}

	/**
	 * Constructor.
	 *
	 * @param graph reference to the graph object
	 * @param quality reference to the quality object
	 * @param algorithmParameters reference to the parameters object
	 */
	AlgorithmLouvain(
			GraphUndirectedGroupable & graph
			, const Criterion & quality
			, const ProgramParameters & algorithmParameters=argumentsDefault)
	:
		AlgorithmBase(graph,quality,algorithmParameters)
	,qltc(cg,quality.type(),algorithmParameters)
	{
	}

	/**
	 * Function that converts this object to a string representation.
	 * Might be useful for debugging.
	 *
	 * @param sf is a StringFormater object that facilitates formating
	 * @return the string representing this object
	 */
	const std::string toString(const StringFormatter & sf=defaultStringFormatter)const{
		StringFormatter f=sf;
		std::stringstream ss;
		if(!sf.isDefault()){
			f.build(ss,"");
			++f;
		}
		ss << AlgorithmBase::toString(sf);
		f.header("cg:");
		ss << cg.toString(f);
		return ss.str();
	}

};

#endif /* SRC_ALGORITHMLOUVAIN_H_ */
