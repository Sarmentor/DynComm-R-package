/*
 * quality.h
 *
 *  Created on: 19/08/2018
 *      Author: poltergeist0
 */

#ifndef QUALITYBASE_H_
#define QUALITYBASE_H_

#include "graphUndirectedGroupable.h"
#include "qualityInterface.h"

class QualityBase: public QualityInterface{
protected:
	const GraphUndirectedGroupable & g; // network of nodes to compute communities for. Can not be changed by the algorithm
	const ProgramParameters & prmtrs;//quality parameters. Can not be changed by the algorithm
//	const std::string nm;

public:

	/**
	 * Constructor
	 * @param graph
	 * @param parameters
	 */
	QualityBase(const GraphUndirectedGroupable & graph, const ProgramParameters & parameters=argumentsDefault):g(graph),prmtrs(parameters){}

	/**
	 *
	 * @return
	 */
//	const ProgramParameters & parameters()const{return prmtrs;}

	/**
	 * Destructor
	 */
	virtual ~QualityBase(){}

};

#endif /* QUALITYBASE_H_ */
