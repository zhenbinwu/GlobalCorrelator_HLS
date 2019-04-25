// ===========================================================================
// 
//       Filename:  simple_jetcluster_hw.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  02/07/2019 10:42:20 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), zhenbin.wu@gmail.com
//        Company:  UIC, CMS@LPC, CDF@FNAL
// 
// ===========================================================================


#ifndef  MY_SIMPLE_JETCLUSTER_HW_INC
#define  MY_SIMPLE_JETCLUSTER_HW_INC

#include "ap_int.h"
#include "simple_algo_find_max.h"
#include "../firmware/data.h"
#include "../firmware/simple_fullpfalgo.h"
#include "../puppi/firmware/simple_puppi.h"

void test_JetClu(PFChargedObj inch[NTRACK], PFNeutralObj inpho[NPHOTON], PFNeutralObj inne[NSELCALO], PFChargedObj inmu[NMU]);
void GetCombination( const etaphi_t etas[NPARTICLE], const etaphi_t phis[NPARTICLE], ap_int<1> combs[NPARTICLE][NPARTICLE], etaphi2_t combdRs[2*NPARTICLE]);
void DynamicGroups( const ap_int<NPARTICLE_BITS> combs[2][2*NPARTICLE], ap_int<1> groups[12][NPARTICLE], ap_int<1> groupcombs[12][2*NPARTICLE]);
int _lut_shift15_divide(ap_uint<17> num, ap_uint<9> den);
void _lut_shift15_invert_init(ap_uint<16> _table[512]);
binindex_t MetricAK( ap_int<1> groupcomb[2*NPARTICLE], ap_int<NPARTICLE_BITS> combs[2][2*NPARTICLE], etaphi2_t combdRs[2*NPARTICLE], pt_t pts[NPARTICLE]);
#endif   // ----- #ifndef MY_SIMPLE_JETCLUSTER_HW_INC  -----

