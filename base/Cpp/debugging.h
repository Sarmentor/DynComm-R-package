/*
 *  Created on: 16 Feb 2015
 *      Author: poltergeist0
 *
 * Defines several functions used to print debug messages to standard error and an object capable of
 * printing those debug messages to file
 */

#ifndef SRC_DEBUGGING_H_
#define SRC_DEBUGGING_H_

/**
 * Comment the following define to enable asserts and add other debug and/or testing code
 * Debugging seriously slows down the program and should not be used in production
 */
#define NDEBUG

/**
 * Define that the compilation is to occur under Windows Cygwin.
 * The default is linux (namely ubuntu)
 * This is used to accommodate differences between windows and linux in type naming.
 * Currently only affects the global defines header. Some other changes might have to be introduced for the code to cleanly compile under Cygwin.
 */
//#define FLAG_CYGWIN

/**
 * Define the following to enable direct access to private members of classes for testing code to verify correct implementation
 * Debugging must be enabled (NDEBUG flag undefined)
 * Must not be used in production
 */
#define TEST

#include "debuggingInternals.h"

/**
 * Define the amount of time for the assert delay
 */
//#define ASSERT_DELAYED_TIMEOUT 3000 //in milliseconds (3 seconds)
#define ASSERT_DELAY 3000 //in milliseconds (3 seconds)

#define ASSERT_PRECISION_DEFAULT 1e-6

/**
 * Assert that is delayed for a few seconds so that can finish writing debug messages
 * @deprecated
 */
//#define ASSERT_DELAYED(expr) _assertt_(expr)

#define ASSERT_BACKTRACE_BUFFER_SIZE 1024

#define ASSERT_APPROXIMATE_MESSAGE(value1,value2,precision,message) ASSERT3(#value1,#value2,value1,value2,precision,message)
#define ASSERT_APPROXIMATE(value1,value2,precision) ASSERT3(#value1,#value2,value1,value2,precision,"")
#define ASSERT_EQUAL_MESSAGE(value1,value2,message) ASSERT2(#value1,#value2,value1,value2,message)
#define ASSERT_EQUAL(value1,value2) ASSERT2(#value1,#value2,value1,value2,"")
#define ASSERT_MESSAGE(value,message) ASSERT1(#value,value,message)
#define ASSERT(value) ASSERT1(#value,value,"")
#define ASSERT_EQUAL_ITERATOR_MESSAGE(value1,value2,message) ASSERT_ITERATOR(#value1,#value2,&(*value1),&(*value2),value1,value2,message)
#define ASSERT_EQUAL_ITERATOR(value1,value2) ASSERT_ITERATOR(#value1,#value2,&(*value1),&(*value2),value1,value2,"")

#define ASSERT_NOT_APPROXIMATE_MESSAGE(value1,value2,precision,message) ASSERT3NOT(#value1,#value2,value1,value2,precision,message)
#define ASSERT_NOT_APPROXIMATE(value1,value2,precision) ASSERT3NOT(#value1,#value2,value1,value2,precision,"")
#define ASSERT_NOT_EQUAL_MESSAGE(value1,value2,message) ASSERT2NOT(#value1,#value2,value1,value2,message)
#define ASSERT_NOT_EQUAL(value1,value2) ASSERT2NOT(#value1,#value2,value1,value2,"")
#define ASSERT_NOT_EQUAL_ITERATOR_MESSAGE(value1,value2,message) ASSERT_ITERATOR_NOT(#value1,#value2,&(*value1),&(*value2),value1,value2,message)
#define ASSERT_NOT_EQUAL_ITERATOR(value1,value2) ASSERT_ITERATOR_NOT(#value1,#value2,&(*value1),&(*value2),value1,value2,"")


///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////

#ifdef NDEBUG
	//if debugging is disabled, disable testing
	#undef TEST
#endif


#endif /* SRC_DEBUGGING_H_ */
