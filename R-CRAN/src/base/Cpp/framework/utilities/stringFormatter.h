/************************************************************************
 ************************* Developer Notice *****************************
 ************************************************************************
 * @details
 *
 * This file defines the StringFormatter class which, as the name says, is
 * used to aid in formatting strings.
 *
 *
 * @author poltergeist0
 *
 * @date 2018-10-08
 ************************************************************************
 ************************************************************************
 ************************************************************************/

#ifndef STRINGFORMATER_H_
#define STRINGFORMATER_H_

#include <string>
#include <sstream>

/**
 * @brief StringFormatter class.
 *
 * @details
 * This class provides a generic, easy way to format the insertion of header,
 * tail, sets, tuples, indentation and new lines in strings.
 * It is specially useful in repetitive situations, as an example, when the
 * same header and tail must be set in every line of an output.
 *
 * @author poltergeist0
 *
 * @date 2018-10-08
 */
class StringFormatter{
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

public:

	/**
	 * default constructor
	 */
	StringFormatter():
		indentLevel(0),hdr(""),tl(""),lnBrk(true),tplOpn("("),tplCls(")"),vlSprtr(";"),
		tplSprtr(""),stOpn("{"),stCls("}")
		//,isSet(false)
	{}

	/**
	 * Copy constructor
	 *
	 * @param copy
	 * @param autoIndent if true indents the contents by one tabulation relative to the original
	 */
	StringFormatter(const StringFormatter & copy, const bool & autoIndent=true):
		indentLevel(copy.indentLevel),hdr(copy.hdr),tl(copy.tl),lnBrk(copy.lnBrk),tplOpn(copy.tplOpn),
		tplCls(copy.tplCls),vlSprtr(copy.vlSprtr),
		tplSprtr(copy.stOpn),stOpn("{"),stCls(copy.stCls)
		//,isSet(copy.isSet)
	{
		if(autoIndent && !copy.isDefault()) indentLevel++;
	}

	/**
	 * @brief Copy constructor with indentation level
	 *
	 * @details
	 * Indents the contents by the given value relative to the original.
	 *
	 * If the given value is negative, it removes indentations until it either
	 * becomes zero or equal to original indentation level minus given level.
	 *
	 *
	 * @param copy
	 * @param indent is the indentation level to add/subtract
	 */
	StringFormatter(const StringFormatter & copy, const int indent):
		indentLevel(copy.indentLevel),hdr(copy.hdr),tl(copy.tl),lnBrk(copy.lnBrk),tplOpn(copy.tplOpn),
		tplCls(copy.tplCls),vlSprtr(copy.vlSprtr),
		tplSprtr(copy.stOpn),stOpn("{"),stCls(copy.stCls)
		//,isSet(copy.isSet)
	{
		if(indentLevel+indent>0) indentLevel+=indent;
		else indentLevel=0;
	}

	/**
	 * @param indent is the indentation to add/subtract to copy
	 * @return copy of this StringFormatter object
	 */
	StringFormatter operator()(const int indent) const {
		return StringFormatter(*this,indent);
	}

	/**
	 *
	 * @return the currently set header
	 */
	const std::string& header() const {return hdr;}

	/**
	 * Set a new header
	 * @param header
	 */
	void header(const std::string& header) {hdr = header;}

	/**
	 *
	 * @return the string that represents closing of a set
	 */
	const std::string& setClose() const {return stCls;}

	/**
	 * Set the string that represents closing of a set
	 * @param setClose
	 */
	void setClose(const std::string& setClose) {stCls = setClose;}

	/**
	 *
	 * @return the string that represents opening of a set
	 */
	const std::string& setOpen() const {return stOpn;}

	/**
	 * Set the string that represents opening of a set
	 * @param setOpen
	 */
	void setOpen(const std::string& setOpen) {stOpn = setOpen;}

	/**
	 *
	 * @return the currently set tail
	 */
	const std::string& tail() const {return tl;}

	/**
	 * Set a new tail
	 * @param tail
	 */
	void tail(const std::string& tail) {tl = tail;}


	/**
	 *
	 * @return the string that represents closing of a tuple
	 */
	const std::string& tupleClose() const {return tplCls;}

	/**
	 * Set the string that represents closing of a tuple
	 * @param tupleClose
	 */
	void tupleClose(const std::string& tupleClose) {tplCls = tupleClose;}

	/**
	 *
	 * @return the string that represents opening of a tuple
	 */
	const std::string& tupleOpen() const {return tplOpn;}

	/**
	 * Set the string that represents opening of a tuple
	 * @param tupleOpen
	 */
	void tupleOpen(const std::string& tupleOpen) {tplOpn = tupleOpen;}

	/**
	 *
	 * @return the string that represents a tuple separator
	 */
	const std::string& tupleSeparator() const {return tplSprtr;}

	/**
	 * Set the string that represents a tuple separator
	 * @param tupleSeparator
	 */
	void tupleSeparator(const std::string& tupleSeparator) {tplSprtr = tupleSeparator;}

	/**
	 *
	 * @return the string that represents a value separator
	 */
	const std::string& valueSeparator() const {return vlSprtr;}

	/**
	 * Set the string that represents a value separator
	 * @param valueSeparator
	 */
	void valueSeparator(const std::string& valueSeparator) {vlSprtr = valueSeparator;}

	/**
	 *
	 * @return a string with the current indentation level
	 */
	std::string indent()const{
		return std::string(indentLevel,'\t');
	}

	/**
	 * Start writing to string stream
	 *
	 * @param ss
	 * @param set if true automatically opens a set
	 * @return
	 */
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

	/**
	 * Start writing to string
	 * @param set if true automatically opens a set
	 * @return
	 */
	std::string start(const bool & set=false) const {
		std::stringstream ss;
		return start(ss,set).str();
	}

	/**
	 * @brief End writing to string stream.
	 * @details
	 * If the set argument was true when the start function was called, the set
	 * argument in this function should also be true unless the set was closed
	 * manually.
	 *
	 * @param ss
	 * @param set if true automatically closes a set
	 * @return
	 */
	std::stringstream & end(std::stringstream & ss,const bool & set=false) const {
		if(set){
			ss << stCls;
//			isSet=false;
		}
		ss << tl ;
		if(lnBrk) ss << "\n";
		return ss;
	}

	/**
	 * @brief End writing to string.
	 * @details
	 * If the set argument was true when the start function was called, the set
	 * argument in this function should also be true unless the set was closed
	 * manually.
	 *
	 * @param set if true automatically closes a set
	 * @return
	 */
	std::string end(const bool & set=false) const {
		std::stringstream ss;
		return end(ss,set).str();
	}

	/**
	 * @brief Build a formatted message
	 * @details
	 * Automatically calls the start function, prints the message and calls the
	 * end function.
	 *
	 * @param ss
	 * @param message
	 * @return
	 */
	std::stringstream & build(std::stringstream & ss, const std::string & message) {
		start(ss);
		ss << message;
		return end(ss);
	}

	/**
	 * @brief Build a formatted message
	 * @details
	 * Automatically calls the start function, prints the message and calls the
	 * end function.
	 *
	 * @param message
	 * @return
	 */
	std::string build(const std::string & message) {
		std::stringstream ss;
		return build(ss,message).str();
	}

	/**
	 * Increment the current indentation level
	 * @return this object
	 */
	StringFormatter & operator++(){//prefix operator
		indentLevel++;
		return *this;
	}

	/**
	 * Decrement the current indentation level
	 * @return this object
	 */
	StringFormatter & operator--(){//prefix operator
		if(indentLevel>0) indentLevel--;
		return *this;
	}

	/*
	 * needs to be implemented outside the class definition in order to be able
	 * to compare the this object with the default string formatter object
	 */
	/**
	 *
	 * @return true if this object is the default string formatter object
	 */
	bool isDefault() const ;

}const defaultStringFormatter;

bool StringFormatter::isDefault() const {
	return this==&defaultStringFormatter;
}


#endif /* STRINGFORMATER_H_ */
