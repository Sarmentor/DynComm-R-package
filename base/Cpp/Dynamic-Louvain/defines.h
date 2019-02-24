/*
 * defines.h
 *
 *  Created on: 19/08/2018
 *      Author: poltergeist0
 */

#ifndef DEFINES_H_
#define DEFINES_H_

/**
 * Define the following flag if compiling for R CRAN.
 * Otherwise, code is compiled to be standalone
 */
#define FLAG_RCPP

/**
 * Redirect output and error streams and program exit to the correct place if compiling for R CRAN
 */
#ifdef FLAG_RCPP

#include <Rcpp.h>
#define COUT Rcpp::Rcout
#define CERR Rcpp::Rcerr
#define exit(code) Rcpp::stop(std::to_string(code));

#define TRAP(x) "Can not use "##x
#define cout TRAP("std::cout")
#define cerr TRAP("std::cerr")

#else //FLAG_RCPP

#define COUT std::cout
#define CERR std::cerr

#endif //FLAG_RCPP

#endif /* DEFINES_H_ */
