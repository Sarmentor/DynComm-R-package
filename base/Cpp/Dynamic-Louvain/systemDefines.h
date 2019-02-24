/*
 * defines.h
 *
 *  Created on: 19/08/2018
 *      Author: poltergeist0
 */

#ifndef SYSTEMDEFINES_H_
#define SYSTEMDEFINES_H_

#include <cstdint>  //uint
#include <string>   //basic_string,stoi


# if defined(WIN32)
/**
 * Windows System Definitions section
 */
	#define getpid() _getpid()
	const char PATH_SEPARATOR ='\\';
	const std::string PATH_SEPARATOR_STRING ="\\";
	typedef u_int8_t uint8;
	typedef u_int16_t uint16;
	typedef u_int32_t uint32;
	typedef u_int64_t uint64;
# else
/**
 * Non-Windows System Definitions section
 */
	//TODO differentiate between linux installations (debian, arch,red hat...)
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
# endif

/**
 * Define unsigned char
**/
typedef u_char uchar;

/**
 * Define unsigned string
**/
typedef std::basic_string<uchar> ustring;


#endif /* SYSTEMDEFINES_H_ */
