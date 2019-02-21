/*
 * quality.h
 *
 *  Created on: 19/08/2018
 *      Author: poltergeist0
 */

#ifndef QUALITY_H_
#define QUALITY_H_

#include "qualityModularity.h"
#include "qualityBalMod.h"

/**
 * Quality dispatcher class
 */
class Quality: public QualityInterface{
public:

	enum class QUALITY:unsigned int {MODULARITY=1,BALMOD};

private:

	const QUALITY q;
	Modularity mod;
	BalMod bal;

public:

	Quality(GraphUndirectedGroupable & graph,const QUALITY & qual, const ProgramParameters & parameters=argumentsDefault)
	:
	q(qual)
	,mod(graph,parameters)
	,bal(graph,parameters)
	{}

	~Quality(){}

	typeQuality gain(const typeNode & node,const typeCommunity & comm)const{
		switch(q){
		default:
		case QUALITY::MODULARITY: return mod.gain(node,comm);
		case QUALITY::BALMOD: return bal.gain(node,comm);
		}
	}

	typeQuality quality()const{
		switch(q){
		default:
		case QUALITY::MODULARITY: return mod.quality();
		case QUALITY::BALMOD: return bal.quality();
		}
	}

	/**
	 *
	 * @return the chosen quality formula
	 */
	QUALITY type()const{return q;}
};

#endif /* QUALITY_H_ */
