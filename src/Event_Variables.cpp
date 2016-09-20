#include "Event_Variables.h"
#ifndef ct
#define ct 87.03
#endif
ClassImp(Event_Variables);

Event_Variables::Event_Variables()
{
    NEvents=0;
    Weight=1;

    METInTrans=0;
    EtaTauInVBF=0;
    dRtautau=0;
    dEtatautau=0;
    PtHiggs=0;
    MET=0;
    PtSumTauTau=0;
    MassTauTau=0;
    MassVBFJets=0;
    dEtaVBFJets=0;
    Ptj1=0;
    Ptj2=0;
    CPPhi4PiSys=0;
    CPPhiImP=0;
    PassQ=0;
    
    fevent=NULL;
}

Event_Variables::Event_Variables(Event_Self *event)
{
    NEvents=0;
    Weight=1;

    METInTrans=0;
    EtaTauInVBF=0;
    dRtautau=0;
    dEtatautau=0;
    PtHiggs=0;
    MET=0;
    PtSumTauTau=0;
    MassTauTau=0;
    MassVBFJets=0;
    dEtaVBFJets=0;
    Ptj1=0;
    Ptj2=0;
    CPPhi4PiSys=0;
    CPPhiImP=0;

    PassQ=0;

    fevent = event;
}

void Event_Variables::SetVariables()
{
    METInTrans=GetMETInTrans();
    EtaTauInVBF=GetEtaTauInVBF();
    dRtautau=GetdRtautau();
    dEtatautau=GetdEtatautau();
    PtHiggs=GetPtHiggs();
    PtSumTauTau=GetPtSumTauTau();
    MassTauTau=GetMassTauTau();
    MassVBFJets=GetMassVBFJets();
    dEtaVBFJets=GetdEtaVBFJets();
    CPPhi4PiSys=GetCPPhi4PiSys();
    CPPhiImP=GetCPPhiImp();
    MET=GetMET();
}

void Event_Variables::SetCuts(AdvancedCuts cuts, vector<int> &PassQ, int cate)
{
   if (cate == 10)
   {
     SetCutsVH(cuts,PassQ);
   }
   else if (cate == 20)
   {
     SetCutsVBFHighPT(cuts,PassQ);
   }
   else if (cate == 21)
   {
     SetCutsVBFLowPTtight(cuts,PassQ);
   }
   else if (cate == 22)
   {
     SetCutsVBFLowPTloose(cuts,PassQ);
   }
   else
   {
    cout<<"Unrecognized Category."<<endl;
   }
}

void Event_Variables::SetCutsVH(AdvancedCuts cuts, vector<int> &PassQ)
{
    if (PtSumTauTau>cuts.PtSumTauMin)
    {
      //PassQ[3] = 1;
      PassQ.push_back(1);
    }
    else
    {
      PassQ.push_back(0);
    }
}

void Event_Variables::SetCutsVBFHighPT(AdvancedCuts cuts, vector<int> &PassQ)
{
    // PassQ=0;
    if (dRtautau>cuts.dRtautau[0] && dRtautau < cuts.dRtautau[1])
    {
      //PassQ[3] = 1;
      PassQ.push_back(1);
    }
    else
    {
      PassQ.push_back(0);
    }
    if (dEtatautau<cuts.dEtatautau)
    {
      // PassQ[4] = 1;
      PassQ.push_back(1);
    }
    else
    {
      PassQ.push_back(0);
    }
    if (PtHiggs>cuts.PtHiggsMin)
    {
      // PassQ[5] = 1;
      PassQ.push_back(1);
    }
    else
    {
      PassQ.push_back(0);
    }
    if (METInTrans>=cuts.METInTransQ)
    {
      // PassQ[6] = 1;
      PassQ.push_back(1);
    }
    else
    {
      PassQ.push_back(0);
    }
    if (EtaTauInVBF>=cuts.EtaTauInVBFQ)
    {
      // PassQ[7] = 1;
      PassQ.push_back(1);
    }
    else
    {
      PassQ.push_back(0);
    }
}

void Event_Variables::SetCutsVBFLowPTtight(AdvancedCuts cuts, vector<int> &PassQ)
{
    if (dRtautau>cuts.dRtautau[0] && dRtautau < cuts.dRtautau[1])
    {
      // PassQ[3] = 1;
      PassQ.push_back(1);
    }
    else
    {
      PassQ.push_back(0);
    }
    if (dEtatautau<cuts.dEtatautau)
    {
      // PassQ[4] = 1;
      PassQ.push_back(1);
    }
    else
    {
      PassQ.push_back(0);
    }
    if (dRtautau>1.5 || PtHiggs < 140.0)
    {
      // PassQ[5] = 1;
      PassQ.push_back(1);
    }
    else
    {
      PassQ.push_back(0);
    }
    if (METInTrans>=cuts.METInTransQ)
    {
      // PassQ[6] = 1;
      PassQ.push_back(1);
    }
    else
    {
      PassQ.push_back(0);
    }
    if (EtaTauInVBF>=cuts.EtaTauInVBFQ)
    {
      // PassQ[7] = 1;
      PassQ.push_back(1);
    }
    else
    {
      PassQ.push_back(0);
    }
    if (MassVBFJets > (-250*dEtaVBFJets + 1550))
    {
      // PassQ[8] = 1;
      PassQ.push_back(1);
    }
    else
    {
      PassQ.push_back(0);
    }
}

void Event_Variables::SetCutsVBFLowPTloose(AdvancedCuts cuts, vector<int> &PassQ)
{
    if (dRtautau>cuts.dRtautau[0] && dRtautau < cuts.dRtautau[1])
    {
      // PassQ[3] = 1;
      PassQ.push_back(1);
    }
    else
    {
      PassQ.push_back(0);
    }
    if (dEtatautau<cuts.dEtatautau)
    {
      // PassQ[4] = 1;
      PassQ.push_back(1);
    }
    else
    {
      PassQ.push_back(0);
    }
    if (dRtautau>1.5 || PtHiggs < 140.0)
    {
      // PassQ[5] = 1;
      PassQ.push_back(1);
    }
    else
    {
      PassQ.push_back(0);
    }
    if (METInTrans>=cuts.METInTransQ)
    {
      // PassQ[6] = 1;
      PassQ.push_back(1);
    }
    else
    {
      PassQ.push_back(0);
    }
    if (EtaTauInVBF>=cuts.EtaTauInVBFQ)
    {
      // PassQ[7] = 1;
      PassQ.push_back(1);
    }
    else
    {
      PassQ.push_back(0);
    }
    if (MassVBFJets < (-250*dEtaVBFJets + 1550))
    {
      // PassQ[8] = 1;
      PassQ.push_back(1);
    }
    else
    {
      PassQ.push_back(0);
    }
}

int Event_Variables::GetMETInTrans()
{
  if (fevent->fJet->fTauJets.size()<2)
  {
    return -1;
  }
  TLorentzVector tau1 = fevent->fJet->fTauJets[0].VisP;
  TLorentzVector tau2 = fevent->fJet->fTauJets[1].VisP;
  TLorentzVector met = ((MissingET *)fevent->fMET->farray->At(0))->P4();
  double dphi_st = tau1.DeltaPhi(tau2);
  double dphi1 = met.DeltaPhi(tau1);
  double dphi2 = met.DeltaPhi(tau2);
  if ((dphi1<dphi_st && dphi2 < dphi_st) || (dphi1 < 3.1415926/4.0 || dphi2 < 3.1415926/4.0))
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

double Event_Variables::GetMET()
{
  return ((MissingET *)fevent->fMET->farray->At(0))->MET;
}

int Event_Variables::GetEtaTauInVBF()
{
  if (fevent->fJet->fTauJets.size()<2 || fevent->fJet->fVBFJets.size()<2)
  {
    return -1;
  }
  double etaj1 = fevent->fJet->fVBFJets[0].Eta();
  double etaj2 = fevent->fJet->fVBFJets[1].Eta();
  double etajmin = etaj1>etaj2?etaj2:etaj1;
  double etajmax = etaj1>etaj2?etaj1:etaj2;
  double etatau1 = fevent->fJet->fTauJets[0].VisP.Eta();
  double etatau2 = fevent->fJet->fTauJets[1].VisP.Eta();
  if (etajmin < etatau1 && etatau1 < etajmax && etajmin < etatau2 && etatau2 < etajmax)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

double Event_Variables::GetdEtaVBFJets()
{
  if (fevent->fJet->fVBFJets.size()>=2)
  {
    return fabs(fevent->fJet->fVBFJets[0].Eta()-fevent->fJet->fVBFJets[1].Eta());
  }
  else
  {
    return -999;
  }
}

double Event_Variables::GetdRtautau()
{
    if (fevent->fJet->fTauJets.size()>=2)
    {
      return fevent->fJet->fTauJets[0].VisP.DeltaR(fevent->fJet->fTauJets[1].VisP);
    }
    else
    {
      return -999;
    }
    
}

double Event_Variables::GetdEtatautau()
{
    if (fevent->fJet->fTauJets.size()>=2)
    {
      return fabs(fevent->fJet->fTauJets[0].VisP.Eta()-fevent->fJet->fTauJets[1].VisP.Eta());
    }
    else
    {
      return -999;
    }
    
}

double Event_Variables::GetPtHiggs()
{
    if (fevent->fJet->fTauJets.size()<2)
    {
      return -999;
    }
    TLorentzVector met = ((MissingET *)fevent->fMET->farray->At(0))->P4();
    TLorentzVector tau1 = fevent->fJet->fTauJets[0].VisP;
    TLorentzVector tau2 = fevent->fJet->fTauJets[1].VisP;
    return (met + tau1 + tau2).Pt();
}

double Event_Variables::GetPtSumTauTau()
{
    if (fevent->fJet->fTauJets.size()<2)
    {
      return -999;
    }
    TLorentzVector tau1 = fevent->fJet->fTauJets[0].VisP;
    TLorentzVector tau2 = fevent->fJet->fTauJets[1].VisP;
    return tau1.Pt() + tau2.Pt();
}

double Event_Variables::GetMassTauTau()
{
    if (fevent->fJet->fTauJets.size()<2)
    {
      return -999;
    }
    TLorentzVector tau1 = fevent->fJet->fTauJets[0].PAppro;
    TLorentzVector tau2 = fevent->fJet->fTauJets[1].PAppro;
    return (tau1 + tau2).M();   
}

double Event_Variables::GetMassVBFJets()
{
    if (fevent->fJet->fVBFJets.size()<2)
    {
      return -999;
    }
    TLorentzVector j1 = fevent->fJet->fVBFJets[0];
    TLorentzVector j2 = fevent->fJet->fVBFJets[1];
    return (j1 + j2).M();
}

TLorentzVector BoostSelf(TLorentzVector v1, TLorentzVector v2) // Boost v1 to the rest frame of v2
{
  double beta = v2.Beta();
  double gamma = v2.Gamma();

  TVector3 unit = v2.Vect();
  unit = (1.0/unit.Mag())*unit;

  double Erest = gamma*(v1.Energy()-beta*(unit.Dot(v1.Vect())));
  TVector3 prest = v1.Vect() + (gamma-1)*(unit.Dot(v1.Vect()))*unit - ( gamma * beta * v1.Energy())*unit;

  return TLorentzVector(prest,Erest);

}

double Event_Variables::GetCPPhi4PiSys()
{
    if (fevent->fJet->fTauJets.size()<2)
    {
      return -999;
    }
    TauJet tau1 = fevent->fJet->fTauJets[0];
    TauJet tau2 = fevent->fJet->fTauJets[1];
    TLorentzVector pic1 = tau1.Pic;
    TLorentzVector pic2 = tau2.Pic;
    TLorentzVector pi01 = tau1.Pi0;
    TLorentzVector pi02 = tau2.Pi0;
    double selection = (pic1.Energy()-pi01.Energy())*(pic2.Energy()-pi02.Energy());
    double angle = GetPhiZMF(pic1,pi01,pic2,pi02);
   if (selection < 0)
   {
      if (angle > 0)
      {
        angle = angle - TMath::Pi();
      }
      else
      {
        angle = angle + TMath::Pi();
      }
   }
   return angle;
}

double Event_Variables::GetCPPhiImp()
{
    if (fevent->fJet->fTauJets.size()<2||fevent->fTauTrue.size()<2)
    {
      return -999;
    }
    ImpactParameter imp(fevent->fJet->fTauJets[0].VisP,fevent->fTauTrue[0],fevent->fJet->fTauJets[1].VisP,fevent->fTauTrue[1]);
    return imp.GetCPPhi();
}

ImpactParameter::ImpactParameter(TLorentzVector pi1, TLorentzVector tau1, TLorentzVector pi2, TLorentzVector tau2)
{
    SetUpMomentum(pi1,tau1,pi2,tau2);
}

void ImpactParameter::SetUpMomentum(TLorentzVector pi1, TLorentzVector tau1, TLorentzVector pi2, TLorentzVector tau2)
{
    double dr1 = pi1.DeltaR(tau1);
    double dr2 = pi1.DeltaR(tau2);
    //cout<<dr1<<"  "<<dr2<<endl;
    if (dr1 < dr2)
    {
       ftau1 = tau1;
       ftau2 = tau2;
    }
    else
    {
      ftau1 = tau2;
      ftau2 = tau1;
    }
    fpi1 = pi1;
    //ftau1 = tau1;
    fpi2 = pi2;
    //ftau2 = tau2;
}

void ImpactParameter::GetImpactParameter()
{
    TVector3 pi1V = fpi1.Vect();
    TVector3 tau1V = ftau1.Vect();
    TVector3 Imp1 = pi1V.Cross(tau1V.Cross(pi1V));

    double Ltau = gRandom->Exp(ct);
    double gamma = ftau1.Gamma();
    double cangle = pi1V.Dot(tau1V)/(pi1V.Mag()*tau1V.Mag());
    cangle = fabs(cangle)>1?1:cangle;
    //cout<<cangle<<endl;
    double sangle = sqrt(1-cangle*cangle);
    double mag = Ltau * gamma * sangle;
    //cout<<mag<<endl;
    Imp1.SetMag(mag);
    fImp1 = TLorentzVector(Imp1,0);

    TVector3 pi2V = fpi2.Vect();
    TVector3 tau2V = ftau2.Vect();
    TVector3 Imp2 = pi2V.Cross(tau2V.Cross(pi2V));

    Ltau = gRandom->Exp(ct);
    gamma = ftau2.Gamma();
    cangle = pi2V.Dot(tau2V)/(pi2V.Mag()*tau2V.Mag());
    cangle = fabs(cangle)>1?1:cangle;
    sangle = sqrt(1-cangle*cangle);
    mag = Ltau * gamma * sangle;
    Imp2.SetMag(mag);
    fImp2 = TLorentzVector(Imp2,0);
}

double ImpactParameter::GetCPPhi()
{
    GetImpactParameter();
    return GetPhiZMF(fpi1,fImp1,fpi2,fImp2);
}

double GetPhiZMF(TLorentzVector pic1,TLorentzVector pi01,TLorentzVector pic2,TLorentzVector pi02)
{
    TLorentzVector pictotal = pic1 + pic2;
   //boost = pictotal.BoostVector();
   pic1=BoostSelf(pic1,pictotal);
   pic2=BoostSelf(pic2,pictotal);
   pi01=BoostSelf(pi01,pictotal);
   pi02=BoostSelf(pi02,pictotal);

   //TLorentzVector check = pic1+pic2;
   //cout<<check.Px()<<" "<<check.Py()<<"  "<<check.Pz()<<endl;
   
   TVector3 z = pic1.Vect();
   TVector3 pi013 = pi01.Vect();
   TVector3 pi023 = pi02.Vect();

   TVector3 plane1 = (z.Cross(pi013)).Cross(z);
   TVector3 plane2 = (z.Cross(pi023)).Cross(z);

   double angle = acos(plane1.Dot(plane2)/(plane1.Mag()*plane2.Mag()));
   double sign = (plane1.Cross(plane2)).Dot(z);
   if (sign < 0)
   {
     angle = -angle;
   }
   return angle;
}