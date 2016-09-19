#include "Utility.h"

double PhiCenters[360];
double EtaCenters[500];
for (int i = 0; i < 360; ++i)
{
    PhiCenters[i]=((double)i+0.5)/180.0*3.1415926;
}
for (int i = 0; i < 500; ++i)
{
    EtaCenters[i]= i*0.02 - 4.99;
}

Smearing::Smearing()
{
    fFormulaTrack = new DelphesFormula;
    fFormulaECal = new DelphesFormula;
    fFormulaTrack->Compile("(abs(eta) <= 0.5) * (pt > 0.1) * sqrt(0.06^2 + pt^2*1.3e-3^2) + (abs(eta) > 0.5 && abs(eta) <= 1.5) * (pt > 0.1) * sqrt(0.10^2 + pt^2*1.7e-3^2) + (abs(eta) > 1.5 && abs(eta) <= 2.5) * (pt > 0.1) * sqrt(0.25^2 + pt^2*3.1e-3^2)");
    fFormulaECal->Compile("(abs(eta) <= 3.2) * sqrt(energy^2*0.0017^2 + energy*0.101^2) + (abs(eta) > 3.2 && abs(eta) <= 4.9) * sqrt(energy^2*0.0350^2 + energy*0.285^2)");
    
}

void Smearing::TrackPTSmearing(TLorentzVector &v1)
{
    double eta = v1.Eta();
    double phi = v1.Phi();
    double pt = v1.Pt();
    double e = v1.E();

    double sigma = pt * fFormulaTrack->Eval(pt,eta,phi,e);
    pt = gRandom->Gaus(pt,sigma);
    v1.SetPtEtaPhiE(pt,eta,phi,pt*TMath::CosH(eta));
}
double Smearing::LogNormal(double mean,double sigma)
{
    double a, b;

    if(mean > 0.0)
    {
        b = TMath::Sqrt(TMath::Log((1.0 + (sigma*sigma)/(mean*mean))));
        a = TMath::Log(mean) - 0.5*b*b;

        return TMath::Exp(a + b*gRandom->Gaus(0.0, 1.0));
    }
    else
    {
        return 0.0;
    }
}

void Smearing::ECalEnergySmearing(TLorentzVector &v1,int flags)
{
    double eta = v1.Eta();
    double phi = v1.Phi();
    double pt = v1.Pt();
    double e = v1.E();
    if (flags == 12)
    {
        // eta = gRandom->Uniform(eta-0.05,eta+0.05);
        eta = EtaCenters[(int)((eta+5)/0.02)];
        // phi = gRandom->Uniform(phi-5.0/180.0*3.14159,phi+5.0/180.0*3.14159);
        phi = PhiCenters[((int)((phi/(3.1415926)*180)+360))%(360)];
    }
    double sigma = fFormulaECal->Eval(0,eta,0,e);
    e = LogNormal(e,sigma);
    v1.SetPtEtaPhiE(e/TMath::CosH(eta),eta,phi,e);
}
