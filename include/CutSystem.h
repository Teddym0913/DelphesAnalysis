#ifndef CutSystem_H
#define CutSystem_H

#include <string>
#include <vector>
#include <fstream>
#include "TreeReader.h"
//#include "LepSys.h"
#include "TClonesArray.h"
#include <stdlib.h>

class Cuts
{
public:
    Cuts();
    ~Cuts(){}

    void Clear();
    double PTmin;
    double Eta[4];
    vector<double> LeadingPTmin;
    double dRSelf;

    int NofObject[2]; // Min and Max  -1 means no cut on that side

};

class JetCuts:public Cuts
{
public:
    JetCuts();
    JetCuts(const char *);
    ~JetCuts(){}

    void Clear();
    void ReadFile(const char *file);
    int NofBJets[2];
    int NofTauJets[2];
    int NofNormalJets[2];
    vector<double> LeadingPTBJets;
    vector<double> LeadingPTTauJets;
    vector<double> LeadingPTNormalJets;
    double dPhiJMET[2];
    int TauComponentQ;
};

class LepCuts:public Cuts
{
public:
    LepCuts();
    LepCuts(const char *);
    ~LepCuts(){}

    void Clear();
    void ReadFile(const char *file);
    double dRwithJet;
    int NofZBoson;
    double MinvMin;
    double MinvMax;
};

class VBFCuts:public Cuts
{
public:
    VBFCuts();
    VBFCuts(const char *file);
    ~VBFCuts(){}

    void Clear();
    void ReadFile(const char *file);
    int NofForwardObj;
    vector<double> PTofForwardObj;
    double deltaEta;
    double mjjmin;
    int VBFQ;
    //How should we add VBF Cuts?
};

class BasicCuts
{
public:
    BasicCuts();
    BasicCuts(const char *file);
    ~BasicCuts(){}

    void Clear(){fJetCuts.Clear();fLepCuts.Clear();fVBFCuts.Clear();METmin=0;}
    void ReadFile(const char *file);
    int Output(const char *file); // Output value of basic cuts used to the final directory;
    JetCuts fJetCuts;
    LepCuts fLepCuts;
    VBFCuts fVBFCuts;
    double METmin;

private:
    char inputfilename[200];

};

class AdvancedCuts
{
public:
    AdvancedCuts();
    AdvancedCuts(const char *file);
    ~AdvancedCuts(){};

    void ReadFile(const char *file);
    void Clear();
    int NCuts;
    double PtSumTauMin;
    double dRtautau[2];
    double dEtatautau;
    double PtHiggsMin;

    int METInTransQ;
    int EtaTauInVBFQ;

    int TightorLooseQ;
};



#endif