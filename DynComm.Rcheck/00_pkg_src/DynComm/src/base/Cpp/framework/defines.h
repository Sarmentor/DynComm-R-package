/************************************************************************
 ************************* Developer Notice *****************************
 ************************************************************************
 * @details
 *
 * This file is where all pre-compilation flags and function macros are
 * defined.
 *
 *
 * @author poltergeist0
 *
 * @date 2018-08-19
 ************************************************************************
 ************************************************************************
 ************************************************************************/

#ifndef DEFINES_H_
#define DEFINES_H_

/**
 * Define the version of the C++ source code.
 * Must be updated before submitting each stable version
 */

const unsigned long DYNCOMM_CPP_VERSION=20200121;

/**
 * Define the following flag if compiling for R CRAN. Otherwise, code is compiled
 * to be standalone
 */
#define FLAG_RCPP

/**
 * Define the following flag to compile with debug logging enabled
 */
//#define FLAG_DEBUG_LOGGING

/**
 * Define the following flag to compile with debug actions enabled.
 * Debug actions are functions that are run to verify the integrity of objects
 * and of actions (functions called) performed to them.
 */
//#define FLAG_DEBUG_ACTIONS	//NOT YET USED

enum class DEBUG_LEVEL:unsigned int{
	NONE=0	//no debugging
	,TRACE=100	//function calls without parameters
	,CALLS=200	//function calls with parameters and return
	,MODIFICATIONS=300	//function calls with parameters and pre and post operation snapshots
	,ACTIONS=400	//function calls with parameters and pre and post operation snapshots and actions performed inside function
	,VERIFY=5000	//verify operations give correct result
	,ALL=10000	//prints everything
};

/*
 * Redirect output and error streams, and program exit to the correct place if
 * compiling for R CRAN.
 * These macros must be used at all times instead of the normal std::cout,
 * std::cerr and exit.
 * If compiling for R CRAN and the appropriate macros are
 * not used, a compilation error will occur.
 */
#ifdef FLAG_RCPP

#include <Rcpp.h>
#define COUT Rcpp::Rcout
#define CERR Rcpp::Rcerr
#define exit(code) Rcpp::stop(std::to_string(code));

/*
 * macro that forces the error
 */
#define TRAP(x) "Can not use "##x

/*
 * trap cout whether it is prepended with std or not
 */
#define cout TRAP("std::cout")

/*
 * trap cerr whether it is prepended with std or not
 */
#define cerr TRAP("std::cerr")

#else //FLAG_RCPP

/*
 * define macro to use instead of cout if not compiling for R
 */
#define COUT std::cout

/*
 * define macro to use instead of cerr if not compiling for R
 */
#define CERR std::cerr

#endif //FLAG_RCPP

#endif /* DEFINES_H_ */
