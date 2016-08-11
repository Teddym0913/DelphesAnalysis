#ifndef JetSystem_H
#define JetSystem_H
#include "TreeReader.h"
#include "CutSystem.h"
#include "METSystem.h"
#include "TRandom3.h"
#include "Utility.h"

typedef enum TauMode
{ 
    onepi=1,
    twopi=2,
    threepi=3
} TauMode;

class TauJet
{
public:
    TauJet(){};
    TauJet(TauMode mode = twopi);
    TauJet(const TauJet &);
    ~TauJet(){};

    //bool SetUpComponentPiMass(Jet *j1, int ComponentQ);
    bool SetUpComponent(Jet *j1, int ComponentQ);
    bool SetUpComponentConstituents(Jet *j1, int ComponentQ);
    bool SetUpComponentParticles(Jet *j1, int ComponentQ);

//private:
    int charge;
    TLorentzVector VisP;
    TLorentzVector Pic;
    TLorentzVector Pi0;
    TLorentzVector neu;
    TLorentzVector PAppro; // The total tau momentum under the colinear approximation
    TLorentzVector TauTrue;
    TauMode fmode;
    Smearing fSmearing;

};

class JetSystem
{
public:
    JetSystem(){farray=NULL;Ntotal=0;}
    ~JetSystem(){};

    void Clear();
    void Update(int,BasicCuts);
    void SetUp(TClonesArray *array){farray=array;}
    //void SetUp(TreeReader::TBranchArray::iterator ite){fite=ite;}
    void PTminEtaCuts(BasicCuts);
    bool NumberCuts(BasicCuts);
    bool TauNumberCuts(BasicCuts);
    bool VBFNumberCuts(BasicCuts);
    bool PTLeadingCuts(BasicCuts);
    bool VBFSpecialCutsEta(BasicCuts);
    bool VBFSpecialCutsMass(BasicCuts);


    bool PTSumTauJets(BasicCuts);
    bool HiggsPTCuts(METSystem *);
    bool TauEtaCuts();
    bool TauAndMET(METSystem *);
    bool DeltaRTauJets();
    bool MinvTauPair();
    

    bool dPhiwithMET(BasicCuts, METSystem *);


    bool SetUpTauJet(int, int);
    void ColinearAppro(METSystem *);
    //void ColinearAppro(METSystem *,int);
//private:
    int Ntotal;
    int bac_fake;
    int NVBFJets; // Actually it should be 2 if we focus on VBF topology. This number records whether this event contain enough Jets
    int NBJets;
    int NTauJets;
    int NNormalJets; // Beside B and Tau Jet and VBF Jets.

    vector<int> IndexBJets;
    vector<int> IndexTauJets;
    vector<int> IndexNormalJets;
    vector<int> IndexVBFJets;
    vector<TauJet> fTauJets;
    vector<TLorentzVector> fVBFJets;

    //TreeReader::TBranchArray::iterator fite;
    TClonesArray *farray;

};

#endif