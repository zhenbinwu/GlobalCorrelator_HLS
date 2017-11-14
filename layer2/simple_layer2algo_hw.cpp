#include "simple_layer2algo_hw.h"
#include <cmath>
#include <cassert>
#ifndef __SYNTHESIS__
#include <cstdio>
#endif


void test_PFHT(PFChargedObj inch[NTRACK], PFNeutralObj inpho[NPHOTON], PFNeutralObj inne[NSELCALO], PFChargedObj inmu[NMU], ap_uint<32> &HT)
{
    #pragma HLS ARRAY_PARTITION variable=inch complete
    #pragma HLS ARRAY_PARTITION variable=inpho complete
    #pragma HLS ARRAY_PARTITION variable=inne complete
    #pragma HLS ARRAY_PARTITION variable=inmu complete

  HT = 0;
  for (int ich = 0; ich < NTRACK; ++ich) {
#pragma HLS unroll
    HT  = HT +inch[ich].hwPt;
  }
  for (int iph = 0; iph < NPHOTON; ++iph) {
#pragma HLS unroll
    HT  = HT + inpho[iph].hwPt;
  }
  for (int ine = 0; ine < NSELCALO; ++ine) {
#pragma HLS unroll
    HT  = HT + inne[ine].hwPt;
  }
  for (int imu = 0; imu < NMU; ++imu) {
#pragma HLS unroll
    HT  = HT+ inmu[imu].hwPt;
  }
}
