#include "METSystem.h"

bool METSystem::METCuts(BasicCuts cuts)
{
    double METlimit = cuts.METmin;
    double MET = ((MissingET*)farray->At(0))->MET;
    if (MET<METlimit)
    {
        return false;
    }
    return true;
}