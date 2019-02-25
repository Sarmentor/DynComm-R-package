/*
 * DynComm.h
 *
 *  Created on: 02/02/2019
 *      Author: poltergeist0
 */

#ifndef SRC_ALGORITHM_H_
#define SRC_ALGORITHM_H_

#include "algorithmInterface.h"
#include "algorithmLouvain.h"
#include "quality.h"

/**
 * Simple class that only dispatches function calls to the correct algorithm.
 */
class Algorithm: private AlgorithmBase{
public:
	/**
	 * Enumeration with the list of supported Dynamic Communities algorithms.
	 * This enumeration must start at 1 since R indexing (for arrays, etc) starts at 1 instead of 0.
	 * Otherwise C++ would assign the first algorithm a 0.
	 */
	enum class ALGORITHM:unsigned int{LOUVAIN=1,SHAKEN};

private:
	const ALGORITHM algrthm;//algorithm selection. Can not be changed after initialization
	Louvain algorithmLouvain;
//	Shaken algorithmShaken;

	bool addRemoveEdgePre(const typeNode & source, const typeNode & destination, const typeWeight & weight=1.0){
		switch(algrthm){
		default:
		case ALGORITHM::LOUVAIN:
			return algorithmLouvain.addRemoveEdgePre(source,destination,weight);
			break;
//		case ALGORITHM::SHAKEN:
//			return louvain.addRemoveEdges(reader);
//		case ALGORITHM::
		}
	}

	bool addRemoveEdgePost(const typeNode & source, const typeNode & destination, const typeWeight & weight=1.0){
		switch(algrthm){
		default:
		case ALGORITHM::LOUVAIN:
			return algorithmLouvain.addRemoveEdgePost(source,destination,weight);
		break;
		//		case ALGORITHM::SHAKEN:
		//			return louvain.addRemoveEdges(reader);
		//		case ALGORITHM::
		}
	}

	bool run(){
		switch(algrthm){
		default:
		case ALGORITHM::LOUVAIN:
			return algorithmLouvain.run();
			break;
		//		case ALGORITHM::SHAKEN:
		//			return louvain.addRemoveEdges(reader);
		//		case ALGORITHM::
		}
	}

public:
	/**
	 * Default constructor not acceptable.
	 * Must be passed at least the chosen algorithm and the graph
	 */
	Algorithm()=delete;

	/**
	 * Constructor
	 */
	Algorithm(
			GraphUndirectedGroupable & grph
			, ReaderInterface<Edge> * reader
			,Quality & quality
			, ALGORITHM algorithm=ALGORITHM::LOUVAIN
			, ProgramParameters & algorithmParameters=argumentsDefault
	)
	:
		AlgorithmBase(grph,quality,algorithmParameters)
		,algrthm(algorithm)
		//initialize all algorithms. Only the one passed a reader instead of a dummy reader will work
		,algorithmLouvain(grph,quality,algorithmParameters)
	{
	}

	/**
	 * Function to add and remove edges from the graph.
	 * Any edge with a weight different from zero is inserted.
	 * Any edge with a weight of exactly zero is removed.
	 * @return true if adding/removing succeeded
	 */
	bool addRemoveEdges(ReaderInterface<Edge> * reader){
		//TODO add reader error handling
		ReaderInterface<Edge>::NEXTTYPE n=reader->hasNext();
		if(n==ReaderInterface<Edge>::NEXTTYPE::CANNOTOPEN) return false;
		while(n!=ReaderInterface<Edge>::NEXTTYPE::CANNOTOPEN && n!=ReaderInterface<Edge>::NEXTTYPE::ENDOFFILE){
			if(n==ReaderInterface<Edge>::NEXTTYPE::VALUE){
				Edge ed=reader->next();
				addRemoveEdgePre(ed.source(),ed.destination(),ed.weight());
				if(ed.weight()==0){
//					CERR << "remove edge=" << ed.toString() << "\n";
					grph.removeEdge(ed);
				}
				else{
//					CERR << "add edge=" << ed.toString() << "\n";
					grph.addEdge(ed,true);
				}
				addRemoveEdgePost(ed.source(),ed.destination(),ed.weight());
//				CERR << toString()<< "\n";
			}
			else{
				reader->next();//skip anything else other than a value
			}
			n=reader->hasNext();
		}
		return run();
	}

	  const std::string toString(const StringFormater & sf=defaultStringFormater)const{
			switch(algrthm){
			default:
			case ALGORITHM::LOUVAIN:
				return algorithmLouvain.toString(sf);
			break;
			//		case ALGORITHM::SHAKEN:
			//			return louvain.addRemoveEdges(reader);
			//		case ALGORITHM::
			}
	  }
};

#endif /* SRC_ALGORITHM_H_ */
