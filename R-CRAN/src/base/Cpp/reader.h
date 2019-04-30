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
class ReaderFileEdge: public ReaderInterface<Edge>{
private:
	std::string stts;
	const ProgramParameters & par;
	std::ifstream finput;
	unsigned int lineNumber=1;
	Edge ed;
	int state=0;//=0 error; =1 reads edge; =4 reads newline; =5 end of file

public:
	ReaderFileEdge(const ProgramParameters & parameters):stts("Ok"),par(parameters),lineNumber(1),ed(noEdge),state(4){
		finput.open(parameters.filename,std::fstream::in);
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
		//TODO
		switch(state){
		default:
		case 0: return NEXTTYPE::CANNOTOPEN;
		case 1: return NEXTTYPE::VALUE;
		case 4: return NEXTTYPE::NEWLINE;
		case 5: return NEXTTYPE::ENDOFFILE;
		}
	}

	/**
	 *
	 * @param type is currently ignored
	 * @return the next object of the requested type or the error message as indicated by hasNext()
	 */
	Edge next(READTYPE type=READTYPE::VALUE){
		if (!finput.is_open()) {
			std::stringstream ss;
			ss<<"The file " << par.filename << " does not exist\n";
			return noEdge;
		}
			switch(state){
			case 0:
				break;
			case 1:
				state=4;
				return ed;
				break;
			case 4:
				typeVertex src;
				typeVertex dest;
				typeWeight weight;
				std::string line;
				std::getline(finput, line);
				if (finput) {
					std::istringstream line_buffer(line);
					line_buffer >> src >> std::ws >> dest >> std::ws >> weight;
//					CERR << "fail="<< line_buffer.fail()<< "; eof="<< line_buffer.eof()<< "; bad="<< line_buffer.bad()<< "; count="<< line_buffer.gcount()<< "\n";
					if(!line_buffer){//failed
						line_buffer.clear();//clear all ifstream errors
						line_buffer.seekg(0,line_buffer.beg);//reset stream read position
						//attemp read unweighted edge
						line_buffer >> src >> std::ws >> dest;
//						CERR << "fail="<< line_buffer.fail()<< "; eof="<< line_buffer.eof()<< "; bad="<< line_buffer.bad()<< "; count="<< line_buffer.gcount()<< "\n";
						if(!line_buffer){//failed
							line_buffer.clear();//clear all ifstream errors
							line_buffer.seekg(0,line_buffer.beg);//reset stream read position
								//attemp read comment line
								std::string s;
								line_buffer >> s;
//								CERR << "fail="<< line_buffer.fail()<< "; eof="<< line_buffer.eof()<< "; bad="<< line_buffer.bad()<< "; count="<< line_buffer.gcount()<< "\n";
								state=4;
						}
						else{//success reading without weight
							state=1;
							weight=1;
							ed=Edge(src,dest,weight);
							lineNumber++;
						}
					}
					else{//success reading with weight
						state=1;
						if(par.type==LINK_WEIGHT::UNWEIGHTED){//ignore weight read from stream
							if(weight!=0) weight=1;//only ignore if edge is to be inserted
						}
						ed=Edge(src,dest,weight);
						lineNumber++;
					}
					return noEdge;
				}
				else{
					if(finput.eof()){
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
			}
		state=5;
		stts="End of file\n";
		return noEdge;
	}

	std::string status(){return stts;}
};

class ReaderStringEdge: public ReaderInterface<Edge>{
private:
	std::string stts;
	const ProgramParameters & par;
	std::stringstream str;
	unsigned int lineNumber=1;
	Edge ed;
	int state=0;//=0 error; =1 reads edge; =4 reads newline; =5 end of file

public:
	ReaderStringEdge(std::string data,const ProgramParameters & parameters):stts("Ok"),par(parameters),str(data),lineNumber(1),ed(noEdge),state(4){
		next();
	}

	~ReaderStringEdge(){
	}

	/**
	 *
	 * @return the type of the next object or error condition
	 */
	NEXTTYPE hasNext(){
			//TODO
			switch(state){
			default:
			case 0: return NEXTTYPE::CANNOTOPEN;
			case 1: return NEXTTYPE::VALUE;
			case 4: return NEXTTYPE::NEWLINE;
			case 5: return NEXTTYPE::ENDOFFILE;
			}
	}

	/**
	 *
	 * @param type is currently ignored
	 * @return the next object of the requested type or the error message as indicated by hasNext()
	 */
	Edge next(READTYPE type=READTYPE::VALUE){
			switch(state){
			case 0:
				break;
			case 1:
				state=4;
				return ed;
				break;
			case 4:
				typeVertex src;
				typeVertex dest;
				typeWeight weight;
				std::string line;
				std::getline(str, line);
				if (str) {
					std::istringstream line_buffer(line);
					line_buffer >> src >> std::ws >> dest >> std::ws >> weight;
//					CERR << "fail="<< line_buffer.fail()<< "; eof="<< line_buffer.eof()<< "; bad="<< line_buffer.bad()<< "; count="<< line_buffer.gcount()<< "\n";
					if(!line_buffer){//failed
						line_buffer.clear();//clear all ifstream errors
						line_buffer.seekg(0,line_buffer.beg);//reset stream read position
						//attemp read unweighted edge
						line_buffer >> src >> std::ws >> dest;
//						CERR << "fail="<< line_buffer.fail()<< "; eof="<< line_buffer.eof()<< "; bad="<< line_buffer.bad()<< "; count="<< line_buffer.gcount()<< "\n";
						if(!line_buffer){//failed
							line_buffer.clear();//clear all ifstream errors
							line_buffer.seekg(0,line_buffer.beg);//reset stream read position
								//attemp read comment line
								std::string s;
								line_buffer >> s;
//								CERR << "fail="<< line_buffer.fail()<< "; eof="<< line_buffer.eof()<< "; bad="<< line_buffer.bad()<< "; count="<< line_buffer.gcount()<< "\n";
								state=4;
						}
						else{//success reading without weight
							state=1;
							weight=1;
							ed=Edge(src,dest,weight);
							lineNumber++;
						}
					}
					else{//success reading with weight
						state=1;
						if(par.type==LINK_WEIGHT::UNWEIGHTED){//ignore weight read from stream
							if(weight!=0) weight=1;//only ignore if edge is to be inserted
						}
						ed=Edge(src,dest,weight);
						lineNumber++;
					}
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
			}
		state=5;
		stts="End of file\n";
		return noEdge;
	}

	std::string status(){return stts;}
};

#ifdef FLAG_RCPP

/**
 * Reader for R Matrix
 */
class ReaderMatrixEdge: public ReaderInterface<Edge>{
private:
  std::string stts;
  const ProgramParameters & par;
  const Rcpp::NumericMatrix & dt;
  unsigned int lineNumber=0;
  Edge ed;
  int state=0;//=0 error; =1 reads edge; =4 reads newline; =5 end of file
  
public:
  ReaderMatrixEdge(const Rcpp::NumericMatrix & data,const ProgramParameters & parameters):stts("Ok"),par(parameters),dt(data),lineNumber(0),ed(noEdge),state(4){
    next();
  }
  
  ~ReaderMatrixEdge(){
  }
  
  /**
  *
  * @return the type of the next object or error condition
  */
  NEXTTYPE hasNext(){
    //TODO
    switch(state){
    default:
    case 0: return NEXTTYPE::CANNOTOPEN;
    case 1: return NEXTTYPE::VALUE;
    case 4: return NEXTTYPE::NEWLINE;
    case 5: return NEXTTYPE::ENDOFFILE;
    }
  }
  
  /**
  *
  * @param type is currently ignored
  * @return the next object of the requested type or the error message as indicated by hasNext()
  */
  Edge next(READTYPE type=READTYPE::VALUE){
    switch(state){
    case 0:
      break;
    case 1:
      state=4;
      return ed;
      break;
    case 4:
      typeVertex src;
      typeVertex dest;
      typeWeight weight;
      if(dt.rows()>lineNumber){//there are rows to process
        if(dt.cols()==3){//weighted
          src=dt(lineNumber,0);
          dest=dt(lineNumber,1);
          weight=dt(lineNumber,2);
          if(par.type==LINK_WEIGHT::UNWEIGHTED){//ignore weight
            if(weight!=0) weight=1;//only ignore if edge is to be inserted
          }
          ed=Edge(src,dest,weight);
          lineNumber++;
          state=1;
        }
        else if(dt.cols()==2){//unweighted
          src=dt(lineNumber,0);
          dest=dt(lineNumber,1);
          weight=1;
          ed=Edge(src,dest,weight);
          lineNumber++;
          state=1;
        }
        else{//error. Too much or too little data
          state=4;
        }
      }
      else{//no more data
        state=5;
        stts="End of file\n";
      }
      return noEdge;
      break;
    }
    state=5;
    stts="End of file\n";
    return noEdge;
  }
  
  std::string status(){return stts;}
};
#endif //FLAG_RCPP

#endif /* SRC_READER_H_ */
