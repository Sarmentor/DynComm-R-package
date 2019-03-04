/*
 * louvainAlgorithm.h
 *
 *  Created on: 02/02/2019
 *      Author: poltergeist0
 */

#ifndef SRC_ALGORITHMLOUVAIN_H_
#define SRC_ALGORITHMLOUVAIN_H_

#include "algorithmBase.h"

/**
 * Dynamic Louvain implementation
 */
class Louvain: private AlgorithmBase, private QualityInterface{
private:
	GraphUndirectedGroupable cg;//community to community graph to use after the first run
	Quality qltc;//quality for after the first run

	/**
	 * On first run use the reference (g) to calculate node grouping into communities.
	 * On subsequent runs, use the cg having the nodes being the communities found in the reference
	 */
	bool firstRun=true;

	/**
	 *
	 * @param node
	 * @return the community of the given node or a special community of zero if the node does not exist
	 */
	const typeCommunity & community(const typeNode & node)const{
		if(!firstRun){
			const typeCommunity & c=cg.community(node);
			if(c!=noGroup) return c;
		}
		return grph.community(node);
	}

	/**
	 * set the community of a single node or replace old community by new community on all nodes
	 * @param node is either the node to be assigned a new community or the old community depending whether replaceAll is respectively false or true
	 * @param community is the new community
	 * @param replaceAll if true replaces the old community indicated by parameter node by the new community, otherwise replaces the community only for the given node
	 * @return true if the node exists and insertion succeeded or replacement succeeded
	 */
	bool community(const typeNode & node, const typeCommunity & com){
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

	const typeNodeList & getNodes()const{
		if(firstRun) return grph.getNodes();
		return cg.getNodes();
	}

	typeCommunityListRange nodes(const typeCommunity & c){
		if(firstRun) return grph.nodes(c);
		return cg.nodes(c);
	}

	typeLinksRangeConst neighbors(const typeNode & node)const {
		if(firstRun) return grph.neighbors(node);
		return cg.neighbors(node);
	}

	unsigned int neighborsCount(const typeNode & node)const{
		if(firstRun) return grph.neighborsCount(node);
		return cg.neighborsCount(node);
	}

	unsigned int neighborsCommunityCount(const typeNode & node)const{
		if(firstRun)return grph.neighborsCommunityCount(node);
		return cg.neighborsCommunityCount(node);
	}

	typeWeight neighborsCommunityWeight(const typeNode & node, const typeCommunity & com){
		if(firstRun)return grph.neighborsCommunityWeight(node,com);
		return cg.neighborsCommunityWeight(node,com);
	}

	typeWeight neighborsCommunityWeight(const typeNode & node){
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

	const typeWeight nodeCount()const{
		if(firstRun)return grph.nodeCount();
		return cg.nodeCount();
	}

	const typeWeight edgeCount()const{
		if(firstRun)return grph.edgeCount();
		return cg.edgeCount();
	}

	const typeWeight communityCount()const{
		if(firstRun)return grph.communityCount();
		return cg.communityCount();
	}

	typeWeight weighted_degree(const typeNode & node)const{
		if(firstRun) return grph.weighted_degree(node);
		return cg.weighted_degree(node);
	}

	typeQuality gain(const typeNode & node,const typeCommunity & comm)const{
		if(firstRun) return qlt.gain(node,comm);
		return qltc.gain(node,comm);
	}

	typeQuality quality()const{
		if(firstRun) return qlt.quality();
		return qltc.quality();
	}

	void disband(const typeCommunity c1,const typeCommunity c2){
		//remove affected communities from cg by removing all edges to their respective neighbors
//		CERR << "disband("<< c1 << ";" << c2 << ")\n";
		typeLinksRangeConst nc1=cg.neighboringCommunities(c1);
//		CERR << "disband("<< c1 << ";" << c2 << ")->iterator="<< c1 << ";" << h.destination() << "\n" << toString(defaultStringFormater(1)) << "\n";
		{
	//		for(typeLinksIteratorConst it=nc1.first;it!=nc1.second;++it){
			typeLinksIteratorConst it=nc1.first;
			while(it!=nc1.second){
				const typeLinksPair & p=*it;
				if(p.first!=c1) break;
				const HalfEdge & h=p.second;
//				CERR << "disband("<< c1 << ";" << c2 << ")->before remove edge(c,c)="<< c1 << ";" << h.destination() << "\n" << toString(defaultStringFormater(1)) << "\n";
				++it;
				cg.removeEdge(c1,h.destination());
//				CERR << "disband("<< c1 << ";" << c2 << ")->after remove edge(c,c)="<< c1 << ";" << h.destination() << "\n" << toString(defaultStringFormater(1)) << "\n";
			}
		}
		{
			typeLinksRangeConst nc2=cg.neighboringCommunities(c2);
	//		for(typeLinksIteratorConst it=nc2.first;it!=nc2.second;++it){
			typeLinksIteratorConst it=nc2.first;
			while(it!=nc2.second){
				const typeLinksPair & p=*it;
				if(p.first!=c2) break;
				const HalfEdge & h=p.second;
//				CERR << "disband("<< c1 << ";" << c2 << ")->before remove edge(c,c)="<< c2 << ";" << h.destination() << "\n" << toString(defaultStringFormater(1)) << "\n";
				++it;
				cg.removeEdge(c2,h.destination());
//				CERR << "disband("<< c1 << ";" << c2 << ")->after remove edge(c,c)="<< c2 << ";" << h.destination() << "\n" << toString(defaultStringFormater(1)) << "\n";
			}
		}
		//remove inner edges
//		CERR << "disband("<< c1 << ";" << c2 << ")->before remove inner edge(c)="<< c1 << "\n" << toString(defaultStringFormater(1)) << "\n";
		cg.removeEdge(c1,c1);
//		CERR << "disband("<< c1 << ";" << c2 << ")->before remove inner edge(c)="<< c2 << "\n" << toString(defaultStringFormater(1)) << "\n";
		cg.removeEdge(c2,c2);
//		CERR << "disband("<< c1 << ";" << c2 << ")->after remove\n" << toString(defaultStringFormater(1)) << "\n";
		//take nodes of affected communities from g and add them to cg disbanded by adding edges to their neighbors
		std::set<typeNode> ns;
		typeCommunityListRange rc1=grph.nodes(c1);
		for(typeCommunityListRangeIteratorConst it=rc1.first;it!=rc1.second;++it){
			const typeCommunityListRangePair & p=*it;
			ns.insert(p.second);
			cg.community(p.second,p.second);//set community of node to node
		}
		typeCommunityListRange rc2=grph.nodes(c2);
		for(typeCommunityListRangeIteratorConst it=rc2.first;it!=rc2.second;++it){
			const typeCommunityListRangePair & p=*it;
			ns.insert(p.second);
			cg.community(p.second,p.second);//set community of node to node
		}
		for(std::set<typeNode>::const_iterator it=ns.cbegin();it!=ns.cend();++it){
			const typeNode & n=*it;
//			const typeCommunity & c=g.community(n);
			typeLinksRangeConst r=grph.neighbors(n);
//			for(typeLinksIteratorConst itn=r.first;itn!=r.second;++itn){
			typeLinksIteratorConst itn=r.first;
			while(itn!=r.second){
				const typeLinksPair & p=*itn;
				++itn;
				if(p.first!=n) break;
				const HalfEdge & h=p.second;
				const typeNode & nei=h.destination();
				const typeCommunity & cn=grph.community(nei);
				if(cn!=c1 && cn!=c2){
//					CERR << "disband("<< c1 << ";" << c2 << ")->added edge(n,c)="<< n << ";" << cn << "\n";
					cg.addEdge(n,cn,h.weight());//add edge between community of neighbor and node
				}
				else{
//					CERR << "disband("<< c1 << ";" << c2 << ")->added edge(n,n)="<< n << ";" << nei << "\n";
					cg.addEdge(n,nei,h.weight());//add edge between neighbor and node
				}
//				cg.community(n,n);//set community of node to node
//				CERR << "disband("<< c1 << ";" << c2 << ")->after add\n" << toString(defaultStringFormater(1)) << "\n";
			}
		}
		//disband g
//		CERR << "disband("<< c1 << ";" << c2 << ")->before disband("<< c1 << ")\n" << toString(defaultStringFormater(1)) << "\n";
		grph.disband(c1);
//		CERR << "disband("<< c1 << ";" << c2 << ")->before disband("<< c2 << ")\n" << toString(defaultStringFormater(1)) << "\n";
		grph.disband(c2);
//		CERR << "disband("<< c1 << ";" << c2 << ")->end disband\n" << toString(defaultStringFormater(1)) << "\n";
//		for(std::set<typeNode>::const_iterator it=ns.cbegin();it!=ns.cend();++it){
//			const typeNode & n=*it;
//			cg.community(n,n);//set community of node to node
//		}
	}

	std::map<typeCommunity, typeWeight> neigh_comm(const typeNode & node)const {
		std::map<typeCommunity, typeWeight> a;
		if(node==noNode) return a;
		a[community(node)]=0;
		//get neighbors of node
		typeLinksRangeConst p = neighbors(node);
		//for all neighbors of node
		for (typeLinksIteratorConst it=p.first ; it!=p.second ; it++){
			//get neighbor, community and weight
			const typeLinksPair & b=*it;
			const HalfEdge & c=b.second;
			const typeNode & neigh  = c.destination();
			const typeCommunity & neigh_comm = community(neigh);
			const typeWeight & neigh_w = c.weight();

			//if neighbor is not the given node
			if (neigh!=node) {
				//increment weight
				a[neigh_comm]+=neigh_w;
			}
		}
		return a;
	}

  /**
   * Function where the actual algorithm is implemented
   */
    bool one_level(){
		bool improvement=false ;
		int nb_moves;
//		int nb_pass_done = 0;
		long double new_qual = quality();
		long double cur_qual = new_qual;

		const typeNodeList nodes=getNodes();
		// repeat while
		//   there is an improvement of quality
		//   or there is an improvement of quality greater than a given epsilon
		//   or a predefined number of passes have been done
		do {
			cur_qual = new_qual;
			nb_moves = 0;
//			nb_pass_done++;

			// for each node: remove the node from its community and insert it in the best community
			for (typeNodeListIteratorConst node_tmp = nodes.cbegin() ; node_tmp != nodes.cend() ; node_tmp++) {
				const typeNode & node = *node_tmp;
				typeCommunity node_comm = community(node);

				// computation of all neighbor node communities of current node
				std::map<typeCommunity, typeWeight> nc=neigh_comm(node);

				// compute the nearest community for node
				// default choice for future insertion is the former community
				typeCommunity best_comm = node_comm;
				typeWeight best_increase = 0.0L;
				//for all neighbors
				for (std::map<typeCommunity, typeWeight>::iterator it=nc.begin() ; it!=nc.end() ; it++){
					const std::pair<typeCommunity, typeWeight> & p=*it;
					//calculate gain in quality by inserting the given node in the community of the neighbor
					typeWeight increase=gain(node,p.first);
					if (increase>best_increase) {
						best_comm = p.first;
						best_increase = increase;
					}
				}
				// insert node in the nearest community
				community(node,best_comm);
				if (best_comm!=node_comm){
					nb_moves++;
				}
			}

			new_qual = quality();

			if (nb_moves>0)
				improvement=true;

		} while (nb_moves>0 && new_qual-cur_qual > prmtrs.precision);

		//sync changed communities back to reference graph
//		COUT << "start sync\nfirst="<< firstRun << "\n";
		if(firstRun){
			typeCommunities coms=grph.communities();//get all found communities
			for(typeCommunities::const_iterator itc=coms.cbegin();itc!=coms.cend();++itc){
				const typeCommunity & srcc=*itc;
				//handle inner edges
				const typeWeight & in=grph.innerEdges(srcc);
				if(in!=0){
					cg.addEdge(srcc,srcc,in);
				}
//				COUT << "outer edges\n";
				//handle outer edges
				typeLinksRangeConst neighbors=grph.neighboringCommunities(srcc);
				for(typeLinksIteratorConst itn=neighbors.first;itn!=neighbors.second;++itn){
					const typeLinksPair & p=*itn;
					const HalfEdge & he=p.second;
					const typeNode & destc=he.destination();
					const typeWeight & weight=he.weight();
//				COUT << srcc << ";" << destc << ";" << weight << "\n";
					cg.addEdge(srcc,destc,weight);
				}
			}
			firstRun=false;
		}
		else{//not the first run
			typeNodeListConst coms=cg.getNodes();
			for(typeNodeListIteratorConst itc=coms.cbegin();itc!=coms.cend();++itc){
				const typeNode & n=*itc;
				const typeCommunity & c=cg.community(n);
//				COUT << n << ";" << c << "\n";
				if(n!=c){//community has changed
					typeCommunityListRange r=grph.nodes(n);
					for(typeCommunityListRangeIteratorConst itr=r.first;itr!=r.second;){
						const typeCommunityListRangePair & p=*itr;
						const typeNode & nd=p.second;
						++itr;
//						const typeCommunity & cm=p.first;
//            COUT <<"community change ("<< nd << ";" << c << ") start\n";
						grph.community(nd,c);
//						COUT <<"community change ("<< nd << ";" << c << ") end\n";
					}
				}
			}
//			COUT << "communities to graph\n";
			//TODO recreate cg.graph with communities of cg.cc
			cg.communitiesToGraph();
		}
//		COUT << "end sync\nimprovement="<< improvement<< "\n";

		return improvement;
//	  return true;
  }

public:
	bool addRemoveEdgePre(const typeNode & source, const typeNode & destination, const typeWeight & weight=1.0){
		if(weight!=0.0){//add edge

		}
		else{//remove edge
			if(!firstRun){
				const typeCommunity & c1=grph.community(source);
				const typeCommunity & c2=grph.community(destination);
				typeWeight w=cg.weight(c1,c2);//get weight of link if it exists
				if(isnan(w)){//edge does not exist
	//				cg.addEdge(c1,c2,weight);
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

	bool addRemoveEdgePost(const typeNode & source, const typeNode & destination, const typeWeight & weight=1.0){
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
					//				disband(c2);
				}
			}
		}
		else{//remove edge

		}
		return true;
	}

	bool run(){
			bool improvement = true;

//			long double qual = quality();
//			long double new_qual;

			int level = 0;

			//#ifdef MODIFIED
		//	Graph gg(g);
		//	Quality* qq;
		//	init_quality(&qq,&gg,0);
		//	Louvain refLouvain(-1,precision,qq);//copy the current graph and quality to use as reference when adding/removing nodes
		//	bool first=true;
//			unsigned int index=0;
//			bool has_add_file = false;
//			bool has_rem_file = false;

			//#endif //MODIFIED

			//main cycle
			do {
//				if (prmtrs.verbose) {
//					CERR << "level " << level << ":\n";
//					display_time("  start computation");
//					CERR << "  network size: "
//							<< grph.nodeCount() << " nodes, "
//							<< grph.edgeCount() << " links, "
//							<< grph.totalWeight() << " weight\n";
//				}
//				CERR << "**** pre one level ****\n"<< toString();
				//group nodes into communities
				improvement = one_level();
				//get quality of the new grouping
//				new_qual = quality();

		//		if (++level==parameters.display_level)
		////			c.display();
		//			std::cerr << c.toString();
		//		if (parameters.display_level==-1)
		//			c.display_partition();
				++level;
//				CERR << "**** post one level ****\n"<< toString();

		//#ifdef REFACTORED
		//		g.display();
		//#endif //REFACTORED
		//		g = c.partition2graph_binary();
				//#ifdef MODIFIED
		//		init_quality(&g, nb_calls);
				//#endif	//MODIFIED
	//			nb_calls++;

		//		c = Louvain(-1, precision, q);
	//			if (parameters.verbose)
	//				std::cerr << "  quality increased from " << quality << " to " << new_qual << endl;
//				CERR << "  quality modified from " << qual << " to " << new_qual << "\n";
				//quality = (c.qual)->quality();
//				qual = new_qual;

	//			if (parameters.verbose)
	//				display_time("  end computation");

//				if (prmtrs.filename_part!="" && level==1) // do at least one more computation if partition is provided
//					improvement=true;

				//#ifdef	//MODIFIED
				//sync back modifications to reference
				//sync back communities
		//		for (int node=0 ; node < q->size ; node++){
		//			int &new_community=c.qual->n2c[node];
		//			std::vector<int> &refN2c=refLouvain.qual->n2c;
		//			for(int i=0;i<refN2c.size();i++){
		//				if(refN2c[i]==node){
		//					refN2c[i]=new_community;
		//				}
		//			}
		//		}
				//#endif	//MODIFIED
				//add and remove requested links
	//			if (!improvement) add_remove_edges(qlt,index,has_add_file,has_rem_file); ************* TODO
				//debug print
		//		c.display_partition();
//				CERR << toString();
		//		g.display();
				//#endif	//MODIFIED

			} while(improvement);
//			CERR << toString()<< "\n";
			return true;
	  }

public:
  /**
  * Default constructor not acceptable.
  * Must be passed at least the graph
  */
  Louvain()=delete;

  /**
   * Destructor
   */
  ~Louvain(){}

  /**
  * Constructor
  */
  Louvain(
		  GraphUndirectedGroupable & graph
		  , Quality & quality
		  , ProgramParameters & algorithmParameters=argumentsDefault)
    :
    	AlgorithmBase(graph,quality,algorithmParameters)
  	  ,qltc(cg,quality.type(),algorithmParameters)
  {
  }

  const std::string toString(const StringFormater & sf=defaultStringFormater)const{
  		StringFormater f=sf;
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
