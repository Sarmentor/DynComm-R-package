/************************************************************************
 ************************* Developer Notice *****************************
 ************************************************************************
 * @details
 *
 * This file defines several time related functions and classes.
 *
 *
 * @author poltergeist0
 *
 * @date 2018-11-12
 ************************************************************************
 ************************************************************************
 ************************************************************************/

#ifndef SRC_TIMEKEEPER_H_
#define SRC_TIMEKEEPER_H_


#include <sys/time.h>	//gettimeofday

#include "../defines.h"
#include "../systemDefines.h"
#include <limits>

namespace Time {

/**
 * @brief longTime Converts a timeval type time to an unsigned 64 bit integer
 * @param t is the timeval to convert
 * @return an unsigned 64 bit integer representing time in nanoseconds
 */
// inline uint64 longTime(const timeval& t){
// 	uint64 a=static_cast<uint64>(t.tv_sec);
// 	a*=static_cast<uint64>(1000000);
// 	a+=static_cast<uint64>(t.tv_usec);
// 	a*=static_cast<uint64>(1000);//convert to nanoseconds
//     return a;
// }

/**
 * @brief longTime Converts a timespec type time to an unsigned 64 bit integer
 * @param t is the timespec to convert
 * @return an unsigned 64 bit integer representing time in nanoseconds
 */
// inline uint64 longTime(const timespec& t){
// 	uint64 a=static_cast<uint64>(t.tv_sec);
// 	a*=static_cast<uint64>(1000000000);
// 	a+=static_cast<uint64>(t.tv_nsec);
//     return a;
// }

/**
 * @brief Get the current system time in nanoseconds as an unsigned 64 bit integer
 * @return the current time or epoch (0) if an error occurred
 */
// inline uint64 currentTime(){
// //	timeval tm;
// //	if(gettimeofday(&tm,NULL)==0){//DEPRECATED but is the one to use in fedora13 since there is no other better
// 	timespec tm;
// 	if(clock_gettime(CLOCK_REALTIME, &tm)==0){//this is the one to use on modern systems
// 		return longTime(tm);
// 	}
// 	return 0;//return epoch on error so that further processing can detect and possibly correct
// }

/**
 * @brief Simple class used to keep track of elapsed time.
 *
 * @details
 * keeps track of elapsed time since given time or epoch, if time=0
 *
 * @author poltergeist0
 *
 * @date 2018-11-12
 */
class TimeKeeper{
private:
	/*
	 * reference time used for calculations of elapsed time
	 */
	uint64 r;

	/*
	 * base time (time referential) to be used instead of epoch
	 */
	uint64 t;

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
	 * If there is no base time and the given time is zero, return the current time (time since epoch)
	 * If there is no base time and the given time is not zero, return the elapsed time since the last call to set or the constructor
	 * If there is base time and the given time is zero, return the elapsed time since reference time in regard to the configured base time
	 * If there is base time and the given time is not zero, return the time difference of the given time compared to the reference time in regard to the configured base time
	 *
	 * @param time
	 * @return
	 */
	inline uint64 get(const uint64& time=0) const{
		if(t==0){
			if(time==0){
				return currentTime();//return the current time (time since epoch)
			}
			else{
				return validateTime(time)-r;//return the difference of the given time compared to the reference time (stored on the last call of set or the constructor)
			}
		}
		else{
			if(time==0){
				return currentTime()-r+t;//return the elapsed time since reference time in regard to the configured base time
			}
			else{
				uint64 x=validateTime(time);
				//return the time difference of the given time compared to the reference time in regard to the configured base time
				return ((x==std::numeric_limits<uint64>::max())?std::numeric_limits<uint64>::max():x-r+t);
			}
		}
	}
};

TimeKeeper timeKeep;//by default make it return the current time

}  // namespace Time





#endif /* SRC_TIMEKEEPER_H_ */
