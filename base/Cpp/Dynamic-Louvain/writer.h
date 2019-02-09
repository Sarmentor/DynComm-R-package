/*
 * reader.h
 *
 *  Created on: 06/02/2019
 *      Author: poltergeist0
 */

#ifndef SRC_WRITER_H_
#define SRC_WRITER_H_

#include <string>
#include "program.h"

/**
 * Interface for a simple stream writer.
 * It can only append to the end.
 */
class WriterInterface{
public:
	/**
	 * Enumeration of the types of objects that can be read
	 */
	enum class WRITETYPE: unsigned int {LINE=1,VALUE,COMMENT};

	virtual ~WriterInterface(){}

	template<typename T,typename U>
	virtual bool write(T & sink, const U & object,WRITETYPE type=WRITETYPE::VALUE)=0;
};

class WriterFile: public WriterInterface{
	//TODO implement this class
private:
	const ProgramParameters & par;
	std::ofstream foutput;
	unsigned int lineNumber=1;
	int state=0;//=0 error; =1 write object; =2 write comment; = 3 write newline; =4 write object separator

public:
	WriterFile(const ProgramParameters & parameters):par(parameters),lineNumber(1),state(4){
//		foutput.open(par.outfilename,std::fstream::out);
	}

	~WriterFile(){
		foutput.close();
	}

	/**
	 *
	 * @param type is currently ignored
	 * @return the next object of the requested type or the error message as indicated by hasNext()
	 */
	bool write(const GraphUndirectedGroupable & object,WRITETYPE type=WRITETYPE::VALUE){
//		if (!foutput.is_open()) {
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
		return false;
	}

};


#endif /* SRC_WRITER_H_ */
