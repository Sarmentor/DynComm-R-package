/*
 * algorithmInterface.h
 *
 *  Created on: 02/02/2019
 *      Author: poltergeist0
 */

#ifndef SRC_ALGORITHMBASE_H_
#define SRC_ALGORITHMBASE_H_

#include "program.h"
#include "reader.h"
#include "algorithmInterface.h"
#include "quality.h"

/**
 * Dynamic Communities class API.
 */
class AlgorithmBase:public AlgorithmInterface {
protected:
	GraphUndirectedGroupable & grph;//reference to graph with edges
	const ProgramParameters & prmtrs;//algorithm parameters. Can not be changed by the algorithm
	const Quality & qlt;//reference to quality evaluators. Can not be changed by the algorithm

public:
	/**
	 * Default constructor not acceptable.
	 * Must be passed at least the graph
	 */
	AlgorithmBase()=delete;

	/**
	 * Destructor
	 */
	virtual ~AlgorithmBase(){}

	/**
	 * Constructor
	 */
	AlgorithmBase(
			GraphUndirectedGroupable & graph
			,const Quality & quality
			,const ProgramParameters & parameters=argumentsDefault)
	:
	grph(graph)
	,prmtrs(parameters)
	,qlt(quality)
	{
	}

	const std::string toString(const StringFormater & sf=defaultStringFormater)const{
		StringFormater f=sf;
		std::stringstream ss;
		if(!sf.isDefault()){
			f.build(ss,"");
			++f;
		}
		f.header("Quality:");
		ss << grph.toString(f);
		return ss.str();
	}
};

#endif /* SRC_ALGORITHMBASE_H_ */
