#ifndef Event_Variables_H
#define Event_Variables_H
#include "Event_Self.h"

class Event_Variables
{
public:
    Event_Variables();
    Event_Variables(Event_Self *event);
    ~Event_Variables(){};

    void SetVariables();

    int NEvents;
    double Weight;
    int PassQ;

    int METInTrans;
    int EtaTauInVBF;
    double dRtautau;
    double dEtatautau;
    double PtHiggs;
    double PtSumTauTau;
    double MassTauTau;
    double dEtaVBFJets;
    double MassVBFJets;
    double CPPhi4PiSys;
    double CPPhiImP;


    double GetdRtautau();
    double GetdEtatautau();
    double GetPtHiggs();
    double GetPtSumTauTau();
    double GetMassTauTau();
    double GetMassVBFJets();
    double GetdEtaVBFJets();
    double GetCPPhi4PiSys();
    double GetCPPhiImp();
    int GetMETInTrans();
    int GetEtaTauInVBF();

    void SetCuts(AdvancedCuts cuts, vector<int> &PassQ, int cate);
    void SetCutsVH(AdvancedCuts cuts, vector<int> &PassQ);
    void SetCutsVBFHighPT(AdvancedCuts cuts, vector<int> &PassQ);
    void SetCutsVBFLowPTtight(AdvancedCuts cuts, vector<int> &PassQ);
    void SetCutsVBFLowPTloose(AdvancedCuts cuts, vector<int> &PassQ);
//private:
    Event_Self *fevent; //!

    ClassDef(Event_Variables,1);
};

class ImpactParameter
{
public:
    ImpactParameter(){};
    ImpactParameter(TLorentzVector pim, TLorentzVector taum,TLorentzVector pip, TLorentzVector taup);
    ~ImpactParameter(){};

    void SetUpMomentum(TLorentzVector pim, TLorentzVector taum,TLorentzVector pip, TLorentzVector taup);
    void GetImpactParameter();
    double GetCPPhi();
private:
    TLorentzVector fpi1;
    TLorentzVector ftau1;
    TLorentzVector fpi2;
    TLorentzVector ftau2;

    TLorentzVector fImp1;
    TLorentzVector fImp2;
};

TLorentzVector BoostSelf(TLorentzVector v1, TLorentzVector v2);
double GetPhiZMF(TLorentzVector m1, TLorentzVector d1, TLorentzVector m2, TLorentzVector d2);
#endif
