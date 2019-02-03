#include <cstdio>
#include "firmware/simple_fullpfalgo.h"
#include "layer2/simple_layer2algo_hw.h"
#include "layer2/simple_layer2algo_ref.h"
#include "utils/DiscretePFInputs_IO.h"
#include "utils/pattern_serializer.h"
#include "layer2/pattern_reader.h"

int main() {
    // output PF objects
    PFChargedObj outch[NTRACK];
    PFNeutralObj outpho[NPHOTON];
    PFNeutralObj outne[NSELCALO];
    PFChargedObj outmupf[NMU];
    MP7DataWord data_in[CTP7_NCHANN_OUT];

    for (int i = 0; i < NTRACK; ++i) clear(outch[i]);
    for (int i = 0; i < NPHOTON; ++i) clear(outpho[i]);
    for (int i = 0; i < NSELCALO; ++i) clear(outne[i]);
    for (int i = 0; i < NMU; ++i) clear(outmupf[i]);
    for (int i = 0; i < CTP7_NCHANN_OUT; ++i) { data_in[i] = 0; }

    CTP7PatternReader readPatterns ( "ctp7_output_patterns_nomux.txt",CTP7_NCHANN_OUT);

    //readPatterns.TestMP7Word(data_in);
    if (readPatterns.ReadEvent())
    {
      readPatterns.GetMP7Word(data_in);
    }
    //readPatterns.TestMP7Word(data_in);

    mp7wrapped_unpack_out(data_in, outch, outpho, outne, outmupf);
    //readPatterns.TestMP7Word(data_in);


    // Compareap_uint<32> HT
    //
    ap_uint<32> HT;
    HT=0;
    test_PFHT(outch, outpho, outne, outmupf, HT);
    test_PFHT_ref(outch, outpho, outne, outmupf, HT);
    printf("Test output HT %i \n" , int(HT));
}



