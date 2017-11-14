#ifndef SIMPLE_LAYER2_ALGO_REF
#define SIMPLE_LAYER2_ALGO_REF

#include "ap_int.h"
#include "firmware/data.h"
#include "firmware/simple_fullpfalgo.h"

void test_PFHT_ref(PFChargedObj inch[NTRACK], PFNeutralObj inpho[NPHOTON], PFNeutralObj inne[NSELCALO], PFChargedObj inmu[NMU], ap_uint<32> &HT);

#endif
