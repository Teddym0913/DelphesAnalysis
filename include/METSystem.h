#ifndef METSystem_HH
#define METSystem_HH
#include "TreeReader.h"
#include "CutSystem.h"

class METSystem
{
public:
    METSystem(){Ntotal=0;farray=NULL;}
    ~METSystem(){}

    void Clear(){Ntotal=0;}
    void SetUp(TClonesArray *array){farray=array;}
    void Update(int bac_fake,BasicCuts cuts){Ntotal=farray->GetEntriesFast();}

    bool METCuts(BasicCuts);

    TClonesArray *farray;
    int Ntotal;
    
};

#endif