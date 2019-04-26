// ===========================================================================
// 
//       Filename:  simple_jetcluster_hw.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  02/07/2019 10:42:57 AM
//       Compiler:  g++ -std=c++11
// 
//         Author:  Zhenbin Wu (benwu)
//          Email:  zhenbin.wu@gmail.com
//        Company:  UIC, CMS@LPC, CDF@FNAL
// 
// ===========================================================================


#include "simple_jetcluster_hw.h"
#include <cmath>
#include <cassert>
#ifndef __SYNTHESIS__
#include <cstdio>
#endif

//inline ap_int<10> GetRegion(etaphi_t eta, etaphi_t phi)
//{
  //const ap_fixed<10,6> phigap = 0.628;
  //const ap_fixed<10,6> etagap = 0.5;
  //ap_int<5> phiidx = (phi + 3.14)/ phigap;
  //ap_int<5> etaidx = (eta+5)/etagap;

  //return (etaidx << 5) + phiidx;
//}


void test_JetClu(PFChargedObj inch[NTRACK], PFNeutralObj inpho[NPHOTON], PFNeutralObj inne[NSELCALO], PFChargedObj inmu[NMU])
{

#pragma HLS ARRAY_PARTITION variable=inch complete
#pragma HLS ARRAY_PARTITION variable=inpho complete
#pragma HLS ARRAY_PARTITION variable=inne complete
#pragma HLS ARRAY_PARTITION variable=inmu complete
  //Merge input and regionalized
  //Use base type as CaloObj
  unsigned idx = 0;

  static pt_t pts[NPARTICLE] = {0};
  static etaphi_t etas[NPARTICLE] = {0};
  static etaphi_t phis[NPARTICLE] = {0};

  // 2D array for storing index of particle pairs, within dEta, dPhi
  static ap_int<1> combs2D[NPARTICLE][NPARTICLE] = {0};
  static ap_int<NPARTICLE> combs1D[NPARTICLE] = {0};
  // dR^2 of the pairs
  static etaphi2_t combdRs[2*NPARTICLE] = {0};
  // Forming group of particles, based on the combination
  static ap_int<1> groups[12][NPARTICLE] = {0};
  // Index of combination in the group
  static ap_int<1> groupcombs[12][2*NPARTICLE] = {0};

#pragma HLS ARRAY_PARTITION variable=pts         complete
#pragma HLS ARRAY_PARTITION variable=etas        complete
#pragma HLS ARRAY_PARTITION variable=phis        complete
#pragma HLS ARRAY_PARTITION variable=combs       complete
#pragma HLS ARRAY_PARTITION variable=combdRs     complete
#pragma HLS ARRAY_PARTITION variable=groups      complete
#pragma HLS ARRAY_PARTITION variable=groupcombs  complete
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Unrolling all the inputs, ignoring types ~~~~~
unrollCH: for (int ich = 0; ich < NTRACK; ++ich) {
#pragma HLS unroll
    pts[idx+ich] = inch[ich].hwPt;
    etas[idx+ich] = inch[ich].hwEta;
    phis[idx+ich] = inch[ich].hwPhi;
  }
  idx = NTRACK;

unrollPho: for (int iph = 0; iph < NPHOTON; ++iph) {
#pragma HLS unroll
    pts[idx+iph] = inpho[iph].hwPt;
    etas[idx+iph] = inpho[iph].hwEta;
    phis[idx+iph] = inpho[iph].hwPhi;
  }
  idx += NPHOTON;

unrollNE:for (int ine = 0; ine < NSELCALO; ++ine) {
#pragma HLS unroll
    pts[idx+ine] = inne[ine].hwPt;
    etas[idx+ine] = inne[ine].hwEta;
    phis[idx+ine] = inne[ine].hwPhi;
  }
  idx += NSELCALO;
unrollMu:for (int imu = 0; imu < NMU; ++imu) {
#pragma HLS unroll
    pts[idx+imu] = inmu[imu].hwPt;
    etas[idx+imu] = inmu[imu].hwEta;
    phis[idx+imu] = inmu[imu].hwPhi;
  }


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Getting the neiboring combination ~~~~~
  //GetCombination(etas, phis, combs, combdRs);
  //DynamicGroups(combs, groups, groupcombs);
////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Getting metrics ~~~~~


////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Run the algo per group ~~~~~
  //for (int g = 0; g < 12; ++g)
  //{
//#pragma HLS pipeline
    //binindex_t idx = MetricAK(groupcombs[g], combs, combdRs, pts );
  //}
}


void GetCombination2D( const etaphi_t etas[NPARTICLE], const etaphi_t phis[NPARTICLE], ap_int<1> combs[NPARTICLE][NPARTICLE])
{ 

#pragma HLS ARRAY_PARTITION variable=etas    complete
#pragma HLS ARRAY_PARTITION variable=phis    complete
#pragma HLS ARRAY_PARTITION variable=combs   complete

  Combination1: for (int i = 0; i < NPARTICLE; ++i)
  {
#pragma HLS UNROLL
  Combination2: for (int j = 0; j < NPARTICLE;++j)
    {
#pragma HLS UNROLL
      if (j <= i) continue;
      etaphi_t ddeta = etas[i] - etas[j];
      etaphi_t ddphi = phis[i] - phis[j];
      if( ddeta <= DETA && ddphi <= DPHI)
      {
        combs[i][j] = 1;
      }
    }
  }
}

void GetCombination1D( const etaphi_t etas[NPARTICLE], const etaphi_t phis[NPARTICLE], ap_uint<NPARTICLE> combs[NPARTICLE])
{ 

#pragma HLS ARRAY_PARTITION variable=etas    complete
#pragma HLS ARRAY_PARTITION variable=phis    complete
#pragma HLS ARRAY_PARTITION variable=combs   complete

  Combination1: for (int i = 0; i < NPARTICLE; ++i)
  {
#pragma HLS UNROLL
  Combination2: for (int j = 0; j < NPARTICLE;++j)
    {
#pragma HLS UNROLL
      if (j <= i) continue;
      etaphi_t ddeta = etas[i] - etas[j];
      etaphi_t ddphi = phis[i] - phis[j];
      if( ddeta <= DETA && ddphi <= DPHI)
      {
        combs[i] &= 1 << j;
      }
    }
  }
}


void GetCombination( const etaphi_t etas[NPARTICLE], const etaphi_t phis[NPARTICLE], ap_int<1> combs[NPARTICLE][NPARTICLE], etaphi2_t combdRs[2*NPARTICLE])
{
  const int deta = 2; // temp for eta gap, assuming 2 eta section ~0.4
  const int dphi = 2; // temp for eta gap, assuming 2 eta section ~0.4

#pragma HLS ARRAY_PARTITION variable=etas    complete
#pragma HLS ARRAY_PARTITION variable=phis    complete
#pragma HLS ARRAY_PARTITION variable=combs   complete
#pragma HLS ARRAY_PARTITION variable=combdRs complete

  Combination1: for (int i = 0; i < NPARTICLE; ++i)
  {
#pragma HLS UNROLL
  Combination2: for (int j = 0; j < NPARTICLE;++j)
    {
#pragma HLS UNROLL
      if (j <= i) continue;
      etaphi_t ddeta = etas[i] - etas[j];
      etaphi_t ddphi = phis[i] - phis[j];
      if( ddeta <= deta && ddphi <= dphi)
      {
        combs[i][j] = 1;
      }
    }
  }
}


void DynamicGroups( const ap_uint<NPARTICLE> combs[NPARTICLE], ap_uint<NPARTICLE> grouping[NPARTICLE])
{
#pragma HLS ARRAY_PARTITION variable=combs complete
#pragma HLS ARRAY_PARTITION variable=grouping complete
//#pragma HLS ARRAY_PARTITION variable=groupcombs complete
  grouping[NPARTICLE] = {0};


  // First grouping to form relationship
  Grouping1: for (int i = 0; i < NPARTICLE; ++i)
  {
#pragma HLS UNROLL
    grouping[i] = combs[i];
  Combination2: for (int j = 0; j < NPARTICLE;++j)
    {
#pragma HLS UNROLL
      if( (grouping[i]  & combs[j]) > 0)
        grouping[i] = grouping[i] | combs[j];
    }
  }

  // Second grouping to detect remote relatives
  Grouping2: for (int i = 0; i < NPARTICLE; ++i)
  {
#pragma HLS UNROLL
  Combination2: for (int j = 0; j < NPARTICLE;++j)
    {
#pragma HLS UNROLL
      if( (grouping[i]  & grouping[j]) > 0)
        grouping[i] = grouping[i] | grouping[j];
    }
  }
}

void CountGroups( const ap_uint<NPARTICLE> grouping[NPARTICLE])
{
#pragma HLS ARRAY_PARTITION variable=grouping complete

  ap_uint<12> partlabel[NPARTICLE];

  // Each particle labels the assocaite group
  Grouping1: for (int i = 0; i < NPARTICLE; ++i)
  {
#pragma HLS UNROLL
  Combination2: for (int j = 0; j < NPARTICLE;++j)
    {
#pragma HLS UNROLL
      if( (grouping[i] & 1<<j) > 0)
        partlabel[j] = i;
    }
  }

  ap_uint<1> grplabel[NPARTICLE] = {0};
  // Getting the uniqe group
  Grouping1: for (int i = 0; i < NPARTICLE; ++i)
  {
#pragma HLS UNROLL
    grplabel[partlabel[j]] = 1;
  }

  // 



  


}



//void _lut_shift15_invert_init(ap_uint<16> _table[512]) { // returns 2^15 / x
	//_table[0] = 32768; // this is 2^15
	//for (int i = 1; i <= 511; ++i) {
		//_table[i] = (32768 / i);
	//}
//}
//int _lut_shift15_divide(ap_uint<17> num, ap_uint<9> den) { // returns (num * 2^15) / den
	//ap_uint<16> _table[512];
	//_lut_shift15_invert_init(_table);
	//return (num * _table[den]);
//}

//binindex_t MetricAK( ap_int<1> groupcomb[2*NPARTICLE], ap_int<NPARTICLE_BITS> combs[2][2*NPARTICLE],
  //etaphi2_t combdRs[2*NPARTICLE], pt_t pts[NPARTICLE])
//{
  //binindex_t AK2[2*NPARTICLE];
////#pragma HLS ARRAY_PARTITION variable=combs complete
////#pragma HLS ARRAY_PARTITION variable=groupcombs complete
////#pragma HLS ARRAY_PARTITION variable=pts complete
////#pragma HLS ARRAY_PARTITION variable=combdRs complete
////#pragma HLS ARRAY_PARTITION variable=AK2 complete

//calAK:for (int i = 0; i < 2*NPARTICLE; ++i)
  //{
//#pragma HLS unroll
    //if(groupcomb[i] == 1)
    //{
      //pt_t pt1 = pts[combs[0][i]];
      //pt_t pt2 = pts[combs[1][i]];
      //pt_t maxpt = pt1 > pt2 ? pt1 : pt2;
      //int mypt2 = (maxpt*maxpt) >> 5;
      //pt_t pt2_shift = (mypt2 < 131071 ? mypt2 : 131071);
      //ap_uint<10> dr2short = combdRs[i] >> 5;  //why?
      //int term = _lut_shift15_divide(dr2short, pt2_shift);
      //AK2[i] = binindex_t(term);
    //}
  //}
  //binindex_t max_index = 0;
  ////ParallelFindMax(AK2, max_index);
  //return max_index;
//} 
