/************************************************************************
 ************************* Developer Notice *****************************
 ************************************************************************
 * @details
 *
 * Utilities to assist working with std::multimap implemented in C++11.
 *
 *
 * @author poltergeist0
 *
 * @date 2018-08-19
 ************************************************************************
 ************************************************************************
 ************************************************************************/

#ifndef MULTIMAPUTILITIES_H_
#define MULTIMAPUTILITIES_H_

#include <map>
#include <type_traits>
#include <string>
#include <sstream>

#include "../utilities/stringFormatter.h"

namespace multimap {

/**
 *
 * @param m
 * @return the unique keys of a multimap
 */
template<typename T, typename U> std::set<T> keys(std::multimap<T, U> const& m) {
	std::set<T> r;
	for (typename std::multimap<T,U>::const_iterator it=m.begin();it!=m.end();it++) {
		std::pair<T,U> p=*it;
		r.insert(p.first);
	}
	return r;
}

/**
 *
 * @param m
 * @return the unique values of a multimap
 */
template<typename T, typename U> std::set<T> values(std::multimap<T, U> const& m) {
	std::set<T> r;
	for (typename std::multimap<T,U>::const_iterator it=m.begin();it!=m.end();it++) {
		std::pair<T,U> p=*it;
		r.insert(p.second);
	}
	return r;
}

/**
 * Find exact (key,value) pair in a multimap
 * There must be an implicit type conversion between typeMultimapKey and typeKey
 * There must be an implicit type conversion between typeMultimapValue and typeValue
 * @param m
 * @param first
 * @param second
 * @return an iterator to the first pair found or multimap::end() if not found
 */
template <typename typeMultimapKey, typename typeMultimapValue, typename typeKey, typename typeValue> typename std::multimap<typeMultimapKey,typeMultimapValue>::iterator find(std::multimap<typeMultimapKey,typeMultimapValue> & m, const typeKey & first, const typeValue & second){
	std::pair<typename std::multimap<typeMultimapKey,typeMultimapValue>::iterator,typename std::multimap<typeMultimapKey,typeMultimapValue>::iterator> a=m.equal_range(first);
	for(typename std::multimap<typeMultimapKey,typeMultimapValue>::iterator it=a.first;it!=a.second;it++){
		std::pair<typeMultimapKey,typeMultimapValue> p=*it;
		if(p.second==second){
			return it;
		}
	}
	//link does not exist
	return m.end();
}

/**
 * Find exact (key,value) pair in a multimap
 * Const version
 * There must be an implicit type conversion between typeMultimapKey and typeKey
 * There must be an implicit type conversion between typeMultimapValue and typeValue
 * @param m
 * @param first
 * @param second
 * @return an iterator to the first pair found or multimap::end() if not found
 */
template <typename typeMultimapKey, typename typeMultimapValue, typename typeKey, typename typeValue> typename std::multimap<typeMultimapKey,typeMultimapValue>::const_iterator find(const std::multimap<typeMultimapKey,typeMultimapValue> & m, const typeKey & first, const typeValue & second){
	std::pair<typename std::multimap<typeMultimapKey,typeMultimapValue>::const_iterator,typename std::multimap<typeMultimapKey,typeMultimapValue>::const_iterator> a=m.equal_range(first);
	for(typename std::multimap<typeMultimapKey,typeMultimapValue>::const_iterator it=a.first;it!=a.second;it++){
		const std::pair<typeMultimapKey,typeMultimapValue> & p=*it;
		if(p.second==second){
			return it;
		}
	}
	//link does not exist
	return m.cend();
}

/**
 * Default equalValues function used by the find method declared next
 * @param first
 * @param second
 * @return
 */
template <typename T, typename U> bool equalValues(const T & first, const U & second){
	return (first==second);
}

/**
 * Find exact (key,value) pair in a multimap using comparator
 * There must be an implicit type conversion between typeMultimapKey and typeKey
 * There must be an implicit type conversion between typeMultimapValue and typeValue if using the default comparator
 * @param m
 * @param first
 * @param second
 * @return an iterator to the first pair found or multimap::end() if not found
 */
template <typename typeMultimapKey, typename typeMultimapValue, typename typeKey, typename typeValue> typename std::multimap<typeMultimapKey,typeMultimapValue>::iterator find(std::multimap<typeMultimapKey,typeMultimapValue> & m, const typeKey & first, const typeValue & second, bool (*equalValues)(const typeMultimapValue &, const typeValue &)){
	std::pair<typename std::multimap<typeMultimapKey,typeMultimapValue>::iterator,typename std::multimap<typeMultimapKey,typeMultimapValue>::iterator> a=m.equal_range(first);
	for(typename std::multimap<typeMultimapKey,typeMultimapValue>::iterator it=a.first;it!=a.second;it++){
		std::pair<typeMultimapKey,typeMultimapValue> p=*it;
		if(equalValues(p.second,second)){
			return it;
		}
	}
	//link does not exist
	return m.end();
}

/**
 * Find exact (key,value) pair in a multimap using comparator
 * There must be an implicit type conversion between typeMultimapKey and typeKey
 * There must be an implicit type conversion between typeMultimapValue and typeValue if using the default comparator
 * @param m
 * @param first
 * @param second
 * @return an iterator to the first pair found or multimap::end() if not found
 */
template <typename typeMultimapKey, typename typeMultimapValue, typename typeKey, typename typeValue> typename std::multimap<typeMultimapKey,typeMultimapValue>::const_iterator find(const std::multimap<typeMultimapKey,typeMultimapValue> & m, const typeKey & first, const typeValue & second, bool (*equalValues)(const typeMultimapValue &, const typeValue &)){
	std::pair<typename std::multimap<typeMultimapKey,typeMultimapValue>::const_iterator,typename std::multimap<typeMultimapKey,typeMultimapValue>::const_iterator> a=m.equal_range(first);
	for(typename std::multimap<typeMultimapKey,typeMultimapValue>::const_iterator it=a.first;it!=a.second;it++){
		const std::pair<typeMultimapKey,typeMultimapValue> & p=*it;
		if(equalValues(p.second,second)){
			return it;
		}
	}
	//link does not exist
	return m.cend();
}

template<typename T, typename U,typename std::enable_if<std::is_fundamental<T>::value && std::is_fundamental<U>::value,T>::type=0 > std::string toString(std::multimap<T, U> const& m, const StringFormatter & formater=defaultStringFormatter) {
	std::stringstream ss;
	formater.start(ss,true);
	for(typename std::multimap<T,U>::const_iterator it=m.cbegin();it!=m.cend();it++){
		const std::pair<T,U> & p=*it;
		ss << formater.tupleOpen() << p.first << formater.valueSeparator() << p.second << formater.tupleClose();
	}
	formater.end(ss,true);
	return ss.str();
}

template<typename T, typename U,typename std::enable_if<std::is_fundamental<T>::value && std::is_fundamental<U>::value,T>::type=0 > std::string debugPrint(std::multimap<T, U> const& m) {
	std::stringstream ss;
	for(typename std::multimap<T,U>::const_iterator it=m.cbegin();it!=m.cend();it++){
		const std::pair<T,U> & p=*it;
		ss << p.first << "+" << p.second << ";";
	}
	return ss.str();
}

/**
 * get the minimum value attributed to a given key
 * @param map
 * @param key
 * @return an iterator to the pair containing the minimum value or multimap::end() if not found
 */
template<typename T, typename U> typename std::multimap<T,U>::iterator & minimumValue(std::multimap<T,U> & m, const T & key){
	std::pair<typename std::multimap<T,U>::iterator,typename std::multimap<T,U>::iterator> a=m.equal_range(key);
	typename std::multimap<T,U>::iterator itm=m.end();
	U min;
	bool first=true;
	for(typename std::multimap<T,U>::iterator it=a.first;it!=a.second;it++){
		std::pair<T,U> & p=*it;
		U & v=p.second;
		if(first){
			//can not compare min with v if it is the first value (min has no value yet)
			min=v;
			first=false;
		}
		else{
			if(v<min){//use < because it is the default weak comparison operator for most classes
				min=v;
				itm=it;
			}
		}
	}
	return itm;
}


}  // namespace multimap

#endif /* MULTIMAPUTILITIES_H_ */
