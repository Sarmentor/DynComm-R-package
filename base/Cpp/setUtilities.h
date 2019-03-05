/*
 * mapUtilities.h
 *
 *  Created on: 19/08/2018
 *      Author: poltergeist0
 */

#ifndef SETUTILITIES_H_
#define SETUTILITIES_H_

#include <set>
#include <map>
#include <type_traits>
#include <string>
#include <sstream>

namespace set {

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
