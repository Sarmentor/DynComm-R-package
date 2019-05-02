/*
 * mapReversable_test.h
 *
 *  Created on: 15/11/2018
 *      Author: poltergeist0
 */

#ifndef SRC_MAPREVERSABLE_TEST_H_
#define SRC_MAPREVERSABLE_TEST_H_

#include "mapReversable.h"

/**
 *
 * @return true if all tests passed
 */
bool mapReversable_test(){
	MapReversable<int,int> mr;
	mr.add(1,2);
	mr.add(1,3);//fail to add. Already exists
	mr.add(2,3);
	mr.add(2,1,true);//replace value
	mr.add(4,1);
	mr.add(5,1);
	mr.add(6,1);
	std::cout << mr.toString();

	mr.remove(2);
	std::cout << "mr.remove(2)" << endl;
	std::cout << mr.toString();

	mr.replaceKey(5,8);
	std::cout << "mr.replaceKey(5,8)" << endl;
	std::cout << mr.toString();

	mr.replace(1,9);
	std::cout << "mr.replace(1,9)" << endl;
	std::cout << mr.toString();

	return true;
}

#endif /* SRC_MAPREVERSABLE_TEST_H_ */
