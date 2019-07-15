/************************************************************************
 ************************* Developer Notice *****************************
 ************************************************************************
 * @details
 * 
 * This file holds the list available algorithms and two auxiliary functions
 * that indicate if a given algorithm is implemented in a single black box or
 * is split into algorithm, criterion and graph.
 * 
 * Refer to "DynCommBase" for details about single and split algorithm 
 * implementations.
 *
 *
 * @author poltergeist0
 *
 * @date 2019-07-13
 ************************************************************************
 ************************************************************************
 ************************************************************************/

#ifndef SRC_ALGORITHMLIST_H_
#define SRC_ALGORITHMLIST_H_


/* **********************************************************************
 ************************************************************************
 * TODO: Add algorithm name to enumeration. Use the same name used in R
 ************************************************************************
 ************************************************************************/
	/**
	 * Enumeration with the list of supported Dynamic Communities algorithms.
	 * This enumeration must start at 1 since this code is used in R and R
	 * indexing (for arrays, etc) starts at 1 instead of 0.
	 * Otherwise C++ would assign the first algorithm a 0.
	 */
	enum class ALGORITHM:unsigned int{
	  LOUVAIN=1
	  ,SHAKEN
   };

	/* **********************************************************************
	 ************************************************************************
	 * TODO: Add algorithm switch case if the algorithm is implemented as single
	 ************************************************************************
	 ************************************************************************/
	/**
	 * Indicates if the given algorithm is implemented as single
	 */
	inline bool isSingle(const ALGORITHM & alg){
	  // switch(alg){
	  // case : return true;
	  // }
	  return false;
	}
	
	/**
	 * Indicates if the given algorithm is implemented as split
	 */
	inline bool isSplit(const ALGORITHM & alg){return !isSingle(alg);}

#endif /* SRC_ALGORITHMLIST_H_ */
