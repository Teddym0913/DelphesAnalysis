#ifndef LepSystem_H
#define LepSystem_H
#include "TreeReader.h"
#include "CutSystem.h"
#include "METSystem.h"
#include "JetSystem.h"

class EleMuonSystem
{
public:
    EleMuonSystem(int id){farray = NULL; Ntotal=0;PID=id;}
    ~EleMuonSystem(){}

    void Clear(){Ntotal=0;}
    void SetUp(TClonesArray *array){farray=array;}
    void Update(){Ntotal=farray->GetEntriesFast();}

    void PTminEtaCuts(BasicCuts);
    void dRwithJet(BasicCuts, JetSystem*);

    TClonesArray *farray;
    int Ntotal;
    int PID; // 11 for Ele, 13 for Muon
    
};

class ZCandidate
{
public:
    ZCandidate(){}
    ZCandidate(double invM, int iPlus, int iMinus, int EorM){Minv = invM; indexPlus=iPlus; indexMinus=iMinus;EleorMuon=EorM;}
    // 11 for Electron, 13 for Muon
    ~ZCandidate(){}

    double GetInvM(){return Minv;}
    //void GetIndex(int &iPlus, int &iMinus, int &EorM){iPlus = indexPlus; iMinus=indexMinus; EorM = EleorMuon;}

//private:
    double Minv;
    int indexPlus;
    int indexMinus;
    int EleorMuon;
};

class ZPeakSystem
{
public:
    ZPeakSystem(){}
    ~ZPeakSystem(){}
    void Clear();
    bool Update(EleMuonSystem *, EleMuonSystem *,BasicCuts cuts); // For saving time, we will sort the lepton system according to the cut requirement.


private:
    int N_Leps;
    int N_ZCandidates;
    int N_UnPairLeps;
    
    int N_Elec;
    int N_Muon;
    
    vector<ZCandidate> ZPeakCandidates;

    vector<int> UnPair_ElePlus_Index; //
    vector<int> UnPair_EleMinus_Index; //
    vector<int> UnPair_MuonPlus_Index; //
    vector<int> UnPair_MuonMinus_Index; //

    void SetUpSignFlavorSystem(); // Consider How to implement this.
    
};

class LepSystem
{
public:
    LepSystem(){NLepTot=0;fEle=new EleMuonSystem(11); fMuon=new EleMuonSystem(13); fZPeak=new ZPeakSystem();}
    ~LepSystem();


    void Clear(){fEle->Clear();fMuon->Clear();fZPeak->Clear();NLepTot=0;}
    void Update(int,BasicCuts);
    void SetUpEle(TClonesArray *array){fEle->SetUp(array);}
    void SetUpMuon(TClonesArray *array){fMuon->SetUp(array);}
    bool SetUpZSystem(BasicCuts cuts){ return fZPeak->Update(fEle,fMuon,cuts);} // Set the Z-peak system according to our cuts requirement;
    

    bool NumberCuts(BasicCuts cuts);
    void PTminEtaCuts(BasicCuts cuts){fEle->PTminEtaCuts(cuts);fMuon->PTminEtaCuts(cuts);}
    bool PTLeadingCuts(BasicCuts cuts);
    void dRwithJet(BasicCuts cuts, JetSystem *fJet){fEle->dRwithJet(cuts,fJet);fMuon->dRwithJet(cuts,fJet);}
    bool MTCuts(METSystem*);

    bool ElecTrigger();
    bool MuonTrigger();
    bool Trigger(BasicCuts);

private:
    EleMuonSystem *fEle;
    EleMuonSystem *fMuon;
    ZPeakSystem *fZPeak;
    int NLepTot;

};


#endif