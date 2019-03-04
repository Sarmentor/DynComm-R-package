/*
 * mapUtilities.h
 *
 *  Created on: 19/08/2018
 *      Author: poltergeist0
 */

#ifndef MAPUTILITIES_H_
#define MAPUTILITIES_H_

#include <set>
#include <map>
#include <type_traits>
#include <string>
#include <sstream>
#include "stringFormater.h"

namespace map {

/**
 *
 * @param m
 * @return the unique keys of a map
 */
template<typename T, typename U> std::set<T> keys(std::map<T, U> const& m) {
	std::set<T> r;
	for (typename std::map<T,U>::const_iterator it=m.begin();it!=m.end();it++) {
		std::pair<T,U> p=*it;
		r.insert(p.first);
	}
	return r;
}

/**
 *
 * @param m
 * @return the unique values of a map
 */
template<typename T, typename U> std::set<T> values(std::map<T, U> const& m) {
	std::set<T> r;
	for (typename std::map<T,U>::const_iterator it=m.begin();it!=m.end();it++) {
		std::pair<T,U> p=*it;
		r.insert(p.second);
	}
	return r;
}

template<typename T, typename U,typename std::enable_if<std::is_fundamental<T>::value && std::is_fundamental<U>::value,T>::type=0 > std::string toString(std::map<T, U> const& m, const StringFormater & formater=defaultStringFormater) {
	std::stringstream ss;
	formater.start(ss,true);
	for(typename std::map<T,U>::const_iterator it=m.cbegin();it!=m.cend();it++){
		const std::pair<T,U> & p=*it;
		ss << formater.tupleOpen() << p.first << formater.valueSeparator() << p.second << formater.tupleClose();
	}
	formater.end(ss,true);
	std::string s=ss.str();
	return s;
}

}  // namespace map


#endif /* MAPUTILITIES_H_ */
