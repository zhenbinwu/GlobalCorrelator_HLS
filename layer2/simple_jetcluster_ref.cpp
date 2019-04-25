// ===========================================================================
// 
//       Filename:  simple_jetcluster_ref.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  02/07/2019 10:44:22 AM
//       Compiler:  g++ -std=c++11
// 
//         Author:  Zhenbin Wu (benwu)
//          Email:  zhenbin.wu@gmail.com
//        Company:  UIC, CMS@LPC, CDF@FNAL
// 
// ===========================================================================

#include "simple_jetcluster_ref.h"

void test_JetClu_ref(PFChargedObj inch[NTRACK], PFNeutralObj inpho[NPHOTON], PFNeutralObj inne[NSELCALO], PFChargedObj inmu[NMU])
{
  //Merge input and regionalized
  //Use base type as CaloObj
  unsigned idx = 0;

  pt_t pts[NPARTICLE];
  etaphi_t etas[NPARTICLE];
  etaphi_t phis[NPARTICLE];

  // 2D array for storing index of particle pairs, within dEta, dPhi
  ap_int<NPARTICLE_BITS> combs[2][2*NPARTICLE];
  // dR^2 of the pairs
  etaphi2_t combdRs[2*NPARTICLE];
  // Forming group of particles, based on the combination
  ap_int<1> groups[12][NPARTICLE];
  // Index of combination in the group
  ap_int<1> groupcombs[12][2*NPARTICLE];

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Unrolling all the inputs, ignoring types ~~~~~
  for (int ich = 0; ich < NTRACK; ++ich) {
#pragma HLS unroll
    pts[idx+ich] = inch[ich].hwPt;
    etas[idx+ich] = inch[ich].hwEta;
    phis[idx+ich] = inch[ich].hwPhi;
  }
  idx = NTRACK;

  for (int iph = 0; iph < NPHOTON; ++iph) {
    pts[idx+iph] = inpho[iph].hwPt;
    etas[idx+iph] = inpho[iph].hwEta;
    phis[idx+iph] = inpho[iph].hwPhi;
  }
  idx += NPHOTON;

  for (int ine = 0; ine < NSELCALO; ++ine) {
    pts[idx+ine] = inne[ine].hwPt;
    etas[idx+ine] = inne[ine].hwEta;
    phis[idx+ine] = inne[ine].hwPhi;
  }
  idx += NSELCALO;
  for (int imu = 0; imu < NMU; ++imu) {
    pts[idx+imu] = inmu[imu].hwPt;
    etas[idx+imu] = inmu[imu].hwEta;
    phis[idx+imu] = inmu[imu].hwPhi;
  }


////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Getting the neiboring combination ~~~~~
  GetCombination_ref(etas, phis, combs, combdRs);
  DynamicGroups_ref(combs, groups, groupcombs);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Getting metrics ~~~~~
}

void GetCombination_ref( const etaphi_t etas[NPARTICLE], const etaphi_t phis[NPARTICLE], ap_int<NPARTICLE_BITS> combs[2][2*NPARTICLE], etaphi2_t combdRs[2*NPARTICLE])
{
  const int deta = 2; // temp for eta gap, assuming 2 eta section ~0.4
  const int dphi = 2; // temp for eta gap, assuming 2 eta section ~0.4
  int idx = 0;


  for (int i = 0; i < NPARTICLE; ++i)
  {
    for (int j = i; j < NPARTICLE; ++j)
    {
      int ddeta = abs(etas[i] - etas[j] ) ;
      int ddphi = abs(phis[i] - phis[j] );
      if( ddeta <= deta && ddphi <= dphi && idx <= 2*NPARTICLE)
      {
        combs[0][idx] = i;
        combs[1][idx] = j;
        combdRs[idx] = ddeta *ddeta + ddphi*ddphi;
        idx +=1;
      }
    }
  }
}

void DynamicGroups_ref( const ap_int<NPARTICLE_BITS> combs[2][2*NPARTICLE], ap_int<1> groups[12][NPARTICLE], ap_int<1> groupcombs[12][2*NPARTICLE])
{
  int j=0, k=0;
  int maxidx = 0;
  for (int i = 0; i < 2*NPARTICLE; ++i)
  {
    groups[j][combs[0][i]]=1;
    groups[j][combs[1][i]]=1;
    groupcombs[j][i] = 1;
    if (combs[1][i] > maxidx) 
      maxidx = combs[1][i];
    if (combs[0][i] > maxidx)
    {
      j++;
    }
  }
}
