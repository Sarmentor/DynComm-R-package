/************************************************************************
 ************************* Developer Notice *****************************
 ************************************************************************
 * @details
 *
 * Doubly indexed map implemented in C++11.
 *
 * Unlike an ordinary std::map which is indexed by key only, this map is
 * also indexed by value.
 *
 *
 * @author poltergeist0
 *
 * @date 2018-08-19
 ************************************************************************
 ************************************************************************
 ************************************************************************/

#ifndef MAPREVERSABLE_H_
#define MAPREVERSABLE_H_

#include <map>
#include <set>
#include <list>
#include <vector>
#include "mapUtilities.h"
#include "multimapUtilities.h"
#include <iostream>

/**
 * @brief Doubly indexed map
 *
 * @details
 * Class that implements a map in which the values are also indexed.
 * Unlike an ordinary std::map which is indexed by key only, this map is
 * also indexed by value.
 * It internally uses a std::multimap to achieve this.
 *
 *
 * @author poltergeist0
 *
 * @date 2018-08-19
 */
template <typename T, typename U> class MapReversable{
private:
	std::map<T,U> m; //direct listing
	std::multimap<U,T> r;//reverse listing

	typedef typename std::map<T,U>::iterator iterator;
	typedef typename std::multimap<U,T>::iterator range_iterator;

public:

	typedef typename std::map<T,U>::const_iterator const_iterator;
	typedef std::pair<T,U> typePair;
	typedef typename std::multimap<U,T>::const_iterator range_iterator_const;
	typedef std::pair<U,T> typeRangePair;
	typedef std::pair<range_iterator_const,range_iterator_const> typeRange;

	/**
	 * default constructor
	 */
	MapReversable(){}

	const size_t size() const {return m.size();}

	/**
	 * Find the value attributed to a given key
	 * @param key
	 * @return a constant iterator that points to the pair that holds the (key, value) or cend() if not found
	 */
	const_iterator find(const T& key)const{return m.find(key);}

	/**
	 * Compatibility function with multimap since in a map all keys are unique
	 * @return all unique keys in the map
	 */
	std::set<T> keys()const {return keys(m);}

	/**
	 * get all the keys that have a given value
	 * @param value
	 * @return a pair of constant range iterators pointing, respectively, to the beginning and to the end of the range of keys that holds the given value
	 */
	range_iterator_const key(const U & value)const {return r.find(value);}

	/**
	 * get all the keys that have a given value
	 * @param value
	 * @return a pair of constant range iterators pointing, respectively, to the beginning and to the end of the range of keys that holds the given value
	 */
	typeRange keys(const U & value)const {return r.equal_range(value);}

	/**
	 *
	 * @return all unique values in the map
	 */
	std::set<T> values()const {return values(m);}

	const_iterator value(const T& key)const{return find(key);}

	/**
	 *
	 * @return a constant iterator to the beginning of the map
	 */
	const_iterator cbegin()const{return m.cbegin();}
	/**
	 *
	 * @return a constant iterator to the end of the map
	 */
	const_iterator cend()const{return m.cend();}

	/**
	 *
	 * @return a constant iterator to the beginning of the multimap
	 */
	range_iterator_const cbeginr()const{return r.cbegin();}
	/**
	 *
	 * @return a constant iterator to the end of the multimap
	 */
	range_iterator_const cendr()const{return r.cend();}

	/**
	 * Get value associated with key. If the key entry does not exists it is created with the default value
	 * @param key
	 * @return either the existing value or the newly created value
	 */
	U operator[](const T & key) const {
		U a;
		const_iterator it=m.find(key);
		if(it!=m.cend()){//value exists
			a=it->second;
		}
		return a;
	}

	/**
	 * Add a (key, value) pair to the map
	 * @param key
	 * @param value
	 * @param replace , if true and key exists, replaces the value otherwise fails. If false and key exists fails.
	 * @return true if insertion succeeds
	 */
	bool add(const T & key, const U & value, const bool & replace=false){
		bool result=false;
		//try to insert pair
		std::pair<iterator,bool>a=m.insert(std::make_pair(key,value));
		if(a.second){//element does not exist
			//insert entry in the reverse list
			r.insert(std::make_pair(value,key));
			result=true;
		}
		else{//element exists
			if(replace){//if replace is true
				const std::pair<T,U> & b=*(a.first);
				//try to find the existing pair in the reverse listing
				range_iterator it=multimap::find(r,b.second,b.first);
				if(it!=r.end()) r.erase(it); //erase reverse entry
				m.erase(a.first);//erase entry
				m.insert(std::make_pair(key,value));//insert entry
				r.insert(std::make_pair(value,key));//insert reverse entry
				result=true;
			}
		}
		return result;
	}

	/**
	 * remove a (key,value) pair
	 * @param first
	 * @param second
	 * @return true if the element existed otherwise false
	 */
	bool remove(const T & first, const U & second){
		bool result=false;
		iterator a=m.find(first);//find pair
		if(a!=m.end()){//element exists
			std::pair<T,U> b=*a;
			//try find in reverse listing
			range_iterator it=multimap::find(r,b.second,b.first);
			if(it!=r.end()) r.erase(it);//erase reverse entry
			m.erase(a);//erase entry
			result=true;
		}
		return result;
	}

	/**
	 * remove a (key,value) pair
	 * @param first
	 * @return true if the element existed otherwise false
	 */
	bool remove(const T & first){
		bool result=false;
		iterator a=m.find(first);//find pair
		if(a!=m.end()){//element exists
			std::pair<T,U> b=*a;
			//try find in reverse listing
			range_iterator it=multimap::find(r,b.second,b.first);
			r.erase(it);//erase reverse entry
			m.erase(a);//erase entry
			result=true;
		}
		return result;
	}

	/**
	 * Replace all occurrences of oldValue by newValue
	 * @param oldValue
	 * @param newValue
	 * @return true if replacement succeeded
	 */
	bool replace(const U & oldValue, const U & newValue){
		if(oldValue==newValue)return true;//ignore if values are the same
		std::pair<range_iterator,range_iterator> p=r.equal_range(oldValue);//find range with old value
		if(p.first==p.second)return false;
		std::set<T> s;
		for(range_iterator_const it=p.first;it!=p.second;++it){//for all keys in range
			const std::pair<U,T> & a=*it;//get pair
			s.insert(a.second);//get key
		}
		r.erase(oldValue);//erase old value reverse entry
		for(typename std::set<T>::const_iterator it=s.cbegin();it!=s.cend();++it){//for all keys in range
			const T & k=*it;//get key
			m.erase(k);//erase key entry
			m.insert(std::make_pair(k,newValue));//insert new value entry
			r.insert(std::make_pair(newValue,k));//insert new value reverse entry
		}
		return true;
	}

	/**
	 * Replace all occurrences of oldKey by newKey
	 * @param oldKey
	 * @param newKey
	 * @return true if replacement succeeded
	 */
	bool replaceKey(const T & oldKey, const T & newKey){
		if(oldKey==newKey)return true;//ignore if values are the same
		iterator it=m.find(oldKey);//find oldKey
		if(it==m.end())return false;
		const U & v=it->second;
		m.insert(std::make_pair(newKey,v));//insert new value entry
		r.insert(std::make_pair(v,newKey));//insert new value reverse entry
		r.erase(multimap::find(r,v,oldKey));//erase old value reverse entry
		m.erase(it);
		return true;
	}

	/**
	 *
	 * @return a string representation of this map list and reverse list
	 */
	std::string toString(const StringFormatter & sf=defaultStringFormatter)const{
		StringFormatter f=sf;
		std::stringstream ss;
		if(!sf.isDefault()){
			f.build(ss,"");
			++f;
		}
		f.header("map=");
		ss << map::toString(m,f);
		f.header("reverse map=");
		ss << multimap::toString(r,f);
		return ss.str();
	}

	std::string debugPrint()const{
		std::stringstream ss;
		ss << "m" << map::debugPrint(m) << "\n";
		ss << "r"<< multimap::debugPrint(r);
		return ss.str();
	}

};


#endif /* MAPREVERSABLE_H_ */
