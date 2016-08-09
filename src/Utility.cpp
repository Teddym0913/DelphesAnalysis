#include "Utility.h"

void TrackPTSmearing(TLorentzVector &v1)
{
    double eta = v1.Eta();
    double phi = v1.Phi();
    double pt = v1.Pt();
    double e = v1.E();

    DelphesFormula *fFormula = new DelphesFormula;
    fFormula->Compile("(abs(eta) <= 0.5) * (pt > 0.1) * sqrt(0.06^2 + pt^2*1.3e-3^2) + (abs(eta) > 0.5 && abs(eta) <= 1.5) * (pt > 0.1) * sqrt(0.10^2 + pt^2*1.7e-3^2) + (abs(eta) > 1.5 && abs(eta) <= 2.5) * (pt > 0.1) * sqrt(0.25^2 + pt^2*3.1e-3^2)");
    double sigma = pt * fFormula->Eval(pt,eta,phi,e);
    pt = gRandom->Gaus(pt,sigma);
    v1.SetPtEtaPhiE(pt,eta,phi,pt*TMath::CosH(eta));
    delete fFormula;
}
double LogNormal(double mean,double sigma)
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

void ECalEnergySmearing(TLorentzVector &v1)
{
    double eta = v1.Eta();
    double phi = v1.Phi();
    double pt = v1.Pt();
    double e = v1.E();
    eta = gRandom->Uniform(eta-0.05,eta+0.05);
    phi = gRandom->Uniform(phi-5.0/180.0*3.14159,phi+5.0/180.0*3.14159);
    DelphesFormula *fFormula = new DelphesFormula;
    fFormula->Compile("(abs(eta) <= 3.2) * sqrt(energy^2*0.0017^2 + energy*0.101^2) + (abs(eta) > 3.2 && abs(eta) <= 4.9) * sqrt(energy^2*0.0350^2 + energy*0.285^2)");
    double sigma = fFormula->Eval(0,eta,0,e);
    e = LogNormal(e,sigma);
    v1.SetPtEtaPhiE(e/TMath::CosH(eta),eta,phi,e);
    delete fFormula;
}
