/*
 * modularity.h
 *
 *  Created on: 20/08/2018
 *      Author: poltergeist0
 */

#ifndef QUALITYMODULARITY_H_
#define QUALITYMODULARITY_H_

#include "qualityBase.h"

class Modularity: public QualityBase{
public:

	/**
	 * Constructor
	 * @param graph
	 * @param parameters
	 */
	Modularity(GraphUndirectedGroupable & graph,const ProgramParameters & parameters=argumentsDefault):QualityBase(graph,parameters) {}

	/**
	 * Destructor
	 */
	~Modularity(){}


	typeQuality gain(const typeVertex & node,const typeCommunity & comm)const{
		//TODO review the formulas
		const typeWeight & w_degree=g.weighted_degree(node);
		const typeCommunity & oc=g.community(node);
//		std::map<typeCommunity,typeWeight>::const_iterator it=tot.find(comm);
//		typeWeight totc=it->second;
		typeWeight totc = g.totalEdges(comm);
		if(comm==oc){
//			std::map<typeCommunity,typeWeight>::const_iterator ito=tot.find(dlt.getOriginalCommunity());
//			const typeWeight toto = ito->second;
//			totc=toto-dlt.getOriginalTotalEdges();
//			totc=totc-dlt.getOriginalTotalEdges();
			totc=totc-g.weighted_degree(node);
		}
//		else{
//			std::map<typeCommunity,typeWeight>::const_iterator it=tot.find(comm);
//			totc=it->second;
//		}
		const typeWeight & m2   = g.totalWeight();
//		CERR << "gain: node=" << node << "; comm=" << comm << "; dnodecomm=" << g.neighborsCommunityWeight(node,comm) << "; w_degree=" << w_degree << "; tot[comm]=" << totc << "; m2=" << m2 << "; gain=" << (g.neighborsCommunityWeight(node,comm) - totc*w_degree/m2) << "\n";
		return (g.neighborsCommunityWeight(node,comm) - totc*w_degree/m2);
	}

	typeQuality quality()const{
		//TODO review the formulas
		typeWeight q  = 0.0L;
		const typeWeight & m2 = g.totalWeight();
		for (typeVertexListIterator it=g.getVertices().begin() ; it!=g.getVertices().end() ; it++){
//		for (typeCommunities::const_iterator it=g.communities().cbegin() ; it!=g.communities().cend() ; ++it){
			const typeVertex & i=*it;
//			std::map<typeCommunity,typeWeight>::const_iterator itt=in.find(i);
//			const typeWeight & a = itt->second;
			const typeWeight & a = g.innerEdges(i);
//			itt=tot.find(i);
//			const typeWeight & t = itt->second;
			const typeWeight & t = g.totalEdges(i);
//			CERR << "quality: i=" << i << "; tot[i]=" << t << "; m2=" << m2 ;
			if (t > 0.0L){
//				CERR << "; in[i]=" << a << "; previous q=" << q << "; delta q=" << a - (t*t) / m2;
				q += a - (t*t) / m2;
			}
//			CERR << "; new q=" << q << "\n";
		}
		q /= m2;
//		CERR << "quality: final q=" << q << "\n";
		return q;
	}

};

#endif /* QUALITYMODULARITY_H_ */
