/*
 * mapReversable.h
 *
 *  Created on: 19/08/2018
 *      Author: poltergeist0
 */

#ifndef MULTIMAPREVERSABLE_H_
#define MULTIMAPREVERSABLE_H_

#include <map>
#include <set>
#include <list>
//#include "mapUtilities.h"
#include "multimapUtilities.h"
#include <iostream>
//#include "defines.h"

/**
 * class that implements a map in which the values are also indexed
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

//	iterator & begin()const{return m.begin();}
//	iterator & end()const{return m.end();}
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
//				static_assert(it==r.end(),"Implementation error. Missing reverse element.");
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
//			static_assert(it==r.end(),"Implementation error. Missing reverse element.");
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

//	const_range_iterator & minimumKey(const U & value){
//		range_iterator it=
//
//	}

	/**
	 *
	 * @return a string representation of this map list and reverse list
	 */
	std::string toString()const{
		std::stringstream ss;
		ss << "map:";
//		for(typename std::map<T,U>::const_iterator it=m.begin();it!=m.end();it++){
//			const std::pair<T,U> & p=*it;
//			ss << "( " << p.first << " , " << p.second << " )" << endl;
//		}
		ss << multimap::toString(f);
		ss << "reverse map:";
//		for(typename std::multimap<U,T>::const_iterator it=r.begin();it!=r.end();it++){
//			const std::pair<U,T> & p=*it;
//			ss << "( " << p.first << " , " << p.second << " )" << endl;
//		}
		ss << multimap::toString(r);
		return ss.str();
	}
};

/**
 * class that implements a map in which the values (U) are also indexed
 * V provides an additional storage that is not indexed
 */
//template <typename T, typename U,typename V> class MapReversable2{
//private:
//	typedef std::list<V> typeStorage;
//	typedef typename std::list<V>::iterator typeStorageIterator;
//	typedef typename std::list<V>::const_iterator typeStorageIteratorConst;
//	typedef std::pair<U,typeStorageIteratorConst> pair;
//	typedef std::pair<T,typeStorageIteratorConst> rpair;
//
//	typeStorage storage;//additional storage
//	std::map<T,pair> m; // map
//	std::multimap<U,rpair> r;//reverse listing
//
//	typedef typename std::map<T,pair>::iterator iterator;
//	typedef typename std::multimap<T,rpair>::iterator range_iterator;
//
//public:
//
//	typedef typename std::map<T,pair>::const_iterator const_iterator;
//	typedef typename std::multimap<T,rpair>::const_iterator const_range_iterator;
//
//	/**
//	 * default constructor
//	 */
//	MapReversable2(){}
//
//	/**
//	 * Find the value attributed to a given key
//	 * @param key
//	 * @return a constant iterator that points to the pair that holds the (key, value) or cend() if not found
//	 */
//	const_iterator find(const T& key)const{return m.find(key);}
//
//	/**
//	 * Compatibility function with multimap since in a map all keys are unique
//	 * @return all unique keys in the map
//	 */
//	std::set<T> keys()const {return keys(m);}
//
//	/**
//	 * get all the keys that have a given value
//	 * @param value
//	 * @return a pair of constant range iterators pointing, respectively, to the beginning and to the end of the range of keys that holds the given value
//	 */
//	std::pair<const_range_iterator,const_range_iterator> keys(const U & value)const {return r.equal_range(value);}
//
//	/**
//	 *
//	 * @return all unique values in the map
//	 */
//	std::set<T> values()const {return values(m);}
//
////	iterator & begin()const{return m.begin();}
////	iterator & end()const{return m.end();}
//	/**
//	 *
//	 * @return a constant iterator to the beginning of the map
//	 */
//	const_iterator & cbegin()const{return m.cbegin();}
//	/**
//	 *
//	 * @return a constant iterator to the end of the map
//	 */
//	const_iterator cend()const{return m.cend();}
//
//
//	/**
//	 * Add a (key, value) pair to the map
//	 * @param key
//	 * @param value
//	 * @param replace , if true and key exists, replaces the value otherwise fails. If false and key exists fails.
//	 * @return true if insertion succeeds
//	 */
//	bool add(const T & key, const U & value, const V & additional, const bool & replace=false){
//		//try to insert pair
//		std::pair<iterator,bool>a=m.insert(std::make_pair(key,std::make_pair(value,additional)));
//		if(a.second){//element does not exist
//			//insert entry in the reverse list
//			typeStorageIteratorConst it=storage.insert(storage.end(),additional);
//			r.insert(std::make_pair(value,std::make_pair(key,it)));
//			return true;
//		}
//		else{//element exists
//			if(replace){//if replace is true
//				const std::pair<T,pair> & b=*(a.first);
//				//try to find the existing pair in the reverse listing
//				range_iterator it=multimap::find(r,b.second,b.first);
////				static_assert(it==r.end(),"Implementation error. Missing reverse element.");
//				if(it!=r.end()) r.erase(it); //erase reverse entry
//				m.erase(a.first);//erase entry
//				m.insert(std::make_pair(key,value));//insert entry
//				r.insert(std::make_pair(value,key));//insert reverse entry
//				return true;
//			}
//		}
//		return false;
//	}
//
//	/**
//	 * remove a (key,value) pair
//	 * @param first
//	 * @param second
//	 * @return true if the element existed otherwise false
//	 */
//	bool remove(const T & first, const U & second){
//		iterator a=m.find(first);//find pair
//		if(a!=m.end()){//element exists
//			std::pair<T,U> b=*a;
//			//try find in reverse listing
//			range_iterator it=multimap::find(r,b.first,b.second);
////			static_assert(it==r.end(),"Implementation error. Missing reverse element.");
//			if(it!=r.end()) r.erase(it);//erase reverse entry
//			m.erase(a);//erase entry
//			return true;
//		}
//		return false;
//	}
//
//	/**
//	 * Replace all occurrences of oldValue by newValue
//	 * @param oldValue
//	 * @param newValue
//	 */
//	void replace(const U & oldValue, const U & newValue){
//		if(oldValue==newValue)return;//ignore if values are the same
//		std::pair<range_iterator,range_iterator> p=r.equal_range(oldValue);//find range with old value
//		for(const_iterator it=p.first;it!=p.second;it++){//for all keys in range
//			const std::pair<U,T> & a=*it;//get pair
//			const T & k=a.second;//get key
//			m.erase(k);//erase key entry
//			m.insert(std::make_pair(k,newValue));//insert new value entry
//			r.insert(std::make_pair(newValue,k));//insert new value reverse entry
//		}
//		r.erase(oldValue);//erase old value reverse entry
//	}
//
//	/**
//	 *
//	 * @return a string representation of this map list and reverse list
//	 */
//	std::string toString()const{
//		std::stringstream ss;
//		ss << "map:";
////		for(typename std::map<T,U>::const_iterator it=m.begin();it!=m.end();it++){
////			const std::pair<T,U> & p=*it;
////			ss << "( " << p.first << " , " << p.second << " )" << endl;
////		}
//		ss << map::toString(m);
//		ss << "reverse map:";
////		for(typename std::multimap<U,T>::const_iterator it=r.begin();it!=r.end();it++){
////			const std::pair<U,T> & p=*it;
////			ss << "( " << p.first << " , " << p.second << " )" << endl;
////		}
//		ss << multimap::toString(r);
//		return ss.str();
//	}
//};

#endif /* MULTIMAPREVERSABLE_H_ */
