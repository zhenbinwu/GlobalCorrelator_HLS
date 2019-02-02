#include <cstdio>
#include <iomanip>
#include "firmware/simple_fullpfalgo.h"
#include "vertexing/firmware/simple_vtx.h"
#include "puppi/firmware/simple_puppi.h"
#include "utils/random_inputs.h"
#include "utils/DiscretePFInputs_IO.h"
#include "utils/pattern_serializer.h"
#include "utils/test_utils.h"

#define NTEST 312


int main() {

    // input format: could be random or coming from simulation
    //RandomPFInputs inputs(37); // 37 is a good random number
    DiscretePFInputs inputs("regions_TTbar_PU140.dump");
    
    // input TP objects
    HadCaloObj calo[NCALO]; EmCaloObj emcalo[NEMCALO]; TkObj track[NTRACK]; z0_t hwZPV;
    HadCaloObj calo_subem[NCALO], calo_subem_ref[NCALO]; 
    MuObj mu[NMU];

    // output PF objects
    PFChargedObj outch[NTRACK], outch_ref[NTRACK];
    PFNeutralObj outpho[NPHOTON], outpho_ref[NPHOTON];
    PFNeutralObj outne[NSELCALO], outne_ref[NSELCALO];
    PFChargedObj outmupf[NMU], outmupf_ref[NMU];
#if defined(TESTMP7)
    /*
    MP7PatternSerializer serInPatterns( "mp7_input_patterns.txt", HLS_pipeline_II,HLS_pipeline_II-1); // mux each event into HLS_pipeline_II frames
    MP7PatternSerializer serOutPatterns("mp7_output_patterns.txt",HLS_pipeline_II,HLS_pipeline_II-1); // assume only one PF core running per chip,
    MP7PatternSerializer serInPatterns2( "mp7_input_patterns_magic.txt", HLS_pipeline_II,-HLS_pipeline_II+1); // mux each event into HLS_pipeline_II frames
    MP7PatternSerializer serOutPatterns2("mp7_output_patterns_magic.txt",HLS_pipeline_II,-HLS_pipeline_II+1); // assume only one PF core running per chip,
    MP7PatternSerializer serInPatterns3( "mp7_input_patterns_nomux.txt");  // 
    MP7PatternSerializer serOutPatterns3("mp7_output_patterns_nomux.txt"); // ,
    */

    MP7PatternSerializer serInPatterns( "mp7_input_patterns.txt");
    MP7PatternSerializer serOutPatterns("mp7_output_patterns.txt");
    MP7PatternSerializer serInPatterns2( "mp7_input_patterns_magic.txt");
    MP7PatternSerializer serOutPatterns2("mp7_output_patterns_magic.txt");
    MP7PatternSerializer serInPatterns3( "mp7_input_patterns_nomux.txt");  // 
    MP7PatternSerializer serOutPatterns3("mp7_output_patterns_nomux.txt"); // ,
#endif
#if defined(TESTCTP7)
    CTP7PatternSerializer serInPatterns4( "ctp7_input_patterns_nomux.txt",CTP7_NCHANN_IN, true);  // 
    CTP7PatternSerializer serOutPatterns4("ctp7_output_patterns_nomux.txt",CTP7_NCHANN_OUT, false); // fill the rest of the lines with empty events for now
#endif
    HumanReadablePatternSerializer serHR("human_readable_patterns.txt");
    //HumanReadablePatternSerializer serHR("-");
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

        // get the inputs from the input object
        if (!inputs.nextRegion(calo, emcalo, track, mu, hwZPV)) break;

        VtxObj curvtx;    
        simple_vtx_ref(track,&curvtx);
        printf("Vertex Z   %i\n",(int)(curvtx.hwZ0));

        MP7DataWord data_in[MP7_NCHANN], data_out[MP7_NCHANN];
        // initialize
        for (int i = 0; i < MP7_NCHANN; ++i) {
            data_in[i] = 0;
            data_out[i] = 0;
        }
        mp7wrapped_pack_in(emcalo, calo, track, mu, data_in, curvtx.hwZ0);
        MP7_TOP_FUNC(data_in, data_out);
        //tk2em_dr_t   drvals_tk2em[NTRACK][NPHOTON];
        //tk2calo_dr_t drvals_tk2calo[NTRACK][NSELCALO];
        //pfalgo3_full(emcalo, calo, track, mu, outch, outpho, outne, outmupf, drvals_tk2em, drvals_tk2calo);
        //mp7wrapped_unpack_out(data_out, outch, outpho, outne, outmupf);
        mp7wrapped_unpack_out_necomb(data_out, outch, outpho, outne, outmupf,curvtx.hwZ0);
		// for (int ii = 0; ii < 72; ++ii){ std::cout << ii << ", " << data_in[ii] << std::endl; }

        for (unsigned int di = 0; di < MP7_NCHANN; di++) {
            std::cout<< std::setfill('0') << std::setw(8) << std::hex << (unsigned int)(data_out[di]) << "  ";
            if (di%48==47) std::cout << std::endl;
        }

        std::cout<<"\n-----------"<<std::endl;

        unsigned int index = 0;
        for (unsigned int fi = 0; fi < 3; fi++) {
            std::cout << "INPUT 0x" << std::setfill('0') << std::setw(4) << std::hex << 3*(test-1)+fi;
            for (unsigned int ch = 0; ch < 48; ++ch){
                std::cout << "    0x";
                if (fi==0) {
                    if (index >= MP7_NCHANN) std::cout << "00000000";
                    else std::cout << std::setfill('0') << std::setw(8) << std::hex << (unsigned int)(data_in[index]);
                    index++;
                    std::cout << std::setfill('0') << std::setw(6) << std::hex << (((unsigned int)(curvtx.hwZ0.range(9,0))) << 14) << "dd";
                }
                else {
                    if (index+48 >= MP7_NCHANN) std::cout << "00000000";
                    else std::cout << std::setfill('0') << std::setw(8) << std::hex << (unsigned int)(data_in[index+48]);
                    if (index >= MP7_NCHANN) std::cout << "00000000";
                    else std::cout << std::setfill('0') << std::setw(8) << std::hex << (unsigned int)(data_in[index]);
                    index++;
                }
            }
            if (fi>0) index = index + 48;
            std::cout << "    " << std::endl;
        }
        index = 0;
        for (unsigned int fi = 0; fi < 3; fi++) {
            std::cout << "OUTPUT 0x" << std::setfill('0') << std::setw(4) << std::hex << 3*(test-1)+fi;
            for (unsigned int ch = 0; ch < 48; ++ch){
                std::cout << "    0x";
                if (fi==0) {
                    if (index >= MP7_NCHANN) std::cout << "00000000";
                    else std::cout << std::setfill('0') << std::setw(8) << std::hex << (unsigned int)(data_out[index]);
                    index++;
                    std::cout << std::setfill('0') << std::setw(6) << std::hex << (((unsigned int)(curvtx.hwZ0.range(9,0))) << 14) << "dd";
                }
                else {
                    if (index+48 >= MP7_NCHANN) std::cout << "00000000";
                    else std::cout << std::setfill('0') << std::setw(8) << std::hex << (unsigned int)(data_out[index+48]);
                    if (index >= MP7_NCHANN) std::cout << "00000000";
                    else std::cout << std::setfill('0') << std::setw(8) << std::hex << (unsigned int)(data_out[index]);
                    index++;
                }
            }
            if (fi>0) index = index + 48;
            std::cout << "    " << std::endl;
        }


        MP7_REF_FUNC(emcalo, calo, track, mu, outch_ref, outpho_ref, outne_ref, outmupf_ref);

        // write out patterns for MP7 board hardware or simulator test
        serInPatterns(data_in); serOutPatterns(data_out);
        serInPatterns2(data_in); serOutPatterns2(data_out);
        serInPatterns3(data_in); serOutPatterns3(data_out);

        // write out human-readable patterns
        serHR(emcalo, calo, track, mu, outch, outpho, outne, outmupf);

        ap_uint<8> weights[NNEUTRALS];

        PFNeutralObj outallne_ref[NNEUTRALS];
        // sort/merge neutrals (at the end of the PF algo?) - do it in test bench for now
        for (int ipfne = 0; ipfne < NPHOTON; ++ipfne){
            outpho_ref[ipfne].hwPtPuppi = 0;
            outallne_ref[ipfne] = outpho_ref[ipfne];
        }
        for (int ipfne = NPHOTON; ipfne < NNEUTRALS; ++ipfne){
            outne_ref[ipfne-NPHOTON].hwPtPuppi = 0;
            outallne_ref[ipfne] = outne_ref[ipfne-NPHOTON];
        }

        simple_puppi_ref( outch_ref, outallne_ref, curvtx.hwZ0);
        //simple_puppi_hw(  outch_ref, outallne,     curvtx.hwZ0);
        // weight_t curweight;
        // compute_puppi_weight_hw( 100, curweight );
        // std::cout << "curweight = " << curweight << std::endl;

        // -----------------------------------------
        // validation against the reference algorithm
        int errors = 0; int ntot = 0, npho = 0, nch = 0, nneu = 0, nmu = 0;

        // check charged hadrons
        for (int i = 0; i < NTRACK; ++i) {
            if (!pf_equals(outch_ref[i], outch[i], "PF Charged", i)) errors++;
            if (outch_ref[i].hwPt > 0) { ntot++; nch++; }
        }
        // check photon 
        for (int i = 0; i < NPHOTON; ++i) {
            if (!pf_equals(outpho_ref[i], outpho[i], "Photon", i)) errors++;
            if (outpho_ref[i].hwPt > 0) { ntot++; npho++; }
        }
        for (int i = 0; i < NSELCALO; ++i) {
            if (!pf_equals(outne_ref[i], outne[i], "PF Neutral", i)) errors++;
            if (outne_ref[i].hwPt > 0) { ntot++; nneu++; }
        }
        for (int i = 0; i < NMU; ++i) {
            if (!pf_equals(outmupf_ref[i], outmupf[i], "PF Muon", i)) errors++;
            if (outmupf_ref[i].hwPt > 0) { ntot++; nmu++; }
        }        

        if (errors != 0) {
            printf("Error in pf test %d (%d)\n", test, errors);
            printf("Inputs: \n"); debugHR.dump_inputs(emcalo, calo, track, mu);
            printf("Reference output: \n"); debugHR.dump_outputs(outch_ref, outpho_ref, outne_ref, outmupf_ref);
            printf("Current output: \n"); debugHR.dump_outputs(outch, outpho, outne, outmupf);
            return 1;
        } else {
            printf("Passed pf test %d (%d, %d, %d, %d)\n", test, ntot, nch, npho, nneu);
        }

        int puperrors = 0;
        for (int i = 0; i < NPHOTON; ++i){
            printf("hwpt = %i, hwptpuppi = %i, refptpuppi = %i, hw-ref_ptpuppi = %i \n", (int) outpho[i].hwPt, (int) outpho[i].hwPtPuppi, (int) outallne_ref[i].hwPtPuppi, int(outpho[i].hwPtPuppi-outallne_ref[i].hwPtPuppi));
            if (outpho[i].hwPtPuppi-outallne_ref[i].hwPtPuppi != 0 && outpho[i].hwPt>0) puperrors++;
        }
        for (int i = 0; i < NSELCALO; ++i){
            printf("hwpt = %i, hwptpuppi = %i, refptpuppi = %i, hw-ref_ptpuppi = %i \n", (int) outne[i].hwPt, (int) outne[i].hwPtPuppi, (int) outallne_ref[i+NPHOTON].hwPtPuppi, int(outne[i].hwPtPuppi-outallne_ref[i+NPHOTON].hwPtPuppi));
            if (outne[i].hwPtPuppi-outallne_ref[i+NPHOTON].hwPtPuppi != 0 && outne[i].hwPt>0) puperrors++;
        }
        std::cout << "end of test ---- " << test << std::endl;

        if (puperrors>0) {
            printf("Found %i errors in puppi test!\n", puperrors);
            //return errors;
        }
    }
    return 0;
}
