/*
 *  Created on: 16 Feb 2015
 *      Author: poltergeist0
 *
 */

#ifndef SRC_DEBUGGINGINTERNALS_H_
#define SRC_DEBUGGINGINTERNALS_H_

/**************************************************************************
 **************************************************************************
 **************************************************************************
 **************************************************************************
 *
 * 			Debug functions for internal use only
 * 			DO NOT USE any of the following functions DIRECTLY
 *
 **************************************************************************
 **************************************************************************
 **************************************************************************
 **************************************************************************
 */
/**************************************************************************
 **************************************************************************
 **************************************************************************
 **************************************************************************
 *
 * 			Debug functions for internal use only
 * 			DO NOT USE any of the following functions DIRECTLY
 *
 **************************************************************************
 **************************************************************************
 **************************************************************************
 **************************************************************************
 */
/**************************************************************************
 **************************************************************************
 **************************************************************************
 **************************************************************************
 *
 * 			Debug functions for internal use only
 * 			DO NOT USE any of the following functions DIRECTLY
 *
 **************************************************************************
 **************************************************************************
 **************************************************************************
 **************************************************************************
 */

#include "../systemDefines.h"
/**
 * define the internals for an equal assert
 * the default delay is 3 seconds
 */
#ifndef FLAG_DEBUG

  #define ZERT_DO_NOT_USE_DIRECTLY3(expression1,expression2,value1,value2,precision,message) do{}while(0)
  #define ZERT_DO_NOT_USE_DIRECTLY3NOT(expression1,expression2,value1,value2,precision,message) do{}while(0)
  #define ZERT_DO_NOT_USE_DIRECTLY2S(expression1,expression2,value1,value2,precision,message) do{}while(0)
  #define ZERT_DO_NOT_USE_DIRECTLY2SE(expression1,expression2,value1,value2,precision,message) do{}while(0)
  #define ZERT_DO_NOT_USE_DIRECTLY2G(expression1,expression2,value1,value2,precision,message) do{}while(0)
  #define ZERT_DO_NOT_USE_DIRECTLY2GE(expression1,expression2,value1,value2,precision,message) do{}while(0)
  #define ZERT_DO_NOT_USE_DIRECTLY2(expression1,expression2,value1,value2,message) do{}while(0)
  #define ZERT_DO_NOT_USE_DIRECTLY2NOT(expression1,expression2,value1,value2,message) do{}while(0)
  #define ZERT_DO_NOT_USE_DIRECTLY1(expression,value,message) do{}while(0)
  #define ZERT_DO_NOT_USE_DIRECTLY1NOT(expression,value,message) do{}while(0)
  #define ZERT_DO_NOT_USE_DIRECTLYNAN(expression,value,message) do{}while(0)
  #define ZERT_DO_NOT_USE_DIRECTLYNAN_NOT(expression,value,message) do{}while(0)
  #define ZERT_DO_NOT_USE_DIRECTLY_ITERATOR(expression1,expression2,value1,value2,valu1,valu2,message) do{}while(0)
  #define ZERT_DO_NOT_USE_DIRECTLY_ITERATOR_NOT(expression1,expression2,value1,value2,valu1,valu2,message) do{}while(0)

#else //FLAG_DEBUG

	// #include <execinfo.h>
	#include <cassert>
	#include <chrono>
	#include <thread>
	#include <cmath>
	#include <sstream>
	#include <iostream>
	#include <exception>

  // /* if compiling with no c++ debugging support, define the __ASSERT_FUNCTION
  //  * macro so that program runtime debugging still has pretty function names
  //  * in the debugging information.
  //  * The following code was extracted from assert.h without modification.
  //  */
  // #ifdef NDEBUG
  //   /* Version 2.4 and later of GCC define a magical variable `__PRETTY_FUNCTION__'
  //    which contains the name of the function currently being defined.
  //    This is broken in G++ before version 2.6.
  //    C9x has a similar variable called __func__, but prefer the GCC one since
  //    it demangles C++ function names.  */
  //   # if defined __cplusplus ? __GNUC_PREREQ (2, 6) : __GNUC_PREREQ (2, 4)
  //   #   define __ASSERT_FUNCTION	__extension__ __PRETTY_FUNCTION__
  //   # else
  //   #  if defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
  //   #   define __ASSERT_FUNCTION	__func__
  //   #  else
  //   #   define __ASSERT_FUNCTION	((const char *) 0)
  //   #  endif
  //   # endif
  // #endif /* NDEBUG.  */

	// inline static void debug_backtrace(std::stringstream & ss, const int & backtraceBufferSize){
	// 	int nptrs;
	// 	void *buffer[backtraceBufferSize];
	// 	char **strings;
	// 	nptrs = backtrace(buffer, backtraceBufferSize);
	// 	strings = backtrace_symbols(buffer, nptrs);
	// 	if (strings == NULL) {
	// 		ss<< "ERROR retrieving backtrace symbols\n";
	// 		for (int j = 0; j < nptrs; j++){
	// 			ss<< buffer[j] << "\n";
	// 		}
	// 	}
	// 	else{
	// 		for (int j = 0; j < nptrs; j++){
	// 			ss<< strings[j] << "\n";
	// 		}
	// 		free(strings);
	// 	}
	// }

	inline static std::string debug_assert_throw(
			const char *file,unsigned int line,
			const char *function,const std::string & expression){
		std::stringstream ss;
		ss << "DynComm" << ": " << file << ":" << line << ": " << function << ": Assertion '" << expression  << "' failed\n";
		return ss.str();
	}

	inline static void debug_assert(
			const std::string & expression,
			const std::string & value,
			const bool & result,
			const int & delay, const int & backtraceBufferSize,
			const char *file,unsigned int line, const char *function,const std::string & message=""){
		if(!result){
			std::this_thread::sleep_for(std::chrono::milliseconds(delay));
			std::stringstream ss;
			if(message.size()>0)ss << message << "\n";
			ss << expression << "\n" << value << "\n";
			debug_backtrace(ss,backtraceBufferSize);
			CERR << ss.str();
//			__assert_fail (expression.c_str(), file, line, function);
			throw std::logic_error(debug_assert_throw(file, line, function, expression));
		}
	}

	enum class debug_assert_comparison:unsigned int{EQUAL,INEQUAL,SMALLER,SMALLER_EQUAL,GREATER,GREATER_EQUAL};

	inline static void debug_assert(
			const std::string & expression1,const std::string & expression2,
			const std::string & value1,const std::string & value2,
//			const bool & equal,
			const debug_assert_comparison & comp,
			const bool & result,
			const int & delay, const int & backtraceBufferSize,
			const char *file,unsigned int line, const char *function,const std::string & message=""){
		if(!result){
			std::this_thread::sleep_for(std::chrono::milliseconds(delay));
			std::stringstream ss;
			ss << expression1;
//			if(equal) ss << "==";
//			else ss << "!=";
			switch(comp){
				case debug_assert_comparison::EQUAL: ss << "==";break;
				case debug_assert_comparison::INEQUAL: ss << "!=";break;
				case debug_assert_comparison::SMALLER: ss << "<";break;
				case debug_assert_comparison::SMALLER_EQUAL: ss << "<=";break;
				case debug_assert_comparison::GREATER: ss << ">";break;
				case debug_assert_comparison::GREATER_EQUAL: ss << ">=";break;
				default: ss << "!?";break;
			}
			ss << expression2;
			std::string s=ss.str();
			ss.str("");
			if(message.size()>0)ss << message << "\n";
			ss << expression1;
//			if(equal) ss << "==";
//			else ss << "!=";
			switch(comp){
				case debug_assert_comparison::EQUAL: ss << "==";break;
				case debug_assert_comparison::INEQUAL: ss << "!=";break;
				case debug_assert_comparison::SMALLER: ss << "<";break;
				case debug_assert_comparison::SMALLER_EQUAL: ss << "<=";break;
				case debug_assert_comparison::GREATER: ss << ">";break;
				case debug_assert_comparison::GREATER_EQUAL: ss << ">=";break;
				default: ss << "!?";break;
			}
			ss << expression2 << "\n" << value1;
//			if(equal) ss << "==";
//			else ss << "!=";
			switch(comp){
				case debug_assert_comparison::EQUAL: ss << "==";break;
				case debug_assert_comparison::INEQUAL: ss << "!=";break;
				case debug_assert_comparison::SMALLER: ss << "<";break;
				case debug_assert_comparison::SMALLER_EQUAL: ss << "<=";break;
				case debug_assert_comparison::GREATER: ss << ">";break;
				case debug_assert_comparison::GREATER_EQUAL: ss << ">=";break;
				default: ss << "!?";break;
			}
			ss << value2 << "\n";
			debug_backtrace(ss,backtraceBufferSize);
			CERR << ss.str();
//			__assert_fail (s.c_str(), file, line, function);
			throw std::logic_error(debug_assert_throw(file, line, function, s));
		}
	}


#define ZERT_DO_NOT_USE_DIRECTLY3(expression1,expression2,value1,value2,precision,message) debug_assert(expression1,expression2,#value1,#value2,debug_assert_comparison::EQUAL,fabsl((value1)-(value2))<precision,ASSERT_DELAY,ASSERT_BACKTRACE_BUFFER_SIZE, __FILE__, __LINE__, __ASSERT_FUNCTION,message)
#define ZERT_DO_NOT_USE_DIRECTLY3NOT(expression1,expression2,value1,value2,precision,message) debug_assert(expression1,expression2,#value1,#value2,debug_assert_comparison::INEQUAL,fabsl((value1)-(value2))>=precision,ASSERT_DELAY,ASSERT_BACKTRACE_BUFFER_SIZE, __FILE__, __LINE__, __ASSERT_FUNCTION,message)
#define ZERT_DO_NOT_USE_DIRECTLY2S(expression1,expression2,value1,value2,precision,message) debug_assert(expression1,expression2,#value1,#value2,debug_assert_comparison::SMALLER,value1<value2+precision,ASSERT_DELAY,ASSERT_BACKTRACE_BUFFER_SIZE, __FILE__, __LINE__, __ASSERT_FUNCTION,message)
#define ZERT_DO_NOT_USE_DIRECTLY2SE(expression1,expression2,value1,value2,precision,message) debug_assert(expression1,expression2,#value1,#value2,debug_assert_comparison::SMALLER_EQUAL,value1<=value2+precision,ASSERT_DELAY,ASSERT_BACKTRACE_BUFFER_SIZE, __FILE__, __LINE__, __ASSERT_FUNCTION,message)
#define ZERT_DO_NOT_USE_DIRECTLY2G(expression1,expression2,value1,value2,precision,message) debug_assert(expression1,expression2,#value1,#value2,debug_assert_comparison::GREATER,value1+precision>value2,ASSERT_DELAY,ASSERT_BACKTRACE_BUFFER_SIZE, __FILE__, __LINE__, __ASSERT_FUNCTION,message)
#define ZERT_DO_NOT_USE_DIRECTLY2GE(expression1,expression2,value1,value2,precision,message) debug_assert(expression1,expression2,#value1,#value2,debug_assert_comparison::GREATER_EQUAL,value1+precision>=value2,ASSERT_DELAY,ASSERT_BACKTRACE_BUFFER_SIZE, __FILE__, __LINE__, __ASSERT_FUNCTION,message)
#define ZERT_DO_NOT_USE_DIRECTLY2(expression1,expression2,value1,value2,message) debug_assert(expression1,expression2,#value1,#value2,debug_assert_comparison::EQUAL,value1==value2,ASSERT_DELAY,ASSERT_BACKTRACE_BUFFER_SIZE, __FILE__, __LINE__, __ASSERT_FUNCTION,message)
#define ZERT_DO_NOT_USE_DIRECTLY2NOT(expression1,expression2,value1,value2,message) debug_assert(expression1,expression2,#value1,#value2,debug_assert_comparison::INEQUAL,value1!=value2,ASSERT_DELAY,ASSERT_BACKTRACE_BUFFER_SIZE, __FILE__, __LINE__, __ASSERT_FUNCTION,message)
#define ZERT_DO_NOT_USE_DIRECTLY1(expression,value,message) debug_assert(expression,#value,value,ASSERT_DELAY,ASSERT_BACKTRACE_BUFFER_SIZE, __FILE__, __LINE__, __ASSERT_FUNCTION,message)
#define ZERT_DO_NOT_USE_DIRECTLY1NOT(expression,value,message) debug_assert(expression,#value,value,ASSERT_DELAY,ASSERT_BACKTRACE_BUFFER_SIZE, __FILE__, __LINE__, __ASSERT_FUNCTION,message)
#define ZERT_DO_NOT_USE_DIRECTLYNAN(expression,value,message) debug_assert(expression,#value,std::isnan(value),ASSERT_DELAY,ASSERT_BACKTRACE_BUFFER_SIZE, __FILE__, __LINE__, __ASSERT_FUNCTION,message)
#define ZERT_DO_NOT_USE_DIRECTLYNAN_NOT(expression,value,message) debug_assert(expression,#value,!std::isnan(value),ASSERT_DELAY,ASSERT_BACKTRACE_BUFFER_SIZE, __FILE__, __LINE__, __ASSERT_FUNCTION,message)
#define ZERT_DO_NOT_USE_DIRECTLY_ITERATOR(expression1,expression2,value1,value2,valu1,valu2,message) debug_assert(expression1,expression2,#value1,#value2,debug_assert_comparison::EQUAL,valu1==valu2,ASSERT_DELAY,ASSERT_BACKTRACE_BUFFER_SIZE, __FILE__, __LINE__, __ASSERT_FUNCTION,message)
#define ZERT_DO_NOT_USE_DIRECTLY_ITERATOR_NOT(expression1,expression2,value1,value2,valu1,valu2,message) debug_assert(expression1,expression2,#value1,#value2,debug_assert_comparison::INEQUAL,valu1!=valu2,ASSERT_DELAY,ASSERT_BACKTRACE_BUFFER_SIZE, __FILE__, __LINE__, __ASSERT_FUNCTION,message)

#endif //FLAG_DEBUG

#endif /* SRC_DEBUGGINGINTERNALS_H_ */
