/*
 * DynComm.h
 *
 *  Created on: 02/02/2019
 *      Author: poltergeist0
 */

#ifndef SRC_DYNCOMMBASE_H_
#define SRC_DYNCOMMBASE_H_

#include "DynCommBaseInterface.h"
#include "algorithm.h"
#include "quality.h"

/**
 * Dynamic Communities class API.
 * Simple class that only dispatches function calls to the correct algorithm.
 */
class DynCommBase:private DynCommBaseInterface{
private:
	Algorithm algrthm;
	GraphUndirectedGroupable grph;//graph with edges
	Quality qlt;
	ProgramParameters prmtrs;//algorithm parameters

public:
	/**
	 * Default constructor not acceptable.
	 * Must be passed at least the chosen algorithm and the graph
	 */
	DynCommBase()=delete;

	/**
	 * Constructor
	 */
	DynCommBase(
			ReaderInterface<Edge> * reader
			, Algorithm::ALGORITHM algorithm=Algorithm::ALGORITHM::LOUVAIN
			,const Quality::QUALITY & quality=Quality::QUALITY::MODULARITY
			, ProgramParameters & algorithmParameters=argumentsDefault
	)
	:
		algrthm(grph,reader,qlt,algorithm,algorithmParameters)
		,qlt(grph,quality,algorithmParameters)
		,prmtrs(algorithmParameters)
	{
		//TODO validate errors
		addRemoveEdges(reader);
	}

	/**
	 * Function to add and remove edges from the graph.
	 * Any edge with a weight different from zero is inserted.
	 * Any edge with a weight of exactly zero is removed.
	 * @return true if adding/removing succeeded
	 */
	bool addRemoveEdges(ReaderInterface<Edge> * reader){
		return algrthm.addRemoveEdges(reader);
	}

	/**
	 * @return the current quality measure of the community mapping on the graph
	 */
	typeQuality quality(){
		return qlt.quality();
	}

	/**
	 * @return the number of communities
	 */
	int communityCount(){
		return grph.communityCount();
	}

	/**
	 * @return a list of all communities
	 */
	typeCommunities communities(){
		return grph.communities();
	}

	typeWeight communityInnerEdgesWeight(int community){return grph.innerEdges(community);}

	//		int communityInnerEdgesCount(int community){return grph.i

	typeWeight communityTotalWeight(int community){return grph.totalEdges(community);}

	//		int communityTotalEdgesCount(int community){

	int communityNodeCount(int community){return grph.community(community);}

	/**
	 * @return a list of all nodes belonging to the given community
	 */
	typeNodeList nodes(int community){
		typeNodeList lst;
		typeCommunityListRange r=grph.nodes(community);
		for(typeCommunityListRangeIteratorConst it=r.first; it!=r.second; ++it){
			typeCommunityListRangePair p=*it;
			lst.insert(p.second);
		}
		return lst;
	}

	/**
	 * Get a snapshot of the current community mapping
	 * The differential parameter will probably be moved inside the writer as a parameter
	 * @return true if the operation succeeded
	 */
	bool results(WriterInterface * writer,bool differential=true){
		return true;
	}

};

#endif /* SRC_DYNCOMMBASE_H_ */
