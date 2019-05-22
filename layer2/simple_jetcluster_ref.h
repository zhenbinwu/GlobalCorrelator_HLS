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
#include "../../firmware/data.h"
#include "../../firmware/simple_fullpfalgo.h"


void GetCombination_ref(const pt_t pts[NPARTICLE], const etaphi_t etas[NPARTICLE], const etaphi_t phis[NPARTICLE], ap_uint<NPARTICLE> combsdR[NPARTICLE], ap_uint<NPARTICLE> combsPt[NPARTICLE]);
void CountGroups_ref( const ap_uint<NPARTICLE> grouping[NPARTICLE]);
void DynamicGroups_ref( const ap_uint<NPARTICLE> combs[NPARTICLE], ap_uint<NPARTICLE> grouping[NPARTICLE]);
void test_JetClu_ref(PFNeutralObj input[NPARTICLE], ap_uint<NPARTICLE> grouping[NPARTICLE]);
#endif   // ----- #ifndef MY_SIMPLE_JETCLUSTER_REF_INC  -----
