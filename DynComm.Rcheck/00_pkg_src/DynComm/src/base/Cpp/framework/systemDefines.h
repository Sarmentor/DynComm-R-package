/************************************************************************
 ************************* Developer Notice *****************************
 ************************************************************************
 * @details
 *
 * This file defines data types and other system dependent constants to
 * be used seemingly in the program.
 * Only in this file one needs to be aware of the system to which the
 * program is going to be compiled.
 *
 *
 * @author poltergeist0
 *
 * @date 2018-08-19
 ************************************************************************
 ************************************************************************
 ************************************************************************/

#ifndef SYSTEMDEFINES_H_
#define SYSTEMDEFINES_H_

#include <cstdint>  //uint
#include <string>   //basic_string,stoi


# if defined(WIN32) //Windows
/**
 * Windows System Definitions section
 */
	#define getpid() _getpid()
	const char PATH_SEPARATOR ='\\';
	const std::string PATH_SEPARATOR_STRING ="\\";
	typedef unsigned char u_char; //windows does not define u_char
	# if defined(__MINGW32__) //MinGW 32bit and 64bit
  	/**
  	 * MinGW under Windows System Definitions section
  	 */
  	typedef uint8_t uint8;
  	typedef uint16_t uint16;
  	typedef uint32_t uint32;
  	typedef uint64_t uint64;
  # else //MinGW 32bit and 64bit
  	/**
  	 * Windows System Definitions section
  	 */
  	typedef u_int8_t uint8;
  	typedef u_int16_t uint16;
  	typedef u_int32_t uint32;
  	typedef u_int64_t uint64;
  # endif //MinGW 32bit and 64bit
  	
  	inline static void debug_backtrace(std::stringstream & ss, const int & backtraceBufferSize){
  	  //on windows systems this does nothing (for now)
  	}
  	
  	/* if compiling with no c++ debugging support, define the __ASSERT_FUNCTION
  	 * macro so that program runtime debugging still has pretty function names
  	 * in the debugging information.
  	 * The following code was extracted from assert.h without modification.
  	 */
    // #ifdef NDEBUG
      	/* Version 2.4 and later of GCC define a magical variable `__PRETTY_FUNCTION__'
      	 which contains the name of the function currently being defined.
      	 This is broken in G++ before version 2.6.
      	 C9x has a similar variable called __func__, but prefer the GCC one since
      	 it demangles C++ function names.  */
    // # if defined __cplusplus ? __GNUC_PREREQ (2, 6) : __GNUC_PREREQ (2, 4)
    // #   define __ASSERT_FUNCTION	__extension__ __PRETTY_FUNCTION__
    // # else
    // #  if defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
    // #   define __ASSERT_FUNCTION	__func__
    // #  else
    #   define __ASSERT_FUNCTION	((const char *) 0)
    // #  endif
    // # endif
    // #endif /* NDEBUG.  */
# elif defined(__sun)
/**
 * Solaris System Definitions section
 */

#define getpid() _getpid()
	#define getpid() getpid()
	const char PATH_SEPARATOR ='/';
	const std::string PATH_SEPARATOR_STRING ="/";
	typedef uint8_t uint8;
	typedef uint16_t uint16;
	typedef uint32_t uint32;
	typedef uint64_t uint64;
	inline static void debug_backtrace(std::stringstream & ss, const int & backtraceBufferSize){
		//on Solaris systems this does nothing (for now)
	}
	# define __ASSERT_FUNCTION	((const char *) 0)

# else
/**
 * Non-Windows and non-Solaris System Definitions section
 */
	/*
	 * TODO differentiate between linux installations (debian, arch,red hat...)
	 * since some have things defined in different locations or with different
	 * names.
	 */
	#include <unistd.h>
	#define getpid() getpid()
	const char PATH_SEPARATOR ='/';
	const std::string PATH_SEPARATOR_STRING ="/";
	typedef uint8_t uint8;
	typedef uint16_t uint16;
	typedef uint32_t uint32;
	typedef uint64_t uint64;

  #include <execinfo.h>
	inline static void debug_backtrace(std::stringstream & ss, const int & backtraceBufferSize){
	  int nptrs;
//	  void *buffer[backtraceBufferSize];
	  void **buffer=new void*[backtraceBufferSize];
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
	  delete [] buffer;
	}
	
	/* if compiling with no c++ debugging support, define the __ASSERT_FUNCTION
	 * macro so that program runtime debugging still has pretty function names
	 * in the debugging information.
	 * The following code was extracted from assert.h without modification.
	 */
  #ifdef NDEBUG
  // Defines gi__GNUC_PREREQ if glibc's features.h isn't available.
  # ifndef __GNUC_PREREQ
  #  if defined(__GNUC__) && defined(__GNUC_MINOR__)
  #   define __GNUC_PREREQ(maj, min) \
         ((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
  #  else
  #   define __GNUC_PREREQ(maj, min) 0
  #  endif
  # endif

	/* Version 2.4 and later of GCC define a magical variable `__PRETTY_FUNCTION__'
  	 which contains the name of the function currently being defined.
  	 This is broken in G++ before version 2.6.
  	 C9x has a similar variable called __func__, but prefer the GCC one since
  	 it demangles C++ function names.
  	 On Clang, we assume `__PRETTY_FUNCTION__` is always available.
  	 */
  # if defined __clang__ || (defined __cplusplus ? __GNUC_PREREQ (2, 6) : __GNUC_PREREQ (2, 4))
  #	 define __ASSERT_FUNCTION	__extension__ __PRETTY_FUNCTION__
  # elif defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
  #  define __ASSERT_FUNCTION	__func__
  # else
  #  define __ASSERT_FUNCTION	((const char *) 0)
  # endif
  #endif /* NDEBUG.  */
  	
	
/**
 * End of System Definitions section
 */
# endif //Windows

/**
 * Define unsigned char
**/
typedef u_char uchar;

/**
 * Define unsigned string
**/
typedef std::basic_string<uchar> ustring;


#endif /* SYSTEMDEFINES_H_ */
