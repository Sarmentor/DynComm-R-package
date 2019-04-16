/*
 * stringFormater.h
 *
 *  Created on: 08/10/2018
 *      Author: poltergeist0
 */

#ifndef STRINGFORMATER_H_
#define STRINGFORMATER_H_

#include <string>
#include <sstream>

class StringFormater{
private:
	unsigned int indentLevel=0;//start the string with these many tabulation characters before header
	std::string hdr="";//header
	std::string tl="";//tail
	bool lnBrk=true;//end the string with a line break after tail
	std::string tplOpn="(";//insert at beginning of tuple
	std::string tplCls=")";//insert at end of tuple
	std::string vlSprtr=";";//insert between values
	std::string tplSprtr="";//insert between tuples
	std::string stOpn="{";//insert at the beginning of a set of values/tuples
	std::string stCls="}";//insert at the end of a set
//	bool isSet=false;//indicates if a set of values/tuples is being formated

public:

	StringFormater():
		indentLevel(0),hdr(""),tl(""),lnBrk(true),tplOpn("("),tplCls(")"),vlSprtr(";"),
		tplSprtr(""),stOpn("{"),stCls("}")
		//,isSet(false)
	{}

	StringFormater(const StringFormater & copy, const bool & autoIndent=true):
		indentLevel(copy.indentLevel),hdr(copy.hdr),tl(copy.tl),lnBrk(copy.lnBrk),tplOpn(copy.tplOpn),
		tplCls(copy.tplCls),vlSprtr(copy.vlSprtr),
		tplSprtr(copy.stOpn),stOpn("{"),stCls(copy.stCls)
		//,isSet(copy.isSet)
	{
		if(autoIndent && !copy.isDefault()) indentLevel++;
	}

	StringFormater(const StringFormater & copy, const int indent):
		indentLevel(copy.indentLevel),hdr(copy.hdr),tl(copy.tl),lnBrk(copy.lnBrk),tplOpn(copy.tplOpn),
		tplCls(copy.tplCls),vlSprtr(copy.vlSprtr),
		tplSprtr(copy.stOpn),stOpn("{"),stCls(copy.stCls)
		//,isSet(copy.isSet)
	{
		if(indentLevel+indent>0) indentLevel+=indent;
		else indentLevel=0;
	}

	StringFormater operator()(const int indent) const {
		return StringFormater(*this,indent);
	}

	const std::string& header() const {return hdr;}

	void header(const std::string& header) {hdr = header;}

	const std::string& setClose() const {return stCls;}

	void setClose(const std::string& setClose) {stCls = setClose;}

	const std::string& setOpen() const {return stOpn;}

	void setOpen(const std::string& setOpen) {stOpn = setOpen;}

	const std::string& tail() const {return tl;}

	void tail(const std::string& tail) {tl = tail;}

	const std::string& tupleClose() const {return tplCls;}

	void tupleClose(const std::string& tupleClose) {tplCls = tupleClose;}

	const std::string& tupleOpen() const {return tplOpn;}

	void tupleOpen(const std::string& tupleOpen) {tplOpn = tupleOpen;}

	const std::string& tupleSeparator() const {return tplSprtr;}

	void tupleSeparator(const std::string& tupleSeparator) {tplSprtr = tupleSeparator;}

	const std::string& valueSeparator() const {return vlSprtr;}

	void valueSeparator(const std::string& valueSeparator) {vlSprtr = valueSeparator;}

	std::string indent()const{
		return std::string(indentLevel,'\t');
	}

	std::stringstream & start(std::stringstream & ss,const bool & set=false) const {
		ss << indent() << hdr;
		if(set){
			ss << stOpn;
//			isSet=true;
		}
//		else{
//			isSet=false;
//		}
		return ss;
	}

	std::string start(const bool & set=false) const {
		std::stringstream ss;
		return start(ss,set).str();
	}

	std::stringstream & end(std::stringstream & ss,const bool & set=false) const {
		if(set){
			ss << stCls;
//			isSet=false;
		}
		ss << tl ;
		if(lnBrk) ss << "\n";
		return ss;
	}

	std::string end(const bool & set=false) const {
		std::stringstream ss;
		return end(ss,set).str();
	}

	std::stringstream & build(std::stringstream & ss, const std::string & message) {
		start(ss);
		ss << message;
		return end(ss);
	}

	/**
	 * build a string with the given message and the currently configured formating elements
	 * @param message
	 * @return
	 */
	std::string build(const std::string & message) {
		std::stringstream ss;
//		ss << indent() << hdr << message << tl << end();
//		return ss.str();
		return build(ss,message).str();
	}

	StringFormater & operator++(){//prefix operator
		indentLevel++;
		return *this;
	}

	StringFormater & operator--(){//prefix operator
		if(indentLevel>0) indentLevel--;
		return *this;
	}

	bool isDefault() const ;

}const defaultStringFormater;

bool StringFormater::isDefault() const {
	return this==&defaultStringFormater;
}


#endif /* STRINGFORMATER_H_ */
