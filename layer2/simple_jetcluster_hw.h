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

#include  <hls_math.h>

typedef struct ptsort
{
  pt_t pt;
  ap_uint<50> idx;

  inline bool operator>(const ptsort &a) const
  {
    return pt > a.pt;
  }
  inline bool operator<(const ptsort &a) const
  {
    return pt < a.pt;
  }
} ptsort;
//void test_JetClu(PFChargedObj inch[NTRACK], PFNeutralObj inpho[NPHOTON], PFNeutralObj inne[NSELCALO], PFChargedObj inmu[NMU]);
//void test_JetClu(PFChargedObj inch[NTRACK], PFNeutralObj inpho[NPHOTON], PFNeutralObj inne[NSELCALO], PFChargedObj inmu[NMU], ap_uint<NPARTICLE> grouping[NPARTICLE]);


void test_JetClu(PFNeutralObj input[NPARTICLE], PFNeutralObj output[NPARTICLE/2]);
bool SortPartbyPt( const pt_t pts[NPARTICLE], ap_uint<NPARTICLE> combsdR[NPARTICLE], ap_uint<INPUT_DATA_SIZE> partlabel[NPARTICLE]);
void FormingJets_simple( const pt_t pts[NPARTICLE], const etaphi_t etas[NPARTICLE], const etaphi_t phis[NPARTICLE], ap_uint<INPUT_DATA_SIZE> partlabel[NPARTICLE], PFNeutralObj output[INPUT_DATA_SIZE]);

void FormingJets( const pt_t pts[NPARTICLE], const etaphi_t etas[NPARTICLE], const etaphi_t phis[NPARTICLE], PFNeutralObj output[INPUT_DATA_SIZE]);
void GetCombination( const etaphi_t etas[NPARTICLE], const etaphi_t phis[NPARTICLE], ap_int<1> combs[NPARTICLE][NPARTICLE], etaphi2_t combdRs[2*NPARTICLE]);
int _lut_shift15_divide(ap_uint<17> num, ap_uint<9> den);
void _lut_shift15_invert_init(ap_uint<16> _table[512]);
binindex_t MetricAK( ap_int<1> groupcomb[2*NPARTICLE], ap_int<NPARTICLE_BITS> combs[2][2*NPARTICLE], etaphi2_t combdRs[2*NPARTICLE], pt_t pts[NPARTICLE]);

void GetCombination1D( const etaphi_t etas[NPARTICLE], const etaphi_t phis[NPARTICLE], ap_uint<NPARTICLE> combs[NPARTICLE]);
void GetCombination2D( const etaphi_t etas[NPARTICLE], const etaphi_t phis[NPARTICLE], ap_uint<NPARTICLE> combs1d[NPARTICLE]);
bool SortPartbyPt( const pt_t pts[NPARTICLE], ap_uint<NPARTICLE> combsPt[NPARTICLE]);
void GetCombinationPt( const pt_t pts[NPARTICLE], ap_uint<NPARTICLE> combsPt[NPARTICLE]);

void CountGroups( const ap_uint<NPARTICLE> grouping[NPARTICLE]);
void DynamicGroups( const ap_uint<NPARTICLE> combs[NPARTICLE], ap_uint<NPARTICLE> grouping[NPARTICLE]);


template<class array_t>
void BitonicSortOptimizedInline(array_t work_array[INPUT_DATA_SIZE]);
template<class array_t, class index_t, class mask_t>
void elementLoop(array_t a[], index_t j, mask_t mask);
template<class array_t, class index_t>
void exchange(array_t a[], index_t i, index_t j);
#endif   // ----- #ifndef MY_SIMPLE_JETCLUSTER_HW_INC  -----

