/*
 * algorithmInterface.h
 *
 *  Created on: 02/02/2019
 *      Author: poltergeist0
 */

#ifndef SRC_ALGORITHMBASE_H_
#define SRC_ALGORITHMBASE_H_

//#include "graphUndirectedGroupable.h"
#include "program.h"
#include "reader.h"
#include "algorithmInterface.h"

/**
 * Dynamic Communities class API.
 */
//template<typename ALG , typename std::enable_if<std::is_base_of<AlgorithmInterface, ALG>::value>::type* = nullptr>
class AlgorithmBase:public AlgorithmInterface {
private:
	GraphUndirectedGroupable & grph;//reference to graph with edges
	ProgramParameters & prmtrs;//algorithm parameters

public:
	/**
	 * Default constructor not acceptable.
	 * Must be passed at least the graph
	 */
	AlgorithmBase()=delete;

	/**
	 * Destructor
	 */
	virtual ~AlgorithmBase(){}

	/**
	 * Constructor
	 */
	template<typename T , typename std::enable_if<std::is_base_of<ReaderInterface, T>::value>::type* = nullptr>
	AlgorithmBase(T& reader,const ProgramParameters & parameters=argumentsDefault)
	:
	prmtrs(parameters)
	{
		ReaderInterface::NEXTTYPE n=reader.hasNext();
		while(n!=ReaderInterface::NEXTTYPE::ERROR && n!=ReaderInterface::NEXTTYPE::ENDOFFILE){
			if(n==ReaderInterface::NEXTTYPE::VALUE){
				typeNode src=std::stoi(reader.next());
				typeNode dest=std::stoi(reader.next());
				typeNode weight=std::stoi(reader.next());
				Criterion::addEdge(src,dest,weight);
			}
			if(n==ReaderInterface::NEXTTYPE::NEWLINE){
				reader.next();//skip new line
			}
			n=reader.hasNext();
		}
		run();
	}

	/**
	 * Function to add and remove edges from the graph.
	 * Any edge with a weight different from zero is inserted.
	 * Any edge with a weight of exactly zero is removed.
	 * @return true if adding/removing succeeded
	 */
	template<typename T , typename std::enable_if<std::is_base_of<ReaderInterface, T>::value>::type* = nullptr>
	bool addRemoveEdge(T& reader){
		//do something
		return run();
	}


	/**
	 * @return the current quality measure of the community mapping on the graph
	 */
	double quality(){
		return 0;
	}

	/**
	 * @return the number of communities
	 */
	int communityCount(){
		return 10;
	}

	/**
	 * @return a list of all communities
	 */
	std::list<long> communities(){
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
		return std::list<long>();
	}

	/**
	 * @return a list of all nodes belonging to the given community
	 */
	std::list<long> nodes(int community){
		return std::list<long>();
	}

	/**
	 * Get a snapshot of the current community mapping
	 * The differential parameter will probably be moved inside the writer as a parameter
	 * @return true if the operation succeeded
	 */
	template<typename T , typename std::enable_if<std::is_base_of<WriterInterface, T>::value>::type* = nullptr>
	bool results(T & writer,bool differential=true){
		return false;
	}

};

#endif /* SRC_ALGORITHMBASE_H_ */
