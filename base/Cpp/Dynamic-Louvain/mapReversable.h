/*
 * mapReversable.h
 *
 *  Created on: 19/08/2018
 *      Author: poltergeist0
 */

#ifndef MAPREVERSABLE_H_
#define MAPREVERSABLE_H_

#include "debugging.h"
#include <map>
#include <set>
#include <list>
#include <vector>
#include "mapUtilities.h"
#include "multimapUtilities.h"
#include <iostream>
//#include "defines.h"

/**
 * class that implements a map in which the values are also indexed
 */
template <typename T, typename U> class MapReversable{
private:
	std::map<T,U> m; //direct listing
	std::multimap<U,T> r;//reverse listing

	/******************************************************************************
	 ******************************************************************************
	 ******************************************************************************
	 * Functions used for testing ONLY
	 * Allows direct access to private members of the class to verify correct
	 * implementation
	 ******************************************************************************
	 ******************************************************************************
	 ******************************************************************************/
#ifdef TEST

public:
	const std::map<T,U> & testGetListing() const {return m;}

	const std::multimap<U,T> & testGetReverseListing() const {return r;}

#endif	//TEST

private:
	/******************************************************************************
	 ******************************************************************************
	 ******************************************************************************
	 * Functions used for debugging ONLY
	 * Verify consistency of the class member as a set
	 ******************************************************************************
	 ******************************************************************************
	 ******************************************************************************/
#ifndef NDEBUG
	std::map<T,U> debugm; //direct listing
	std::multimap<U,T> debugr;//reverse listing
#endif	//NDEBUG

	void debugVerify() const{
#ifndef NDEBUG
		//verify that sizes match
		ASSERT_EQUAL_MESSAGE(m.size(),r.size(),this->toString());
		//verify that values match in direct and reverse listing
		for(typename std::map<T,U>::const_iterator it=m.cbegin();it!=m.cend();++it){
			const std::pair<T,U> & p=*it;
			ASSERT_NOT_EQUAL_ITERATOR_MESSAGE(multimap::find(r,p.second,p.first),r.cend(),this->toString());
		}
#endif	//NDEBUG
	}

	static std::vector<typename std::map<T,U>::const_iterator> debugVerify(const std::map<T,U> & m1,const std::map<T,U> & m2,const std::multimap<U,T> & r1,const std::multimap<U,T> & r2, const T & key, const U & value, const bool & excludeKey, const bool & excludeValue){
		//verify if old values are still present except for the excluded key/value
		std::vector<typename std::map<T,U>::const_iterator> res;
#ifndef NDEBUG
		for(typename std::map<T,U>::const_iterator it=m1.cbegin();it!=m1.cend();++it){
			const std::pair<T,U> & p=*it;
			if((excludeKey && p.first==key)||(excludeValue && p.second==value)){//if it was excluded
				//add to list of excluded
				res.push_back(it);
			}
			else{// if it was not excluded
				ASSERT_NOT_EQUAL_ITERATOR(m2.find(p.first),m2.cend());
				ASSERT_EQUAL_ITERATOR((m2.find(p.first))->second,p.second);
				ASSERT_NOT_EQUAL_ITERATOR(multimap::find(r1,p.second,p.first),r1.cend());
				ASSERT_NOT_EQUAL_ITERATOR(multimap::find(r2,p.second,p.first),r2.cend());
			}
		}
#endif	//NDEBUG
		return res;
	}

	void debugSaveState(){
		debugVerify();
#ifndef NDEBUG
		//copy listings
		debugm=std::map<T,U>(m);
		debugr=std::multimap<U,T>(r);
		//verify sizes
		ASSERT_EQUAL_MESSAGE(debugm.size(),m.size(),this->toString());
		ASSERT_EQUAL_MESSAGE(debugm.size(),debugr.size(),this->toString());
		debugVerify(debugm,m,debugr,r,0,0,false,false);
#endif	//NDEBUG
	}

	void debugAdd(const bool & added,const T & key, const U & value, const bool & replace=false) const{
		debugVerify();
#ifndef NDEBUG
		//verify sizes
		if(added){
			if(debugm.size()>0 && debugm.find(key)!=debugm.cend()){//entry existed
				//might have been replaced or not. Irrelevant for checking sizes
				ASSERT_EQUAL_MESSAGE(m.size(),debugm.size(),this->toString());
				ASSERT_EQUAL_MESSAGE(r.size(),debugr.size(),this->toString());
			}
			else{//entry was added
				ASSERT_EQUAL_MESSAGE(m.size(),debugm.size()+1,this->toString());
				ASSERT_EQUAL_MESSAGE(r.size(),debugr.size()+1,this->toString());
			}
			//verify new value was added/replaced
			ASSERT_NOT_EQUAL_ITERATOR_MESSAGE(m.find(key),m.cend(),this->toString());
			ASSERT_EQUAL_ITERATOR_MESSAGE((m.find(key))->second,value,this->toString());
			ASSERT_NOT_EQUAL_ITERATOR_MESSAGE(multimap::find(r,value,key),r.cend(),this->toString());
		}
		else{//operation failed
			ASSERT_EQUAL_MESSAGE(m.size(),debugm.size(),this->toString());
			ASSERT_EQUAL_MESSAGE(r.size(),debugr.size(),this->toString());
		}
		debugVerify(debugm,m,debugr,r,key,value,true,false);
#endif	//NDEBUG
	}

	void debugRemove(const bool & removed,const T & key, const U & value) const{
		debugVerify();
#ifndef NDEBUG
		//verify sizes
		if(removed){
			ASSERT_EQUAL_MESSAGE(m.size(),debugm.size()-1,this->toString());
			ASSERT_EQUAL_MESSAGE(r.size(),debugr.size()-1,this->toString());
			//verify value was removed
			ASSERT_EQUAL_ITERATOR_MESSAGE(m.find(key),m.cend(),this->toString());
			ASSERT_EQUAL_ITERATOR_MESSAGE(multimap::find(r,value,key),r.cend(),this->toString());
		}
		else{
			ASSERT_EQUAL_MESSAGE(m.size(),debugm.size(),this->toString());
			ASSERT_EQUAL_MESSAGE(r.size(),debugr.size(),this->toString());
		}
		ASSERT_EQUAL_ITERATOR_MESSAGE(m.find(key),m.cend(),this->toString());
		ASSERT_EQUAL_ITERATOR_MESSAGE(multimap::find(r,value,key),r.cend(),this->toString());
		debugVerify(debugm,m,debugr,r,key,value,true,false);
#endif	//NDEBUG
	}

	void debugRemove(const bool & removed,const T & key) const{
		debugVerify();
#ifndef NDEBUG
		//verify sizes
		if(removed){
			ASSERT_EQUAL_MESSAGE(m.size(),debugm.size()-1,this->toString());
			ASSERT_EQUAL_MESSAGE(r.size(),debugr.size()-1,this->toString());
			//verify value was removed
			ASSERT_EQUAL_ITERATOR_MESSAGE(m.find(key),m.cend(),this->toString());
			ASSERT_EQUAL_ITERATOR_MESSAGE(multimap::find(r,(debugm.find(key))->second,key),r.cend(),this->toString());
		}
		else{
			ASSERT_EQUAL_MESSAGE(m.size(),debugm.size(),this->toString());
			ASSERT_EQUAL_MESSAGE(r.size(),debugr.size(),this->toString());
		}
		debugVerify(debugm,m,debugr,r,key,0,true,false);
#endif	//NDEBUG
	}

	void debugReplace(const U & oldValue, const U & newValue) const{
		debugVerify();
#ifndef NDEBUG
		//verify sizes
		ASSERT_EQUAL_MESSAGE(m.size(),debugm.size(),this->toString());
		ASSERT_EQUAL_MESSAGE(r.size(),debugr.size(),this->toString());
		//verify values
		std::vector<typename std::map<T,U>::const_iterator> res=debugVerify(debugm,m,debugr,r,0,oldValue,false,true);
		for(typename std::vector<typename std::map<T,U>::const_iterator>::const_iterator it=res.cbegin();it!=res.cend();++it){
			const typename std::map<T,U>::const_iterator & q=*it;
			const std::pair<T,U> & p=*q;
			ASSERT_NOT_EQUAL_ITERATOR_MESSAGE(m.find(p.first),m.cend(),this->toString());
			ASSERT_EQUAL_ITERATOR_MESSAGE((m.find(p.first))->second,newValue,this->toString());
			ASSERT_NOT_EQUAL_ITERATOR_MESSAGE(multimap::find(r,newValue,p.first),r.cend(),this->toString());
		}
#endif	//NDEBUG
	}

	void debugReplaceKey(const T & oldKey, const T & newKey) const{
		debugVerify();
#ifndef NDEBUG
		//verify sizes
		ASSERT_EQUAL_MESSAGE(m.size(),debugm.size(),this->toString());
		ASSERT_EQUAL_MESSAGE(r.size(),debugr.size(),this->toString());
		ASSERT_NOT_EQUAL_ITERATOR_MESSAGE(m.find(newKey),m.cend(),this->toString());
		ASSERT_EQUAL_ITERATOR_MESSAGE((m.find(newKey))->second,(debugm.find(oldKey))->second,this->toString());
		ASSERT_NOT_EQUAL_ITERATOR_MESSAGE(multimap::find(r,(debugm.find(oldKey))->second, newKey),r.cend(),this->toString());
		debugVerify(debugm,m,debugr,r,oldKey,0,true,false);
#endif	//NDEBUG
	}

private:

	typedef typename std::map<T,U>::iterator iterator;
	typedef typename std::multimap<U,T>::iterator range_iterator;

public:

	typedef typename std::map<T,U>::const_iterator const_iterator;
	typedef std::pair<T,U> typePair;
	typedef typename std::multimap<U,T>::const_iterator const_range_iterator;
	typedef std::pair<U,T> typeRangePair;
	typedef std::pair<const_range_iterator,const_range_iterator> typeRange;

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
	const_range_iterator key(const U & value)const {return r.find(value);}

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

//	iterator & begin()const{return m.begin();}
//	iterator & end()const{return m.end();}
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
	const_range_iterator cbeginr()const{return r.cbegin();}
	/**
	 *
	 * @return a constant iterator to the end of the multimap
	 */
	const_range_iterator cendr()const{return r.cend();}

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
//		else{
//			a=m[key];//create with default value
//			r.insert(std::make_pair(a,key));//insert in reverse listing
//		}
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
		debugSaveState();
		T debugKey=key;
		U debugValue=value;
		bool debugReplace=replace;
		bool result=false;
		//try to insert pair
		std::pair<iterator,bool>a=m.insert(std::make_pair(key,value));
		if(a.second){//element does not exist
			//insert entry in the reverse list
			r.insert(std::make_pair(value,key));
//			return true;
			result=true;
		}
		else{//element exists
			if(replace){//if replace is true
				const std::pair<T,U> & b=*(a.first);
				//try to find the existing pair in the reverse listing
				range_iterator it=multimap::find(r,b.second,b.first);
//				static_assert(it==r.end(),"Implementation error. Missing reverse element.");
				if(it!=r.end()) r.erase(it); //erase reverse entry
				m.erase(a.first);//erase entry
				m.insert(std::make_pair(key,value));//insert entry
				r.insert(std::make_pair(value,key));//insert reverse entry
//				return true;
				result=true;
			}
		}
//		return false;
		debugAdd(result,debugKey,debugValue,debugReplace);
		return result;
	}

	/**
	 * remove a (key,value) pair
	 * @param first
	 * @param second
	 * @return true if the element existed otherwise false
	 */
	bool remove(const T & first, const U & second){
		debugSaveState();
		T debugFirst=first;
		U debugSecond=second;
		bool result=false;
		iterator a=m.find(first);//find pair
		if(a!=m.end()){//element exists
			std::pair<T,U> b=*a;
			//try find in reverse listing
			range_iterator it=multimap::find(r,b.second,b.first);
//			static_assert(it==r.end(),"Implementation error. Missing reverse element.");
			if(it!=r.end()) r.erase(it);//erase reverse entry
			m.erase(a);//erase entry
//			return true;
			result=true;
		}
//		return false;
		debugRemove(result,debugFirst,debugSecond);
		return result;
	}

	/**
	 * remove a (key,value) pair
	 * @param first
	 * @return true if the element existed otherwise false
	 */
	bool remove(const T & first){
		debugSaveState();
		T debugFirst=first;
		bool result=false;
		iterator a=m.find(first);//find pair
		if(a!=m.end()){//element exists
			std::pair<T,U> b=*a;
			//try find in reverse listing
			range_iterator it=multimap::find(r,b.second,b.first);
//			static_assert(it==r.end(),"Implementation error. Missing reverse element.");
//			if(it!=r.end())
			r.erase(it);//erase reverse entry
			m.erase(a);//erase entry
//			return true;
			result=true;
		}
//		return false;
		debugRemove(result,debugFirst);
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
		debugSaveState();
		U debugOldValue=oldValue;
		U debugNewValue=newValue;
		std::set<T> s;
		for(const_iterator it=p.first;it!=p.second;++it){//for all keys in range
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
		debugReplace(debugOldValue,debugNewValue);
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
		debugSaveState();
		T debugOldKey=oldKey;
		T debugNewKey=newKey;
		const U & v=it->second;
		m.insert(std::make_pair(newKey,v));//insert new value entry
		r.insert(std::make_pair(v,newKey));//insert new value reverse entry
		r.erase(multimap::find(r,v,oldKey));//erase old value reverse entry
		m.erase(it);
		debugReplaceKey(debugOldKey,debugNewKey);
		return true;
	}

//	const_range_iterator & minimumKey(const U & value){
//		range_iterator it=
//
//	}

	/**
	 *
	 * @return a string representation of this map list and reverse list
	 */
	std::string toString(const StringFormater & sf=defaultStringFormater)const{
		StringFormater f=sf;
		std::stringstream ss;
		if(!sf.isDefault()){
			f.build(ss,"");
			++f;
		}
		f.header("map=");
		ss << map::toString(m,f);
//		if(sf.isDefault())
		f.header("reverse map=");
		ss << multimap::toString(r,f);
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

#endif /* MAPREVERSABLE_H_ */
