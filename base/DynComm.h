/*
 * DynComm.h
 *
 *  Created on: 02/02/2019
 *      Author: poltergeist0
 */

#ifndef SRC_DYNCOMM_H_
#define SRC_DYNCOMM_H_

//#include "defines.h"
#include "DynCommBaseInterface.h"
//#include "graphUndirectedGroupable.h"

/**
 * Dynamic Communities class API.
 * Simple class that only dispatches function calls to the correct algorithm.
 */
class DynCommBase:private DynCommBaseInterface{
public:
	/**
	 * Enumeration with the list of supported Dynamic Communities algorithms.
	 * This enumeration must start at 1 since R indexing (for arrays, etc) starts at 1 instead of 0.
	 * Otherwise C++ would assign the first algorithm a 0.
	 */
	enum class ALGORITHM:unsigned int{LOUVAIN=1};

private:
//	GraphUndirectedGroupable grph;//graph with edges
	ALGORITHM algrthm=ALGORITHM::LOUVAIN;//algorithm selection
//	Louvain algorithmLouvain;
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
	template<typename T , typename std::enable_if<std::is_base_of<ReaderInterface, T>::value>::type* = nullptr>
	DynCommBase(
			T & reader, ALGORITHM algorithm=ALGORITHM::LOUVAIN, ProgramParameters & algorithmParameters=argumentsDefault
	)
	:
		algrthm(algorithm)
	//	,algorithmLouvain(graph,algorithmParameters)
	,prmtrs(algorithmParameters)
	{
		//load graph from file
		ReaderInterface::NEXTTYPE n=reader.hasNext();
		while(n!=ReaderInterface::NEXTTYPE::ERROR && n!=ReaderInterface::NEXTTYPE::ENDOFFILE){
			if(n==ReaderInterface::NEXTTYPE::VALUE){
				long src=std::stoi(reader.next());
				long dest=std::stoi(reader.next());
				double weight=std::stod(reader.next());
//				Criterion::addEdge(src,dest,weight);//add/remove edge here
			}
			if(n==ReaderInterface::NEXTTYPE::NEWLINE){
				reader.next();//skip new line
			}
			n=reader.hasNext();
		}
	}

	/**
	 * Function to add and remove edges from the graph.
	 * Any edge with a weight different from zero is inserted.
	 * Any edge with a weight of exactly zero is removed.
	 * @return true if adding/removing succeeded
	 */
	template<typename T , typename std::enable_if<std::is_base_of<ReaderInterface, T>::value>::type* = nullptr>
	bool addRemoveEdges(T & reader){
		switch(algrthm){
		default:
		case ALGORITHM::LOUVAIN:
//			return louvain.addRemoveEdges(reader);
//		case
		}
		return false;
	}


	/**
	 * @return the current quality measure of the community mapping on the graph
	 */
	double quality(){
		switch(algrthm){
		default:
		case ALGORITHM::LOUVAIN:
//			return louvain.quality();
//		case
		}
		return 0;
	}

	/**
	 * @return the number of communities
	 */
	int communityCount(){
		switch(algrthm){
		default:
		case ALGORITHM::LOUVAIN:
//			return louvain.communityCount();
//		case
		}
		return 0;
	}

	/**
	 * @return a list of all communities
	 */
	std::list<long> communities(){
		switch(algrthm){
		default:
		case ALGORITHM::LOUVAIN:
//			return louvain.communities();
//		case
		}
		return std::list<long>();
	}

	/**
	 * @return a list with information about the selected community. The information provided is:
	 *  - total weight of inner edges
	 *  - total weight of outer edges
	 *  - total weight of edges
	 *  - number of nodes in community
	 */
	std::list<long> community(int community){
		switch(algrthm){
		default:
		case ALGORITHM::LOUVAIN:
//			return louvain.community(community);
//		case
		}
		return std::list<long>();
	}

	/**
	 * @return a list of all nodes belonging to the given community
	 */
	std::list<long> nodes(int community){
		switch(algrthm){
		default:
		case ALGORITHM::LOUVAIN:
//			return louvain.nodes(community);
//		case
		}
		return std::list<long>();
	}

	/**
	 * Get a snapshot of the current community mapping
	 * The differential parameter will probably be moved inside the writer as a parameter
	 * @return true if the operation succeeded
	 */
	template<typename T , typename std::enable_if<std::is_base_of<WriterInterface, T>::value>::type* = nullptr>
	bool results(T & writer,bool differential=true){
		switch(algrthm){
		default:
		case ALGORITHM::LOUVAIN:
//			return louvain.results(writer);
//		case
		}
		return false;
	}

};

#endif /* SRC_DYNCOMM_H_ */
