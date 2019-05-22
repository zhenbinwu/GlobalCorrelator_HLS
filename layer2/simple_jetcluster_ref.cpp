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

void test_JetClu_ref(PFNeutralObj input[NPARTICLE], ap_uint<NPARTICLE>  grouping[NPARTICLE])
{
  //Merge input and regionalized
  //Use base type as CaloObj
  unsigned idx = 0;

  pt_t pts[NPARTICLE];
  etaphi_t etas[NPARTICLE];
  etaphi_t phis[NPARTICLE];

  // 2D array for storing index of particle pairs, within dEta, dPhi
  ap_uint<NPARTICLE> combs[NPARTICLE] = {0};
  ap_uint<NPARTICLE> combsPt[NPARTICLE] = {0};
  for (int i = 0; i < NPARTICLE; ++i)
  {
    grouping[i] = 0;
  }
  //ap_uint<NPARTICLE> grouping[NPARTICLE] = {0};
  //// dR^2 of the pairs
  //etaphi2_t combdRs[2*NPARTICLE];
  //// Forming group of particles, based on the combination
  //ap_int<1> groupcombs[12][2*NPARTICLE];

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Unrolling all the inputs, ignoring types ~~~~~
  for (int ich = 0; ich < NPARTICLE; ++ich) {
    pts[ich] = input[ich].hwPt;
    etas[ich] = input[ich].hwEta;
    phis[ich] = input[ich].hwPhi;
  }

////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Getting the neiboring combination ~~~~~
  GetCombination_ref(pts, etas, phis, combs, combsPt);
  DynamicGroups_ref(combs, grouping);
  CountGroups_ref(grouping);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Getting metrics ~~~~~
}

void GetCombination_ref(const pt_t pts[NPARTICLE], const etaphi_t etas[NPARTICLE], const etaphi_t phis[NPARTICLE], ap_uint<NPARTICLE> combsdR[NPARTICLE], ap_uint<NPARTICLE> combsPt[NPARTICLE])
{
  const int deta = 2; // temp for eta gap, assuming 2 eta section ~0.4
  const int dphi = 2; // temp for eta gap, assuming 2 eta section ~0.4

  for (int i = 0; i < NPARTICLE; ++i)
  {
    for (int j = 0; j < NPARTICLE; ++j)
    {
      int ddeta = abs(etas[i] - etas[j] );
      int ddphi = abs(phis[i] - phis[j] );
      if( ddeta <= deta && ddphi <= dphi)
      {
        combsdR[i].set_bit(j, 1);
      }
      if (pts[i] > pts[j])
        combsPt[i].set_bit(j, 1);
    }
  }

  for (int i = 0; i < NPARTICLE; ++i)
  {
    std::cout << combsdR[i].to_string(2) << std::endl;
  }
}

void DynamicGroups_ref( const ap_uint<NPARTICLE> combs[NPARTICLE], ap_uint<NPARTICLE> grouping[NPARTICLE])
{
  // First grouping to form relationship
  for (int i = 0; i < NPARTICLE; ++i)
  {
    std::cout << " org " << i <<" " << grouping[i].to_string(2) << std::endl;
    grouping[i] = combs[i];
    for(int j = 0; j < NPARTICLE;++j)
    {
      std::cout << " i " << i <<" j " << j <<   " group " << grouping[i].to_string(2)  <<" Comb " << combs[j].to_string(2) <<" and " << (grouping[i]  & combs[j]).to_string(2) <<" "
        <<(grouping[i] | combs[j]).to_string(2) << std::endl;
      if( (grouping[i]  & combs[j]) > 0)
        grouping[i] = grouping[i] | combs[j];
    }
  }

  // Second grouping to detect remote relatives
  for (int i = 0; i < NPARTICLE; ++i)
  {
    for (int j = 0; j < NPARTICLE;++j)
    {
      if( (grouping[i]  & grouping[j]) > 0)
        grouping[i] = grouping[i] | grouping[j];
    }
  }

  for(int j = 0; j < NPARTICLE;++j)
  {
    std::cout <<" j " << j <<   " group " << grouping[j].to_string(2) <<std::endl;
  }
}

void CountGroups_ref( const ap_uint<NPARTICLE> grouping[NPARTICLE])
{
  ap_uint<NPARTICLE> partlabel[NPARTICLE];

  // Each particle labels the assocaite group
  for (int i = 0; i < NPARTICLE; ++i)
  {
    for (int j = 0; j < NPARTICLE;++j)
    {
      if( (grouping[i] & 1<<j) > 0)
        partlabel[j] = i;
    }
  }

  ap_uint<1> grplabel[NPARTICLE];
  for (int i = 0; i < NPARTICLE; ++i)
  {
    grplabel[i] = 0;
  }

  // Getting the uniqe group
  for (int i = 0; i < NPARTICLE; ++i)
  {
    grplabel[partlabel[i]] = 1;
  }

  for (int i = 0; i < NPARTICLE; ++i)
  {
    std::cout << i <<" glabel: " << grplabel[i].to_string(2) << " parlabel " <<  partlabel[i].to_string(2) << std::endl;
  }

}
