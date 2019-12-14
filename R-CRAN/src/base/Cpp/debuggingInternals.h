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

/**
 * define the internals for an equal assert
 * the default delay is 3 seconds
 */
#ifndef FLAG_DEBUG

	#define ZERT_DO_NOT_USE_DIRECTLY3(expression1,expression2,value1,value2,precision,message) do{}while(0)
	#define ZERT_DO_NOT_USE_DIRECTLY3NOT(expression1,expression2,value1,value2,precision,message) do{}while(0)
	#define ZERT_DO_NOT_USE_DIRECTLY2(expression1,expression2,value1,value2,message) do{}while(0)
	#define ZERT_DO_NOT_USE_DIRECTLY2NOT(expression1,expression2,value1,value2,message) do{}while(0)
	#define ZERT_DO_NOT_USE_DIRECTLY1(expression,value,message) do{}while(0)
	#define ZERT_DO_NOT_USE_DIRECTLY1NOT(expression,value,message) do{}while(0)
	#define ASSERT_ITERATOR(expression1,expression2,value1,value2,valu1,valu2,message) do{}while(0)
	#define ASSERT_ITERATOR_NOT(expression1,expression2,value1,value2,valu1,valu2,message) do{}while(0)

#else //FLAG_DEBUG

	#include <execinfo.h>
	#include <cassert>
	#include <chrono>
	#include <thread>
	#include <cmath>
	#include <sstream>
	#include <iostream>

	inline static void debug_backtrace(std::stringstream & ss, const int & backtraceBufferSize){
		int nptrs;
		void *buffer[backtraceBufferSize];
		char **strings;
		nptrs = backtrace(buffer, backtraceBufferSize);
		strings = backtrace_symbols(buffer, nptrs);
		if (strings == NULL) {
			ss<< "ERROR retrieving backtrace symbols\n";
			for (int j = 0; j < nptrs; j++){
				ss<< buffer[j] << "\n";
			}
		}
		else{
			for (int j = 0; j < nptrs; j++){
				ss<< strings[j] << "\n";
			}
			free(strings);
		}
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
			std::cerr << ss.str();
			__assert_fail (expression.c_str(), file, line, function);
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
			std::cerr << ss.str();
			__assert_fail (s.c_str(), file, line, function);
		}
	}


#define ZERT_DO_NOT_USE_DIRECTLY3(expression1,expression2,value1,value2,precision,message) debug_assert(expression1,expression2,#value1,#value2,debug_assert_comparison::EQUAL,fabs((value1)-(value2))<precision,ASSERT_DELAY,ASSERT_BACKTRACE_BUFFER_SIZE, __FILE__, __LINE__, __ASSERT_FUNCTION,message)
#define ZERT_DO_NOT_USE_DIRECTLY3NOT(expression1,expression2,value1,value2,precision,message) debug_assert(expression1,expression2,#value1,#value2,debug_assert_comparison::INEQUAL,fabs((value1)-(value2))>=precision,ASSERT_DELAY,ASSERT_BACKTRACE_BUFFER_SIZE, __FILE__, __LINE__, __ASSERT_FUNCTION,message)
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
