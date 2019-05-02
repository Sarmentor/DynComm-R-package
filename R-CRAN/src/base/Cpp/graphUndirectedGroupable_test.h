/*
 * graphUndirected_test.h
 *
 *  Created on: 25/09/2018
 *      Author: poltergeist0
 */

#ifndef GRAPHUNDIRECTEDGROUPABLE_TEST_H_
#define GRAPHUNDIRECTEDGROUPABLE_TEST_H_

#include "graphUndirectedGroupable.h"
#include "graphUndirected_test.h"

/**
 *
 * @return true if all tests passed
 */
bool graphUndirectedGroupable_test(){
	GraphUndirectedGroupable g;
	typeWeight w12=1.1;
	typeWeight w13=1.3;
	typeWeight w23=0.2;
	typeWeight w36=0.1;
	typeWeight w46=2.1;
	typeWeight w45=2.3;
	typeWeight w47=100;
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
	g.addEdge(6,7,w67);

	ASSERT_EQUAL(g.nodeCount(),7);
	ASSERT_EQUAL(g.edgeCount(),16);
	ASSERT_EQUAL(g.maxWeight(),w67);
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

	ASSERT_EQUAL(g.innerEdges(1),0);
	ASSERT_EQUAL(g.innerEdges(2),0);
	ASSERT_EQUAL(g.innerEdges(3),0);
	ASSERT_EQUAL(g.innerEdges(4),0);
	ASSERT_EQUAL(g.innerEdges(5),0);
	ASSERT_EQUAL(g.innerEdges(6),0);
	ASSERT_EQUAL(g.innerEdges(7),0);

	ASSERT_APPROXIMATE(g.totalEdges(1),2*(w12+w13),ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.totalEdges(2),2*(w12+w23),ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.totalEdges(3),2*(w13+w23+w36),ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.totalEdges(4),2*(w46+w45),ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.totalEdges(5),2*(w45+w57),ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.totalEdges(6),2*(w36+w46+w67),ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.totalEdges(7),2*(w57+w67),ASSERT_PRECISION_DEFAULT);

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
	ASSERT_EQUAL(g.community(1),0);//no community for node 1 which was replaced with node 10
	ASSERT_EQUAL(g.community(10),10);

	ASSERT_EQUAL(g.weight(6,7),w67);

	g.removeEdge(10,2);
	ASSERT_EQUAL(g.nodeCount(),7);
	ASSERT_EQUAL(g.edgeCount(),14);
	ASSERT_EQUAL(g.maxWeight(),w67);
	ASSERT_APPROXIMATE(g.totalWeight(),2*(w13+w23+w36+w46+w45+w57+w67),ASSERT_PRECISION_DEFAULT);
	ASSERT_EQUAL(g.neighborsCount(10),1);
	ASSERT_APPROXIMATE(g.neighborsWeight(10),2*(w13),ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.weighted_degree(10),w13,ASSERT_PRECISION_DEFAULT);

	//verify communities
	ASSERT_EQUAL(g.community(1),0);//no community for node 1 which was replaced with node 10
	ASSERT_EQUAL(g.community(10),10);
	ASSERT_EQUAL(g.community(2),2);
	ASSERT_EQUAL(g.community(3),3);
	ASSERT_EQUAL(g.community(4),4);
	ASSERT_EQUAL(g.community(5),5);
	ASSERT_EQUAL(g.community(6),6);
	ASSERT_EQUAL(g.community(7),7);
	ASSERT_EQUAL(g.communityCount(),7);

	g.community(7,5);//assign node 7 to community 5
	ASSERT_EQUAL(g.community(7),5);
	ASSERT_APPROXIMATE(g.innerEdges(5),2*w57,ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.totalEdges(5),2*(w45+w57+w67),ASSERT_PRECISION_DEFAULT);
	ASSERT_EQUAL(g.neighboringCommunitiesCount(5),2);//communities 4 and 6
	ASSERT_APPROXIMATE(g.neighboringCommunitiesWeight(5),2*(w45+w67),ASSERT_PRECISION_DEFAULT);
	ASSERT_EQUAL(g.neighborsCommunityCount(5),1);//only node 7 is in same community
	ASSERT_APPROXIMATE(g.neighborsCommunityWeight(5,4),2*(w45),ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.weightCommunity(5,6),w67,ASSERT_PRECISION_DEFAULT);

	g.community(4,5);//assign node 4 to community 5
	ASSERT_EQUAL(g.community(4),4);
	ASSERT_EQUAL(g.community(5),4);
	ASSERT_EQUAL(g.community(7),4);
	ASSERT_APPROXIMATE(g.innerEdges(5),0,ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.totalEdges(5),0,ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.innerEdges(4),2*(w45+w57),ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.totalEdges(4),2*(w46+w45+w57+w67),ASSERT_PRECISION_DEFAULT);
	ASSERT_EQUAL(g.neighboringCommunitiesCount(5),0);
	ASSERT_EQUAL(g.neighboringCommunitiesCount(4),1);//community 6
	ASSERT_APPROXIMATE(g.neighboringCommunitiesWeight(4),2*(w46+w67),ASSERT_PRECISION_DEFAULT);
	ASSERT_EQUAL(g.neighborsCommunityCount(4),1);//nodes 5 and 7 are in same community but only node 5 is neighbor of node 4
	ASSERT_APPROXIMATE(g.neighborsCommunityWeight(5,4),2*(w45+w57),ASSERT_PRECISION_DEFAULT);
	typeWeight a=g.weightCommunity(4,6);
	ASSERT_APPROXIMATE(a,w46+w67,ASSERT_PRECISION_DEFAULT);

	g.community(7,7);//assign node 7 back to community 7
	ASSERT_EQUAL(g.community(7),7);
	ASSERT_EQUAL(g.community(4),4);
	ASSERT_EQUAL(g.community(5),4);
	ASSERT_APPROXIMATE(g.innerEdges(7),0,ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.totalEdges(7),2*(w57+w67),ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.innerEdges(4),2*(w45),ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.totalEdges(4),2*(w46+w45+w57),ASSERT_PRECISION_DEFAULT);
	ASSERT_EQUAL(g.neighboringCommunitiesCount(7),2);//communities 4 and 6
	ASSERT_EQUAL(g.neighboringCommunitiesCount(4),2);//communities 6 and 7
	ASSERT_APPROXIMATE(g.neighboringCommunitiesWeight(7),2*(w57+w67),ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.neighboringCommunitiesWeight(4),2*(w46+w57),ASSERT_PRECISION_DEFAULT);
	ASSERT_EQUAL(g.neighborsCommunityCount(7),0);
	ASSERT_EQUAL(g.neighborsCommunityCount(4),1);//node 5
	ASSERT_APPROXIMATE(g.neighborsCommunityWeight(7,4),2*(w57),ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.weightCommunity(7,6),w67,ASSERT_PRECISION_DEFAULT);

	g.removeEdge(5,7);//remove inter community edge
	ASSERT_EQUAL(g.nodeCount(),7);
	ASSERT_EQUAL(g.edgeCount(),12);
	ASSERT_EQUAL(g.community(7),7);
	ASSERT_EQUAL(g.community(5),4);
	ASSERT_APPROXIMATE(g.innerEdges(7),0,ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.totalEdges(7),2*(w67),ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.innerEdges(4),2*(w45),ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.totalEdges(4),2*(w46+w45),ASSERT_PRECISION_DEFAULT);
	ASSERT_EQUAL(g.neighboringCommunitiesCount(7),1);//community 6
	ASSERT_EQUAL(g.neighboringCommunitiesCount(4),1);//community 6
	ASSERT_APPROXIMATE(g.neighboringCommunitiesWeight(7),2*(w67),ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.neighboringCommunitiesWeight(4),2*(w46),ASSERT_PRECISION_DEFAULT);
	ASSERT_EQUAL(g.neighborsCommunityCount(7),0);
	ASSERT_EQUAL(g.neighborsCommunityCount(4),1);//node 5
	ASSERT_APPROXIMATE(g.neighborsCommunityWeight(7,4),0,ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.weightCommunity(7,6),w67,ASSERT_PRECISION_DEFAULT);

	g.removeEdge(5,4);//remove inner community edge
	ASSERT_EQUAL(g.nodeCount(),6);
	ASSERT_EQUAL(g.edgeCount(),10);
	ASSERT_EQUAL(g.community(4),4);
	ASSERT_EQUAL(g.community(5),0);//no community. Last edge connecting to node 5 was removed so node was removed
	ASSERT_APPROXIMATE(g.innerEdges(5),0,ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.totalEdges(5),0,ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.innerEdges(4),0,ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.totalEdges(4),2*(w46),ASSERT_PRECISION_DEFAULT);
	ASSERT_EQUAL(g.neighboringCommunitiesCount(5),0);
	ASSERT_EQUAL(g.neighboringCommunitiesCount(4),1);//community 6
	ASSERT_APPROXIMATE(g.neighboringCommunitiesWeight(5),0,ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.neighboringCommunitiesWeight(4),2*(w46),ASSERT_PRECISION_DEFAULT);
	ASSERT_EQUAL(g.neighborsCommunityCount(5),0);
	ASSERT_EQUAL(g.neighborsCommunityCount(4),0);
	ASSERT_APPROXIMATE(g.neighborsCommunityWeight(5,4),0,ASSERT_PRECISION_DEFAULT);
//	ASSERT_APPROXIMATE(g.weightCommunity(7,6),w67);

	g.addEdge(4,7,w47);
	ASSERT_EQUAL(g.nodeCount(),6);
	ASSERT_EQUAL(g.edgeCount(),12);
	ASSERT_EQUAL(g.maxWeight(),w47);
	ASSERT_EQUAL(g.community(7),7);
	ASSERT_EQUAL(g.community(4),4);
	ASSERT_APPROXIMATE(g.innerEdges(7),0,ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.totalEdges(7),2*(w47+w67),ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.innerEdges(4),0,ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.totalEdges(4),2*(w46+w47),ASSERT_PRECISION_DEFAULT);
	ASSERT_EQUAL(g.neighboringCommunitiesCount(7),2);//communities 4 and 6
	ASSERT_EQUAL(g.neighboringCommunitiesCount(4),2);//communities 6 and 7
	ASSERT_APPROXIMATE(g.neighboringCommunitiesWeight(7),2*(w47+w67),ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.neighboringCommunitiesWeight(4),2*(w46+w47),ASSERT_PRECISION_DEFAULT);
	ASSERT_EQUAL(g.neighborsCommunityCount(7),0);
	ASSERT_EQUAL(g.neighborsCommunityCount(4),0);
	ASSERT_APPROXIMATE(g.neighborsCommunityWeight(7,4),2*(w47),ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.weightCommunity(7,6),w67,ASSERT_PRECISION_DEFAULT);

	g.addEdge(4,5,w45);
	ASSERT_EQUAL(g.nodeCount(),7);
	ASSERT_EQUAL(g.edgeCount(),14);

	g.community(5,4);
	g.disband(4);//disband community 4 (has nodes 4 and 5)
	ASSERT_EQUAL(g.community(4),4);
	ASSERT_EQUAL(g.community(5),5);
	ASSERT_APPROXIMATE(g.innerEdges(5),0,ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.totalEdges(5),2*(w45),ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.innerEdges(4),0,ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.totalEdges(4),2*(w46+w45+w47),ASSERT_PRECISION_DEFAULT);
	ASSERT_EQUAL(g.neighboringCommunitiesCount(5),1);
	ASSERT_EQUAL(g.neighboringCommunitiesCount(4),3);
	ASSERT_APPROXIMATE(g.neighboringCommunitiesWeight(4),2*(w46+w45+w47),ASSERT_PRECISION_DEFAULT);
	ASSERT_EQUAL(g.neighborsCommunityCount(4),0);
	ASSERT_APPROXIMATE(g.neighborsCommunityWeight(5,4),2*(w45),ASSERT_PRECISION_DEFAULT);
	ASSERT_APPROXIMATE(g.weightCommunity(4,6),w46,ASSERT_PRECISION_DEFAULT);

	return true;
}



#endif /* GRAPHUNDIRECTEDGROUPABLE_TEST_H_ */
