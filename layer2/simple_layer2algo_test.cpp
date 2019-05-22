#include <cstdio>
//#include "firmware/simple_fullpfalgo.h"
#include "simple_layer2algo_hw.h"
#include "simple_layer2algo_ref.h"
#include "simple_jetcluster_hw.h"
#include "simple_jetcluster_ref.h"
#include "../../utils/DiscretePFInputs_IO.h"
#include "../../utils/pattern_serializer.h"
#include <cstdlib>
#include <cmath>
#include "pattern_reader.h"


void mp7wrapped_unpack_out_necomb( MP7DataWord data[MP7_NCHANN], PFChargedObj pfch[NTRACK], PFNeutralObj pfpho[NPHOTON], PFNeutralObj pfne[NSELCALO], PFChargedObj pfmu[NMU], z0_t &z0);

int main() {
    // create output PF objects
    PFNeutralObj inputs[NPARTICLE];
    PFNeutralObj outputs[NPARTICLE/2];
    srand (time(NULL));
    const int maxeta = pow(2, 10);
    const int maxpt = pow(2, 15);

    for (unsigned int i = 0; i < NPARTICLE-1; ++i) {
        inputs[i].hwPt  = (rand() % maxpt);
        inputs[i].hwPtPuppi  = (rand() % maxpt);
        inputs[i].hwEta = (rand() % maxeta) - (maxeta/2);
        inputs[i].hwPhi = (rand() % maxeta) - (maxeta/2);
        inputs[i].hwId  =  0;
    }

    for (unsigned int i = 0; i < NPARTICLE/2; ++i) {
        outputs[i].hwPt  = 0;
        outputs[i].hwEta = 0;
        outputs[i].hwPhi = 0;
        outputs[i].hwId  = 0;
    }

    for (unsigned int i = 0; i < NPARTICLE; ++i) {
        std::cout << " Input " <<  i <<" : " << inputs[i].hwEta  << " - " << inputs[i].hwPhi<< std::endl;
    }

    ap_uint<NPARTICLE> grouping_ref[NPARTICLE];
    ap_uint<NPARTICLE> grouping_hw[NPARTICLE];
    for (int i = 0; i < NPARTICLE;     ++i) 
    {
      grouping_ref[i] = 0;
      grouping_hw[i] = 0;
    }

    test_JetClu(inputs, outputs);
    test_JetClu_ref(inputs, grouping_ref);

    for (int i = 0; i < NPARTICLE;     ++i) 
    {
      printf("Test ref grouping %i %s \n" ,i ,grouping_ref[i].to_string(2));
      printf("Test hw grouping %i %s \n" ,i ,grouping_hw[i].to_string(2));
    }
}




void mp7wrapped_unpack_out_necomb( MP7DataWord data[MP7_NCHANN], PFChargedObj pfch[NTRACK], PFNeutralObj pfpho[NPHOTON], PFNeutralObj pfne[NSELCALO], PFChargedObj pfmu[NMU], z0_t &z0) {
    #pragma HLS ARRAY_PARTITION variable=data complete
    #pragma HLS ARRAY_PARTITION variable=pfch complete
    #pragma HLS ARRAY_PARTITION variable=pfpho complete
    #pragma HLS ARRAY_PARTITION variable=pfne complete
    #pragma HLS ARRAY_PARTITION variable=pfmu complete
    // unpack outputs
    assert(2*NTRACK + 2*NPHOTON + 2*NSELCALO + 2*NMU <= MP7_NCHANN);
    #define PHOOFFS 2*NTRACK
    #define NHOFFS 2*NPHOTON+PHOOFFS
    #define PFMUOFFS 2*NSELCALO+NHOFFS
    for (unsigned int i = 0; i < NTRACK; ++i) {
        pfch[i].hwPt  = data[2*i+0](15, 0);
        pfch[i].hwId  = data[2*i+0](18,16);
        pfch[i].hwEta = data[2*i+1](9, 0);
        pfch[i].hwPhi = data[2*i+1](19,10);
        pfch[i].hwZ0  = data[2*i+1](29,20);
    }
    for (unsigned int i = 0; i < NPHOTON; ++i) {
        pfpho[i].hwPt  = data[2*i+0+PHOOFFS](15, 0);
        pfpho[i].hwPtPuppi  = data[2*i+0+PHOOFFS](31,16);
        pfpho[i].hwEta = data[2*i+1+PHOOFFS](9, 0);
        pfpho[i].hwPhi = data[2*i+1+PHOOFFS](19,10);
        pfpho[i].hwId  = data[2*i+1+PHOOFFS](22,20);
    }
    for (unsigned int i = 0; i < NSELCALO; ++i) {
        pfne[i].hwPt  = data[2*i+0+NHOFFS](15, 0);
        pfne[i].hwPtPuppi  = data[2*i+0+NHOFFS](31, 16);
        pfne[i].hwEta = data[2*i+1+NHOFFS](9, 0);
        pfne[i].hwPhi = data[2*i+1+NHOFFS](19,10);
        pfne[i].hwId  = data[2*i+1+NHOFFS](22,20);
    }
    for (unsigned int i = 0; i < NMU; ++i) {
        pfmu[i].hwPt  = data[2*i+0+PFMUOFFS](15, 0);
        pfmu[i].hwId  = data[2*i+0+PFMUOFFS](18,16);
        pfmu[i].hwEta = data[2*i+1+PFMUOFFS](9, 0);
        pfmu[i].hwPhi = data[2*i+1+PFMUOFFS](19,10);
        pfmu[i].hwZ0  = data[2*i+1+PFMUOFFS](29,20);
    }
    z0 = 0.5*(data[MP7_NCHANN-1](15, 0) + data[MP7_NCHANN-2](15, 0));
}

