#ifndef MCTREE_H
#define MCTREE_H

struct Config {
    int year;
    float lumi;
    bool isData;
    TString json;
    FactorizedJetCorrector* jetCorr;
};

class BabyTree {

    /* --> Meta Branches <-- */
    TBranch* b_run;
    TBranch* b_lumi;
    TBranch* b_event;
    TBranch* b_scale1fb;
    TBranch* b_isGold;
    TBranch* b_isHEM;
    TBranch* b_met_pt;
    TBranch* b_met_phi;
    TBranch* b_rawMet_pt;
    TBranch* b_rawMet_phi;
    TBranch* b_HLT_singleMu; 
    TBranch* b_HLT_singleEl; 
    TBranch* b_passFilters;
    /* --> Special Branches <-- */
    TBranch* b_genRecoGamma_dR;
    TBranch* b_genRecoPhi_dR;
    TBranch* b_genRecoRho_dR;
    TBranch* b_recoMagAng_cosThetaStar;
    TBranch* b_recoMagAng_cosTheta1;
    TBranch* b_recoMagAng_cosTheta2;
    TBranch* b_recoMagAng_Phi;
    TBranch* b_recoMagAng_Phi1;
    TBranch* b_recoMagAng_m1;
    TBranch* b_recoMagAng_m2;
    TBranch* b_genMagAng_cosThetaStar;
    TBranch* b_genMagAng_cosTheta1;
    TBranch* b_genMagAng_cosTheta2;
    TBranch* b_genMagAng_Phi;
    TBranch* b_genMagAng_Phi1;
    TBranch* b_genMagAng_m1;
    TBranch* b_genMagAng_m2;

    /* --> Gen Branches <-- */
    // W Boson
    TBranch* b_genW_pt;
    TBranch* b_genW_eta;
    TBranch* b_genW_phi;
    TBranch* b_genW_mass;
    // Lepton from W Boson
    TBranch* b_genWLepton_id;
    TBranch* b_genWLepton_pt;
    TBranch* b_genWLepton_eta;
    TBranch* b_genWLepton_phi;
    // Higgs Boson
    TBranch* b_genHiggs_pt;
    TBranch* b_genHiggs_eta;
    TBranch* b_genHiggs_phi;
    TBranch* b_genHiggs_mass;
    // Mesons from Higgs
    TBranch* b_genHiggsMeson_id;
    TBranch* b_genHiggsMeson_pt;
    TBranch* b_genHiggsMeson_eta;
    TBranch* b_genHiggsMeson_phi;
    TBranch* b_genHiggsMeson_mass;
    TBranch* b_genHiggsMesonGamma_dR;
    // Gamma from Higgs
    TBranch* b_genGamma_pt;
    TBranch* b_genGamma_phi;
    TBranch* b_genGamma_eta;
    // K- from Phi
    TBranch* b_genKm_pt;
    TBranch* b_genKm_phi;
    TBranch* b_genKm_eta;
    // K+ from Phi
    TBranch* b_genKp_pt;
    TBranch* b_genKp_phi;
    TBranch* b_genKp_eta;
    // dR between Kaons
    TBranch* b_genKpKm_dR;

    /* --> Reco Branches <-- */
    // Reconstructed Higgs
    TBranch* b_recoHiggs_mass;
    // Reconstructed Mesons
    TBranch* b_recoMeson_nCands;
    // Reconstructed Phi
    TBranch* b_recoPhi_mass;
    TBranch* b_recoPhi_pt;
    TBranch* b_recoPhi_eta;
    TBranch* b_recoPhi_phi;
    TBranch* b_recoPhi_relIso;
    TBranch* b_recoPhiGamma_dR;
    // K- from Phi
    TBranch* b_recoKm_pt;
    TBranch* b_recoKm_eta;
    TBranch* b_recoKm_phi;
    TBranch* b_recoKm_relIso;
    // K+ from Phi
    TBranch* b_recoKp_pt;
    TBranch* b_recoKp_eta;
    TBranch* b_recoKp_phi;
    TBranch* b_recoKp_relIso;
    // dR between Kaons
    TBranch* b_recoKpKm_dR;
    // Reconstructed Rho
    TBranch* b_recoRho_mass;
    TBranch* b_recoRho_pt;
    TBranch* b_recoRho_eta;
    TBranch* b_recoRho_phi;
    TBranch* b_recoRho_relIso;
    TBranch* b_recoRhoGamma_dR;
    // Pi- from Rho
    TBranch* b_recoPim_pt; 
    TBranch* b_recoPim_eta;
    TBranch* b_recoPim_phi;
    TBranch* b_recoPim_relIso;
    // Pi+ from Rho
    TBranch* b_recoPip_pt;
    TBranch* b_recoPip_eta;
    TBranch* b_recoPip_phi;
    TBranch* b_recoPip_relIso;
    // dR between Pions
    TBranch* b_recoPipPim_dR;
    // Photons
    TBranch* b_recoGamma_pt;
    TBranch* b_recoGamma_phi;
    TBranch* b_recoGamma_eta;
    TBranch* b_recoGamma_relIso;
    TBranch* b_genRecoGamma_isMatch;
    TBranch* b_minGammaParton_dR;
    // Leptons
    TBranch* b_recoWLepton_id;
    TBranch* b_recoWLepton_pt;
    TBranch* b_recoWLepton_eta;
    TBranch* b_recoWLepton_phi;
    TBranch* b_recoWLepton_nLep;
    TBranch* b_recoWLepton_nLepVeto;
    TBranch* b_recoGammaWLepton_dR;

    public:
        // TTree
        TTree* t;

        // Config Struct
        Config config;

        /* --> Meta Branch Values <-- */
        // Event info
        int run;
        int lumi;
        int event;
        float scale1fb;
        int isGold;
        int isHEM;
        // MET
        float met_pt;
        float met_phi;
        float rawMet_pt;
        float rawMet_phi;
        // Triggers
        int HLT_singleMu; 
        int HLT_singleEl; 
        // Filters
        int passFilters;
        /* --> Special Branch Values <-- */
        // dR(gen, reco)
        float genRecoGamma_dR;
        float genRecoPhi_dR;
        float genRecoRho_dR;
        // Magic Angles
        float recoMagAng_cosThetaStar;
        float recoMagAng_cosTheta1;
        float recoMagAng_cosTheta2;
        float recoMagAng_Phi;
        float recoMagAng_Phi1;
        float recoMagAng_m1;
        float recoMagAng_m2;
        float genMagAng_cosThetaStar;
        float genMagAng_cosTheta1;
        float genMagAng_cosTheta2;
        float genMagAng_Phi;
        float genMagAng_Phi1;
        float genMagAng_m1;
        float genMagAng_m2;

        /* --> Gen Branch Values <-- */
        // W Boson
        float genW_pt;
        float genW_eta;
        float genW_phi;                                                             
        float genW_mass;
        // Lepton from W Boson
        int genWLepton_id;
        float genWLepton_pt;
        float genWLepton_eta;
        float genWLepton_phi;                                                             
        float recoGammaWLepton_dR;
        // Higgs Boson
        float genHiggs_pt;
        float genHiggs_eta;
        float genHiggs_phi;                                                             
        float genHiggs_mass;
        // Mesons from Higgs
        int genHiggsMeson_id;
        float genHiggsMeson_pt;
        float genHiggsMeson_eta;
        float genHiggsMeson_phi;
        float genHiggsMeson_mass;
        float genHiggsMesonGamma_dR;
        // Gamma from Higgs
        float genGamma_pt;
        float genGamma_phi;
        float genGamma_eta;
        // K- from Phi
        float genKm_pt;
        float genKm_phi;
        float genKm_eta;
        // K+ from Phi
        float genKp_pt;
        float genKp_phi;
        float genKp_eta;
        // dR between Kaons
        float genKpKm_dR;

        /* --> Reco Branch Values <-- */
        // Reconstructed Higgs
        float recoHiggs_mass;
        // Reconstructed Phi
        float recoPhi_mass;
        float recoPhi_pt;
        float recoPhi_eta;
        float recoPhi_phi;
        float recoPhi_relIso;
        float recoPhiGamma_dR;
        // K- from Phi
        float recoKm_pt;
        float recoKm_eta;
        float recoKm_phi;
        float recoKm_relIso;
        // K+ from Phi
        float recoKp_pt;
        float recoKp_eta;
        float recoKp_phi;
        float recoKp_relIso;
        // dR between Kaons
        float recoKpKm_dR;
        // Reconstructed Mesons
        int recoMeson_nCands;
        // Reconstructed Rho
        float recoRho_mass;
        float recoRho_pt;
        float recoRho_eta;
        float recoRho_phi;
        float recoRho_relIso;
        float recoRhoGamma_dR;
        // Pi- from Rho
        float recoPim_pt; 
        float recoPim_eta;
        float recoPim_phi;
        float recoPim_relIso;
        // Pi+ from Rho
        float recoPip_pt;
        float recoPip_eta;
        float recoPip_phi;
        float recoPip_relIso;
        // dR between Pions
        float recoPipPim_dR;
        // Photons
        float recoGamma_pt;
        float recoGamma_phi;
        float recoGamma_eta;
        float recoGamma_relIso;
        int genRecoGamma_isMatch;
        float minGammaParton_dR;
        // Leptons
        int recoWLepton_id;
        float recoWLepton_pt;
        float recoWLepton_eta;
        float recoWLepton_phi;
        int recoWLepton_nLep;
        int recoWLepton_nLepVeto;

        /* --> Methods <-- */
        // Constructor
        BabyTree();
        // Reset Vars
        void Reset();
        // Get dR
        float dR(float, float, float, float);
        // Fill config struct
        void MakeConfig(TString);
        // Fill gen-level branches
        int FillGenBranches();
        // Fill reco branches
        int FillRecoBranches();
        // Fill gen-reco branches
        int FillGenRecoBranches();
};

#endif
