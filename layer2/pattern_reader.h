// ===========================================================================
// 
//       Filename:  pattern_reader.h
// 
//    Description:  A similar code to read the output pattern from fullpf
// 
//        Version:  1.0
//        Created:  11/11/2017 12:21:38
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), zhenbin.wu@gmail.com
//        Company:  UIC, CMS@LPC, CDF@FNAL
// 
// ===========================================================================


#ifndef  MY_CTP7PATTERNREADER_INC
#define  MY_CTP7PATTERNREADER_INC

#include "firmware/data.h"
#include <cstdio>
#include <vector>
#include "string.h"

// ===========================================================================
//        Class:  CTP7PatternReader
//  Description:  
// ===========================================================================
class CTP7PatternReader
{
  public:

    // ====================  LIFECYCLE     ===============================
    CTP7PatternReader (const std::string &fname, const unsigned int nchann_max);                             // constructor
    CTP7PatternReader ( const CTP7PatternReader &other );   // copy constructor
    ~CTP7PatternReader ();                            // destructor

    // ====================  ACCESSORS     ===============================
    void GetMP7Word( MP7DataWord data_out[CTP7_NCHANN_OUT]);
    void TestMP7Word( MP7DataWord data_out[CTP7_NCHANN_OUT]);
    bool ReadEvent();

    // ====================  MUTATORS      ===============================

    // ====================  OPERATORS     ===============================

    CTP7PatternReader& operator = ( const CTP7PatternReader &other ); // assignment operator

    // ====================  DATA MEMBERS  ===============================

  protected:
    // ====================  METHODS       ===============================

    // ====================  DATA MEMBERS  ===============================

  private:
    // ====================  METHODS       ===============================
    const std::string fname_;
    //const unsigned int nmux_, nchann_, nempty_;
    //const bool fillmagic_;
    FILE *file_;

    char* line;
    size_t len;
    ssize_t readl;
    unsigned NoLine;

    // ====================  DATA MEMBERS  ===============================

}; // -----  end of class CTP7PatternReader  -----

#endif   // ----- #ifndef MY_CTP7PATTERNREADER_INC  -----
