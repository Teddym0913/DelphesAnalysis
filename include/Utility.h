#ifndef UTILITY_H
#define UTILITY_H
#include "TLorentzVector.h"
#include <vector>
#include <iostream>
#include "TRandom3.h"
#include "classes/DelphesFormula.h"

using namespace std;

class Smearing
{
public:
    Smearing();
    Smearing(double phigran, double etagran);
    ~Smearing(){delete fFormulaTrack;delete fFormulaECal;}

    DelphesFormula *fFormulaTrack;
    DelphesFormula *fFormulaECal;
    void SetGranularity(double phigran=0.02,double etagran=0.02);
    void TrackPTSmearing(TLorentzVector &v1);
    double LogNormal(double mean,double sigma);
    void ECalEnergySmearing(TLorentzVector &v1,int flags);
    vector<double> PhiCenters;
    vector<double> EtaCenters;
    double PhiGranu;
    double EtaGranu;
    int PhiTotal;
    int EtaTotal;
};

extern Smearing *gSmearing; //Global Smearing 
#endif