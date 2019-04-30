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
#include "edge.h"

/**
 * Interface for a simple stream writer.
 * It can only append to the end.
 */
//template<typename TYPEOBJECT>
class WriterInterface{
public:
	/**
	 * Enumeration of the types of objects that can be read
	 */
	enum class WRITETYPE: unsigned int {LINE=1,VALUE,COMMENT};

	virtual ~WriterInterface(){}

	/**
	 * @return true if the sink is ready to be written
	 */
	virtual bool isReady()=0;

	/**
	 *
	 * @param object
	 * @param type
	 * @return
	 */
	virtual bool write(const std::string & object, const WRITETYPE & type=WRITETYPE::VALUE)=0;

	/**
	 *
	 * @return either an ok message or an error message
	 */
	virtual std::string status()=0;
};

class WriterFile: public WriterInterface{
private:
	std::string stts;
	const ProgramParameters & par;
	std::ofstream foutput;
	unsigned int lineNumber=1;
	int state=0;//=0 error; =1 write object; =2 write comment; = 3 ready/waiting/start of line

public:
	WriterFile(const ProgramParameters & parameters):stts("Ok"),par(parameters),lineNumber(1){
		foutput.open(par.outfilename,std::fstream::out);
		if(foutput.is_open()) state=3;
		else{
			std::stringstream ss;
			ss << "The file " << par.outfilename << " does not exist\n";
			stts=ss.str();
			state=0;
		}
	}

	~WriterFile(){
		foutput.close();
	}

	bool isReady(){
		if (state==5) return true;
		return false;
	}
	/**
	 *
	 * @param type is currently ignored
	 * @return true if writing succeeded
	 */
	bool write(const std::string & object,const WRITETYPE & type=WRITETYPE::VALUE){
		switch(state){
		case 1://previously wrote object
			switch(type){
			case WRITETYPE::COMMENT://requesting write comment. Must write new line before
				foutput << "\n# "<< object<<"\n";
				state=3;
				break;
			case WRITETYPE::LINE://requesting write new line after value
				foutput << " "<< object<<"\n";
				state=3;
				break;
			case WRITETYPE::VALUE://requesting write another value. Must insert separator before
				foutput << " "<< object;
				state=1;
				break;
			}
			return true;
			break;
		case 3://ready/waiting/start of line
			switch(type){
			case WRITETYPE::COMMENT://requesting write comment
				foutput << "# "<< object<<"\n";
				state=3;
				break;
			case WRITETYPE::LINE://requesting write new line after value
				foutput << object<<"\n";
				state=3;
				break;
			case WRITETYPE::VALUE://requesting write value
				foutput << object;
				state=1;
				break;
			}
			break;
		}
		return false;
	}

	std::string status(){return stts;}
};

class WriterStringStream: public WriterInterface{
private:
	std::stringstream & str;
	std::string stts;
	const ProgramParameters & par;
	unsigned int lineNumber=1;
	int state=0;//=0 error; =1 write object; =2 write comment; = 3 ready/waiting/start of line

public:
	WriterStringStream(std::stringstream & stream,const ProgramParameters & parameters):str(stream),stts("Ok"),par(parameters),lineNumber(1),state(3){}

	~WriterStringStream(){}

	bool isReady(){
		if (state==5) return true;
		return false;
	}

	/**
	 *
	 * @param type is currently ignored
	 * @return true if writing succeeded
	 */
	bool write(const std::string & object, const WRITETYPE & type=WRITETYPE::VALUE){
		switch(state){
		case 1://previously wrote object
			switch(type){
			case WRITETYPE::COMMENT://requesting write comment. Must write new line before
				str << "\n# "<< object<<"\n";
				state=3;
				break;
			case WRITETYPE::LINE://requesting write new line after value
				str << " "<< object<<"\n";
				state=3;
				break;
			case WRITETYPE::VALUE://requesting write another value. Must insert separator before
				str << " "<< object;
				state=1;
				break;
			}
			return true;
			break;
		case 3://ready/waiting/start of line
			switch(type){
			case WRITETYPE::COMMENT://requesting write comment
				str << "# "<< object<<"\n";
				state=3;
				break;
			case WRITETYPE::LINE://requesting write new line after value
				str << object<<"\n";
				state=3;
				break;
			case WRITETYPE::VALUE://requesting write value
				str << object;
				state=1;
				break;
			}
			break;
		}
		return false;
	}

	std::string status(){return stts;}
};


#endif /* SRC_WRITER_H_ */
