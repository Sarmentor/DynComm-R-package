/************************************************************************
 ************************* Developer Notice *****************************
 ************************************************************************
 * @details
 *
 * This file defines the writer class interface and classes to write to
 * stream.
 *
 * There should never be any reason to change it unless to add more
 * writers.
 *
 *
 * @author poltergeist0
 *
 * @date 2019-02-06
 ************************************************************************
 ************************************************************************
 ************************************************************************/

#ifndef SRC_WRITER_H_
#define SRC_WRITER_H_

#include <string>
#include "../program.h"
#include "../graph/edge.h"
#include <fstream>
#include <ostream>

/**
 * @brief Interface for a simple stream forward writer.
 *
 * @details
 * Writers that implement this interface can not write backwards, only append
 * to the end.
 *
 * @author poltergeist0
 *
 * @date 2019-02-06
 */
class WriterInterface{
public:
	/**
	 * Enumeration of the types of objects that can be written
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

/**
 * @brief Writer to generic output stream.
 *
 * @details
 * This writer can not write backwards, only append to the end.
 *
 * @author poltergeist0
 *
 * @date 2019-02-06
 */
class WriterStream: public WriterInterface{
private:
	std::ostream & str;
	std::string stts;
	const ProgramParameters & par;
	unsigned int lineNumber=1;
	int state=0;//=0 error; =1 write object; =2 write comment; = 3 ready/waiting/start of line

public:
	WriterStream(std::ostream & stream,const ProgramParameters & parameters):str(stream),stts("Ok"),par(parameters),lineNumber(1),state(3){}

	~WriterStream(){
		str.flush();//force flush
	}

	bool isReady(){
		if (state>0) return true;
		return false;
	}

	/**
	 * @brief Write a certain type of object to stream
	 * @details
	 * The type parameter defines the type of object being written. This is used
	 * to automatically add carriage returns, value separators and prepare writing
	 * of the next object type.
	 *
	 * @param type defines what is being written
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

/**
 * @brief Writer to file output stream.
 *
 * @details
 * This writer can not write backwards, only append to the end.
 *
 * @author poltergeist0
 *
 * @date 2019-02-06
 */
class WriterFile: public WriterInterface{
private:
	std::ofstream foutput;
	WriterStream f;

public:
	WriterFile(const ProgramParameters & parameters):f(foutput,parameters){
		foutput.open(parameters.outfilename,std::fstream::out);
	}

	~WriterFile(){
		if(foutput.is_open()){
			foutput.close();
		}
	}

	bool isReady(){
		if(!foutput.is_open()){
			return false;
		}
		return f.isReady();
	}

	/**
	 * @brief Write a certain type of object to file stream
	 * @details
	 * The type parameter defines the type of object being written. This is used
	 * to automatically add carriage returns, value separators and prepare writing
	 * of the next object type.
	 *
	 * @param type defines what is being written
	 * @return true if writing succeeded
	 */
	bool write(const std::string & object,const WRITETYPE & type=WRITETYPE::VALUE){
		if(!foutput.is_open()){
			return false;
		}
		return f.write(object,type);
	}

	std::string status(){
		if(!foutput.is_open()){
			return "The file does not exist\n";
		}
		return f.status();
	}
};

/**
 * @brief Writer to file output stream.
 *
 * @details
 * This writer can not write backwards, only append to the end.
 *
 * @author poltergeist0
 *
 * @date 2019-02-06
 */
class WriterDebugLogFile: public WriterInterface{
private:
	std::ostream & foutput;
	std::ofstream file;
	std::string stts;
	ProgramParameters par;
	unsigned int lineNumber=1;
	int state=0;//=0 error; =1 write object; =2 write comment; = 3 ready/waiting/start of line

public:
	WriterDebugLogFile():foutput(CERR),file(),stts("Not initialized"),lineNumber(1),state(3){}

	void init(const ProgramParameters & parameters){
		par=parameters;
		if(parameters.debugFilename.length()>0){
//			file=new std::ofstream(parameters.debugFilename,std::fstream::out);
			file.open(parameters.debugFilename,std::fstream::out);
			if(file.is_open()){
//				std::ostream* fp = &file;
//				foutput.basic_ostream(__sb)=&std::ostream(*fp);
				foutput.rdbuf(file.rdbuf());
			}

//			std::ostream* fp = &CERR;
//			    std::ofstream fout;
//			    if (argc > 1) {
//			        fout.open(argv[1]);
//			        fp = &fout;
//			    }
//			    process(*fp);
		}
//		else{//use std::err if no file name is given
//			std::ostream* fp = &CERR;
//			foutput=*fp;
//		}
//		if(foutput.is_open()){
//			stts="Ok";
//		}
		stts="Ok";
	}

	~WriterDebugLogFile(){
//		if(foutput.is_open()){
//			foutput.flush();
//			foutput.close();
//		}
		if(file.is_open()){
			file.flush();
			file.close();
		}
	}

	bool isReady(){
//		if(!foutput.is_open()){
//			return false;
//		}
		if (state>0) return true;
		return false;
	}

	/**
	 * @brief Write a certain type of object to file stream
	 * @details
	 * The type parameter defines the type of object being written. This is used
	 * to automatically add carriage returns, value separators and prepare writing
	 * of the next object type.
	 *
	 * @param type defines what is being written
	 * @return true if writing succeeded
	 */
	bool write(const std::string & object,const WRITETYPE & type=WRITETYPE::VALUE){
//		if(!foutput.is_open()){
//			return false;
//		}
		switch(state){
		case 1://previously wrote object
			switch(type){
			case WRITETYPE::COMMENT://requesting write comment. Must write new line before
				foutput << "\n#"<< object<<"\n";
				foutput.flush();
				state=3;
				break;
			case WRITETYPE::LINE://requesting write new line after value
				foutput << object<<"\n";
				foutput.flush();
				state=3;
				break;
			case WRITETYPE::VALUE://requesting write another value. Must insert separator before
				foutput << object;
				state=1;
				break;
			}
			return true;
			break;
		case 3://ready/waiting/start of line
			switch(type){
			case WRITETYPE::COMMENT://requesting write comment
				foutput << "#"<< object<<"\n";
				foutput.flush();
				state=3;
				break;
			case WRITETYPE::LINE://requesting write new line after value
				foutput << object<<"\n";
				foutput.flush();
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

	std::string status(){
//		if(!foutput.is_open()){
//			return "The file does not exist or is not open.\n";
//		}
		return stts;
	}
};


#endif /* SRC_WRITER_H_ */
