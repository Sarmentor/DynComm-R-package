/************************************************************************
 ************************* Developer Notice *****************************
 ************************************************************************
 * @details
 *
 * Interface for criterion implemented in C++11.
 *
 * New criterion must implement the functions defined in the class declared in
 * this file. They should also extend the class "CriterionBase" which stores
 * references to the graph and program parameters objects.
 *
 * Copies of those objects must not be executed because the actual adding and
 * removing from the graph is performed outside the control of the criterion
 * object the developer is implementing. Criterion can never change the graph.
 *
 * Because all criterion have an object of its class instantiated, even if it
 * is not used and is never called, new criterion should have a minimal
 * memory footprint. This is implemented like so to improve performance by
 * getting rid of object pointers and, consequently, of indirection and pointer
 * resolution which would significantly slow down the program. Otherwise, every
 * cycle in the program would require many function calls from object pointers.
 *
 *
 * @author poltergeist0
 *
 * @date 2018-08-19
 ************************************************************************
 ************************************************************************
 ************************************************************************/

#ifndef CRITERIONINTERFACE_H_
#define CRITERIONINTERFACE_H_

#include "../program.h"
#include "../graph/graphUndirectedGroupable.h"

/**
 * Data type for quality
 */
typedef long double typeCriterion;

/**
 * @brief Criterion interface.
 *
 * @details
 *
 * All criterion must implement the functions defined in this class.
 *
 * Also, at the end of this class, there is a comment with the skeleton of a
 * constructor that must be defined in classes that implement this interface.
 *
 *
 * @author poltergeist0
 *
 * @date 2018-08-19
 */
class CriterionInterface{
public:
	virtual ~CriterionInterface(){}

	/**
	 * compute the quality obtained if the given vertex was moved from its current
	 * community to the given community
	 *
	 * @param vertex
	 * @param comm new potential community for given vertex
	 * @return the quality, which may be positive or negative
	 */
	virtual typeCriterion gain(const typeVertex & vertex, const typeCommunity & comm)const=0;

	/**
	 * compute the quality of the current partition scheme
	 *
	 * @return the quality value
	 */
	virtual typeCriterion quality()const=0;

	/*
	 * Constructor.
	 *
	 * Must be implemented with the skeleton provided here, receiving the same
	 * arguments without copying.
	 *
	 * @param graph reference to the graph object
	 * @param parameters reference to the parameters object
	 */
//	CriterionInterface(
//			const GraphUndirectedGroupable & graph
//			, const ProgramParameters & parameters=argumentsDefault
//		):
//			CriterionBase(graph,parameters)
//	{}

};

#endif /* CRITERIONINTERFACE_H_ */
