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

void test_JetClu(PFNeutralObj input[NPARTICLE], PFNeutralObj output[INPUT_DATA_SIZE])
{

  // flatten out the array
  static pt_t pts[NPARTICLE] = {0};
  static etaphi_t etas[NPARTICLE] = {0};
  static etaphi_t phis[NPARTICLE] = {0};
#pragma HLS ARRAY_PARTITION variable=input complete
#pragma HLS ARRAY_PARTITION variable=pts   complete
#pragma HLS ARRAY_PARTITION variable=etas  complete
#pragma HLS ARRAY_PARTITION variable=phis  complete

  // 2D array for storing index of particle pairs, within dEta, dPhi
  static ap_uint<NPARTICLE> combsdR[NPARTICLE] = {0};
  static ap_uint<NPARTICLE> combsPt[NPARTICLE] = {0};
  static ap_uint<INPUT_DATA_SIZE> partlabel[NPARTICLE] = {0};

#pragma HLS ARRAY_PARTITION variable=combsdR      complete
#pragma HLS ARRAY_PARTITION variable=combsPt       complete
#pragma HLS ARRAY_PARTITION variable=grouping      complete
#pragma HLS ARRAY_PARTITION variable=partlabel      complete

//**************************************************************************//
//                       Unroll inputs to flat arrays                       //
//**************************************************************************//
  unroll: for (int iph = 0; iph < NPARTICLE; ++iph) {
#pragma HLS unroll
    pts[iph] = input[iph].hwPt;
    etas[iph] = input[iph].hwEta;
    phis[iph] = input[iph].hwPhi;
  }

//**************************************************************************//
//                        Get particles correlations                        //
//**************************************************************************//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Getting the neiboring combination ~~~~~
  GetCombination2D(etas, phis, combsdR);
  SortPartbyPt(pts, combsdR, partlabel);

//**************************************************************************//
//                             Merging into jets                            //
//**************************************************************************//
  FormingJets_simple( pts, etas, phis, partlabel, output);
  //FormingJets( const pt_t pts[NPARTICLE], const etaphi_t etas[NPARTICLE], 
  //const etaphi_t phis[NPARTICLE], PFNeutralObj output[INPUT_DATA_SIZE]);
}

// ===  FUNCTION  ============================================================
//         Name:  FormingJets_simple
//  Description:
//  https://root.cern.ch/doc/master/TLorentzVector_8h_source.html#l00342 
// ===========================================================================
void FormingJets_simple( const pt_t pts[NPARTICLE], const etaphi_t etas[NPARTICLE], 
    const etaphi_t phis[NPARTICLE], ap_uint<INPUT_DATA_SIZE> partlabel[NPARTICLE], 
    PFNeutralObj output[INPUT_DATA_SIZE])
{
#pragma HLS ARRAY_PARTITION variable=etas    complete
#pragma HLS ARRAY_PARTITION variable=phis    complete
#pragma HLS ARRAY_PARTITION variable=pts   complete

  pt_t jetpt[INPUT_DATA_SIZE];
  etaphi_t jeteta[INPUT_DATA_SIZE], jetphi[INPUT_DATA_SIZE];
#pragma HLS ARRAY_PARTITION variable=jetpt    complete
#pragma HLS ARRAY_PARTITION variable=jeteta    complete
#pragma HLS ARRAY_PARTITION variable=jetphi   complete

  ap_uint<20> rightmostbit[NPARTICLE];
  Mergingjet_1: for (int i = 0; i < NPARTICLE; i++)
  {
#pragma HLS UNROLL
    rightmostbit[i] = partlabel[i] & ~(partlabel[i]-1);
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Merging ~~~~~

  Merging_2: for (int i = 0; i < NPARTICLE; i++)
  {
#pragma HLS UNROLL
    int idx = rightmostbit[i];
    jetpt[idx] += pts[i];
    jeteta[idx] = etas[i];
    jetphi[idx] = phis[i];
  }

  Formjet_3: for (int i = 0; i < INPUT_DATA_SIZE; i++)
  {
#pragma HLS UNROLL
    output[i].hwPt = jetpt[i];
    output[i].hwEta = jeteta[i];
    output[i].hwPhi = jetphi[i];
  }

}       // -----  end of function FormingJets_simple -----

/*
 * 
// ===  FUNCTION  ============================================================
//         Name:  FormingJets
//  Description:
//  https://root.cern.ch/doc/master/TLorentzVector_8h_source.html#l00342 
// ===========================================================================
void FormingJets( const pt_t pts[NPARTICLE], const etaphi_t etas[NPARTICLE], const etaphi_t phis[NPARTICLE], PFNeutralObj output[INPUT_DATA_SIZE])
{
  pt_t px[NPARTICLE], py[NPARTICLE], pz[NPARTICLE];

#pragma HLS ARRAY_PARTITION variable=etas    complete
#pragma HLS ARRAY_PARTITION variable=phis    complete
#pragma HLS ARRAY_PARTITION variable=pts   complete

#pragma HLS ARRAY_PARTITION variable=px    complete
#pragma HLS ARRAY_PARTITION variable=py    complete
#pragma HLS ARRAY_PARTITION variable=pz   complete
  Transform: for (int i = 0; i < NPARTICLE; ++i)
  {
#pragma HLS UNROLL
    px[i] = pts[i] * cos(phis[i]/10.);
    py[i] = pts[i] * sin(phis[i]/10.);
    pz[i] = pts[i] * sinh(eta[i]/10.);
  }
  
  pt_t jetpx[INPUT_DATA_SIZE], jetpy[INPUT_DATA_SIZE], jetpz[INPUT_DATA_SIZE];
#pragma HLS ARRAY_PARTITION variable=jetpx    complete
#pragma HLS ARRAY_PARTITION variable=jetpy    complete
#pragma HLS ARRAY_PARTITION variable=jetpz   complete
  Merging_1: for (int i = 0; i < NPARTICLE/2; i++)
  {
#pragma HLS UNROLL
    jetpx [i] = px[i];
    jetpy [i] = py[i];
    jetpz [i] = pz[i];
  }

  Merging_2: for (int i = 0; i < NPARTICLE/2; i++)
  {
#pragma HLS UNROLL
    jetpx [i] += px[NPARTICLE-i];
    jetpy [i] += py[NPARTICLE-i];
    jetpz [i] += pz[NPARTICLE-i];
  }

  pt_t jetpt[INPUT_DATA_SIZE], jeteta[INPUT_DATA_SIZE], jetphi[INPUT_DATA_SIZE];
#pragma HLS ARRAY_PARTITION variable=jetpt    complete
#pragma HLS ARRAY_PARTITION variable=jeteta    complete
#pragma HLS ARRAY_PARTITION variable=jetphi   complete
  Formjet_1: for (int i = 0; i < INPUT_DATA_SIZE; i++)
  {
#pragma HLS UNROLL
    jetpt [i] = sqrt (float(jetpx[i] *jetpx[i]  + jetpy[i] * jetpy[i]));
    jetphi [i] = atan2(jetpy[i], jetpx[i]);
  }

  Formjet_2: for (int i = 0; i < INPUT_DATA_SIZE; i++)
  {
#pragma HLS UNROLL
    jeteta [i] = atanh(jetpz [i] / sqrt(jetpt[i]*jetpt[i] + jetpz[i] * jetpz[i])  )
  }

  Formjet_3: for (int i = 0; i < INPUT_DATA_SIZE; i++)
  {
#pragma HLS UNROLL
    output[i].hwPt = jetpt[i];
    output[i].hwEta = jeteta[i];
    output[i].hwPhi = jetphi[i];
  }

}       // -----  end of function FormingJets  -----

*/


void GetCombination2D( const etaphi_t etas[NPARTICLE], const etaphi_t phis[NPARTICLE], ap_uint<NPARTICLE> combs1d[NPARTICLE])
{ 

#pragma HLS ARRAY_PARTITION variable=etas    complete
#pragma HLS ARRAY_PARTITION variable=phis    complete
#pragma HLS ARRAY_PARTITION variable=combs1d   complete

  Combination2D_1: for (int i = 0; i < NPARTICLE; ++i)
  {
#pragma HLS UNROLL
  Combination2D_2: for (int j = 0; j < NPARTICLE;++j)
    {
#pragma HLS UNROLL
      //if (j <= i) continue;
      etaphi_t ddeta = etas[i] - etas[j];
      etaphi_t ddphi = phis[i] - phis[j];
      if( ddeta <= DETA && ddphi <= DPHI)
      {
        combs1d[i].set_bit(j, 1);
      }
    }
  }
}

void GetCombinationPt( const pt_t pts[NPARTICLE], ap_uint<NPARTICLE> combsPt[NPARTICLE])
{ 
#pragma HLS ARRAY_PARTITION variable=pts    complete
#pragma HLS ARRAY_PARTITION variable=combsPt   complete
  //Combinationpt_1: for (int i = 0; i < NPARTICLE; ++i)
  //{
//#pragma HLS UNROLL
    //if(pts[i] > 2)
    //{
      //combsPt[i].set_bit(i, 1);
    //}
  //}
  Combinationpt_1: for (int i = 0; i < NPARTICLE; ++i)
  {
#pragma HLS UNROLL
  Combinationpt_2: for (int j = 0; j < NPARTICLE;++j)
    {
#pragma HLS UNROLL
      //if (j <= i) continue;
      if(pts[i] > pts[j])
      {
        combsPt[i].set_bit(j, 1);
      }
    }
  }
}

//void GetCombination1D( const etaphi_t etas[NPARTICLE], const etaphi_t phis[NPARTICLE], ap_uint<NPARTICLE> combs[NPARTICLE])
//{ 

//#pragma HLS ARRAY_PARTITION variable=etas    complete
//#pragma HLS ARRAY_PARTITION variable=phis    complete
//#pragma HLS ARRAY_PARTITION variable=combs   complete

  //Combination1D_1: for (int i = 0; i < NPARTICLE; ++i)
  //{
//#pragma HLS UNROLL
  //Combination1D_2: for (int j = 0; j < NPARTICLE;++j)
    //{
//#pragma HLS UNROLL
      //printf("Test  %i %i %i %i  \n" ,i ,j,  (int)etas[i], (int)etas[j]);
      ////if (j <= i) continue;
      //etaphi_t ddeta = etas[i] - etas[j];
      //etaphi_t ddphi = phis[i] - phis[j];
      //if( ddeta <= DETA && ddphi <= DPHI)
      //{
        //combs[i] &= 1 << j;
      //}
    //}
  //}
//}


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
  ap_uint<NPARTICLE> grouping2[NPARTICLE] = {0};
#pragma HLS ARRAY_PARTITION variable=combs complete
#pragma HLS ARRAY_PARTITION variable=grouping complete
#pragma HLS ARRAY_PARTITION variable=grouping2 complete

  // First grouping to form relationship
  Grouping1: for (int i = 0; i < NPARTICLE; ++i)
  {
#pragma HLS UNROLL
    grouping[i] = combs[i];
  Grouping2: for (int j = 0; j < NPARTICLE;++j)
    {
#pragma HLS UNROLL
      if( (grouping[i]  & combs[j]) > 0)
        grouping[i] = grouping[i] | combs[j];
    }
  }

  // Second grouping to detect remote relatives
  Grouping3: for (int i = 0; i < NPARTICLE; ++i)
  {
#pragma HLS UNROLL
    grouping[i]= grouping2[i] ;
  Grouping4: for (int j = 0; j < NPARTICLE;++j)
    {
#pragma HLS UNROLL
      if( (grouping[i]  & grouping2[j]) > 0)
        grouping[i] = grouping[i] | grouping2[j];
    }
  }
}

void CountGroups( const ap_uint<NPARTICLE> grouping[NPARTICLE])
{
#pragma HLS ARRAY_PARTITION variable=grouping complete

  ap_uint<NPARTICLE> partlabel[NPARTICLE];

  // Each particle labels the assocaite group
  for (int i = 0; i < NPARTICLE; ++i)
  {
#pragma HLS UNROLL
  for (int j = 0; j < NPARTICLE;++j)
    {
#pragma HLS UNROLL
      if( (grouping[i] & 1<<j) > 0)
        partlabel[j] = i;
    }
  }

  ap_uint<1> grplabel[NPARTICLE] = {0};
  // Getting the uniqe group
  for (int i = 0; i < NPARTICLE; ++i)
  {
#pragma HLS UNROLL
    grplabel[partlabel[i]] = 1;
  }
}

template<class array_t, class index_t>
void exchange(array_t a[], index_t i, index_t j) {
    #pragma HLS INLINE
    array_t t = a[i];
    a[i] = a[j];
    a[j] = t;
}

template<class array_t, class index_t, class mask_t>
void elementLoop(array_t a[], index_t j, mask_t mask) {
    #pragma HLS INLINE
    LOOP_OVER_ELEMENTS: for (int i=0; i<INPUT_DATA_SIZE; i++) {
#if (INPUT_DATA_SIZE>=128)
        #pragma HLS UNROLL skip_exit_check //factor=32
#endif
        int ij=i^j;
        if ((ij)>i) {
            if ((i&mask)==0 && a[i] > a[ij]) 
                exchange(a,i,ij);
            if ((i&mask)!=0 && a[i] < a[ij])
                exchange(a,i,ij);
        }
        //select(a,i,j,mask);
    }
}


template<class array_t>
void BitonicSortOptimizedInline(array_t work_array[INPUT_DATA_SIZE]) {
    #pragma HLS PIPELINE II=1
    #pragma HLS ARRAY_PARTITION variable=work_array complete
    int j = 0;

    LOOP2: for (j=1; j>0; j=j>>1) {
        elementLoop(work_array,j,2);
    }

    LOOP4: for (j=2; j>0; j=j>>1) {
        elementLoop(work_array,j,4);
    }

    LOOP8: for (j=4; j>0; j=j>>1) {
        elementLoop(work_array,j,8);
    }

    LOOP16: for (j=8; j>0; j=j>>1) {
        elementLoop(work_array,j,16);
    }

//#if (INPUT_DATA_SIZE>=32)
    //LOOP32: for (j=16; j>0; j=j>>1) {
        //elementLoop(work_array,j,32);
    //}
//#if (INPUT_DATA_SIZE>=64)
    //LOOP64: for (j=32; j>0; j=j>>1) {
        //elementLoop(work_array,j,64);
    //}
//#if (INPUT_DATA_SIZE>=128)
    //LOOP128: for (j=64; j>0; j=j>>1) {
        //elementLoop(work_array,j,128);
    //}
//#endif
//#endif
//#endif
}


// ===  FUNCTION  ============================================================
//         Name:  SortPartbyPt
//  Description:  
// ===========================================================================
bool SortPartbyPt( const pt_t pts[NPARTICLE], ap_uint<NPARTICLE> combsdR[NPARTICLE],  ap_uint<INPUT_DATA_SIZE> partlabel[NPARTICLE])
{
  ptsort pts_cut[INPUT_DATA_SIZE];
#pragma HLS ARRAY_PARTITION variable=pts    complete
#pragma HLS ARRAY_PARTITION variable=pts_cut    complete

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Get ptsort with pt > PTCUT ~~~~~
  int idx = 0;
  Combinationpt_1: for (int i = 0; i < NPARTICLE; ++i)
  {
#pragma HLS UNROLL
    if(pts[i] > PTCUT)
    {
      pts_cut[idx].pt = pts[i];
      pts_cut[idx].idx = i;
      //combsPt[i].set_bit(i, 1);
      idx++;
    }
  }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Sorting ~~~~~
  BitonicSortOptimizedInline<ptsort>(pts_cut);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Getting to particle lables ~~~~~
  ap_uint<INPUT_DATA_SIZE> combsPt[NPARTICLE];
  ap_int<INPUT_DATA_SIZE+1> combsPt2[NPARTICLE];
#pragma HLS ARRAY_PARTITION variable=combsdR   complete
#pragma HLS ARRAY_PARTITION variable=combsPt    complete
#pragma HLS ARRAY_PARTITION variable=combsPt2    complete
#pragma HLS ARRAY_PARTITION variable=partlabel    complete

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Get partile ordered label ~~~~~
  Sortedpt_1: for (int i = 0; i < INPUT_DATA_SIZE; ++i)
  {
#pragma HLS UNROLL
      combsPt[pts_cut[i].idx].set_bit(i, 1);
  }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Unroll dR association with ordered lable ~~~~~
  Sortedpt_2: for (int i = 0; i < NPARTICLE; ++i)
  {
#pragma HLS UNROLL
    Sortedpt_3: for (int j = 0; j < NPARTICLE; ++j)
    {
#pragma HLS UNROLL
        if(combsdR[i].get_bit(j)) 
          combsPt2[j] &= combsPt[i];
    }
  }

  Sortedpt_4: for (int i = 0; i < NPARTICLE; ++i)
  {
#pragma HLS UNROLL
      partlabel[i] = combsPt2[i] & -combsPt2[i];
  }

  return true;
}       // -----  end of function SortPartbyPt  -----


////void _lut_shift15_invert_init(ap_uint<16> _table[512]) { // returns 2^15 / x
	////_table[0] = 32768; // this is 2^15
	////for (int i = 1; i <= 511; ++i) {
		////_table[i] = (32768 / i);
	////}
////}
////int _lut_shift15_divide(ap_uint<17> num, ap_uint<9> den) { // returns (num * 2^15) / den
	////ap_uint<16> _table[512];
	////_lut_shift15_invert_init(_table);
	////return (num * _table[den]);
////}

////binindex_t MetricAK( ap_int<1> groupcomb[2*NPARTICLE], ap_int<NPARTICLE_BITS> combs[2][2*NPARTICLE],
  ////etaphi2_t combdRs[2*NPARTICLE], pt_t pts[NPARTICLE])
////{
  ////binindex_t AK2[2*NPARTICLE];
//////#pragma HLS ARRAY_PARTITION variable=combs complete
//////#pragma HLS ARRAY_PARTITION variable=groupcombs complete
//////#pragma HLS ARRAY_PARTITION variable=pts complete
//////#pragma HLS ARRAY_PARTITION variable=combdRs complete
//////#pragma HLS ARRAY_PARTITION variable=AK2 complete

////calAK:for (int i = 0; i < 2*NPARTICLE; ++i)
  ////{
////#pragma HLS unroll
    ////if(groupcomb[i] == 1)
    ////{
      ////pt_t pt1 = pts[combs[0][i]];
      ////pt_t pt2 = pts[combs[1][i]];
      ////pt_t maxpt = pt1 > pt2 ? pt1 : pt2;
      ////int mypt2 = (maxpt*maxpt) >> 5;
      ////pt_t pt2_shift = (mypt2 < 131071 ? mypt2 : 131071);
      ////ap_uint<10> dr2short = combdRs[i] >> 5;  //why?
      ////int term = _lut_shift15_divide(dr2short, pt2_shift);
      ////AK2[i] = binindex_t(term);
    ////}
  ////}
  ////binindex_t max_index = 0;
  //////ParallelFindMax(AK2, max_index);
  ////return max_index;
////} 
