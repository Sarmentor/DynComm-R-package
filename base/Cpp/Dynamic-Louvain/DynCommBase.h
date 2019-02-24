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
	 * total time used processing
	 */
	uint64 timeTotal=0;

	/**
	 * auxiliary time used to store the start time
	 */
	uint64 timeStart=0;

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
	{
		//TODO validate errors
		if(!addRemoveEdges(reader)){//check for reader errors
			CERR << reader->status();
		}
		timeTotal+=Time::currentTime()-timeStart;
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
		timeTotal+=Time::currentTime()-timeStart;
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

	typeWeight communityInnerEdgesWeight(int community)const {return grph.innerEdges(community);}

	//		int communityInnerEdgesCount(int community){return grph.i

	typeWeight communityTotalWeight(int community)const {return grph.totalEdges(community);}

	//		int communityTotalEdgesCount(int community){

	typeWeight communityEdgeWeight(typeCommunity source,typeCommunity destination)const{
		return grph.weightCommunity(source, destination);
	}

	int communityNodeCount(int community)const {
		unsigned int cnt=0;
		typeCommunityListRange r=grph.nodes(community);
		for(typeCommunityListRangeIteratorConst it=r.first; it!=r.second; ++it){
			++cnt;
		}
		return cnt;
	}

	typeCommunity community(typeNode node)const{
		return grph.community(node);
	}

	unsigned int nodesCount()const{
		return grph.nodeCount();
	}

	typeNodeList nodes()const{
		return grph.getNodes();
	}

	/**
	 * @return a list of all nodes belonging to the given community
	 */
	typeNodeList nodes(int community)const {
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
	bool results(WriterInterface * writer,bool differential=true) const{
		const typeCommunities gc=grph.communities();
		for(typeCommunities::const_iterator it=gc.cbegin();it!=gc.cend();++it){
			const typeCommunity c=*it;
//			COUT << c;
			writer->write(std::to_string(c),WriterInterface::WRITETYPE::VALUE);
			typeNodeList n=nodes(c);
			unsigned int i=1;
			for(typeNodeListIteratorConst itn=n.cbegin();itn!=n.cend();++itn){
//				COUT << " " << *itn;
				if(i==n.size()){//last value
					writer->write(std::to_string(*itn),WriterInterface::WRITETYPE::LINE);
				}
				else{
					writer->write(std::to_string(*itn),WriterInterface::WRITETYPE::VALUE);
				}
				++i;
			}
//			COUT << "\n";
//			writer->write("",WriterInterface::WRITETYPE::LINE);
		}
		return true;
	}

	/**
	 *
	 * @return the total processing time in microseconds
	 */
	uint64 time()const{return timeTotal;}

};

#endif /* SRC_DYNCOMMBASE_H_ */
