/************************************************************************
 ************************* Developer Notice *****************************
 ************************************************************************
 * @details
 *
 * Doubly indexed multimap implemented in C++11.
 *
 * Unlike an ordinary std::multimap which is indexed by key only, this
 * multimap is also indexed by value.
 *
 *
 * @author poltergeist0
 *
 * @date 2018-08-19
 ************************************************************************
 ************************************************************************
 ************************************************************************/

#ifndef MULTIMAPREVERSABLE_H_
#define MULTIMAPREVERSABLE_H_

#include <map>
#include <set>
#include <list>
#include "multimapUtilities.h"
#include <iostream>

/**
 * @brief Doubly indexed multimap
 *
 * @details
 * Class that implements a multimap in which the values are also indexed.
 * Unlike an ordinary std::multimap which is indexed by key only, this
 * multimap is also indexed by value.
 *
 *
 * @author poltergeist0
 *
 * @date 2018-08-19
 */
template <typename T, typename U> class MultimapReversable{
private:
	std::multimap<T,U> f; // forward listing
	std::multimap<U,T> r;//reverse listing

	typedef typename std::multimap<T,U>::iterator iterator;
	typedef typename std::multimap<U,T>::iterator range_iterator;

	typedef bool (*equalValues)(const U & , const U & );

	equalValues eq;

public:

	typedef typename std::multimap<T,U>::const_iterator const_iterator;
	typedef typename std::multimap<U,T>::const_iterator const_range_iterator;

	/**
	 * default constructor
	 */
	MultimapReversable():eq(multimap::equalValues){}

	MultimapReversable(equalValues e):eq(e){}

	/**
	 * Find the value attributed to a given key
	 * @param key
	 * @return a constant iterator that points to the pair that holds the (key, value) or cend() if not found
	 */
	const_iterator find(const T& key)const{return f.find(key);}

	/**
	 * Compatibility function with multimap since in a map all keys are unique
	 * @return all unique keys in the map
	 */
	std::set<T> keys()const {return keys(f);}

	/**
	 * get all the keys that have a given value
	 * @param value
	 * @return a pair of constant range iterators pointing, respectively, to the beginning and to the end of the range of keys that holds the given value
	 */
	std::pair<const_range_iterator,const_range_iterator> keys(const U & value)const {return r.equal_range(value);}

	/**
	 *
	 * @return all unique values in the map
	 */
	std::set<T> values()const {return values(f);}

	/**
	 *
	 * @return a constant iterator to the beginning of the map
	 */
	const_iterator & cbegin()const{return f.cbegin();}
	/**
	 *
	 * @return a constant iterator to the end of the map
	 */
	const_iterator cend()const{return f.cend();}


	/**
	 * Add a (key, value) pair to the map
	 * @param key
	 * @param value
	 * @param replace , if true and key exists, replaces the value otherwise fails. If false and key exists fails.
	 * @return true if insertion succeeds
	 */
	bool add(const T & key, const U & value, const bool & replace=false){
		//try to insert pair
		iterator a=multimap::find(f,key,value,eq);
		if(a==f.end()){//element does not exist
			//insert entry in the reverse list
			f.insert(std::make_pair(key,value));
			r.insert(std::make_pair(value,key));
			return true;
		}
		else{//element exists
			if(replace){//if replace is true
				const std::pair<T,U> & b=*a;
				//try to find the existing pair in the reverse listing
				range_iterator it=multimap::find(r,b.second,b.first);
				if(it!=r.end()) r.erase(it); //erase reverse entry
				f.erase(a);//erase entry
				f.insert(std::make_pair(key,value));//insert entry
				r.insert(std::make_pair(value,key));//insert reverse entry
				return true;
			}
		}
		return false;
	}

	/**
	 * remove a (key,value) pair
	 * @param first
	 * @param second
	 * @return true if the element existed otherwise false
	 */
	bool remove(const T & first, const U & second){
		iterator a=f.find(first);//find pair
		if(a!=f.end()){//element exists
			std::pair<T,U> b=*a;
			//try find in reverse listing
			range_iterator it=multimap::find(r,b.first,b.second);
			if(it!=r.end()) r.erase(it);//erase reverse entry
			f.erase(a);//erase entry
			return true;
		}
		return false;
	}

	/**
	 * Replace all occurrences of oldValue by newValue
	 * @param oldValue
	 * @param newValue
	 */
	void replace(const U & oldValue, const U & newValue){
		if(oldValue==newValue)return;//ignore if values are the same
		std::pair<range_iterator,range_iterator> p=r.equal_range(oldValue);//find range with old value
		for(const_iterator it=p.first;it!=p.second;it++){//for all keys in range
			const std::pair<U,T> & a=*it;//get pair
			const T & k=a.second;//get key
			f.erase(k);//erase key entry
			f.insert(std::make_pair(k,newValue));//insert new value entry
			r.insert(std::make_pair(newValue,k));//insert new value reverse entry
		}
		r.erase(oldValue);//erase old value reverse entry
	}

	/**
	 *
	 * @return a string representation of this map list and reverse list
	 */
	std::string toString()const{
		std::stringstream ss;
		ss << "map:";
		ss << multimap::toString(f);
		ss << "reverse map:";
		ss << multimap::toString(r);
		return ss.str();
	}
};


#endif /* MULTIMAPREVERSABLE_H_ */
