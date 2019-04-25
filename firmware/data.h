#ifndef SIMPLE_PFLOW_DATA_H
#define SIMPLE_PFLOW_DATA_H

#include "ap_int.h"

typedef ap_int<16> pt_t;
typedef ap_int<10>  etaphi_t;
typedef ap_int<5>  vtx_t;
typedef ap_uint<3>  particleid_t;
typedef ap_int<10> z0_t;  // 40cm / 0.1
	
typedef ap_uint<14> tk2em_dr_t;
typedef ap_uint<14> tk2calo_dr_t;

// Variables for Layer2 clustering
#define NPARTICLE 20
#define NPARTICLE_BITS 13
typedef ap_int<20>  etaphi2_t;

enum PID { PID_Charged=0, PID_Neutral=1, PID_Photon=2, PID_Electron=3, PID_Muon=4 };

// VERTEXING
#define NVTXBINS 15
#define NPOW 6
#define NALLTRACK 1 << NPOW
#define NSECTOR 1
#define VTXPTMAX  200

// PF
#ifdef TESTMP7  // reduced input size to fit in a board
   #define NTRACK 10
   #define NCALO 12
   #define NMU 1
   #define NEMCALO 12
   #define NPHOTON NEMCALO
   #define NSELCALO 12
#elif TESTCTP7  // reduced input size to fit in a board
   #define NTRACK 7
   #define NCALO 5
   #define NMU 2
   #define NEMCALO 5
   #define NPHOTON NEMCALO
   #define NSELCALO 4
#else
   // #define NTRACK 15
   // #define NCALO 15
   // #define NMU 4
   // #define NEMCALO 15
   // #define NPHOTON NEMCALO
   // #define NSELCALO 10
   #define NTRACK 7
   #define NCALO 5
   #define NMU 2
   #define NEMCALO 5
   #define NPHOTON NEMCALO
   #define NSELCALO 4
#endif

// PUPPI & CHS
#define NPVTRACK 15
#define NNEUTRALS NPHOTON+NSELCALO

struct CaloObj {
	pt_t hwPt;
	etaphi_t hwEta, hwPhi; // relative to the region center, at calo
};
struct HadCaloObj : public CaloObj {
	pt_t hwEmPt;
   	bool hwIsEM;
};
inline void clear(HadCaloObj & c) {
    c.hwPt = 0; c.hwEta = 0; c.hwPhi = 0; c.hwEmPt = 0; c.hwIsEM = 0; 
}

struct EmCaloObj {
	pt_t hwPt, hwPtErr;
	etaphi_t hwEta, hwPhi; // relative to the region center, at calo
};
inline void clear(EmCaloObj & c) {
    c.hwPt = 0; c.hwPtErr = 0; c.hwEta = 0; c.hwPhi = 0; 
}

struct TkObj {
	pt_t hwPt, hwPtErr;
	etaphi_t hwEta, hwPhi; // relative to the region center, at calo
	z0_t hwZ0;
	bool hwTightQuality;
};
inline void clear(TkObj & c) {
    c.hwPt = 0; c.hwPtErr = 0; c.hwEta = 0; c.hwPhi = 0; c.hwZ0 = 0; c.hwTightQuality = 0;
}

struct MuObj {
	pt_t hwPt, hwPtErr;
	etaphi_t hwEta, hwPhi; // relative to the region center, at vtx(?)
};
inline void clear(MuObj & c) {
    c.hwPt = 0; c.hwPtErr = 0; c.hwEta = 0; c.hwPhi = 0; 
}


struct PFChargedObj {
	pt_t hwPt;
	etaphi_t hwEta, hwPhi; // relative to the region center, at calo
	particleid_t hwId;
	z0_t hwZ0;
};
inline void clear(PFChargedObj & c) {
    c.hwPt = 0; c.hwId = 0; c.hwEta = 0; c.hwPhi = 0; 
    c.hwZ0 =0;
}
struct PFNeutralObj {
	pt_t hwPt;
	etaphi_t hwEta, hwPhi; // relative to the region center, at calo
	particleid_t hwId;
  pt_t hwPtPuppi;
};
inline void clear(PFNeutralObj & c) {
    c.hwPt = 0; c.hwEta = 0; c.hwPhi = 0; 
    c.hwId =0;
}
struct VtxObj {
	pt_t  hwSumPt;
	z0_t  hwZ0;
	vtx_t mult;
	particleid_t hwId;
};

#define MP7_NCHANN 72
#define CTP7_NCHANN_IN 67
#define CTP7_NCHANN_OUT 48
typedef ap_uint<32> MP7DataWord;

#endif
