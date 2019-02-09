/*
 * louvainAlgorithm.h
 *
 *  Created on: 02/02/2019
 *      Author: poltergeist0
 */

#ifndef SRC_ALGORITHMLOUVAIN_H_
#define SRC_ALGORITHMLOUVAIN_H_

#include "algorithmInterface.h"

/**
 * Dynamic Communities class API.
 */
class Louvain: private AlgorithmInterface {
private:
  /**
   * Function where the actual algorithm is implemented
   */
  bool run(){
//		bool improvement=false ;
//		int nb_moves;
//		int nb_pass_done = 0;
////		long double new_qual = crtrn.quality();
//		long double new_qual = quality();
//		long double cur_qual = new_qual;
//
////		const typeNodeList nodes=crtrn.getNodes();
//		const typeNodeList nodes=getNodes();
////		std::list<Criterion::Delta> deltas;
//		// repeat while
//		//   there is an improvement of quality
//		//   or there is an improvement of quality greater than a given epsilon
//		//   or a predefined number of passes have been done
//		do {
//			//calculate selfloops
////			typeSelfLoops selfloops;
////			dcvs
//			cur_qual = new_qual;
//			nb_moves = 0;
//			nb_pass_done++;
//
//			// for each node: remove the node from its community and insert it in the best community
//			//#ifdef	//MODIFIED
//			for (typeNodeListIteratorConst node_tmp = nodes.cbegin() ; node_tmp != nodes.cend() ; node_tmp++) {
//				const typeNode & node = *node_tmp;
////				typeCommunity node_comm = crtrn.community(node);
//				typeCommunity node_comm = community(node);
////				typeWeight w_degree = crtrn.weighted_degree(node);
//
//				// computation of all neighbor node communities of current node
////				std::map<typeCommunity, typeWeight> nc=crtrn.neigh_comm(node);
//				std::map<typeCommunity, typeWeight> nc=neigh_comm(node);
//				// remove node from its current community
////				Delta d=crtrn.remove2(node, nc[node_comm]);
////				crtrn.remove(node, nc[node_comm]);
//
//				// compute the nearest community for node
//				// default choice for future insertion is the former community
//				typeCommunity best_comm = node_comm;
////				typeWeight best_nblinks  = 0.0L;
//				typeWeight best_increase = 0.0L;
//				//for all neighbors
//				for (std::map<typeCommunity, typeWeight>::iterator it=nc.begin() ; it!=nc.end() ; it++){
//					const std::pair<typeCommunity, typeWeight> & p=*it;
//					//calculate gain in quality by inserting the given node in the community of the neighbor
////					typeWeight increase = crtrn.gain(node, p.first, p.second);
////					typeWeight increase = crtrn.gain2(p.first, p.second,d);
////					typeWeight increase=crtrn.gain3(node,p.first);
//					typeWeight increase=gain3(node,p.first);
//	//#ifdef MODIFIED
//	//				if(best_comm>neigh_pos[i]){
//	//					best_comm=neigh_pos[i];
//	//				}
//					if (increase>best_increase) {
//						best_comm = p.first;
////						best_nblinks = p.second;
//						best_increase = increase;
//					}
////					d.update(increase,p.first,p.second);
//				}
//
//				// insert node in the nearest community
////				crtrn.insert(node, best_comm, best_nblinks);
////				crtrn.insert2(d);
////				crtrn.community(node,best_comm);
//				community(node,best_comm);
//				//#ifdef MODIFIED
//				//sync back to rest of n2c every time that a node switches community
////				if(best_comm!=node_comm){
////					for(std::vector<int>::iterator it=crtrn.n2c.begin();it!=crtrn.n2c.end();it++) {
////						int &a=*it;
////						if(a==node_comm) a=best_comm;
////					}
////				}
////				crtrn.
//				//#endif	//MODIFIED
//
////				if (d.getBestCommunity()!=d.getOriginalCommunity()){
////					//sync back community change
////					crtrn.community(d.getOriginalCommunity(),d.getBestCommunity(),true);
////					nb_moves++;
////				}
//				if (best_comm!=node_comm){
//					nb_moves++;
//				}
//			}
//
////			new_qual = crtrn.quality();
//			new_qual = quality();
//
//			if (nb_moves>0)
//				improvement=true;
//
////		} while (nb_moves>0 && new_qual-cur_qual > crtrn.parameters().precision);
//		} while (nb_moves>0 && new_qual-cur_qual > parameters().precision);
//
////		crtrn.endRun();
//		endRun();
//		return improvement;
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
//  ~Louvain(){}

  /**
  * Constructor
  */
  Louvain(GraphUndirectedGroupable & graph, ProgramParameters & algorithmParameters=argumentsDefault)
    :
    	AlgorithmInterface(graph,algorithmParameters)
  {
  }

  /**
  * Function to add and remove edges from the graph.
  * Any edge with a weight different from zero is inserted.
  * Any edge with a weight of exactly zero is removed.
  * @return true if adding/removing succeeded
  */
  bool addRemoveEdge(int source, int destination, double weight){
//		bool b=Criterion::addEdge(source,destination,weight,replace);
//		if(b){
//			const typeCommunity & c1=community(source);
//			const typeCommunity & c2=community(destination);
//			if(c1!=c2){//disband both communities
//				disband(c1,c2);
////				disband(c2);
//			}
//		}
//		return b;
	  return true;
  }

};

#endif /* SRC_ALGORITHMLOUVAIN_H_ */
