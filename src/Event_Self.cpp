#include "Event_Self.h"


Event_Self::Event_Self(TreeReader *reader, int fakeQ)
{
    fJet = new JetSystem();
    fLep = new LepSystem();
    fMET = new METSystem();
    SetUpEventPointer(reader);
    bac_fake = fakeQ;
}

void Event_Self::SetUpEventPointer(TreeReader *reader)
{
	TreeReader::TBranchArray::iterator ite;
    // cout<<"Begin:"<<endl;
	ite = reader->MyBranch.find(make_pair("Electron","Electron"));
    if(ite==reader->MyBranch.end()) cout<<"Wrong!"<<endl;
    fLep->SetUpEle(ite->second);
    // cout<<"Electron Done"<<endl;
	ite = reader->MyBranch.find(make_pair("Muon","Muon"));
	fLep->SetUpMuon(ite->second);
    // cout<<"Muon Done"<<endl;
	ite = reader->MyBranch.find(make_pair("MissingET","MissingET"));
    fMET->SetUp(ite->second);
    // cout<<"MET Done"<<endl;
    ite = reader->MyBranch.find(make_pair("Jet","Jet"));
    // cout<<"Before Jets"<<endl;
    fJet->SetUp(ite->second);
    // cout<<"Jet Done"<<endl;

    ite = reader->MyBranch.find(make_pair("Particle","GenParticle"));
    fParticles = ite->second;

}
// Return 3-bit integer, First for MET Cuts, Second for Lepton related basic cuts, Third for Jets related basic cuts
bool Event_Self::BasicCutsOnEvent(BasicCuts cuts, vector<int> &PassQ)
{
    // cout<<"Here first"<<endl;
    fJet->PTminEtaCuts(cuts);
    // cout<<"Here3"<<endl;
    fLep->PTminEtaCuts(cuts);
    fLep->dRwithJet(cuts,fJet);
    UpdateEvent(cuts); // Previous three steps will remove lepton or jet which does not satisfy the requirement, so we should update the local event record;
    
    bool good=1;
    good *= fMET->METCuts(cuts); //Minimun MET Cuts
    if (!good) return good;
    // PassQ = PassQ | (good << 0);
    //PassQ[istep] = 1;
    //istep++;
    PassQ.push_back(1);

    good *= fLep->NumberCuts(cuts); //Total Leptons number cuts
    if (!good) return good;
    PassQ.push_back(1);

    good *= fLep->PTLeadingCuts(cuts); //The Leading PT cuts on all leptons
    if (!good) return good;
    PassQ.push_back(1);

    good *= fLep->Trigger(cuts); //The Lepton trigger;
    if (!good) return good;
    PassQ.push_back(1);

    good *= fLep->SetUpZSystem(cuts); // Set Up the Z peak systems and then apply the related cuts (Number of Z boson and the invariance mass range)
    if (!good) return good;
    // PassQ = PassQ | (good << 1);
    PassQ.push_back(1);
    PassQ[1] = 1;
    
    good = 1;
    good *= fJet->TauNumberCuts(cuts);
    if(!good) return good;
    PassQ.push_back(1);

    good *= fJet->VBFNumberCuts(cuts);
    if(!good) return good;
    PassQ.push_back(1);

    good *= fJet->NumberCuts(cuts); //all Jets, VBF jets (if), B-jets, Tau-jets, Normal jets
    if (!good) return good;
    PassQ.push_back(1);
    // cout<<"Here4"<<endl;
    good *= fJet->PTLeadingCuts(cuts); //The Leading PT cuts on all categories of jets repectively (all, vbf(if), b, tau, normal)
    if (!good) return good;
    PassQ.push_back(1);
    // cout<<"Here5"<<endl;
    good *= fJet->VBFSpecialCutsEta(cuts); // if VBF, cuts on delta Eta, and Mjj of the two leading VBF jets
    if (!good) return good;
    PassQ.push_back(1);

    good *= fJet->VBFSpecialCutsMass(cuts); // if VBF, cuts on delta Eta, and Mjj of the two leading VBF jets
    if (!good) return good;
    PassQ.push_back(1);
    //cout<<"Here1"<<endl;
    good *= fJet->SetUpTauJet(bac_fake,cuts.fJetCuts.TauComponentQ); // Setup the Tau Component, for all tagged tau-jets according the flag TauComponentQ and bac_fake
    if (!good) return good;
    //cout<<"Here2"<<endl;
    //fJet->ColinearAppro(fMET,bac_fake); // Using colinear approximation, to get the total tau momentum.
    fJet->ColinearAppro(fMET);
    FindTrueTaus();
    // PassQ = PassQ | (good << 2);
    //PassQ[2] = 1;
    PassQ.push_back(1);

    // good = 1;
    // good *= fJet->PTSumTauJets(cuts);
    // //good *= fJet->HiggsPTCuts(fMET);
    // if (!good) return PassQ;
    // // good *= fJet->DeltaRTauJets();
    // // if (!good) return PassQ;
    // PassQ = PassQ | (good << 2);

    // good = 1;
    // good *= fJet->MinvTauPair();
    // if (!good) return PassQ;
    // PassQ = PassQ | (good << 3); 
    
    return good;

}

bool Event_Self::FindTrueTaus()
{
    fTauTrue.clear();
    int ntotal = fParticles->GetEntriesFast();
    GenParticle *particle;
    for (int i = 0; i < ntotal; ++i)
    {
        particle = (GenParticle*)fParticles->At(i);
        if (particle->Status != 3)
        {
            break;
        }
        if (fabs(particle->PID)==15)
        {
            fTauTrue.push_back(particle->P4());
        }
    }
    if (fTauTrue.size()!=2)
    {
        return false;
    }
    return true;
}

