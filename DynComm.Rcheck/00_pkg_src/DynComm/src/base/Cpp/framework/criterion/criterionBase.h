/************************************************************************
 ************************* Developer Notice *****************************
 ************************************************************************
 * @details
 *
 * Base class for criterion implemented in C++11.
 *
 * New criterion should extend the class "CriterionBase" which stores
 * references to the graph and program parameters objects.
 *
 * Copies of those objects must not be executed because the actual adding and
 * removing from the graph is performed outside the control of the criterion
 * object the developer is implementing. Criterion can never change the graph.
 *
 *
 * @author poltergeist0
 *
 * @date 2018-08-19
 ************************************************************************
 ************************************************************************
 ************************************************************************/

#ifndef CRITERIONBASE_H_
#define CRITERIONBASE_H_

#include "criterionInterface.h"
#include "../graph/graphUndirectedGroupable.h"

/**
 * @brief Base class for criterion.
 *
 * @details
 *
 * It simply stores references to the graph and program parameters objects.
 *
 * This class should be extended by all criterion. It strengths the idea that
 * those objects should not be copied and that the graph and program
 * parameters should not be modified.
 *
 *
 * @author poltergeist0
 *
 * @date 2018-08-19
 */
class CriterionBase: public CriterionInterface{
protected:
	const GraphUndirectedGroupable & g; // network of nodes to compute communities for. Can not be changed by the algorithm
	const ProgramParameters & prmtrs;//quality parameters. Can not be changed by the algorithm

public:

	/**
	 * Constructor.
	 *
	 * @param graph reference to the graph object
	 * @param parameters reference to the parameters object
	 */
	CriterionBase(
			const GraphUndirectedGroupable & graph
			, const ProgramParameters & parameters=argumentsDefault
		):
			g(graph)
			,prmtrs(parameters)
	{}

	/**
	 * Destructor
	 */
	virtual ~CriterionBase(){}

};

#endif /* CRITERIONBASE_H_ */
