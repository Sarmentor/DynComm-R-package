/*
 * DynComm.h
 *
 *  Created on: 02/02/2019
 *      Author: poltergeist0
 */

#ifndef SRC_DYNCOMMBASE_H_
#define SRC_DYNCOMMBASE_H_

#include "defines.h"
#include "DynCommBaseInterface.h"
#include "algorithm.h"
#include "quality.h"
#include "timeFunctions.h"

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

	/**
	 * total accumulated time used for processing
	 */
	uint64 timeTotal=0;

	/**
	 * auxiliary time used to store the start time
	 */
	uint64 timeStart=0;

	/**
	 * total time used for processing
	 */
	uint64 timeProcessing=0;

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
	,timeTotal(0)
	,timeStart(Time::currentTime())
	,timeProcessing(0)
	{
	}

	/**
	 * Function to add and remove edges from the graph.
	 * Any edge with a weight different from zero is inserted.
	 * Any edge with a weight of exactly zero is removed.
	 * @return true if adding/removing succeeded
	 */
	bool addRemoveEdges(ReaderInterface<Edge> * reader){
		timeStart=Time::currentTime();
		bool b=algrthm.addRemoveEdges(reader);
		uint64 tm=Time::currentTime();
		timeProcessing=tm-timeStart;
		timeTotal+=timeProcessing;
		return b;
	}

	/**
	 * @return the current quality measure of the community mapping on the graph
	 */
	typeQuality quality()const {
		return qlt.quality();
	}

	/**
	 * @return the number of communities
	 */
	int communityCount()const {
		return grph.communityCount();
	}

	/**
	 * @return a list of all communities
	 */
	typeCommunities communities()const {
		return grph.communities();
	}

	/**
	 *
	 * @param community
	 * @return
	 */
	typeLinksRangeConst communityNeighbours(typeCommunity community)const {
		return grph.neighboringCommunities(community);
	}

	typeWeight communityInnerEdgesWeight(typeCommunity community)const {return grph.innerEdges(community);}

	//		int communityInnerEdgesCount(int community){return grph.i

	typeWeight communityTotalWeight(typeCommunity community)const {return grph.totalEdges(community);}

	//		int communityTotalEdgesCount(int community){

	typeWeight communityEdgeWeight(typeCommunity source,typeCommunity destination)const{
		return grph.weightCommunity(source, destination);
	}

	int communityVertexCount(typeCommunity community)const {
		unsigned int cnt=0;
		typeCommunityListRange r=grph.vertices(community);
		for(typeCommunityListRangeIteratorConst it=r.first; it!=r.second; ++it){
			++cnt;
		}
		return cnt;
	}

	typeCommunity community(typeVertex vertex)const{
		return grph.community(vertex);
	}

	unsigned int vertexCount()const{
		return grph.vertexCount();
	}

	typeVertexList vertices()const{
		return grph.getVertices();
	}

	/**
	 * @return a list of all vertices belonging to the given community
	 */
	typeVertexList vertices(typeCommunity community)const {
		typeVertexList lst;
		typeCommunityListRange r=grph.vertices(community);
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
	bool communityMapping(WriterInterface * writer,bool differential=true) const{
		const typeCommunities gc=grph.communities();
		for(typeCommunities::const_iterator it=gc.cbegin();it!=gc.cend();++it){
			const typeCommunity c=*it;
			writer->write(std::to_string(c),WriterInterface::WRITETYPE::VALUE);
			typeVertexList n=vertices(c);
			unsigned int i=1;
			for(typeVertexListIteratorConst itn=n.cbegin();itn!=n.cend();++itn){
				if(i==n.size()){//last value
					writer->write(std::to_string(*itn),WriterInterface::WRITETYPE::LINE);
				}
				else{
					writer->write(std::to_string(*itn),WriterInterface::WRITETYPE::VALUE);
				}
				++i;
			}
		}
		return true;
	}

	typeLinksRangeConst neighbours(typeVertex vertex)const {
		return grph.neighbors(vertex);
	}

	typeWeight weight(const typeVertex & source, const typeVertex & destination) const {
		return grph.weight(source,destination);
	}

	/**
	 *
	 * @return the total processing time in nanoseconds
	 */
	uint64 time(bool accumulated=true)const{
	  if(!accumulated) return timeProcessing;
	  return timeTotal;
	}

};

#endif /* SRC_DYNCOMMBASE_H_ */
