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
# else //Windows
/**
 * Non-Windows System Definitions section
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
