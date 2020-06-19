/*
 * DebugLog.h
 *
 *  Created on: 31/07/2019
 *      Author: poltergeist0
 */

#ifndef R_CRAN_SRC_BASE_CPP_DEBUGLOG_H_
#define R_CRAN_SRC_BASE_CPP_DEBUGLOG_H_

#include <list>
#include <sstream>
#include "../io/writer.h"
#include "../program.h"

class DebugLog{
//public:
//	enum class DEBUG_LEVEL:unsigned int{
//		NONE=0	//no debugging
//		,TRACE=10	//function calls without parameters
//		,CALLS=20	//function calls with parameters
//		,MODIFICATIONS=30	//function calls with parameters and pre and post operation snapshots
//		,ALL=10000	//prints everything
//	};

private:
	bool initialized=false;
	ProgramParameters p;
	WriterDebugLogFile f;
	std::list<std::string> path;
//	DEBUG_LEVEL lvl=DEBUG_LEVEL::NONE;

//	ProgramParameters defaultParameters()const{
//		ProgramParameters pa;
//		pa.outfilename="/dev/null";
//		return pa;
//	}

//	ProgramParameters initWriter(const ProgramParameters & parameters)const{
//		ProgramParameters pa;
//		pa.outfilename=parameters.debugFilename;
//		return pa;
//	}

	void pathToString(std::stringstream & ss, bool pre=false)const{
		if(path.size()<=0){
			ss << "/";
		}
		for(std::list<std::string>::const_iterator it=path.cbegin();it!=path.cend();++it){
			ss << "/" << (*it);
//			ss << (*it);
		}
		if(pre)	ss << ">";
		else ss << "<";
	}

	void msg(const std::string & message, bool newLine=true, bool withPath=false, bool pre=false){
		if(withPath){
			std::stringstream ss;
			pathToString(ss,pre);
			if(message.size()>0){
				ss << "\n" << message;
			}
			if(newLine){
				f.write(ss.str(), WriterInterface::WRITETYPE::LINE);
			}
			else{
				f.write(ss.str(), WriterInterface::WRITETYPE::VALUE);
			}
		}
		else{
			if(newLine){
				f.write(message, WriterInterface::WRITETYPE::LINE);
			}
			else{
				f.write(message, WriterInterface::WRITETYPE::VALUE);
			}
		}
	}

	std::string writeParameters()const {
		std::stringstream ss;
		ss << "DYNCOMM_CPP_VERSION=" << DYNCOMM_CPP_VERSION << "\n";
		ss << p.toString();
		return ss.str();
	}

public:
	DebugLog():initialized(false),p(argumentsDefault),path(){}

//	DebugLog(const ProgramParameters & parameters):p(parameters),f(initWriter(parameters)){
//		if(f.isReady()) initialized=true;
//	}

	const DEBUG_LEVEL& debugLevel()const{return p.debugLevel;}

	bool init(const ProgramParameters & parameters){
		if(p.debugLevel==DEBUG_LEVEL::NONE){//not initialized yet
			p=parameters;
			if(parameters.debugLevel>DEBUG_LEVEL::NONE){//requested debugging
				f.init(parameters);
				if(f.isReady()){
					initialized=true;
					f.write(writeParameters(),WriterInterface::WRITETYPE::LINE);
				}
			}
			else{
				initialized=true;//succeed to initialize but nothing will ever be printed
			}
		}
		else{
			return false;//failed to initialize because it was already initialized
		}
		return initialized;
	}

	bool isReady()const{return initialized;}

	void msg(DEBUG_LEVEL level=DEBUG_LEVEL::NONE,const std::string & message="", bool newLine=true, bool withPath=false){
		if(p.debugLevel>DEBUG_LEVEL::NONE){//is initialized
			if(level<=p.debugLevel && path.size()<=p.debugDepth){//requested level is lower than maximum and depth was not reached
				msg(message, newLine, withPath);
			}
		}
	}

	void val(DEBUG_LEVEL level=DEBUG_LEVEL::NONE,const std::string & message=""){
		if(p.debugLevel>DEBUG_LEVEL::NONE){//is initialized
			if(level<=p.debugLevel && path.size()<=p.debugDepth){//requested level is lower than maximum and depth was not reached
				msg(message,false,false);
			}
		}
	}

	void pre(DEBUG_LEVEL level=DEBUG_LEVEL::NONE,const std::string & caller="", const std::string & message=""){
		if(p.debugLevel>DEBUG_LEVEL::NONE){//is initialized
			if(DEBUG_LEVEL::TRACE<=p.debugLevel){//trace level is lower than maximum
				path.push_back(caller);
			}
			if(level<=p.debugLevel && path.size()<=p.debugDepth){//requested level is lower than maximum and depth was not reached
				msg(message,true,true,true);
			}
		}
	}

	void post(DEBUG_LEVEL level=DEBUG_LEVEL::NONE,const std::string & message=""){
		if(p.debugLevel>DEBUG_LEVEL::NONE){//is initialized
			if(level<=p.debugLevel && path.size()<=p.debugDepth){//requested level is lower than maximum and depth was not reached
				msg(message,true,true);
			}
			if(DEBUG_LEVEL::TRACE<=p.debugLevel){//trace level is lower than maximum
				if(!path.empty()){//if not at top level yet
					path.pop_back();//remove caller
				}
			}
		}
	}
}dbg;

#define PRE(caller,message) dbg.pre(caller,message)
#define POST(message) dbg.post(message)
#define VAL(message) dbg.val(message)
#define MSG(message,newline,withPath) dbg.msg(message,newline,withPath)


#endif /* R_CRAN_SRC_BASE_CPP_DEBUGLOG_H_ */
