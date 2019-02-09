/*
 * reader.h
 *
 *  Created on: 06/02/2019
 *      Author: poltergeist0
 */

#ifndef SRC_READER_H_
#define SRC_READER_H_

#include <string>
#include "program.h"

/**
 * Interface for a simple stream forward reader.
 * It can not read backwards.
 */
class ReaderInterface{
public:
	/**
	 * Enumeration of the types of objects that can be read
	 */
	enum class READTYPE: unsigned int {LINE=1,VALUE,COMMENT};

	/**
	 * Enumeration of the type of object that is next is line for reading
	 */
	enum class NEXTTYPE: unsigned int {ERROR=1,ENDOFFILE,NEWLINE,VALUE,COMMENT};

	virtual ~ReaderInterface(){}

	/**
	 *
	 * @return the type of the next object that can be read
	 */
	virtual NEXTTYPE hasNext()=0;

	/**
	 *
	 * @param type
	 * @return the next object of the given type ignoring any other type
	 */
	template<typename T>
	virtual T next(READTYPE type=READTYPE::VALUE)=0;
};

/**
 * Reader for files
 */
class ReaderFile: public ReaderInterface{
	//TODO reimplement this class with a more robust way of reading files
private:
	const ProgramParameters & par;
	std::ifstream finput;
	unsigned int lineNumber=1;
//	std::string nxt;//next object
	std::string src;
	std::string dest;
	std::string weight;
	int state=0;//=0 error; =1 reads src; =2 reads dest; = 3 reads weight; =4 reads newline; =5 end of file

public:
	ReaderFile(const ProgramParameters & parameters):par(parameters),lineNumber(1),src(""),dest(""),weight("1.0"),state(4){
		finput.open(par.filename,std::fstream::in);
		next();
	}

	~ReaderFile(){
		finput.close();
	}

	/**
	 *
	 * @return the type of the next object or error condition
	 */
	NEXTTYPE hasNext(){
//		if (!finput.is_open()) return NEXTTYPE::ERROR;
//		if(!finput.eof()){
			//TODO
			switch(state){
			default:
			case 0: return NEXTTYPE::ERROR;
			case 1: return NEXTTYPE::VALUE;
			case 2: return NEXTTYPE::VALUE;
			case 3: return NEXTTYPE::VALUE;
			case 4: return NEXTTYPE::NEWLINE;
			case 5: return NEXTTYPE::ENDOFFILE;
			}
//			return NEXTTYPE::VALUE;
//		}
//		return NEXTTYPE::ENDOFFILE;
	}

	/**
	 *
	 * @param type is currently ignored
	 * @return the next object of the requested type or the error message as indicated by hasNext()
	 */
	std::string next(READTYPE type=READTYPE::VALUE){
		if (!finput.is_open()) {
			state=0;
			std::stringstream ss;
			ss << "The file " << par.filename << " does not exist\n";
			return ss.str();
		}
//		if(!finput.eof()) {
			switch(state){
			case 0:
				break;
			case 1:
				state=2;
				return src;
				break;
			case 2:
				state=3;
				return dest;
				break;
			case 3:
				if(finput.eof()) state=5;
				else state=4;
				return weight;
				break;
			case 4:
				if (par.type==LINK_WEIGHT::WEIGHTED) {
					finput >> src >> std::ws >> dest >> std::ws >> weight;
				} else {
					finput >> src >> std::ws >> dest;
				}
				if (finput) {
					state=1;
					lineNumber++;
					return "\n";
				}
				else{
					std::stringstream ss;
					ss << "The file " << par.filename << " has an error on line " << lineNumber << "\n";
					state=0;
					return ss.str();
				}
				break;
//			case 5://end of file processed outside this switch
//				break;
			}
//		}
		return "End of file\n";
	}

};

/**
 * Reader for R Matrix
 */
class ReaderMatrix: public ReaderInterface{
	//TODO implement this class
//private:
//	const ProgramParameters & par;
//	Rcpp::Matrix & finput;
//	unsigned int lineNumber=1;
////	std::string nxt;//next object
//	std::string src;
//	std::string dest;
//	std::string weight;
//	int state=0;//=0 error; =1 reads src; =2 reads dest; = 3 reads weight; =4 reads newline; =5 end of file
//
//public:
//	ReaderMatrix(const Rcpp::Matrix & finput,const ProgramParameters & parameters):par(parameters),lineNumber(1),src(""),dest(""),weight("1.0"),state(4){
//		next();
//	}
//
//	~ReaderMatrix(){}
//
//	/**
//	 *
//	 * @return the type of the next object or error condition
//	 */
//	NEXTTYPE hasNext(){
////		if (!finput.is_open()) return NEXTTYPE::ERROR;
////		if(!finput.eof()){
//			//TODO
//			switch(state){
//			default:
//			case 0: return NEXTTYPE::ERROR;
//			case 1: return NEXTTYPE::VALUE;
//			case 2: return NEXTTYPE::VALUE;
//			case 3: return NEXTTYPE::VALUE;
//			case 4: return NEXTTYPE::NEWLINE;
//			case 5: return NEXTTYPE::ENDOFFILE;
//			}
////			return NEXTTYPE::VALUE;
////		}
////		return NEXTTYPE::ENDOFFILE;
//	}
//
//	/**
//	 *
//	 * @param type is currently ignored
//	 * @return the next object of the requested type or the error message as indicated by hasNext()
//	 */
//	GraphUndirectedGroupable next(READTYPE type=READTYPE::VALUE){
//		if (!finput.is_open()) {
//			state=0;
//			std::stringstream ss;
//			ss << "The file " << par.filename << " does not exist\n";
//			return ss.str();
//		}
////		if(!finput.eof()) {
//			switch(state){
//			case 0:
//				break;
//			case 1:
//				state=2;
//				return src;
//				break;
//			case 2:
//				state=3;
//				return dest;
//				break;
//			case 3:
//				if(finput.eof()) state=5;
//				else state=4;
//				return weight;
//				break;
//			case 4:
//				if (par.type==LINK_WEIGHT::WEIGHTED) {
//					finput >> src >> std::ws >> dest >> std::ws >> weight;
//				} else {
//					finput >> src >> std::ws >> dest;
//				}
//				if (finput) {
//					state=1;
//					lineNumber++;
//					return "\n";
//				}
//				else{
//					std::stringstream ss;
//					ss << "The file " << par.filename << " has an error on line " << lineNumber << "\n";
//					state=0;
//					return ss.str();
//				}
//				break;
////			case 5://end of file processed outside this switch
////				break;
//			}
////		}
//		return "End of file\n";
//	}
//
};

#endif /* SRC_READER_H_ */
