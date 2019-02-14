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
class Louvain: private AlgorithmBase {
private:
	GraphUndirectedGroupable cg;//community to community graph to use after the first run

	/**
	 * On first run use the reference (g) to calculate node grouping into communities.
	 * On subsequent runs, use the cg having the nodes being the communities found in the reference
	 */
	bool firstRun=true;

	void disband(const typeCommunity c1,const typeCommunity c2){
		//remove affected communities from cg by removing all edges to their respective neighbors
		typeLinksRangeConst nc1=cg.neighboringCommunities(c1);
		for(typeLinksIteratorConst it=nc1.first;it!=nc1.second;++it){
			const typeLinksPair & p=*it;
			if(p.first!=c1) break;
			const HalfEdge & h=p.second;
//			std::cerr << "disband("<< c1 << ";" << c2 << ")->before remove edge(c,c)="<< c1 << ";" << h.destination() << "\n" << toString(defaultStringFormater(1)) << "\n";
			cg.removeEdge(c1,h.destination());
//			std::cerr << "disband("<< c1 << ";" << c2 << ")->after remove edge(c,c)="<< c1 << ";" << h.destination() << "\n" << toString(defaultStringFormater(1)) << "\n";
		}
		typeLinksRangeConst nc2=cg.neighboringCommunities(c2);
		for(typeLinksIteratorConst it=nc2.first;it!=nc2.second;++it){
			const typeLinksPair & p=*it;
			if(p.first!=c2) break;
			const HalfEdge & h=p.second;
//			std::cerr << "disband("<< c1 << ";" << c2 << ")->before remove edge(c,c)="<< c2 << ";" << h.destination() << "\n" << toString(defaultStringFormater(1)) << "\n";
			cg.removeEdge(c2,h.destination());
//			std::cerr << "disband("<< c1 << ";" << c2 << ")->after remove edge(c,c)="<< c2 << ";" << h.destination() << "\n" << toString(defaultStringFormater(1)) << "\n";
		}
		//remove inner edges
//		std::cerr << "disband("<< c1 << ";" << c2 << ")->before remove inner edge(c)="<< c1 << "\n" << toString(defaultStringFormater(1)) << "\n";
		cg.removeEdge(c1,c1);
//		std::cerr << "disband("<< c1 << ";" << c2 << ")->before remove inner edge(c)="<< c2 << "\n" << toString(defaultStringFormater(1)) << "\n";
		cg.removeEdge(c2,c2);
//		std::cerr << "disband("<< c1 << ";" << c2 << ")->after remove\n" << toString(defaultStringFormater(1)) << "\n";
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
//					std::cerr << "disband("<< c1 << ";" << c2 << ")->added edge(n,c)="<< n << ";" << cn << "\n";
					cg.addEdge(n,cn,h.weight());//add edge between community of neighbor and node
				}
				else{
//					std::cerr << "disband("<< c1 << ";" << c2 << ")->added edge(n,n)="<< n << ";" << nei << "\n";
					cg.addEdge(n,nei,h.weight());//add edge between neighbor and node
				}
//				cg.community(n,n);//set community of node to node
//				std::cerr << "disband("<< c1 << ";" << c2 << ")->after add\n" << toString(defaultStringFormater(1)) << "\n";
			}
		}
		//disband g
//		std::cerr << "disband("<< c1 << ";" << c2 << ")->before disband("<< c1 << ")\n" << toString(defaultStringFormater(1)) << "\n";
		grph.disband(c1);
//		std::cerr << "disband("<< c1 << ";" << c2 << ")->before disband("<< c2 << ")\n" << toString(defaultStringFormater(1)) << "\n";
		grph.disband(c2);
//		std::cerr << "disband("<< c1 << ";" << c2 << ")->end disband\n" << toString(defaultStringFormater(1)) << "\n";
//		for(std::set<typeNode>::const_iterator it=ns.cbegin();it!=ns.cend();++it){
//			const typeNode & n=*it;
//			cg.community(n,n);//set community of node to node
//		}
	}

	std::map<typeCommunity, typeWeight> neigh_comm(const typeNode & node)const {
		std::map<typeCommunity, typeWeight> a;
		if(node==noNode) return a;
		a[grph.community(node)]=0;
		//get neighbors of node
		typeLinksRangeConst p = grph.neighbors(node);
		//for all neighbors of node
		for (typeLinksIteratorConst it=p.first ; it!=p.second ; it++){
			//get neighbor, community and weight
			const typeLinksPair & b=*it;
			const HalfEdge & c=b.second;
			const typeNode & neigh  = c.destination();
			const typeCommunity & neigh_comm = grph.community(neigh);
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
		long double new_qual = qlt.quality();
		long double cur_qual = new_qual;

		const typeNodeList nodes=grph.getNodes();
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
				typeCommunity node_comm = grph.community(node);

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
					typeWeight increase=qlt.gain(node,p.first);
					if (increase>best_increase) {
						best_comm = p.first;
						best_increase = increase;
					}
				}
				// insert node in the nearest community
				grph.community(node,best_comm);
				if (best_comm!=node_comm){
					nb_moves++;
				}
			}

			new_qual = qlt.quality();

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
					const typeNode & destc=he.destination();
					const typeWeight & weight=he.weight();
//					std::cout << srcc << ";" << destc << ";" << weight << "\n";
					cg.addEdge(srcc,destc,weight);
				}
			}
			firstRun=false;
		}
		else{//not the first run
			typeNodeListConst coms=cg.getNodes();
			for(typeNodeListIteratorConst itc=coms.cbegin();itc!=coms.cend();++itc){
				const typeNode & n=*itc;
				const typeCommunity & c=grph.community(n);
				grph.community(n,c);
			}
			//TODO recreate cg.graph with communities of cg.cc
			cg.communitiesToGraph();
		}

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
					cg.addEdge(c1,c2,weight);
				}
				else{//edge already exists
					w+=weight;
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

			long double quality = qlt.quality();
			long double new_qual;

//			int level = 0;

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
//			do {
	//			if (parameters.verbose) {
	//				std::cerr << "level " << level << ":\n";
	//				display_time("  start computation");
	//				std::cerr << "  network size: "
	//						<< c.nodeCount() << " nodes, "
	//						<< c.edgeCount() << " links, "
	//						<< c.totalWeight() << " weight" << endl;
	//			}
	//			std::cerr << "**** pre one level ****\n"<< c.toString();
				//group nodes into communities
				improvement = one_level();
				//get quality of the new grouping
				new_qual = qlt.quality();

		//		if (++level==parameters.display_level)
		////			c.display();
		//			std::cerr << c.toString();
		//		if (parameters.display_level==-1)
		//			c.display_partition();
	//				std::cerr << "**** post one level ****\n"<< c.toString();

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
				std::cerr << "  quality modified from " << quality << " to " << new_qual << "\n";
				//quality = (c.qual)->quality();
				quality = new_qual;

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
		//		std::cerr << c.toString();
		//		g.display();
				//#endif	//MODIFIED

//			} while(improvement);
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
  {
  }

};

#endif /* SRC_ALGORITHMLOUVAIN_H_ */
