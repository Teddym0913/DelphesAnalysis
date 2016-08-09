#ifndef UTILITY_H
#define UTILITY_H
#include "TLorentzVector.h"
#include "TRandom3.h"
#include "classes/DelphesFormula.h"

void TrackPTSmearing(TLorentzVector &v1);

double LogNormal(double mean,double sigma);

void ECalEnergySmearing(TLorentzVector &v1);

#endif