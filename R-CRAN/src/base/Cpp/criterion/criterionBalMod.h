/************************************************************************
 ************************* Developer Notice *****************************
 ************************************************************************
 * @details
 *
 * Balanced Modularity criterion implemented in C++11.
 *
 *
 * @author poltergeist0
 *
 * @date 2018-08-20
 ************************************************************************
 ************************************************************************
 ************************************************************************/

#ifndef CRITERIONBALMOD_H_
#define CRITERIONBALMOD_H_

#include "../framework/criterion/criterionBase.h"

/**
 * @brief Balanced Modularity criterion
 *
 *
 * @author poltergeist0
 *
 * @date 2018-08-20
 */
class CriterionBalMod: public CriterionBase{
public:

	/**
	 * Constructor.
	 *
	 *
	 * @param graph reference to the graph object
	 * @param parameters reference to the parameters object
	 */
	CriterionBalMod(const GraphUndirectedGroupable & graph,const ProgramParameters & parameters=argumentsDefault):CriterionBase(graph,parameters) {}

	/**
	 * Destructor
	 */
	~CriterionBalMod(){}

	/**
	 * compute the quality obtained if the given vertex was moved from its current
	 * community to the given community
	 *
	 * @param vertex
	 * @param comm new potential community for given vertex
	 * @return the quality, which may be positive or negative
	 */
	typeCriterion gain(const typeVertex & vertex, const typeCommunity & comm)const{
		const typeWeight & w_degree=g.weighted_degree(vertex);
		const typeCommunity & oc=g.community(vertex);
		typeWeight totc = g.totalEdges(comm);
		if(comm==oc){
			totc=totc-g.weighted_degree(vertex);
		}
		const typeWeight & m2   = g.totalWeight();
		return (g.neighboursCommunityWeight(vertex,comm) - totc*w_degree/m2);
	}

	/**
	 * compute the quality of the current partition scheme
	 *
	 * @return the quality value
	 */
	typeCriterion quality()const{
		typeWeight q  = 0.0L;
		const typeWeight & m2 = g.totalWeight();
		for (typeVertexListIterator it=g.getVertices().begin() ; it!=g.getVertices().end() ; it++){
			const typeVertex & i=*it;
			const typeWeight & a = g.innerEdges(i);
			const typeWeight & t = g.totalEdges(i);
			if (t > 0.0L) q += a - (t*t) / m2;
		}
		q /= m2;
		return q;
	}
};

#endif /* CRITERIONBALMOD_H_ */
