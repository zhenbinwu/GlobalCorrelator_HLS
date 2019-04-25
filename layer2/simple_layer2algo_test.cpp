#include <cstdio>
//#include "firmware/simple_fullpfalgo.h"
#include "simple_layer2algo_hw.h"
#include "simple_layer2algo_ref.h"
#include "simple_jetcluster_hw.h"
#include "simple_jetcluster_ref.h"
#include "../../utils/DiscretePFInputs_IO.h"
#include "../../utils/pattern_serializer.h"
#include "pattern_reader.h"

void mp7wrapped_unpack_out_necomb( MP7DataWord data[MP7_NCHANN], PFChargedObj pfch[NTRACK], PFNeutralObj pfpho[NPHOTON], PFNeutralObj pfne[NSELCALO], PFChargedObj pfmu[NMU], z0_t &z0);

int main() {
    // create output PF objects
    PFChargedObj outch[NTRACK];
    PFNeutralObj outpho[NPHOTON];
    PFNeutralObj outne[NSELCALO];
    PFChargedObj outmupf[NMU];
    z0_t hwZPV;
    MP7DataWord data_in[MP7_NCHANN];

    // Initialize objects
    for (int i = 0; i < NTRACK;     ++i) clear(outch[i]);
    for (int i = 0; i < NPHOTON;    ++i) clear(outpho[i]);
    for (int i = 0; i < NSELCALO;   ++i) clear(outne[i]);
    for (int i = 0; i < NMU;        ++i) clear(outmupf[i]);
    for (int i = 0; i < MP7_NCHANN; ++i) data_in[i] = 0;

    CTP7PatternReader readPatterns ("mp7_output_patterns.txt",MP7_NCHANN);
    readPatterns.GetMP7Word(data_in);
    mp7wrapped_unpack_out_necomb(data_in, outch, outpho, outne, outmupf, hwZPV);

    // Compareap_uint<32> HT
    ap_uint<32> HT;
    HT=0;
    //test_PFHT(outch, outpho, outne, outmupf, HT);
    //test_PFHT_ref(outch, outpho, outne, outmupf, HT);
    test_JetClu(outch, outpho, outne, outmupf);
    test_JetClu_ref(outch, outpho, outne, outmupf);
    printf("Test output HT %i \n" , int(HT));
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
