/************************************************************************
 ************************* Developer Notice *****************************
 ************************************************************************
 * @details
 *
 * Aggregator class for all criterion implemented in C++11.
 *
 * All criterion must be instantiated in the class defined in this file.
 *
 * When adding new criterion, go through this file and implement modifications
 * as instructed by the markers (multi-line comments with dual row asterisks
 * before and after some text that starts with the word "TODO").
 *
 *
 * @author poltergeist0
 *
 * @date 2018-08-19
 ************************************************************************
 ************************************************************************
 ************************************************************************/

#ifndef CRITERION_H_
#define CRITERION_H_

#include "../../criterion/criterionBalMod.h"
#include "../../criterion/criterionModularity.h"
/* **********************************************************************
 ************************************************************************
 * TODO: Add criterion include file here
 ************************************************************************
 ************************************************************************/

/**
 * @brief Aggregator class for all criterion.
 *
 * @details
 *
 * It is a simple class that only instantiates all criterion and dispatches
 * function calls to the correct one.
 *
 *
 * @author poltergeist0
 *
 * @date 2018-08-19
 */
class Criterion: public CriterionInterface{
public:
/* **********************************************************************
 ************************************************************************
 * TODO: Add criterion name to enumeration. Use the same name used in R
 ************************************************************************
 ************************************************************************/
	/**
	 * Enumeration with the list of supported criterion for algorithms
	 * implemented in C++.
	 * This enumeration must start at 1 since this code is used in R and R
	 * indexing (for arrays, etc) starts at 1 instead of 0.
	 * Otherwise C++ would assign the first criterion a 0.
	 */
	enum class CRITERION:unsigned int {MODULARITY=1,BALMOD};

private:

	/*
	 * criterion selection. Can not be changed after initialization
	 */
	const CRITERION crtrn;

	/*
	 * criterion variables
	 */
	CriterionModularity criterionModularity;
	CriterionBalMod criterionBalMod;
/* **********************************************************************
 ************************************************************************
 * TODO: Instantiate criterion here. Give it a identical name to the class
 * but with lower case first letter
 ************************************************************************
 ************************************************************************/

public:

	/**
	 * Constructor
	 *
	 * @param graph
	 * @param criterion
	 * @param parameters
	 */
	Criterion(
			const GraphUndirectedGroupable & graph
			,const CRITERION & criterion
			, const ProgramParameters & parameters=argumentsDefault
		):
			crtrn(criterion)
			,criterionModularity(graph,parameters)
			,criterionBalMod(graph,parameters)
/* **********************************************************************
 ************************************************************************
 * TODO: Initialize criterion variable here
 ************************************************************************
 ************************************************************************/
	{}

	/**
	 * Destructor
	 */
	~Criterion(){}

	/**
	 * compute the quality obtained if the given vertex was moved from its current
	 * community to the given community
	 *
	 * @param vertex
	 * @param comm new potential community for given vertex
	 * @return the quality, which may be positive or negative
	 */
	typeCriterion gain(const typeVertex & vertex,const typeCommunity & comm)const{
		switch(crtrn){
		default:
		case CRITERION::MODULARITY: return criterionModularity.gain(vertex,comm);
		case CRITERION::BALMOD: return criterionBalMod.gain(vertex,comm);
/* **********************************************************************
 ************************************************************************
 * TODO: Add criterion call to gain
 ************************************************************************
 ************************************************************************/
		}
	}

	/**
	 * compute the quality of the current partition scheme
	 *
	 * @return the quality value
	 */
	typeCriterion quality()const{
		switch(crtrn){
		default:
		case CRITERION::MODULARITY: return criterionModularity.quality();
		case CRITERION::BALMOD: return criterionBalMod.quality();
/* **********************************************************************
 ************************************************************************
 * TODO: Add criterion call to quality
 ************************************************************************
 ************************************************************************/
		}
	}

	/**
	 *
	 * @return the chosen criterion
	 */
	CRITERION type()const{return crtrn;}
};

#endif /* CRITERION_H_ */
