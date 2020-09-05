#ifndef PhotonSystem_H
#define PhotonSystem_H
#include "TreeReader.h"
#include "CutSystem.h"

class PhotonSystem
{
public:
    PhotonSystem();
    ~PhotonSystem();
    
    void Clear(){Ntotal=0;}
    void SetUp(TClonesArray *array){farray=array;}
    void Update(int bac_fake,BasicCuts cuts){Ntotal=farray->GetEntriesFast();}

    bool Cuts(BasicCuts);

    TClonesArray *farray;
    int Ntotal;
};



#endif