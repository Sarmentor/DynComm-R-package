/************************************************************************
 ************************* Developer Notice *****************************
 ************************************************************************
 * @details
 *
 * Utilities to assist working with std::set implemented in C++11.
 *
 *
 * @author poltergeist0
 *
 * @date 2018-08-19
 ************************************************************************
 ************************************************************************
 ************************************************************************/

#ifndef SETUTILITIES_H_
#define SETUTILITIES_H_

#include <set>
#include <map>
#include <type_traits>
#include <string>
#include <sstream>

namespace set {

/**
 * Get the string representation of a set.
 *
 * @param m
 * @return a string
 */
template<typename T, typename std::enable_if<std::is_fundamental<T>::value,T>::type=0 > std::string toString(std::set<T> const& m) {
	std::stringstream ss;
	bool first=true;
	ss << "(";
	for(typename std::set<T>::const_iterator it=m.cbegin();it!=m.cend();it++){
		const T & p=*it;
		if(first)first=false;
		else ss << ";";
		ss << p ;
	}
	ss << ")\n";
	return ss.str();
}

}  // namespace map


#endif /* SETUTILITIES_H_ */
