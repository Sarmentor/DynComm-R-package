/*
 * debugUtilities.h
 *
 *  Created on: 03/08/2019
 *      Author: poltergeist0
 */

#ifndef DEBUGUTILITIES_H_
#define DEBUGUTILITIES_H_

#include "../defines.h"

DEBUG_LEVEL fromInt(const int & i){
	switch (i) {
		case 0:
			return DEBUG_LEVEL::NONE;
			break;
		case 100:
			return DEBUG_LEVEL::TRACE;
			break;
		case 200:
			return DEBUG_LEVEL::CALLS;
			break;
		case 300:
			return DEBUG_LEVEL::MODIFICATIONS;
			break;
		case 400:
			return DEBUG_LEVEL::ACTIONS;
			break;
		case 5000:
			return DEBUG_LEVEL::VERIFY;
			break;
		case 10000:
			return DEBUG_LEVEL::ALL;
			break;
		default:
			return DEBUG_LEVEL::NONE;
			break;
	}
}

#endif /* DEBUGUTILITIES_H_ */
