/************************************************************************
 ************************* Developer Notice *****************************
 ************************************************************************
 * @details
 * 
 * Template for Dynamic Communities main algorithms implemented in C++11.
 * 
 * Go through this file and implement modifications as instructed by the
 * markers (multi-line comments with dual row asterisks before and after
 * some text that starts with the word "TODO").
 *
 * The name of your algorithm's class should start with the word
 * "Algorithm" (upper case "A") and be written in camel case.
 *
 * The name of your algorithm's file should start with the word
 * "algorithm" (lower case "a") and be written in camel case.
 *
 * New algorithms should extend the algorithmBase class. That class holds
 * references to the graph, quality and parameters objects. Read the
 * documentation on that class for details about the functions required
 * to be implemented.
 *
 * Because all algorithms have an object of its class instantiated, even if it
 * is not used and is never called, new algorithms should have a minimal
 * memory footprint. This is implemented like so to improve performance by
 * getting rid of object pointers and, consequently, of indirection and pointer
 * resolution which would significantly slow down the program. Otherwise, every
 * cycle in the program would require many function calls from object pointers.
 *
 * Add your C++ algorithms in the "base/Cpp/algorithm.h" file. Follow the
 * instructions on that file for more details.
 * 
 * @author poltergeist0
 * 
 * @date 2019-01-01
 ************************************************************************
 ************************************************************************
 ************************************************************************/

/* **********************************************************************
 ************************************************************************
 * TODO: do not forget to change the name of the define guard
 ************************************************************************
 ************************************************************************/
#ifndef SRC_ALGORITHMTEMPLATE_H_
#define SRC_ALGORITHMTEMPLATE_H_

#include "algorithmBase.h"

/**
 * @brief Class that implements the ... algorithm
 *
 * @details
 * The ... algorithm is ... and does ...
 *
 * @author your name or organization
 * @date whenever year-month-day international numeric format
 */
class AlgorithmTemplate: private AlgorithmBase{
private:
/* **********************************************************************
 ************************************************************************
 * TODO: Add private variables and functions here
 ************************************************************************
 ************************************************************************/

public:

	/**
	 * Function that runs before edges are added or removed to/from the graph.
	 * Useful if pre-processing is required, for example, to update some of
	 * this class' inner variables before the edge can be added or removed.
	 *
	 * @param source source vertex
	 * @param destination destination vertex
	 * @param weight optional if adding an edge. Must be exactly zero to remove an edge.
	 * @return true if all operations succeeded. False, otherwise.
	 */
	bool addRemoveEdgePre(const typeVertex & source, const typeVertex & destination, const typeWeight & weight=1.0){
		if(weight!=0.0){
/* **********************************************************************
 ************************************************************************
 * TODO: execute some pre-processing operations before adding the edge.
 * If this is not required it should just return true
 ************************************************************************
 ************************************************************************/
//			return true;
		}
		else{
/* **********************************************************************
 ************************************************************************
 * TODO: execute some pre-processing operations before removing the edge.
 * If this is not required it should just return true
 ************************************************************************
 ************************************************************************/
//			return true;
		}
/* **********************************************************************
 ************************************************************************
 * TODO: if anything fails return false
 ************************************************************************
 ************************************************************************/
		return false;
	}

	/**
	 * Function that runs after edges are added or removed to/from the graph.
	 * Useful if post-processing is required, for example, to update some of
	 * this class' inner variables after the edge has be added or removed.
	 *
	 * @param source source vertex
	 * @param destination destination vertex
	 * @param weight optional if adding an edge. Must be exactly zero to remove an edge.
	 * @return true if all operations succeeded. False, otherwise.
	 */
	bool addRemoveEdgePost(const typeVertex & source, const typeVertex & destination, const typeWeight & weight=1.0){
		if(weight!=0.0){
/* **********************************************************************
 ************************************************************************
 * TODO: execute some post-processing operations before adding the edge.
 * If this is not required it should just return true
 ************************************************************************
 ************************************************************************/
//			return true;
		}
		else{
/* **********************************************************************
 ************************************************************************
 * TODO: execute some post-processing operations before removing the edge.
 * If this is not required it should just return true
 ************************************************************************
 ************************************************************************/
//			return true;
		}
/* **********************************************************************
 ************************************************************************
 * TODO: if anything fails return false
 ************************************************************************
 ************************************************************************/
		return false;
	}

	/**
	 * Function that has the implementation of the algorithm.
	 *
	 * @return true if all operations succeeded. False, otherwise.
	 */
	bool run(){
/* **********************************************************************
 ************************************************************************
 * TODO: run your algorithm here
 ************************************************************************
 ************************************************************************/
		return false;
	}

public:
	/**
	 * Default constructor not acceptable.
	 * Must be passed at least the graph, quality and parameters
	 */
	AlgorithmTemplate()=delete;

	/**
	 * Destructor
	 */
	~AlgorithmTemplate(){
/* **********************************************************************
 ************************************************************************
 * TODO: free memory (private variables) here
 ************************************************************************
 ************************************************************************/
	}

	/**
	 * Constructor.
	 *
	 * Must be implemented with the skeleton provided here, receiving the same
	 * arguments without copying.
	 *
	 * Must not implement anything and do nothing other than initializing
	 * variables that are required through the lifetime of the object.
	 *
	 * @param graph reference to the graph object
	 * @param quality reference to the quality object
	 * @param algorithmParameters reference to the parameters object
	 */
	AlgorithmTemplate(
			GraphUndirectedGroupable & graph
			, const Criterion & quality
			, const ProgramParameters & algorithmParameters=argumentsDefault)
	:
		AlgorithmBase(graph,quality,algorithmParameters)
/* **********************************************************************
 ************************************************************************
 * TODO: initialize private variables here
 ************************************************************************
 ************************************************************************/
	{
	}

	/**
	 * Function that converts this object to a string representation.
	 * Might be useful for debugging.
	 *
	 * @param sf is a StringFormater object that facilitates formating
	 * @return the string representing this object
	 */
	const std::string toString(const StringFormatter & sf=defaultStringFormatter)const{
		//copy string formatter
		StringFormatter f=sf;
		//initialize a string stream to pass the string formatter
		std::stringstream ss;
		if(!sf.isDefault()){
			f.build(ss,"");
			++f;//indent
		}
		ss << AlgorithmBase::toString(sf);
/* **********************************************************************
 ************************************************************************
 * TODO: Add any private variables that you want to output here
 ************************************************************************
 ************************************************************************/
		//ss << ;
		return ss.str();
	}

};

#endif /* SRC_ALGORITHMTEMPLATE_H_ */
