/*
 *  Created on: 16 Feb 2015
 *      Author: poltergeist0
 *
 * Defines several functions used to print debug messages to standard error and an object capable of
 * printing those debug messages to file
 */

#ifndef SRC_DEBUGGING_H_
#define SRC_DEBUGGING_H_

/**
 * Define to enable asserts and add other debug and/or testing code
 * Debugging seriously slows down the program and should not be used in production
 */
#define FLAG_DEBUG

/**
 * Define to enable auto testing code where available.
 * This code is supposed to verify the consistency of the data inside a class after an operation has taken place.
 * As an example class AC has a method that modifies some of its data members. The verification code is supposed
 * to save the current state of the class at the beginning of the method call, before any other instruction, and
 * use it as comparison in the end of the method, just before the return instruction taking into account any
 * modified data.
 *
 * Example:
 * 	class AC{
 * 		vector v;
 * 		int a;
 *
 * 		add(int val){
 * 			savestate();
 * 			v.insert(val,a);//insert val in position a
 * 			a=val/v.size();
 * 			validatestate(val);
 * 		}
 *
 * 		#ifdef FLAG_DEBUG_VERIFY
 * 			vector debugv
 * 			int debuga
 * 		#endif
 *
 * 		savestate(){
 * 		#ifdef FLAG_DEBUG_VERIFY
 * 			debugv=v
 * 			debuga=a
 * 		#endif
 * 		}
 *
 * 		validatestate(int val){
 * 		#ifdef FLAG_DEBUG_VERIFY
 *			assert(a==val/(debugv.size()+1));
 *			assert(v.size()==debugv.size()+1)
 *			...compare all values of v and debugv except v[val]
 * 		#endif
 * 		}
 */
#define FLAG_DEBUG_VERIFY

/**
 * Define the following to enable direct access to private members of classes for testing code to verify correct implementation
 * Debugging must be enabled (NDEBUG flag undefined)
 * Must not be used in production
 */
#define FLAG_TEST

#include "debuggingInternals.h"

/**
 * Define the amount of time for the assert delay.
 * In other words, the amount of time to wait between the error occurring and the program halting.
 * This allows for buffered messages to finish printing before killing the program.
 */
#define ASSERT_DELAY 3000 //in milliseconds (3 seconds)

/**
 * numeric precision used by ASSERT_APPROXIMATE for comparing two numbers equally
 */
#define ASSERT_PRECISION_DEFAULT 1e-6


/**
 * numeric precision limit used by ASSERT_APPROXIMATE below which two numbers are identical (the same number)
 */
#define ASSERT_PRECISION_LIMIT 1e-16

/**
 * size of buffer used by the backtrace call stack.
 */
#define ASSERT_BACKTRACE_BUFFER_SIZE 1024

#define ASSERT_APPROXIMATE_MESSAGE(value1,value2,precision,message) ZERT_DO_NOT_USE_DIRECTLY3(#value1,#value2,value1,value2,precision,message)
#define ASSERT_APPROXIMATE(value1,value2,precision) ZERT_DO_NOT_USE_DIRECTLY3(#value1,#value2,value1,value2,precision,"")
#define ASSERT_EQUAL_MESSAGE(value1,value2,message) ZERT_DO_NOT_USE_DIRECTLY2(#value1,#value2,value1,value2,message)
#define ASSERT_EQUAL(value1,value2) ZERT_DO_NOT_USE_DIRECTLY2(#value1,#value2,value1,value2,"")
#define ASSERT_SMALLER(value1,value2,precision) ZERT_DO_NOT_USE_DIRECTLY2S(#value1,#value2,value1,value2,precision,"")
#define ASSERT_SMALLER_EQUAL(value1,value2,precision) ZERT_DO_NOT_USE_DIRECTLY2SE(#value1,#value2,value1,value2,precision,"")
#define ASSERT_GREATER(value1,value2,precision) ZERT_DO_NOT_USE_DIRECTLY2G(#value1,#value2,value1,value2,precision,"")
#define ASSERT_GREATER_EQUAL(value1,value2,precision) ZERT_DO_NOT_USE_DIRECTLY2GE(#value1,#value2,value1,value2,precision,"")
#define ASSERT_MESSAGE(value,message) ZERT_DO_NOT_USE_DIRECTLY1(#value,value,message)
#define ASSERT(value) ZERT_DO_NOT_USE_DIRECTLY1(#value,value,"")
#define ASSERT_NAN(value) ZERT_DO_NOT_USE_DIRECTLYNAN(#value,value,"")
#define ASSERT_NOT_NAN(value) ZERT_DO_NOT_USE_DIRECTLYNAN_NOT(#value,value,"")
#define ASSERT_NAN_MESSAGE(value,message) ZERT_DO_NOT_USE_DIRECTLYNAN(#value,value,message)
#define ASSERT_NOT_NAN_MESSAGE(value,message) ZERT_DO_NOT_USE_DIRECTLYNAN_NOT(#value,value,message)
#define ASSERT_EQUAL_ITERATOR_MESSAGE(value1,value2,message) ZERT_DO_NOT_USE_DIRECTLY_ITERATOR(#value1,#value2,&(*value1),&(*value2),value1,value2,message)
#define ASSERT_EQUAL_ITERATOR(value1,value2) ZERT_DO_NOT_USE_DIRECTLY_ITERATOR(#value1,#value2,&(*value1),&(*value2),value1,value2,"")

#define ASSERT_NOT_APPROXIMATE_MESSAGE(value1,value2,precision,message) ZERT_DO_NOT_USE_DIRECTLY3NOT(#value1,#value2,value1,value2,precision,message)
#define ASSERT_NOT_APPROXIMATE(value1,value2,precision) ZERT_DO_NOT_USE_DIRECTLY3NOT(#value1,#value2,value1,value2,precision,"")
#define ASSERT_NOT_EQUAL_MESSAGE(value1,value2,message) ZERT_DO_NOT_USE_DIRECTLY2NOT(#value1,#value2,value1,value2,message)
#define ASSERT_NOT_EQUAL(value1,value2) ZERT_DO_NOT_USE_DIRECTLY2NOT(#value1,#value2,value1,value2,"")
#define ASSERT_NOT_EQUAL_ITERATOR_MESSAGE(value1,value2,message) ZERT_DO_NOT_USE_DIRECTLY_ITERATOR_NOT(#value1,#value2,&(*value1),&(*value2),value1,value2,message)
#define ASSERT_NOT_EQUAL_ITERATOR(value1,value2) ZERT_DO_NOT_USE_DIRECTLY_ITERATOR_NOT(#value1,#value2,&(*value1),&(*value2),value1,value2,"")

//#include "Trace.h"
//#include "../utilities/timeFunctions.h"
//
//namespace Debug {
//
//class Debug{
//private:
//#ifndef NDEBUG
//	Trace trc;
//	std::ofstream& df;
//	uint64 st;//start time. NOTE: This variable MUST be initialized at program start
//#endif //NDEBUG
//
//public:
//	Debug(std::ofstream& debugFile,uint64 start_time)
//	#ifndef NDEBUG
//				:df(debugFile),st(start_time)
//	#endif //FLAG_TRACE
//		{
//		}
//
//	/**
//	 * copy constructor
//	 */
//	Debug(const Debug& d)
//#ifndef NDEBUG
//			:trc(d.trc),df(d.df),st(d.st)
//#endif //NDEBUG
//	{}
//
//	/**
//	 * add a node
//	 * use at the beginning of a function
//	 */
//	inline void add(const std::string& nodeInfo){
//		trc.add(nodeInfo);
//	}
//	/**
//	 * remove last added node
//	 * use just before returning from a function
//	 */
//	inline void remove(){
//		trc.remove();
//	}
//
//	/**
//	 * write message to string stream, if message is not null
//	 * includes trace
//	 */
//	inline void write(std::stringstream& ss,const char * s){
//#ifndef NDEBUG
//		ss << Utilities::currentTime()-st << "\t";
//		trc.write(ss);
//		if(s!=NULL) ss <<"\t"<< s;
//#endif //NDEBUG
//	}
//
//	/**
//	 * write trace to string
//	 */
//	inline std::string write(){
//		std::stringstream ss;
//#ifndef NDEBUG
//		write(ss,NULL);
//#endif //NDEBUG
//		return ss.str();
//	}
//
//	/**
//	 * write message to debug file
//	 * includes trace
//	 * line terminated with new line character
//	 */
//	inline void write(const char * s){
//		std::stringstream ss;
//#ifndef NDEBUG
//		write(ss,s);
//		ss << "\n";
//#endif //NDEBUG
//		df << ss.str();
//	}
//
//	inline void write_out(const char * s){
//#ifndef NDEBUG
//		std::stringstream ss;
//		write(ss,s);
//		ss << "\n";
//		std::cout << ss.str();
//#endif //NDEBUG
//	}
//
//	inline void write_err(const char * s){
//#ifndef NDEBUG
//		std::stringstream ss;
//		write(ss,s);
//		ss << "\n";
//		std::cerr << ss.str();
//#endif //NDEBUG
//	}
//};
//
//}  // namespace Debug
//
//#include "Includes.h"
//
//namespace Debuging {
//
//#ifdef DEBUG
//
////inline void printHex(const uchar s,const std::string header, const std::string tail){
////	std::cerr << header;
////	std::ios_base::fmtflags f(std::cerr.flags());	//save cout configuration
////	int a;
////	a=s;
////	std::cerr << std::hex << a;
////	std::cerr.flags(f);	//restore cout configuration
////	std::cerr << tail << std::endl;
////}
//
////inline void printHex(const uchar* s, int size,const std::string header, const std::string tail){
////	std::cerr << header;
////	std::ios_base::fmtflags f(std::cerr.flags());	//save cout configuration
////	int a;
////	for(int it=0;it!=size;++it){
////		a=s[it];
////		std::cerr << std::hex << a;
////	}
////	std::cerr.flags(f);	//restore cout configuration
////	std::cerr << tail << std::endl;
////}
//
////inline void printHex(const ustring s,const std::string header, const std::string tail){
////	std::cerr << header;
////	std::ios_base::fmtflags f(std::cerr.flags());	//save cout configuration
////	int a;
////	for(auto it=s.begin();it!=s.end();++it){
////		a=*it;
////		std::cerr << std::hex << a;
////	}
////	std::cerr.flags(f);	//restore cout configuration
////	std::cerr << tail << std::endl;
////}
//
////inline void printHex(const uint16 s,const std::string header, const std::string tail){
////	std::cerr << header;
////	std::ios_base::fmtflags f(std::cerr.flags());	//save cout configuration
////	std::cerr << std::hex << s;
////	std::cerr.flags(f);	//restore cout configuration
////	std::cerr << tail << std::endl;
////}
//
////inline void printHex(const uint32 s,const std::string header, const std::string tail){
////	std::cerr << header;
////	std::ios_base::fmtflags f(std::cerr.flags());	//save cout configuration
////	std::cerr << std::hex << s;
////	std::cerr.flags(f);	//restore cout configuration
////	std::cerr << tail << std::endl;
////}
//
///**
// * @brief printError prints an error message formed by a string
// * @param er
// * @param printHeader
// * @param newLine
// */
//inline void printError(const std::string& er,bool printHeader, bool newLine){
//    if(printHeader) std::cerr << "ERROR: ";
//    std::cerr << er;
//    if(newLine)std::cerr << std::endl;
//}
//
///**
// * @brief printError prints an error message formed by an unsigned integer of 64 bit
// * @param er
// * @param printHeader
// * @param newLine
// */
//inline void printError(const uint64 er,bool printHeader, bool newLine){
//    if(printHeader) std::cerr << "ERROR: ";
//    std::cerr << er;
//    if(newLine)std::cerr << std::endl;
//}
//
///**
// * @brief printDebug prints a debug message formed by a string
// * @param db
// * @param printHeader
// * @param newLine
// */
//inline void printDebug(const std::string& db,bool printHeader, bool newLine){
//    if(printHeader) std::cerr << "DEBUG: ";
//    std::cerr << db;
//    if(newLine)std::cerr << std::endl;
//}
//
///**
// * @brief printDebug prints a debug message formed by an unsigned integer of 64 bit
// * @param db
// * @param printHeader
// * @param newLine
// */
//inline void printDebug(uint64 db,bool printHeader, bool newLine){
//    if(printHeader) std::cerr << "DEBUG: ";
//    std::cerr << db;
//    if(newLine)std::cerr << std::endl;
//}
//
///**
// * @brief printInfo prints an information message formed by a string
// * @param db
// * @param printHeader
// * @param newLine
// */
//inline void printInfo(const std::string& db,bool printHeader, bool newLine){
//    if(printHeader) std::cerr << "INFO: ";
//    std::cerr << db;
//    if(newLine)std::cerr << std::endl;
//}
//
///**
// * @brief printInfo prints an information message formed by an unsigned integer of 64 bit
// * @param db
// * @param printHeader
// * @param newLine
// */
//inline void printInfo(uint64 db,bool printHeader, bool newLine){
//    if(printHeader) std::cerr << "INFO: ";
//    std::cerr << db;
//    if(newLine)std::cerr << std::endl;
//}
//
///**
// * @brief The DebugPrint class allows printing of debug messages to a log file
// */
//class DebugPrint{
//private:
////    const unsigned long size = 1024*1024;   //1MB buffer
////    unsigned long long a[size];
//
////        FILE* pFile;
////        pFile = fopen("file.binary", "wb");
////        for (unsigned long long j = 0; j < 1024; ++j){
////            //Some calculations to fill a[]
////            fwrite(a, 1, size*sizeof(unsigned long long), pFile);
////        }
////        fclose(pFile);
//    /**
//     * @brief file is the file stream
//     */
//    std::ofstream file;
//
//public:
//    /**
//     * @brief DebugPrint
//     * @param filename
//     */
//    DebugPrint(const std::string& filename):file( filename ){
//    }
//    ~DebugPrint(){
//        file.flush();
//        file.close();
//    }
//
//    /**
//     * @brief flush forces a file stream flush (write memory buffer to disk). Accumulating too much data in the buffer will slow the program considerably when it is finally written to disk.
//     */
//    inline void flush(){file.flush ();}
//
//    /**
//     * @brief print prints a string
//     * @param s
//     * @param header
//     * @param tail
//     * @param newLine
//     */
//    inline void print(const std::string& s,const std::string& header, const std::string& tail, bool newLine){
//        file << header;
//        file << s;
//        file << tail;
//        if(newLine)file << '\n';//std::endl;
//    }
//
//    /**
//     * @brief printHex prints a single unsigned char in hexadecimal
//     * @param s
//     * @param header
//     * @param tail
//     * @param newLine
//     */
//    inline void printHex(const uchar s,const std::string& header, const std::string& tail, bool newLine){
//        file << header << " (0x)";
//        int a=s;
//        file << std::hex << a;
//        file << tail;
//        if(newLine)file << '\n';//std::endl;
//    }
//
//    /**
//     * @brief printHex prints, in hexadecimal, an unsigned char array in the form of a pointer and a size
//     * @param s
//     * @param size
//     * @param header
//     * @param tail
//     * @param newLine
//     */
//    inline void printHex(const uchar* s, int size,const std::string& header, const std::string& tail, bool newLine){
//        file << header << " (0x)";
//        int a;
//        for(int it=0;it!=size;++it){
//            a=s[it];
//            file << std::hex << a;
//        }
//        file << tail;
//        if(newLine)file << '\n';//std::endl;
//    }
//
//    /**
//     * @brief printHex prints, in hexadecimal, an unsigned string
//     * @param s
//     * @param header
//     * @param tail
//     * @param newLine
//     */
//    inline void printHex(const ustring& s,const std::string& header, const std::string& tail, bool newLine){
//        file << header << " (0x)";
//        int a;
//        for(auto it=s.begin();it!=s.end();++it){
//            a=*it;
//            file << std::hex << a;
//        }
//        file << tail;
//        if(newLine)file << '\n';//std::endl;
//    }
//
//    /**
//     * @brief printHex prints, in hexadecimal, an unsigned integer of 64 bit
//     * @param s
//     * @param header
//     * @param tail
//     * @param newLine
//     */
//    inline void printHex(const uint64 s,const std::string& header, const std::string& tail, bool newLine){
//        file << header << " (0x)";
//        file << std::hex << s;
//        file << tail;
//        if(newLine)file << '\n';//std::endl;
//    }
//
//    inline void printHex(const uint32 s,const std::string header, const std::string tail, bool newLine){
//        file << header << " (0x)";
//        file << std::hex << s;
//        file << tail;
//        if(newLine)file << std::endl;
//    }
//
//    /**
//     * @brief printError prints an error message formed by a string
//     * @param er
//     * @param printHeader
//     * @param newLine
//     */
//    inline void printError(std::string& er,bool printHeader, bool newLine){
//        if(printHeader) file << "ERROR: ";
//        file << er;
//        if(newLine)file << '\n';//std::endl;
//    }
//
//    /**
//     * @brief printDebug prints a debug message formed by a string
//     * @param db
//     * @param printHeader
//     * @param newLine
//     */
//    inline void printDebug(const std::string& db,bool printHeader, bool newLine){
//        if(printHeader) file << "DEBUG: ";
//        file << db;
//        if(newLine)file << '\n';//std::endl;
//    }
//
//    /**
//     * @brief printInfo prints an information message formed by a string
//     * @param db
//     * @param printHeader
//     * @param newLine
//     */
//    inline void printInfo(const std::string& db,bool printHeader, bool newLine){
//        if(printHeader) file << "INFO: ";
//        file << db;
//        if(newLine)file << '\n';//std::endl;
//    }
//
//    inline void printBin(const uchar s,const std::string header, const std::string tail, bool newLine){
//        file << header << " (0b)";
//        file.write(reinterpret_cast<const char*>(&s),1);
//        file << std::endl << tail;
//        if(newLine)file << std::endl;
//    }
//
//    inline void printBin(const uchar* s, int size,const std::string header, const std::string tail, bool newLine){
//        file << header << " (0b)";
//        file.write(reinterpret_cast<const char*>(s),size);
//        file << std::endl << tail;
//        if(newLine)file << std::endl;
//    }
//
//    inline void printBin(const ustring s,const std::string header, const std::string tail, bool newLine){
//        file << header << " (0b)";
//        file.write(reinterpret_cast<const char*>(s.data()),s.size());
//        file << std::endl << tail;
//        if(newLine)file << std::endl;
//    }
//
////    inline void printDec(const uint16 s,const std::string header, const std::string tail){
////        file << header;
////        file << std::dec << s;
////        file << tail << std::endl;
////    }
//
//    inline void printDec(const uint32 s,const std::string header, const std::string tail, bool newLine){
//        file << header;
//        file << std::dec << s;
//        file << tail;
//        if(newLine)file << std::endl;
//    }
//
//    /**
//     * @brief printDecU prints, in decimal, an unsigned integer of 64 bit
//     * @param s
//     * @param header
//     * @param tail
//     * @param newLine
//     */
//    inline void printDecU(const uint64 s,const std::string& header, const std::string& tail, bool newLine){
//        file << header;
//        file << std::dec << s;
//        file << tail;
//        if(newLine)file << '\n';//std::endl;
//    }
//
//    /**
//     * @brief printDec  prints, in decimal, a double
//     * @param s
//     * @param header
//     * @param tail
//     * @param newLine
//     */
//    inline void printDec(const double s,const std::string& header, const std::string& tail, bool newLine){
//        file << header;
//        file << std::dec << s;
//        file << tail;
//        if(newLine)file << '\n';//std::endl;
//    }
//
//};
//
//inline void printHex(const uchar s,const std::string header, const std::string tail){
//	std::cerr << header;
//	std::ios_base::fmtflags f(std::cerr.flags());	//save cout configuration
//	int a;
//	a=s;
//	std::cerr << std::hex << a;
//	std::cerr.flags(f);	//restore cout configuration
//	std::cerr << tail << std::endl;
//}
//
//inline void printHex(const uchar* s, int size,const std::string header, const std::string tail){
//	std::cerr << header;
//	std::ios_base::fmtflags f(std::cerr.flags());	//save cout configuration
//	int a;
//	for(int it=0;it!=size;++it){
//		a=s[it];
//		std::cerr << std::hex << a;
//	}
//	std::cerr.flags(f);	//restore cout configuration
//	std::cerr << tail << std::endl;
//}
//
//inline void printHex(const ustring s,const std::string header, const std::string tail){
//	std::cerr << header;
//	std::ios_base::fmtflags f(std::cerr.flags());	//save cout configuration
//	int a;
//	for(auto it=s.begin();it!=s.end();++it){
//		a=*it;
//		std::cerr << std::hex << a;
//	}
//	std::cerr.flags(f);	//restore cout configuration
//	std::cerr << tail << std::endl;
//}
//
//inline void printHex(const uint16 s,const std::string header, const std::string tail){
//	std::cerr << header;
//	std::ios_base::fmtflags f(std::cerr.flags());	//save cout configuration
//	std::cerr << std::hex << s;
//	std::cerr.flags(f);	//restore cout configuration
//	std::cerr << tail << std::endl;
//}
//
//inline void printHex(const uint32 s,const std::string header, const std::string tail){
//	std::cerr << header;
//	std::ios_base::fmtflags f(std::cerr.flags());	//save cout configuration
//	std::cerr << std::hex << s;
//	std::cerr.flags(f);	//restore cout configuration
//	std::cerr << tail << std::endl;
//}
//
//inline void printError(std::string er,bool printHeader, bool newLine){
//	if(printHeader) std::cerr << "ERROR: ";
//	std::cerr << er;
//	if(newLine)std::cerr << std::endl;
//}
//
//inline void printDebug(std::string db,bool printHeader, bool newLine){
//	if(printHeader) std::cerr << "DEBUG: ";
//	std::cerr << db;
//	if(newLine)std::cerr << std::endl;
//}
//
//inline void printInfo(std::string db,bool printHeader, bool newLine){
//	if(printHeader) std::cerr << "INFO: ";
//	std::cerr << db;
//	if(newLine)std::cerr << std::endl;
//}
//
//DebugPrint dprint("log.txt");
//
//class DebugConsolePrint{
//private:
////    std::ofstream file;
//
//public:
//    void print(const std::stringstream& s){
//        std::cout << s;
//    }
//
//    void print(const std::string& s){
//        std::cout << s << std::endl;
//    }
//};
//
//DebugConsolePrint cprint;
//#endif	//DEBUG
//
//}  // namespace Debuging

///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////
///////////////////////////////// rule enforcement section below this line. DO NOT MODIFY ///////////////////////////

#ifndef FLAG_DEBUG
	//if debugging is disabled:
	//disable consistency verification
	#undef FLAG_DEBUG_VERIFY
	//disable testing
	#undef FLAG_TEST
#endif


#endif /* SRC_DEBUGGING_H_ */
