/*
 * graphUndirected_test.h
 *
 *  Created on: 25/09/2018
 *      Author: poltergeist0
 */

#ifndef GRAPHUNDIRECTED_TEST_H_
#define GRAPHUNDIRECTED_TEST_H_

#include "graphUndirected.h"
#include "debugging.h"

/**
 *
 * @return true if all tests passed
 */
//template <typename T, typename std::enable_if<std::is_base_of<GraphUndirected,T>::value,T>::type=0 > bool graphUndirected_test(T & g){
bool graphUndirected_test(){
	GraphUndirected g;
	typeWeight w12=1.1;
	typeWeight w13=1.3;
	typeWeight w23=0.2;
	typeWeight w36=0.1;
	typeWeight w46=2.1;
	typeWeight w45=2.3;
	typeWeight w57=11;
	typeWeight w67=21;

	//set edges
	g.addEdge(1,2,w12);
	g.addEdge(1,3,w13);
	g.addEdge(2,3,w23);
	g.addEdge(3,6,w36);
	g.addEdge(4,6,w46);
	g.addEdge(4,5,w45);
	g.addEdge(5,7,w57);

	ASSERT_EQUAL(g.nodeCount(),7);
	ASSERT_EQUAL(g.edgeCount(),14);
	ASSERT_EQUAL(g.maxWeight(),w57);
	ASSERT_APPROXIMATE(g.totalWeight(),2*(w12+w13+w23+w36+w46+w45+w57),ASSERT_PRECISION_DEFAULT);
	ASSERT_EQUAL(g.neighborsCount(6),2);
	ASSERT_APPROXIMATE(g.neighborsWeight(6),2*(w36+w46),ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.weighted_degree(6),(w36+w46),ASSERT_PRECISION_DEFAULT);

	g.addEdge(6,7,w67);
	ASSERT_EQUAL(g.nodeCount(),7);
	ASSERT_EQUAL(g.edgeCount(),16);
	ASSERT_EQUAL(g.maxWeight(),w67);
	ASSERT_APPROXIMATE(g.totalWeight(),2*(w12+w13+w23+w36+w46+w45+w57+w67),ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.totalWeight(),2*(w12+w13+w23+w36+w46+w45+w57+w67),ASSERT_PRECISION_DEFAULT);
	ASSERT_EQUAL(g.neighborsCount(1),2);
	ASSERT_APPROXIMATE(g.neighborsWeight(1),2*(w12+w13),ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.weighted_degree(1),(w12+w13),ASSERT_PRECISION_DEFAULT);
	ASSERT_EQUAL(g.neighborsCount(2),2);
	ASSERT_APPROXIMATE(g.neighborsWeight(2),2*(w12+w23),ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.weighted_degree(2),(w12+w23),ASSERT_PRECISION_DEFAULT);
	ASSERT_EQUAL(g.neighborsCount(3),3);
	ASSERT_APPROXIMATE(g.neighborsWeight(3),2*(w13+w23+w36),ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.weighted_degree(3),(w13+w23+w36),ASSERT_PRECISION_DEFAULT);
	ASSERT_EQUAL(g.neighborsCount(4),2);
	ASSERT_APPROXIMATE(g.neighborsWeight(4),2*(w46+w45),ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.weighted_degree(4),(w46+w45),ASSERT_PRECISION_DEFAULT);
	ASSERT_EQUAL(g.neighborsCount(5),2);
	ASSERT_APPROXIMATE(g.neighborsWeight(5),2*(w45+w57),ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.weighted_degree(5),(w45+w57),ASSERT_PRECISION_DEFAULT);
	ASSERT_EQUAL(g.neighborsCount(6),3);
	ASSERT_APPROXIMATE(g.neighborsWeight(6),2*(w36+w46+w67),ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.weighted_degree(6),(w36+w46+w67),ASSERT_PRECISION_DEFAULT);
	ASSERT_EQUAL(g.neighborsCount(7),2);
	ASSERT_APPROXIMATE(g.neighborsWeight(7),2*(w57+w67),ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.weighted_degree(7),(w57+w67),ASSERT_PRECISION_DEFAULT);

	ASSERT_EQUAL(g.nodeCount(),7);
	ASSERT_EQUAL(g.edgeCount(),16);
	ASSERT_EQUAL(g.maxWeight(),w67);
	ASSERT_APPROXIMATE(g.totalWeight(),2*(w12+w13+w23+w36+w46+w45+w57+w67),ASSERT_PRECISION_DEFAULT);
	ASSERT_EQUAL(g.neighborsCount(1),2);
	ASSERT_APPROXIMATE(g.neighborsWeight(1),2*(w12+w13),ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.weighted_degree(1),(w12+w13),ASSERT_PRECISION_DEFAULT);

	g.replace(1,10);//old node is erased
	ASSERT_EQUAL(g.nodeCount(),7);
	ASSERT_EQUAL(g.edgeCount(),16);
	ASSERT_EQUAL(g.maxWeight(),w67);
	ASSERT_APPROXIMATE(g.totalWeight(),2*(w12+w13+w23+w36+w46+w45+w57+w67),ASSERT_PRECISION_DEFAULT);
	ASSERT_EQUAL(g.neighborsCount(1),0);
	ASSERT_EQUAL(g.neighborsWeight(1),0);
	ASSERT_EQUAL(g.weighted_degree(1),0);
	ASSERT_EQUAL(g.neighborsCount(10),2);
	ASSERT_APPROXIMATE(g.neighborsWeight(10),2*(w12+w13),ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.weighted_degree(10),(w12+w13),ASSERT_PRECISION_DEFAULT);

	ASSERT_EQUAL(g.weight(6,7),w67);

	g.removeEdge(10,2);
	ASSERT_EQUAL(g.nodeCount(),7);
	ASSERT_EQUAL(g.edgeCount(),14);
	ASSERT_EQUAL(g.maxWeight(),w67);
	ASSERT_APPROXIMATE(g.totalWeight(),2*(w13+w23+w36+w46+w45+w57+w67),ASSERT_PRECISION_DEFAULT);
	ASSERT_EQUAL(g.neighborsCount(10),1);
	ASSERT_APPROXIMATE(g.neighborsWeight(10),2*(w13),ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.weighted_degree(10),w13,ASSERT_PRECISION_DEFAULT);

	return true;
}



#endif /* GRAPHUNDIRECTED_TEST_H_ */
