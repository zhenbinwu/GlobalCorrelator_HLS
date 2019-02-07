// ===========================================================================
// 
//       Filename:  simple_jetcluster_ref.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  02/07/2019 10:43:34 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), zhenbin.wu@gmail.com
//        Company:  UIC, CMS@LPC, CDF@FNAL
// 
// ===========================================================================


#ifndef  MY_SIMPLE_JETCLUSTER_REF_INC
#define  MY_SIMPLE_JETCLUSTER_REF_INC

#include "ap_int.h"
#include "firmware/data.h"
#include "firmware/simple_fullpfalgo.h"

void test_PFHT_ref(PFChargedObj inch[NTRACK], PFNeutralObj inpho[NPHOTON], PFNeutralObj inne[NSELCALO], PFChargedObj inmu[NMU], ap_uint<32> &HT);

#endif   // ----- #ifndef MY_SIMPLE_JETCLUSTER_REF_INC  -----
