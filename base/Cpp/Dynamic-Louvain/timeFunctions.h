/*
 * timeFunctions.h
 *
 *  Created on: 12/11/2018
 *      Author: poltergeist0
 */

#ifndef SRC_TIMEFUNCTIONS_H_
#define SRC_TIMEFUNCTIONS_H_


#include <sys/time.h>	//gettimeofday

#include "defines.h"
#include <limits>

namespace Time {

/**
 * @brief longTime Converts a timeval type time to an unsigned 64 bit integer
 * @param t is the timeval to convert
 * @return an unsigned 64 bit integer
 */
inline uint64 longTime(const timeval& t){
	uint64 a=static_cast<uint64>(t.tv_sec);
	a*=static_cast<uint64>(1000000);
	a+=static_cast<uint64>(t.tv_usec);
    return a;
}
inline uint64 longTime(const timespec& t){
	uint64 a=static_cast<uint64>(t.tv_sec);
	a*=static_cast<uint64>(1000000000);
	a+=static_cast<uint64>(t.tv_nsec);
	a/=static_cast<uint64>(1000);//convert to microseconds. Can be commented when currentTime() starts using clock_gettime() instead of gettimeofday()
    return a;
}

/**
 * @brief Get the current system time in microseconds as an unsigned 64 bit integer
 */
inline uint64 currentTime(){
	timeval tm;
	if(gettimeofday(&tm,NULL)==0){//DEPRECATED but is the one to use in fedora13 since there is no other better
//	timespec tm;
//	if(clock_gettime(CLOCK_REALTIME, &tm)==0){//this is the one to use on modern systems
		return longTime(tm);
	}
	return 0;//return epoch on error so that further processing can detect and possibly correct
}

/**
 * keeps track of elapsed time since given time or epoch, if time=0
 */
class TimeKeeper{
private:
	uint64 r;//reference time for math
	uint64 t;//intended base time (time referential)
	inline uint64 validateBaseTime(const uint64& time) const{
		if(time<=r){
			return time;
		}
		return 0;
	}
	inline uint64 validateTime(const uint64& time) const{
		if(time>=r){
			return time;
		}
		return std::numeric_limits<uint64>::max();
	}
public:
	TimeKeeper(uint64 time=0):r(currentTime()),t(validateBaseTime(time)){}
	/**
	 * set base (intended) time and possibly update the reference time
	 */
	inline uint64 set(uint64 time=0, bool update=true){
		if(update){
			r=currentTime();
		}
		t=validateBaseTime(time);
		return get();
	}
	/**
	 * get current time or the elapsed time regarding the saved reference time or translate a time to the configured time referential
	 */
	inline uint64 get(const uint64& time=0) const{
		if(t==0){
			if(time==0){
				return currentTime();
			}
			else{
				return validateTime(time);
			}
		}
		else{
			if(time==0){
				return currentTime()-r+t;
			}
			else{
				uint64 x=validateTime(time);
				return ((x==std::numeric_limits<uint64>::max())?std::numeric_limits<uint64>::max():x-r+t);
			}
		}
	}
};

TimeKeeper timeKeep;//by default make it return the current time

}  // namespace Time





#endif /* SRC_TIMEFUNCTIONS_H_ */
