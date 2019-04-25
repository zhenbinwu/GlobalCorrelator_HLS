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

void test_JetClu_ref(PFChargedObj inch[NTRACK], PFNeutralObj inpho[NPHOTON], PFNeutralObj inne[NSELCALO], PFChargedObj inmu[NMU]);
void GetCombination_ref( const etaphi_t etas[NPARTICLE], const etaphi_t phis[NPARTICLE], ap_int<NPARTICLE_BITS> combs[2][2*NPARTICLE], etaphi2_t combdRs[2*NPARTICLE]);
void DynamicGroups_ref( const ap_int<NPARTICLE_BITS> combs[2][2*NPARTICLE], ap_int<1> groups[12][NPARTICLE], ap_int<1> groupcombs[12][2*NPARTICLE]);

#endif   // ----- #ifndef MY_SIMPLE_JETCLUSTER_REF_INC  -----
