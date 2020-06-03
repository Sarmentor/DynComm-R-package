/************************************************************************
 ************************* Developer Notice *****************************
 ************************************************************************
 * @details
 *
 * Base class for main algorithms implemented in C++11.
 *
 * New algorithms should extend the class "AlgorithmBase" which stores
 * references to the graph, quality and program parameters objects.
 *
 * Copies of those objects must not be executed because the actual adding and
 * removing from the graph is performed outside the control of the algorithm
 * object. For that same reason, algorithms should not change the graph unless
 * required.
 *
 *
 * @author poltergeist0
 *
 * @date 2019-01-01
 ************************************************************************
 ************************************************************************
 ************************************************************************/

#ifndef SRC_ALGORITHMBASE_H_
#define SRC_ALGORITHMBASE_H_

#include "../program.h"
#include "../io/reader.h"
#include "algorithmInterface.h"
#include "../criterion/criterion.h"

/**
 * @brief Base class for algorithms.
 *
 * @details
 *
 * It simply stores references to the graph, quality and program parameters
 * objects.
 *
 * This class should be extended by all algorithms. It strengths the idea that
 * those objects should not be copied and that the quality and program
 * parameters should not be modified.
 *
 *
 * @author poltergeist0
 *
 * @date 2019-01-01
 */
class AlgorithmBase:public AlgorithmInterface {
protected:
	GraphUndirectedGroupable & grph;//reference to graph with edges
	const ProgramParameters & prmtrs;//algorithm parameters. Can not be changed by the algorithm
	const Criterion & qlt;//reference to quality evaluators. Can not be changed by the algorithm

public:
	/**
	 * Default constructor not acceptable.
	 * Must be passed at least the graph, quality and parameters.
	 */
	AlgorithmBase()=delete;

	/**
	 * Destructor
	 */
	virtual ~AlgorithmBase(){}

	/**
	 * Constructor.
	 *
	 * @param graph reference to the graph object
	 * @param quality reference to the quality object
	 * @param parameters reference to the parameters object
	 */
	AlgorithmBase(
			GraphUndirectedGroupable & graph
			,const Criterion & quality
			,const ProgramParameters & parameters=argumentsDefault)
	:
	grph(graph)
	,prmtrs(parameters)
	,qlt(quality)
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
		f.header("Quality:");
		ss << grph.toString(f);
		return ss.str();
	}

	const std::string debugPrint()const{
		std::stringstream ss;
		ss << grph.debugPrint()<< "\n";
		return ss.str();
	}

};

#endif /* SRC_ALGORITHMBASE_H_ */
