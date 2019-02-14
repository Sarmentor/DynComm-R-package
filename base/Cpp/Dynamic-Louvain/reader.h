/*
 * reader.h
 *
 *  Created on: 06/02/2019
 *      Author: poltergeist0
 */

#ifndef SRC_READER_H_
#define SRC_READER_H_

#include <string>
#include <fstream>
#include "program.h"
#include "edge.h"

/**
 * Interface for a simple stream forward reader.
 * It can not read backwards.
 */
template<typename TYPERETURN>
class ReaderInterface{
public:
	/**
	 * Enumeration of the types of objects that can be read
	 */
	enum class READTYPE: unsigned int {LINE=1,VALUE,COMMENT};

	/**
	 * Enumeration of the type of object that is next is line for reading
	 */
	enum class NEXTTYPE: unsigned int {CANNOTOPEN=1,ENDOFFILE,NEWLINE,VALUE,COMMENT};

	virtual ~ReaderInterface(){}

	/**
	 *
	 * @param source is the data source to read data from (file, vector, etc)
	 * @return true if operation succeeded. False, otherwise
	 */
//	virtual bool open(TYPESOURCE source)=0;

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
	virtual TYPERETURN next(READTYPE type=READTYPE::VALUE)=0;

	/**
	 *
	 * @return either an ok message or an error message
	 */
	virtual std::string status()=0;
};

/**
 * Dummy Reader
 */
class ReaderDummy: public ReaderInterface<std::string>{
public:
	ReaderDummy(){}

	~ReaderDummy(){}

//	bool open(std::string ignore){return true;}

	/**
	 * @return end of file type
	 */
	NEXTTYPE hasNext(){return NEXTTYPE::ENDOFFILE;}

	/**
	 * @param type is ignored
	 * @return end of file message
	 */
	std::string next(READTYPE type=READTYPE::VALUE){return "End of file\n";}

	std::string status(){return "End of file\n";}
};

/**
 * Reader for files
 */
class ReaderFile: public ReaderInterface<std::string>{
	//TODO reimplement this class with a more robust way of reading files
private:
	std::string stts;
	const ProgramParameters & par;
	std::ifstream finput;
	unsigned int lineNumber=1;
//	std::string nxt;//next object
	std::string src;
	std::string dest;
	std::string weight;
	int state=0;//=0 error; =1 reads src; =2 reads dest; = 3 reads weight; =4 reads newline; =5 end of file

public:
	ReaderFile(const ProgramParameters & parameters):stts("Ok"),par(parameters),lineNumber(1),src(""),dest(""),weight("1.0"),state(4){
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
			case 0: return NEXTTYPE::CANNOTOPEN;
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
			stts=ss.str();
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
					if(!finput.eof()){
						state=5;
						stts="End of file\n";
						return stts;
					}
					else{
						std::stringstream ss;
						ss << "The file " << par.filename << " has an error on line " << lineNumber << "\n";
						state=0;
						stts=ss.str();
						return stts;
					}
				}
				break;
//			case 5://end of file processed outside this switch
//				break;
			}
//		}
		state=5;
		stts="End of file\n";
		return stts;
	}

	std::string status(){return stts;}
};

/**
 * Reader for files
 */
class ReaderFileEdge: public ReaderInterface<Edge>{
	//TODO reimplement this class with a more robust way of reading files
private:
	std::string stts;
	const ProgramParameters & par;
	std::ifstream finput;
	unsigned int lineNumber=1;
//	std::string nxt;//next object
	Edge ed;
	int state=0;//=0 error; =1 reads edge; =4 reads newline; =5 end of file

public:
	ReaderFileEdge(const ProgramParameters & parameters):stts("Ok"),par(parameters),lineNumber(1),ed(noEdge),state(4){
		finput.open(par.filename,std::fstream::in);
		next();
	}

	~ReaderFileEdge(){
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
			case 0: return NEXTTYPE::CANNOTOPEN;
			case 1: return NEXTTYPE::VALUE;
//			case 2: return NEXTTYPE::VALUE;
//			case 3: return NEXTTYPE::VALUE;
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
	Edge next(READTYPE type=READTYPE::VALUE){
		if (!finput.is_open()) {
			state=0;
			std::stringstream ss;
//			ss << "The file " << par.filename << " does not exist\n";
			ss<<"The file " << par.filename << " does not exist\n";
			stts=ss.str();
			return noEdge;
		}
//		if(!finput.eof()) {
			switch(state){
			case 0:
				break;
			case 1:
//				state=2;
				state=4;
				return ed;
				break;
//			case 2:
//				state=3;
//				return dest;
//				break;
//			case 3:
//				if(finput.eof()) state=5;
//				else state=4;
//				return weight;
//				break;
			case 4:
				typeNode src;
				typeNode dest;
				typeWeight weight;
				if (par.type==LINK_WEIGHT::WEIGHTED) {
					finput >> src >> std::ws >> dest >> std::ws >> weight;
				} else {
					finput >> src >> std::ws >> dest;
					weight=1;
				}
				if (finput) {
					state=1;
					ed=Edge(src,dest,weight);
					lineNumber++;
					return noEdge;
				}
				else{
					if(!finput.eof()){
						state=5;
						stts="End of file\n";
						return noEdge;
					}
					else{
						std::stringstream ss;
						ss << "The file " << par.filename << " has an error on line " << lineNumber << "\n";
						state=0;
						stts=ss.str();
						return noEdge;
					}
				}
				break;
//			case 5://end of file processed outside this switch
//				break;
			}
//		}
		state=5;
		stts="End of file\n";
		return noEdge;
	}

	std::string status(){return stts;}
};

class ReaderStringEdge: public ReaderInterface<Edge>{
	//TODO reimplement this class with a more robust way of reading files
private:
	std::string stts;
	const ProgramParameters & par;
	std::stringstream str;
	unsigned int lineNumber=1;
//	std::string nxt;//next object
	Edge ed;
	int state=0;//=0 error; =1 reads edge; =4 reads newline; =5 end of file

public:
	ReaderStringEdge(std::string data,const ProgramParameters & parameters):stts("Ok"),par(parameters),str(data),lineNumber(1),ed(noEdge),state(4){
//		finput.open(par.filename,std::fstream::in);
		next();
	}

	~ReaderStringEdge(){
//		finput.close();
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
			case 0: return NEXTTYPE::CANNOTOPEN;
			case 1: return NEXTTYPE::VALUE;
//			case 2: return NEXTTYPE::VALUE;
//			case 3: return NEXTTYPE::VALUE;
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
	Edge next(READTYPE type=READTYPE::VALUE){
//		if (!finput.is_open()) {
//			state=0;
//			std::stringstream ss;
////			ss << "The file " << par.filename << " does not exist\n";
//			ss<<"The file " << par.filename << " does not exist\n";
//			stts=ss.str();
//			return noEdge;
//		}
//		if(!finput.eof()) {
			switch(state){
			case 0:
				break;
			case 1:
//				state=2;
				state=4;
				return ed;
				break;
//			case 2:
//				state=3;
//				return dest;
//				break;
//			case 3:
//				if(finput.eof()) state=5;
//				else state=4;
//				return weight;
//				break;
			case 4:
				typeNode src;
				typeNode dest;
				typeWeight weight;
				if (par.type==LINK_WEIGHT::WEIGHTED) {
					str >> src >> std::ws >> dest >> std::ws >> weight;
				} else {
					str >> src >> std::ws >> dest;
					weight=1;
				}
				if (str) {
					state=1;
					ed=Edge(src,dest,weight);
					lineNumber++;
					return noEdge;
				}
				else{
					if(str.eof()){
						state=5;
						stts="End of file\n";
						return noEdge;
					}
					else{
						std::stringstream ss;
						ss << "The file " << par.filename << " has an error on line " << lineNumber << "\n";
						state=0;
						stts=ss.str();
						return noEdge;
					}
				}
				break;
//			case 5://end of file processed outside this switch
//				break;
			}
//		}
		state=5;
		stts="End of file\n";
		return noEdge;
	}

	std::string status(){return stts;}
};

/**
 * Reader for R Matrix
 */
//class ReaderMatrixEdge: public ReaderInterface<Edge>{
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
//	ReaderMatrix(const Rcpp::Matrix & finput,const ProgramParameters & parameters):stts("Ok"),par(parameters),lineNumber(1),src(""),dest(""),weight("1.0"),state(4){
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
//};

//class Readers{
//public:
//	enum class READER:unsigned int{FILESTRING,FILEEDGE,RMATRIX};
//private:
//	READER selected;
//public:
//	Readers(READER reader):selected(reader){}
//	void set(READER reader){selected=reader;}
//};

#endif /* SRC_READER_H_ */
