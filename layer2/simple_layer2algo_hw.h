#ifndef SIMPLE_LAYER2_ALGO_HW
#define SIMPLE_LAYER2_ALGO_HW

#include "ap_int.h"
#include "firmware/data.h"
#include "firmware/simple_fullpfalgo.h"

void test_PFHT(PFChargedObj inch[NTRACK], PFNeutralObj inpho[NPHOTON], PFNeutralObj inne[NSELCALO], PFChargedObj inmu[NMU], ap_uint<32> &HT);

#endif
