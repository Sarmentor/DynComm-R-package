/************************************************************************
 ************************* Developer Notice *****************************
 ************************************************************************
 * @details
 *
 * This file defines the reader class interface, a dummy reader class and
 * classes to read edges from string and file.
 *
 * There should never be any reason to change it unless to add more
 * readers.
 *
 *
 * @author poltergeist0
 *
 * @date 2019-02-06
 ************************************************************************
 ************************************************************************
 ************************************************************************/

#ifndef SRC_READER_H_
#define SRC_READER_H_

#include <string>
#include <fstream>
#include "program.h"
#include "edge.h"

/**
 * @brief Interface for a simple stream forward reader.
 *
 * @details
 * Format is one set of values per line, with values separated by any white
 * space (space or tab) in any amount.
 * Readers that implement this interface can not read backwards.
 *
 * @author poltergeist0
 *
 * @date 2019-02-06
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
 * @brief Dummy Reader.
 *
 * @details
 *
 * Can be used when a reader is not going to be used but is required, like when
 * randomly generating edges, or returned when failing to create a reader.
 *
 * @author poltergeist0
 *
 * @date 2019-02-06
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
 * @brief Reader for edges from file.
 *
 * @details
 * Format is one edge per line, with values separated by any white
 * space (space or tab) in any amount.
 * This Reader can not read backwards.
 *
 * @author poltergeist0
 *
 * @date 2019-02-06
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

/**
 * @brief Reader for edges from string
 *
 * @details
 * Format is one edge per line, with values separated by any white
 * space (space or tab) in any amount.
 * This Reader can not read backwards.
 * Example valid string: "1 2\n#comment\n\n1 3\n2 3\n3 6\n4 6\n4 5\n5 7\n6 7"
 *
 * @author poltergeist0
 *
 * @date 2019-02-06
 */
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
 * @brief Reader for edges from R Matrix.
 *
 * @details
 * Format is one edge per line.
 * This Reader can not read backwards.
 *
 * @author poltergeist0
 *
 * @date 2019-02-06
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
        // COUT << dt.cols() << "\n";
        // COUT << ((par.type==LINK_WEIGHT::WEIGHTED)?"WEIGHTED":"UNWEIGHTED") << "\n";
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
