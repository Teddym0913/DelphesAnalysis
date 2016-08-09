#ifndef Event_Self_H
#define Event_Self_H
#include "TreeReader.h"
#include "CutSystem.h"
#include "METSystem.h"
#include "LepSystem.h"
#include "JetSystem.h"

class Event_Self
{
public:
	Event_Self(){};
	Event_Self(TreeReader *, int);
	~Event_Self(){};
	void UpdateEvent(BasicCuts cuts){fJet->Update(bac_fake,cuts);fLep->Update(bac_fake,cuts);fMET->Update(bac_fake,cuts);}
	void SetUpEventPointer(TreeReader *);
	bool BasicCutsOnEvent(BasicCuts cuts, vector<int> &PassQ );
	bool FindTrueTaus();
	//void SetUpEventContent();

//private:
	JetSystem *fJet;
	LepSystem *fLep;
	METSystem *fMET;
	TClonesArray *fParticles;
	vector<TLorentzVector> fTauTrue;

	int bac_fake;
	//int PassQ;

	
};

// class ObjectinEvent
// {
// public:
// 	ObjectinEvent(){farray=NULL;Ntotal=0;}
// 	~ObjectinEvent(){}

// 	void Clear(){Ntotal=0;}
// 	void SetUp(TClonesArray *array){farray=array;}
// 	void Update(){Ntotal = farray->GetEntriesFast();}

// //private:
// 	TClonesArray *farray;
// 	int Ntotal;

// };

#endif