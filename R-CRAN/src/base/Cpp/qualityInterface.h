/*
 * qualityInterface.h
 *
 *  Created on: 19/08/2018
 *      Author: poltergeist0
 */

#ifndef QUALITYINTERFACE_H_
#define QUALITYINTERFACE_H_

#include "program.h"
#include "graphUndirectedGroupable.h"

typedef long double typeQuality;

class QualityInterface{
public:
	virtual ~QualityInterface(){}

	/**
	 * compute the gain in quality by moving the given node from its current community to the given community comm
	 * @param node
	 * @param comm
	 * @return the gain in quality, which may be positive or negative
	 */
	virtual typeQuality gain(const typeVertex & node, const typeCommunity & comm)const=0;

	/**
	 * compute the quality of the current partition scheme
	 * @return the quality value
	 */
	virtual typeQuality quality()const=0;
};

#endif /* QUALITYINTERFACE_H_ */
