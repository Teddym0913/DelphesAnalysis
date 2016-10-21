#include "Utility.h"



Smearing::Smearing()
{
    fFormulaTrack = new DelphesFormula;
    fFormulaECal = new DelphesFormula;
    fFormulaTrack->Compile("(abs(eta) <= 0.5) * (pt > 0.1) * sqrt(0.06^2 + pt^2*1.3e-3^2) + (abs(eta) > 0.5 && abs(eta) <= 1.5) * (pt > 0.1) * sqrt(0.10^2 + pt^2*1.7e-3^2) + (abs(eta) > 1.5 && abs(eta) <= 2.5) * (pt > 0.1) * sqrt(0.25^2 + pt^2*3.1e-3^2)");
    fFormulaECal->Compile("(abs(eta) <= 3.2) * sqrt(energy^2*0.0017^2 + energy*0.101^2) + (abs(eta) > 3.2 && abs(eta) <= 4.9) * sqrt(energy^2*0.0350^2 + energy*0.285^2)");
    SetGranularity();  
}

Smearing::Smearing(double phigran, double etagran)
{
    fFormulaTrack = new DelphesFormula;
    fFormulaECal = new DelphesFormula;
    fFormulaTrack->Compile("(abs(eta) <= 0.5) * (pt > 0.1) * sqrt(0.06^2 + pt^2*1.3e-3^2) + (abs(eta) > 0.5 && abs(eta) <= 1.5) * (pt > 0.1) * sqrt(0.10^2 + pt^2*1.7e-3^2) + (abs(eta) > 1.5 && abs(eta) <= 2.5) * (pt > 0.1) * sqrt(0.25^2 + pt^2*3.1e-3^2)");
    fFormulaECal->Compile("(abs(eta) <= 3.2) * sqrt(energy^2*0.0017^2 + energy*0.101^2) + (abs(eta) > 3.2 && abs(eta) <= 4.9) * sqrt(energy^2*0.0350^2 + energy*0.285^2)");
    SetGranularity(phigran, etagran);
}

void Smearing::SetGranularity(double phigran, double etagran)
{
    PhiCenters.clear();
    EtaCenters.clear();
    PhiTotal = (int)(2*3.1415926/phigran);
    EtaTotal = (int)(10.0/etagran);
    PhiGranu = 2*3.1415926/PhiTotal;
    EtaGranu = 10.0/EtaTotal;
    //PhiCenters = new double(PhiTotal);
    //EtaCenters = new double(EtaTotal);
    // cout<<"Phi: "<<PhiTotal<<" "<<PhiGranu<<endl;
    // cout<<"Eta: "<<EtaTotal<<" "<<EtaGranu<<endl;
    for (int i = 0; i < PhiTotal; ++i)
    {
        //PhiCenters[i]=((double)i+0.5)/180.0*3.1415926;
        //PhiCenters[i]= i*PhiGranu - 3.1415926;
        PhiCenters.push_back(i*PhiGranu-3.1415926);
    }
    for (int i = 0; i < EtaTotal; ++i)
    {
        //EtaCenters[i]= i*EtaGranu - (5.0-EtaGranu/2.0);
        EtaCenters.push_back(i*EtaGranu-(5.0-EtaGranu/2.0));
    } 
    cout<<"Here"<<endl;
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
        // cout<<EtaTotal<<" Eta "<<(int)((eta+5)/EtaGranu)<<endl;
        eta = EtaCenters[(int)((eta+5)/EtaGranu)];
        // phi = gRandom->Uniform(phi-5.0/180.0*3.14159,phi+5.0/180.0*3.14159);
        //phi = PhiCenters[((int)((phi/(3.1415926)*180)+360))%(360)];
        // cout<<PhiTotal<<" Phi "<<(int)((phi+3.1415925)/PhiGranu)<<endl;
        phi = PhiCenters[(int)((phi+3.1415925)/PhiGranu)];
    }
    double sigma = fFormulaECal->Eval(0,eta,0,e);
    e = LogNormal(e,sigma);
    v1.SetPtEtaPhiE(e/TMath::CosH(eta),eta,phi,e);
}

Smearing *gSmearing = new Smearing(0.02,0.02); //Global Smearing 