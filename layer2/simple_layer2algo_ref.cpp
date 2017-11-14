#include "layer2/simple_layer2algo_ref.h"

void test_PFHT_ref(PFChargedObj inch[NTRACK], PFNeutralObj inpho[NPHOTON], PFNeutralObj inne[NSELCALO], PFChargedObj inmu[NMU], ap_uint<32> &HT)
{
  HT = 0;
  for (int ich = 0; ich < NTRACK; ++ich) {
    HT  = HT +inch[ich].hwPt;
  }
  for (int iph = 0; iph < NPHOTON; ++iph) {
    HT  = HT + inpho[iph].hwPt;
  }
  for (int ine = 0; ine < NSELCALO; ++ine) {
    HT  = HT + inne[ine].hwPt;
  }
  for (int imu = 0; imu < NMU; ++imu) {
    HT  = HT+ inmu[imu].hwPt;
  }
}
