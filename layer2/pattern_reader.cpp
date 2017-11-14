// ===========================================================================
// 
//       Filename:  pattern_reader.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  11/11/2017 12:23:54
//       Compiler:  g++ -std=c++11
// 
//         Author:  Zhenbin Wu (benwu)
//          Email:  zhenbin.wu@gmail.com
//        Company:  UIC, CMS@LPC, CDF@FNAL
// 
// ===========================================================================

#include "pattern_reader.h"
//----------------------------------------------------------------------------
//       Class:  CTP7PatternReader
//      Method:  CTP7PatternReader
// Description:  constructor
//----------------------------------------------------------------------------
CTP7PatternReader::CTP7PatternReader (const std::string &fname, const unsigned int nchann_max):
  fname_(fname), file_(NULL), len(1000), line(NULL)
{
    if (!fname.empty()) {
        file_ = fopen(fname.c_str(), "r");
		if (file_ == NULL) {
		  printf("Error opening file: %s", fname.c_str());
		} 
    }

    // Skip the first 3 line header
    readl = getline(&line, &len, file_);  
    readl = getline(&line, &len, file_); 
    readl = getline(&line, &len, file_); 
    NoLine = 3;
}  // -----  end of method CTP7PatternReader::CTP7PatternReader  (constructor)  -----

//----------------------------------------------------------------------------
//       Class:  CTP7PatternReader
//      Method:  CTP7PatternReader
// Description:  copy constructor
//----------------------------------------------------------------------------
CTP7PatternReader::CTP7PatternReader ( const CTP7PatternReader &other )
{
}  // -----  end of method CTP7PatternReader::CTP7PatternReader  (copy constructor)  -----

//----------------------------------------------------------------------------
//       Class:  CTP7PatternReader
//      Method:  ~CTP7PatternReader
// Description:  destructor
//----------------------------------------------------------------------------
CTP7PatternReader::~CTP7PatternReader ()
{
}  // -----  end of method CTP7PatternReader::-CTP7PatternReader  (destructor)  -----

//----------------------------------------------------------------------------
//       Class:  CTP7PatternReader
//      Method:  operator =
// Description:  assignment operator
//----------------------------------------------------------------------------
  CTP7PatternReader&
CTP7PatternReader::operator = ( const CTP7PatternReader &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // -----  end of method CTP7PatternReader::operator =  (assignment operator)  ---



// ===  FUNCTION  ============================================================
//         Name:  CTP7PatternReader::ReadEvent
//  Description:  
// ===========================================================================
bool CTP7PatternReader::ReadEvent()
{
  readl = getline(&line, &len, file_);  
  NoLine++;
  if (readl == -1) return false;

  return true;
}       // -----  end of function CTP7PatternReader::ReadEvent  -----

void CTP7PatternReader::TestMP7Word( MP7DataWord data_out[CTP7_NCHANN_OUT]) 
{
  std::cout << data_out <<std::endl;
  for (int i = 0; i < CTP7_NCHANN_OUT; ++i) { 
    std::cout <<i <<" vvval " << data_out[i] <<std::endl;}
}
// ===  FUNCTION  ============================================================
//         Name:  CTP7PatternReader::GetMP7Word
//  Description:  
// ===========================================================================
void CTP7PatternReader::GetMP7Word( MP7DataWord data_out[CTP7_NCHANN_OUT]) 
{
  char* pch;
  unsigned cnt= 0;
  pch = strtok(strtok (line, "\n"), " ");
  while (pch != NULL)
  {
    if (cnt > 0)
    {
      data_out[cnt-1] = (unsigned)strtol(pch, NULL, 16);
    }
    cnt++;
    pch = strtok(NULL, " ");
  }
}       // -----  end of function CTP7PatternReader::GetMP7Word  -----
