#include <cstdio>
#include "firmware/simple_fullpfalgo.h"
#include "utils/random_inputs.h"
#include "utils/DiscretePFInputs_IO.h"
#include "utils/pattern_serializer.h"
#include "utils/test_utils.h"

#define NTEST 1000


int main() {

    //// input format: could be random or coming from simulation
    ////RandomPFInputs inputs(37); // 37 is a good random number
    //DiscretePFInputs inputs("regions_TTbar_PU140.dump");
    
    //// input TP objects
    //HadCaloObj calo[NCALO]; EmCaloObj emcalo[NEMCALO]; TkObj track[NTRACK]; z0_t hwZPV;
    //HadCaloObj calo_subem[NCALO], calo_subem_ref[NCALO]; 
    //MuObj mu[NMU];

    // output PF objects
    PFChargedObj outch[NTRACK], outch_ref[NTRACK];
    PFNeutralObj outpho[NPHOTON], outpho_ref[NPHOTON];
    PFNeutralObj outne[NSELCALO], outne_ref[NSELCALO];
    PFChargedObj outmupf[NMU], outmupf_ref[NMU];

    MP7PatternSerializer serInPatterns( "mp7_input_patterns.txt",1);
    MP7PatternSerializer serOutPatterns("mp7_output_patterns.txt",1);

    HumanReadablePatternSerializer serHR("human_readable_patterns.txt");
    HumanReadablePatternSerializer debugHR("-"); // this will print on stdout, we'll use it for errors

    // -----------------------------------------
    // run multiple tests
    for (int test = 1; test <= NTEST; ++test) {

        // initialize TP objects
        for (int i = 0; i < NTRACK; ++i) {
            track[i].hwPt = 0; track[i].hwPtErr = 0; track[i].hwEta = 0; track[i].hwPhi = 0; track[i].hwZ0 = 0; 
        }
        for (int i = 0; i < NCALO; ++i) {
            calo[i].hwPt = 0; calo[i].hwEmPt = 0; calo[i].hwEta = 0; calo[i].hwPhi = 0; calo[i].hwIsEM = 0; 
        }
        for (int i = 0; i < NEMCALO; ++i) {
            emcalo[i].hwPt = 0; emcalo[i].hwPtErr = 0;  emcalo[i].hwEta = 0; emcalo[i].hwPhi = 0;
        }
        for (int i = 0; i < NMU; ++i) {
            mu[i].hwPt = 0; mu[i].hwPtErr = 0; mu[i].hwEta = 0; mu[i].hwPhi = 0;
        }

        MP7DataWord data_in[MP7_NCHANN], data_out[MP7_NCHANN];
        // initialize
        for (int i = 0; i < MP7_NCHANN; ++i) {
            data_in[i] = 0;
            data_out[i] = 0;
        }
        mp7wrapped_pack_in(emcalo, calo, track, mu, data_in, hwZPV);
        MP7_TOP_FUNC(data_in, data_out);

        //mp7wrapped_unpack_out(data_out, outch, outpho, outne, outmupf);
        mp7wrapped_unpack_out_necomb(data_out, outch, outpho, outne, outmupf, hwZPV);
		// for (int ii = 0; ii < 72; ++ii){ std::cout << ii << ", " << data_in[ii] << std::endl; }
		


        MP7_REF_FUNC(emcalo, calo, track, mu, outch_ref, outpho_ref, outne_ref, outmupf_ref);

        // write out patterns for MP7 board hardware or simulator test
        serInPatterns(data_in); serOutPatterns(data_out);
        serInPatterns2(data_in); serOutPatterns2(data_out);
        serInPatterns3(data_in); serOutPatterns3(data_out);


    }
    return 0;
}
