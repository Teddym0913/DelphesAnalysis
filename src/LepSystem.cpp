#include "LepSystem.h"

bool DecrePT(double a,double b)
{
    return (a>b);
}

// bool CompareZCandi(ZCandidate a, ZCandidate b)
// {
//     double atemp = fabs(a.GetInvM()-91.888);
//     double btemp = fabs(b.GetInvM()-91.888);
//     return (atemp<btemp);
// }

void LepSystem::Update(int bac_fake,BasicCuts cuts)
{
    Clear(); // Clear All Local Information;

    //Update the Electron and Muon Info.
    fEle->Update();
    fMuon->Update();

    NLepTot = fEle->Ntotal + fMuon->Ntotal;
    
}

void EleMuonSystem::PTminEtaCuts(BasicCuts cuts)
{
    int Ntotal = farray -> GetEntriesFast();
    double pt,eta;
    for (int in = 0; in < Ntotal; ++in)
    {
        if (PID==11)
        {
            pt = ((Electron *)farray->At(in))->PT;
            eta = ((Electron *)farray->At(in))->Eta;
        }
        else if (PID==13)
        {
            pt = ((Muon *)farray->At(in))->PT;
            eta = ((Muon *)farray->At(in))->Eta;
        }
        else
        {
            cout<<"Waring: Neither Ele nor Muon. Temporarily treated as Ele."<<endl;
            pt = ((Electron *)farray->At(in))->PT;
            eta = ((Electron *)farray->At(in))->Eta;
        }
        if( pt < (cuts.fLepCuts).PTmin || eta < (cuts.fLepCuts).Eta[0] || eta > (cuts.fLepCuts).Eta[3] || (eta > (cuts.fLepCuts).Eta[1] && eta < (cuts.fLepCuts).Eta[2]))
        {
            farray->RemoveAt(in);
        }
    }
    farray->Compress();
}

void EleMuonSystem::dRwithJet(BasicCuts cuts, JetSystem *fJet)
{
    int Nele = farray->GetEntriesFast();
    int Njet = fJet->farray->GetEntriesFast();
    TLorentzVector lep,jet;
    double dR;
    double dRlimit = cuts.fLepCuts.dRwithJet;
    for (int ilep = 0; ilep < Nele; ++ilep)
    {
        if (PID==11)
        {
            lep = ((Electron *)farray->At(ilep))->P4();
        }
        else if (PID==13)
        {
            lep = ((Muon *)farray->At(ilep))->P4();
        }
        else
        {
            cout<<"Waring: Neither Ele nor Muon. Temporarily treated as Ele."<<endl;
            lep = ((Electron *)farray->At(ilep))->P4();
        }
        for (int ijet = 0; ijet < Njet; ++ijet)
        {
            jet = ((Jet *)(fJet->farray)->At(ijet))->P4();
            dR = jet.DeltaR(lep);
            if (dR < dRlimit)
            {
                farray->RemoveAt(ilep);
                break;
            }
        }
    }
    farray->Compress();
}

bool LepSystem::PTLeadingCuts(BasicCuts cuts)
{
    vector<double> LepPT;
    int nele = fEle->farray->GetEntriesFast();
    int nmuon = fMuon->farray->GetEntriesFast();
    for (int i = 0; i < nele; ++i)
    {
        double pt = ((Electron *)(fEle->farray)->At(i))->PT;
        LepPT.push_back(pt);
    }
    for (int i = 0; i < nmuon; ++i)
    {
        double pt = ((Muon *)(fMuon->farray)->At(i))->PT;
        LepPT.push_back(pt);
    }
    sort(LepPT.begin(),LepPT.end(),DecrePT); // Sort the LepPT in Decreasing order
    int ncuts = cuts.fLepCuts.LeadingPTmin.size();
    for (int i = 0; i < ncuts; ++i)
    {
        if (LepPT[i]<cuts.fLepCuts.LeadingPTmin[i])
        {
            return false;
        }
    }
    return true;
}

// SameSignSameFlavor or SameSignOppositeFlavor or OppositeSignSameFlavor or OppositeSignOppositeFlavor
bool LepSystem::NumberCuts(BasicCuts cuts)
{
    if (NLepTot < cuts.fLepCuts.NofObject[0] || (NLepTot > cuts.fLepCuts.NofObject[1] && cuts.fLepCuts.NofObject[1]>=0))
    {
        return false;
    }
    // Add Something Z Boson Peak Selection
    return true;
}

bool LepSystem::ElecTrigger()
{
    double pt1,pt2;
    if (fEle->Ntotal >= 2)
    {
        pt1 = ((Electron *)fEle->farray->At(0))->PT;
        pt2 = ((Electron *)fEle->farray->At(1))->PT;
        if ((pt2 > 12 && pt1 > 12) || (pt2 > 7 && pt1 > 24))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool LepSystem::MuonTrigger()
{
    double pt1,pt2;
    if (fMuon->Ntotal >= 2)
    {
        pt1 = ((Muon *)fMuon->farray->At(0))->PT;
        pt2 = ((Muon *)fMuon->farray->At(1))->PT;
        if ((pt2 > 13 && pt1 > 13) || (pt2 > 8 && pt1 > 18))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool LepSystem::Trigger(BasicCuts cuts)
{
    if (cuts.fVBFCuts.VBFQ)
    {
        return true;
    }
    bool ele = ElecTrigger();
    bool muon = MuonTrigger();
    return ele||muon;
}

bool LepSystem::MTCuts(METSystem *met)
{
    if (NLepTot != 1)
    {
        return false;
    }
    int nele = fEle->farray->GetEntriesFast();
    int nmuon = fMuon->farray->GetEntriesFast();
    TLorentzVector lep;
    if (nele == 1)
    {
        lep = ((Electron *)fEle->farray->At(0))->P4();
    }
    else if (nmuon == 1)
    {
        lep = ((Muon *)fMuon->farray->At(0))->P4();
    }
    TLorentzVector pmet = ((MissingET *)(met->farray->At(0)))->P4();
    double mt = (lep+pmet).M();
    return mt>20;
}

void ZPeakSystem::Clear()
{
    N_Leps = 0;
    N_ZCandidates = 0;
    N_UnPairLeps = 0;
    
    N_Elec = 0;
    N_Muon = 0;

    ZPeakCandidates.clear();
    UnPair_ElePlus_Index.clear(); //
    UnPair_EleMinus_Index.clear(); //
    UnPair_MuonPlus_Index.clear(); //
    UnPair_MuonMinus_Index.clear(); //
}

bool ZPeakSystem::Update(EleMuonSystem *fEle, EleMuonSystem *fMuon, BasicCuts cuts)
{
    Clear();
    N_Elec = fEle->farray->GetEntriesFast();
    N_Muon = fMuon->farray->GetEntriesFast();
    if (cuts.fLepCuts.NofZBoson==-1)
    {
        return true;
    }
// 
    //First record electron/muon according to their charge
    for (int iEle = 0; iEle < N_Elec; ++iEle)
    {
        if (((Electron *)fEle->farray->At(iEle))->Charge==1)
        {
            UnPair_ElePlus_Index.push_back(iEle);
        }
        else if (((Electron *)fEle->farray->At(iEle))->Charge==-1)
        {
            UnPair_EleMinus_Index.push_back(iEle);
        }
    }
    for (int iMuon = 0; iMuon < N_Muon; ++iMuon)
    {
        if (((Muon *)fMuon->farray->At(iMuon))->Charge==1)
        {
            UnPair_MuonPlus_Index.push_back(iMuon);
        }
        else if (((Muon *)fMuon->farray->At(iMuon))->Charge==-1)
        {
            UnPair_MuonMinus_Index.push_back(iMuon);
        }
    }
    TLorentzVector plus,minus;
    while(UnPair_EleMinus_Index.size()>0&&UnPair_ElePlus_Index.size()>0)
    {
        int minusTot = UnPair_EleMinus_Index.size();
        int plusTot = UnPair_ElePlus_Index.size();
        double deltaMmin = (cuts.fLepCuts.MinvMax-cuts.fLepCuts.MinvMin)/2.0;
        double InvMtemp;
        int iptemp=-999;
        int imtemp=-999;
        for (int iplus = 0; iplus < plusTot; ++iplus)
        {
            plus = ((Electron *)fEle->farray->At(UnPair_ElePlus_Index[iplus]))->P4();
            for (int iminus = 0; iminus < minusTot; ++iminus)
            {
                minus = ((Electron *)fEle->farray->At(UnPair_EleMinus_Index[iminus]))->P4();            
                double tempdelta = fabs((plus+minus).M()-91.888);
                if (tempdelta<deltaMmin)
                {
                    iptemp = iplus;
                    imtemp = iminus;
                    InvMtemp = (plus+minus).M();
                    deltaMmin = tempdelta;
                }
            }
        }
        if (iptemp==-999&&imtemp==-999)
        {
            break;
        }
        else
        {
            ZCandidate Ztemp(InvMtemp,UnPair_ElePlus_Index[iptemp],UnPair_EleMinus_Index[imtemp],11);
            ZPeakCandidates.push_back(Ztemp);
            UnPair_ElePlus_Index.erase(UnPair_ElePlus_Index.begin()+iptemp);
            UnPair_EleMinus_Index.erase(UnPair_EleMinus_Index.begin()+imtemp);
        }
    }

    while(UnPair_MuonMinus_Index.size()>0&&UnPair_MuonPlus_Index.size()>0)
    {
        int minusTot = UnPair_MuonMinus_Index.size();
        int plusTot = UnPair_MuonPlus_Index.size();
        double deltaMmin = (cuts.fLepCuts.MinvMax-cuts.fLepCuts.MinvMin)/2.0;
        double InvMtemp;
        int iptemp=-999;
        int imtemp=-999;
        for (int iplus = 0; iplus < plusTot; ++iplus)
        {
            plus = ((Muon *)fMuon->farray->At(UnPair_MuonPlus_Index[iplus]))->P4();
            for (int iminus = 0; iminus < minusTot; ++iminus)
            {
                minus = ((Muon *)fMuon->farray->At(UnPair_MuonMinus_Index[iminus]))->P4();            
                double tempdelta = fabs((plus+minus).M()-91.888);
                if (tempdelta<deltaMmin)
                {
                    iptemp = iplus;
                    imtemp = iminus;
                    InvMtemp = (plus+minus).M();
                    deltaMmin = tempdelta;
                }
            }
        }
        if (iptemp==-999&&imtemp==-999)
        {
            break;
        }
        else
        {
            ZCandidate Ztemp(InvMtemp,UnPair_MuonPlus_Index[iptemp],UnPair_MuonMinus_Index[imtemp],13);
            ZPeakCandidates.push_back(Ztemp);
            UnPair_MuonPlus_Index.erase(UnPair_MuonPlus_Index.begin()+iptemp);
            UnPair_MuonMinus_Index.erase(UnPair_MuonMinus_Index.begin()+imtemp);
        }
    }
    if (ZPeakCandidates.size()!=cuts.fLepCuts.NofZBoson)
    {
        return false;
    }
    return true;
}