#include "JetSystem.h"

void JetSystem::Clear()
{
    NVBFJets = 0;
    NBJets = 0;
    NTauJets = 0;
    NNormalJets = 0;
    IndexBJets.clear();
    IndexTauJets.clear();
    IndexNormalJets.clear();
    IndexVBFJets.clear();
    fTauJets.clear();
    fVBFJets.clear();
}

void JetSystem::Update(int bac_fake, BasicCuts cuts)
{
    Clear(); // First Clear All the local information;
    //Ntotal = farray->GetEntriesFast();
    Ntotal = farray->GetEntriesFast();
    int vbfQ = cuts.fVBFCuts.VBFQ;
    //int firstCount;
    TLorentzVector vbfjet;
    vector<int> fakeindex;
    fakeindex.clear();
    if (bac_fake>0)
    {
        while(fakeindex.size()<bac_fake)
        {
            int index = gRandom->Uniform()*Ntotal;
            bool good =1;
            for (int ii = 0; ii < fakeindex.size(); ++ii)
            {
                good*=(index!=fakeindex[ii]);
            }
            if (good)
            {
                fakeindex.push_back(index);
            }
        }
    }
    // if (vbfQ == 1)
    // {
    //     if (Ntotal<2)
    //     {
    //         NVBFJets = Ntotal;
    //         firstCount = Ntotal;
    //     }
    //     else
    //     {
    //         NVBFJets = 2;
    //         firstCount = 2;
    //     }
    //     for (int ivbf = 0; ivbf < NVBFJets; ++ivbf)
    //     {
    //         vbfjet = ((Jet*)farray->At(ivbf))->P4();
    //         fVBFJets.push_back(vbfjet);
    //     }

    // }
    // else
    // {
    //     NVBFJets = 0;
    //     firstCount = 0;
    // }
    int ifake = 0;
    for (int ieve = 0; ieve < Ntotal; ++ieve)
    {
        bool isBjet = 0;
        bool isTaujet = 0;
        // Count the Number of BJets and TauJets and then Count the VBF Jets(if) and Normal Jets. 
        // Also record the position in the farray of each BJet TauJet and VBF Jets(if) Normal Jet.
        isBjet = (((Jet*)farray->At(ieve))->BTag & (1<<0));
        if (bac_fake > 0)
        {
            if (ifake<bac_fake && ieve == fakeindex[ifake])
            {
                isTaujet = 1;
                ifake++;
            }
            else
            {
                isTaujet = 0;
            }
        }
        else
        {
            isTaujet = (((Jet*)farray->At(ieve))->TauTag & (1<<0));
        }
        if(isBjet && !isTaujet)
        {
            NBJets++;
            IndexBJets.push_back(ieve);
        }
        else if (isTaujet && !isBjet)
        {
            NTauJets++;
            IndexTauJets.push_back(ieve);
        }
        else if (!isTaujet && !isBjet)
        {
            if (NVBFJets<2 && vbfQ)
            {
                NVBFJets++;
                vbfjet = ((Jet*)farray->At(ieve))->P4();
                IndexVBFJets.push_back(ieve);
                fVBFJets.push_back(vbfjet);
            }
            else
            {
                NNormalJets++;
                IndexNormalJets.push_back(ieve);
            }
        }
        else
        {
            //cout<<"Warning: There is a jet tagged as both B and Tau. And Now we treat it as Tau"<<endl;
            NTauJets++;
            IndexTauJets.push_back(ieve);
        }
    }
}

void JetSystem::PTminEtaCuts(BasicCuts cuts)
{
    int Ntotal = farray-> GetEntriesFast();
    double pt,eta;
    int vbfQ = cuts.fVBFCuts.VBFQ;
    int *ptetagood = new int[Ntotal];
    //cout<<Ntotal<<endl;
    //cout<<NBJets<<" "<<NTauJets<<"  "<<NNormalJets<<" "<<NVBFJets<<endl;
    for (int ijet = 0; ijet < NBJets; ++ijet)
    {
        //cout<<"Bjets: "<<endl;
        //cout<<IndexBJets[ijet]<<endl;
        pt = ((Jet *)farray->At(IndexBJets[ijet]))->PT;
        eta = ((Jet *)farray->At(IndexBJets[ijet]))->Eta;
        //cout<<pt<<"  "<<eta<<endl;
        if( pt < (cuts.fJetCuts).PTmin || eta < (cuts.fJetCuts).Eta[0] || eta > (cuts.fJetCuts).Eta[3] || (eta > (cuts.fJetCuts).Eta[1] && eta < (cuts.fJetCuts).Eta[2]))
        {
            //farray->RemoveAt(ijet);
            ptetagood[IndexBJets[ijet]]=0;
        }
        else
        {
            ptetagood[IndexBJets[ijet]]=1;
        }
    }
    for (int ijet = 0; ijet < NTauJets; ++ijet)
    {
        //cout<<"TauJets: "<<endl;
        //cout<<IndexTauJets[ijet]<<endl;
        pt = ((Jet *)farray->At(IndexTauJets[ijet]))->PT;
        //cout<<pt<<endl;
        eta = ((Jet *)farray->At(IndexTauJets[ijet]))->Eta;
        //cout<<pt<<"  "<<eta<<endl;
        if( pt < (cuts.fJetCuts).PTmin || eta < (cuts.fJetCuts).Eta[0] || eta > (cuts.fJetCuts).Eta[3] || (eta > (cuts.fJetCuts).Eta[1] && eta < (cuts.fJetCuts).Eta[2]))
        {
            //farray->RemoveAt(ijet);
            ptetagood[IndexTauJets[ijet]]=0;
        }
        else
        {
            ptetagood[IndexTauJets[ijet]]=1;
        }
        //cout<<"end"<<endl;
    }
    for (int ijet = 0; ijet < NNormalJets; ++ijet)
    {
        //cout<<"Normal Jets:"<<endl;
        //cout<<IndexNormalJets[ijet]<<endl;
        pt = ((Jet *)farray->At(IndexNormalJets[ijet]))->PT;
        //cout<<pt<<endl;
        eta = ((Jet *)farray->At(IndexNormalJets[ijet]))->Eta;
        //cout<<eta<<endl;
        if( pt < (cuts.fJetCuts).PTmin || eta < (cuts.fJetCuts).Eta[0] || eta > (cuts.fJetCuts).Eta[3] || (eta > (cuts.fJetCuts).Eta[1] && eta < (cuts.fJetCuts).Eta[2]))
        {
            //farray->RemoveAt(ijet);
            ptetagood[IndexNormalJets[ijet]]=0;
        }
        else
        {
            ptetagood[IndexNormalJets[ijet]]=1;
        }
    }
    if (vbfQ)
    {
        for (int ijet = 0; ijet < NVBFJets; ++ijet)
        {
            pt = ((Jet *)farray->At(IndexVBFJets[ijet]))->PT;
            eta = ((Jet *)farray->At(IndexVBFJets[ijet]))->Eta;
            if( pt < (cuts.fJetCuts).PTmin )
            {
                //farray->RemoveAt(ijet);
                ptetagood[IndexVBFJets[ijet]]=0;
            }
            else
            {
                ptetagood[IndexVBFJets[ijet]]=1;
            }
        }
    }
    for (int ijet = 0; ijet < Ntotal; ++ijet)
    {
        if (!ptetagood[ijet])
        {
            farray->RemoveAt(ijet);
        }
    }
    farray->Compress();
}

bool JetSystem::NumberCuts(BasicCuts cuts)
{
    if (Ntotal<cuts.fJetCuts.NofObject[0] || (Ntotal>cuts.fJetCuts.NofObject[1] && cuts.fJetCuts.NofObject[1]>=0))
    {
        return false;
    }
    if (cuts.fVBFCuts.VBFQ==1 && NVBFJets < cuts.fVBFCuts.NofForwardObj)
    {
        return false;
    }
    if (NBJets<cuts.fJetCuts.NofBJets[0] || (NBJets>cuts.fJetCuts.NofBJets[1] && cuts.fJetCuts.NofBJets[1]>=0))
    {
        return false;
    }

    if (NTauJets<cuts.fJetCuts.NofTauJets[0] || (NTauJets>cuts.fJetCuts.NofTauJets[1] && cuts.fJetCuts.NofTauJets[1]>=0))
    {
        return false;
    }

    if (NNormalJets<cuts.fJetCuts.NofNormalJets[0] || (NNormalJets>cuts.fJetCuts.NofNormalJets[1] && cuts.fJetCuts.NofNormalJets[1]>=0))
    {
        return false;
    }
    return true;
}

bool JetSystem::TauNumberCuts(BasicCuts cuts)
{
    if (NTauJets<cuts.fJetCuts.NofTauJets[0] || (NTauJets>cuts.fJetCuts.NofTauJets[1] && cuts.fJetCuts.NofTauJets[1]>=0))
    {
        return false;
    }
    return true;
}

bool JetSystem::VBFNumberCuts(BasicCuts cuts)
{
    if (cuts.fVBFCuts.VBFQ==1 && NVBFJets < cuts.fVBFCuts.NofForwardObj)
    {
        return false;
    }
    return true;
}

bool JetSystem::dPhiwithMET(BasicCuts cuts, METSystem *fMET)
{
    // After Find the standard Method, please implement it here.
    return true;
}

bool JetSystem::PTLeadingCuts(BasicCuts cuts)
{
    int vbfQ = cuts.fVBFCuts.VBFQ;
    int firstCount=0;
    double pt;
    if (vbfQ == 1)
    {
        int nVBFJets = ((cuts.fVBFCuts).PTofForwardObj).size();
        for (int i = 0; i < nVBFJets; ++i)
        {
            pt = ((Jet *)farray->At(IndexVBFJets[i]))->PT;
            if (pt < ((cuts.fVBFCuts).PTofForwardObj)[i])
            {
                return false;
            }
        }
        // firstCount = 2;
        // for (int i = 0; i < 2; ++i)
        // {
        //     pt = ((Jet *)farray->At(i))->PT;
        //     if (pt < ((cuts.fVBFCuts).PTofForwardObj)[i])
        //     {
        //         return false;
        //     }
        // }
    }
    // Cuts on all jets and also B-jets, Tau-jets and Normal-jets
    // int njetcuts = ((cuts.fJetCuts).LeadingPTmin).size();
    // //double pt;
    // for (int i = 0; i < njetcuts; ++i)
    // {
    //     pt = ((Jet *)farray->At(i+firstCount))->PT;
    //     if (pt < ((cuts.fJetCuts).LeadingPTmin)[i])
    //     {
    //         return false;
    //     }
    // }

    int nNorjetcuts = ((cuts.fJetCuts).LeadingPTNormalJets).size();
    //double pt;
    for (int i = 0; i < nNorjetcuts; ++i)
    {
        pt = ((Jet *)farray->At(IndexNormalJets[i]))->PT;
        if (pt < ((cuts.fJetCuts).LeadingPTNormalJets)[i])
        {
            return false;
        }
    }

    int nbjetcuts = ((cuts.fJetCuts).LeadingPTBJets).size();
    //double pt;
    for (int i = 0; i < nbjetcuts; ++i)
    {
        pt = ((Jet *)farray->At(IndexBJets[i]))->PT;
        if (pt < ((cuts.fJetCuts).LeadingPTBJets)[i])
        {
            return false;
        }
    }

    int ntaujetcuts = ((cuts.fJetCuts).LeadingPTTauJets).size();
    //double pt;
    for (int i = 0; i < ntaujetcuts; ++i)
    {
        pt = ((Jet *)farray->At(IndexTauJets[i]))->PT;
        if (pt < ((cuts.fJetCuts).LeadingPTTauJets)[i])
        {
            return false;
        }
    }
    return true;
}

bool JetSystem::VBFSpecialCutsEta(BasicCuts cuts)
{
    if (cuts.fVBFCuts.VBFQ != 1)
    {
        return true;
    }
    double deltaEta = fabs(fVBFJets[0].Eta()-fVBFJets[1].Eta());
    if (deltaEta < cuts.fVBFCuts.deltaEta)
    {
        return false;
    }
    // double mjj = (fVBFJets[0]+fVBFJets[1]).M();
    // if (mjj < cuts.fVBFCuts.mjjmin)
    // {
    //     return false;
    // }
    return true;
}

bool JetSystem::VBFSpecialCutsMass(BasicCuts cuts)
{
    if (cuts.fVBFCuts.VBFQ != 1)
    {
        return true;
    }
    double mjj = (fVBFJets[0]+fVBFJets[1]).M();
    if (mjj < cuts.fVBFCuts.mjjmin)
    {
        return false;
    }
    return true;
}

bool JetSystem::PTSumTauJets(BasicCuts cuts)
{
    double pt1 = fTauJets[0].VisP.Pt();
    double pt2 = fTauJets[1].VisP.Pt();
    return pt1+pt2>100.0;
}

bool JetSystem::HiggsPTCuts(METSystem *fmet)
{
    TLorentzVector tau1 = fTauJets[0].VisP;
    TLorentzVector tau2 = fTauJets[1].VisP;
    TLorentzVector met = ((MissingET *)fmet->farray->At(0))->P4();
    double higgsPT = (tau1+tau2+met).Pt();
    if (higgsPT>140)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool JetSystem::DeltaRTauJets()
{
    double deltaR = fTauJets[0].VisP.DeltaR(fTauJets[1].VisP);
    return deltaR>0.8&&deltaR<1.5;
}

bool JetSystem::TauAndMET(METSystem *fmet)
{
    double dphi_st = fTauJets[0].VisP.DeltaPhi(fTauJets[1].VisP);
    double dphimet1 = fTauJets[0].VisP.DeltaPhi(((MissingET *)fmet->farray->At(0))->P4());
    double dphimet2 = fTauJets[1].VisP.DeltaPhi(((MissingET *)fmet->farray->At(0))->P4());
    if (dphimet1 < 3.1415926/4 || dphimet2 < 3.1415926/4)
    {
        return true;
    }
    else
    {
        if (dphimet1<dphi_st && dphimet2 < dphi_st)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

bool JetSystem::TauEtaCuts()
{
    double eta1 = fTauJets[0].VisP.Eta();
    double eta2 = fTauJets[0].VisP.Eta();
    double deltaeta = fabs(eta1-eta2);
    if (deltaeta > 1.5)
    {
        return false;
    }
    double etaj1 = fVBFJets[0].Eta();
    double etaj2 = fVBFJets[1].Eta();
    double etamin = etaj1>etaj2?etaj2:etaj1;
    double etamax = etaj1>etaj2?etaj1:etaj2;
    if (eta1<etamin || eta2 < etamin || eta1 > etamax || eta2 > etamax)
    {
        return false;
    }
    return true;
}

bool JetSystem::MinvTauPair()
{
    double temp = (fTauJets[0].PAppro + fTauJets[1].PAppro).M();
    return (temp>100 && temp<150);
}

bool JetSystem::SetUpTauJet(int bac_fake, int ComponentQ)
{
    bool good=1;
    fTauJets.clear();
    int ComponentQFinal=0;
    if ( bac_fake == 0 && ComponentQ > 0)
    {
        ComponentQFinal = 1;
    }
    for (int i = 0; i < NTauJets; ++i)
    {
        TauJet tautemp(twopi);
        //cout<<"Here3"<<endl;
        good = tautemp.SetUpComponentOld((Jet *)farray->At(IndexTauJets[i]),ComponentQFinal);
        //cout<<"Here4"<<endl;
        if (good)
        {
            fTauJets.push_back(tautemp);
        }
        else
        {
            return false;
        }
    }
    return true;
}

void JetSystem::ColinearAppro(METSystem *met)
{
    if (fTauJets.size()<2)
    {
        //cout<<"Warning: No enough tau-jets."<<endl;
        return;
    }
    double MET = ((MissingET *)met->farray->At(0))->MET;
    double metPhi = ((MissingET *)met->farray->At(0))->Phi;
    double METX = MET * cos(metPhi);
    double METY = MET * sin(metPhi);
    double P1visX = fTauJets[0].VisP.Px();
    double P1visY = fTauJets[0].VisP.Py();
    double P2visX = fTauJets[1].VisP.Px();
    double P2visY = fTauJets[1].VisP.Py();
    double ratio1 = (METX*P2visY - METY*P2visX)/(P1visX*P2visY-P1visY*P2visX);
    double ratio2 = (METX*P1visY - METY*P1visX)/(P2visX*P1visY-P2visY*P1visX);
    fTauJets[0].neu = fTauJets[0].VisP*ratio1;
    fTauJets[1].neu = fTauJets[1].VisP*ratio2;

    fTauJets[0].PAppro = fTauJets[0].VisP*(1+ratio1);
    fTauJets[1].PAppro = fTauJets[1].VisP*(1+ratio2);


}

/*
I have changed the way to assign fake tau-jet, so we do not need the following function any more.
void JetSystem::ColinearAppro(METSystem *met,int bac_fake)
{
    if(!bac_fake)
    {
        ColinearAppro(met);
        return;
    }
    int i = gRandom->Uniform()*NTauJets;
    int j;
    while(1)
    {
        j = gRandom->Uniform()*NTauJets;
        if (j!=i) break;
    }
    double MET = ((MissingET *)met->farray->At(0))->MET;
    double metPhi = ((MissingET *)met->farray->At(0))->Phi;
    double METX = MET * cos(metPhi);
    double METY = MET * sin(metPhi);
    double P1visX = fTauJets[i].VisP.Px();
    double P1visY = fTauJets[i].VisP.Py();
    double P2visX = fTauJets[j].VisP.Px();
    double P2visY = fTauJets[j].VisP.Py();
    double ratio1 = (METX*P2visY - METY*P2visX)/(P1visX*P2visY-P1visY*P2visX);
    double ratio2 = (METX*P1visY - METY*P1visX)/(P2visX*P1visY-P2visY*P1visX);
    fTauJets[i].neu = fTauJets[i].VisP*ratio1;
    fTauJets[j].neu = fTauJets[j].VisP*ratio2;

    fTauJets[i].PAppro = fTauJets[i].VisP*(1+ratio1);
    fTauJets[j].PAppro = fTauJets[j].VisP*(1+ratio2);

    TauJet temp1(fTauJets[i]);
    TauJet temp2(fTauJets[j]);
    fTauJets.clear();
    fTauJets.push_back(temp1);
    fTauJets.push_back(temp2);
    NTauJets=fTauJets.size();
}
*/


TauJet::TauJet(TauMode mode)
{
    fmode = mode;
}

TauJet::TauJet(const TauJet &tj):charge(tj.charge),VisP(tj.VisP),Pic(tj.Pic),Pi0(tj.Pi0),neu(tj.neu),PAppro(tj.PAppro)
{
    fmode=tj.fmode;
}

bool TLorentzVectorPTSort(TLorentzVector v1, TLorentzVector v2)
{
    return v1.Pt()>v2.Pt();
}

bool TauJet::SetUpComponent(Jet *jet, int ComponentQ)
{
    if (!ComponentQ)
    {
        charge = jet->Charge;
        VisP = jet->P4();
        return true;
    }
    TLorentzVector picMax;
    TLorentzVector photon1,photon2,photonbothV;
    TObject *object;
    Track *track;
    Tower *tower;
    GenParticle *particle;

    int pidpic1;
    double ptPic = -1;
    double photonPT1 = -1;
    double photonPT2 = -2;
    double photonBothPT = -3;
    int photonBothQ=0;
    //cout<<"Constituents Number: "<<jet->Constituents.GetEntriesFast()<<endl;
    for (int i = 0; i < jet->Constituents.GetEntriesFast(); ++i)
    {
        object = jet->Constituents.At(i);
        //cout<<"Object "<<i<<"in Jet"<<endl;
        if (object->IsA() == Track::Class())
        {
            //cout<<"Track: "<<endl;
            track = (Track*)object;
            int pid = track->PID;
            if (fabs(pid) == 211)
            {
              if (track->PT>ptPic)
              {
                ptPic = track->PT;
                picMax = track->P4();
                pidpic1 = pid;
              }
            }
        }
        if (object->IsA() == Tower::Class())
        {
            //cout<<"Tower: "<<endl;
            tower = (Tower*)object;
            int NinTower = tower->Particles.GetEntriesFast();
            int nPhoton = 0;
            vector<double> photonPTtemp;
            for (int ntower = 0; ntower < NinTower; ++ntower)
            {
             particle=(GenParticle*)tower->Particles.At(ntower);
             int pid = particle->PID;
             if (pid==22)
             {
               nPhoton++;
               photonPTtemp.push_back(particle->PT);
             }
            }
            if (nPhoton==1)
            {
              double pttemp=tower->ET;
              if (pttemp > photonPT1)
              {
                 photonPT2 = photonPT1;
                 photon2 = photon1;
                 photonPT1 = pttemp;
                 photon1 = tower->P4();

              }
              else if (pttemp < photonPT1 && pttemp > photonPT2)
              {
                photonPT2 = pttemp;
                photon2 = tower->P4();
              }
            }
            else if (nPhoton>=2)
            {
             photonBothQ=1;
             double pttemp=tower->ET;
             if (pttemp > photonBothPT)
             {
               photonBothPT = pttemp;
               photonbothV=tower->P4();
             }
            }
        }
    }

    if (ptPic<0||((photonPT1<0||photonPT2<0)&&photonBothPT<0))
    {
        Pic = TLorentzVector::TLorentzVector(0,0,0,0);
        Pi0 = TLorentzVector::TLorentzVector(0,0,0,0);
        VisP = Pic + Pi0;
        charge = 0;
        return false;
    }
    Pic = picMax;
    // if (photonBothQ==1)
    // {
    //     Pi0 = photonbothV;
    // }
    // else
    // {
    //     Pi0 = photon1 + photon2;
    // }
    Pi0 = jet->P4() - Pic;
    charge = pidpic1>0?1:-1;
    VisP = Pic + Pi0;
    return true;

}

// Following is the Old(and probably wrong) method to get the Tau-jets Component
// Using the Particles in Jet (But these Particles are not smeared yet)
// We add smearing by ourself.
bool TauJet::SetUpComponentOld(Jet *jet, int ComponentQ)
{
    if (!ComponentQ)
    {
        charge = jet->Charge;
        VisP = jet->P4();
        return true;
    }
    TLorentzVector picMax;
    TLorentzVector photon1,photon2;
    TObject *object;
    GenParticle *particle;
    double ptPic = -1;
    double photonPT1 = -1;
    double photonPT2 = -2;
    int pidpic1;
    for (int i = 0; i < jet->Particles.GetEntriesFast(); ++i)
    {
     object = jet->Particles.At(i);
     if (object->IsA() == GenParticle::Class())
     {
       particle=(GenParticle*)object;
       int pid = particle->PID;
       if (fabs(pid) == 211)
       {
          if (particle->PT > ptPic)
          {
            ptPic = particle->PT;
            picMax = particle->P4();
            pidpic1 = pid;
          }
          //picP4.push_back(make_pair(particle->P4(),pid));  
          //picP4.push_back(particle->P4());
       }
       else if (pid == 22)
       {
          double pttemp = particle->PT;
          if (pttemp > photonPT1)
          {
             photonPT2 = photonPT1;
             photon2 = photon1;
             photonPT1 = pttemp;
             photon1 = particle->P4();

          }
          else if (pttemp < photonPT1 && pttemp > photonPT2)
          {
            photonPT2 = pttemp;
            photon2 = particle->P4();
          }

         //photonP4.push_back(particle->P4());
       }
     }
    }

    if (ptPic<0||photonPT1<0||photonPT2<0)
    {
        Pic = TLorentzVector::TLorentzVector(0,0,0,0);
        Pi0 = TLorentzVector::TLorentzVector(0,0,0,0);
        VisP = Pic + Pi0;
        charge = 0;
        return false;
    }
    TrackPTSmearing(picMax);
    ECalEnergySmearing(photon1);
    ECalEnergySmearing(photon2);
    Pic = picMax;
    Pi0 = photon1 + photon2;
    charge = pidpic1>0?1:-1;
    VisP = Pic + Pi0;
    return true;
}

// bool TauJet::SetUpComponentPiMass(Jet *jet, int ComponentQ)
// {
//     if (!ComponentQ)
//     {
//         charge = jet->Charge;
//         VisP = jet->P4();
//         return true;
//     }
//     TLorentzVector picMax;
//     TLorentzVector pi0Candi;
//     TObject *object;
//     Track *track;
//     Tower *tower;
//     GenParticle *particle;

//     int pidpic1;
//     double ptPic = -1;
//     vector<TLorentzVector> TowerP4;
//     //cout<<"Constituents Number: "<<jet->Constituents.GetEntriesFast()<<endl;
//     for (int i = 0; i < jet->Constituents.GetEntriesFast(); ++i)
//     {
//         object = jet->Constituents.At(i);
//         //cout<<"Object "<<i<<"in Jet"<<endl;
//         if (object->IsA() == Track::Class())
//         {
//             //cout<<"Track: "<<endl;
//             track = (Track*)object;
//             int pid = track->PID;
//             if (fabs(pid) == 211)
//             {
//               if (track->PT>ptPic)
//               {
//                 ptPic = track->PT;
//                 picMax = track->P4();
//                 pidpic1 = pid;
//               }
//             }
//         }
//         if (object->IsA() == Tower::Class())
//         {
//             //cout<<"Tower: "<<endl;
//             tower = (Tower*)object;
//             int NinTower = tower->Particles.GetEntriesFast();
//             int nPhoton = 0;
//             for (int ntower = 0; ntower < NinTower; ++ntower)
//             {
//              particle=(GenParticle*)tower->Particles.At(ntower);
//              int pid = particle->PID;
//              if (pid==22)
//              {
//                nPhoton++;
//              }
//              else if (pid!=22 && particle->Charge==0)
//              {
//                  nPhoton=0;
//                  break;
//              }
//             }
//             if (nPhoton>=1)
//             {
//               TowerP4.push_back(tower->P4());
//             }
            
//         }
//     }
//     int NtowerCandi = TowerP4.size();
//     sort(TowerP4.begin(),TowerP4.end(),TLorentzVectorPTSort); //Sort according to Decreasing PT
//     if (NtowerCandi>=2)
//     {
//         pi0Candi = TowerP4[0] + TowerP4[1];
//     }
//     else if (NtowerCandi==1)
//     {
//         pi0Candi = TowerP4[0];
//     }

//     if (ptPic<0||NtowerCandi<1)
//     {
//         Pic = TLorentzVector::TLorentzVector(0,0,0,0);
//         Pi0 = TLorentzVector::TLorentzVector(0,0,0,0);
//         VisP = Pic + Pi0;
//         charge = 0;
//         return false;
//     }
//     Pic = picMax;
//     Pi0 = pi0Candi;
//     charge = pidpic1>0?1:-1;
//     VisP = Pic + Pi0;
//     return true;

// }

// bool TauJet::SetUpComponent(Jet *jet, int ComponentQ)
// {
//     if (!ComponentQ)
//     {
//         charge = jet->Charge;
//         VisP = jet->P4();
//         return true;
//     }
//     TLorentzVector picMax;
//     TLorentzVector photon1,photon2,photonbothV;
//     TObject *object;
//     Track *track;
//     Tower *tower;
//     GenParticle *particle;

//     int pidpic1;
//     double ptPic = -1;
//     double photonPT1 = -1;
//     double photonPT2 = -2;
//     double photonBothPT = -3;
//     int photonBothQ=0;
//     //cout<<"Constituents Number: "<<jet->Constituents.GetEntriesFast()<<endl;
//     for (int i = 0; i < jet->Constituents.GetEntriesFast(); ++i)
//     {
//         object = jet->Constituents.At(i);
//         //cout<<"Object "<<i<<"in Jet"<<endl;
//         if (object->IsA() == Track::Class())
//         {
//             //cout<<"Track: "<<endl;
//             track = (Track*)object;
//             int pid = track->PID;
//             if (fabs(pid) == 211)
//             {
//               if (track->PT>ptPic)
//               {
//                 ptPic = track->PT;
//                 picMax = track->P4();
//                 pidpic1 = pid;
//               }
//             }
//         }
//         if (object->IsA() == Tower::Class())
//         {
//             //cout<<"Tower: "<<endl;
//             tower = (Tower*)object;
//             int NinTower = tower->Particles.GetEntriesFast();
//             int nPhoton = 0;
//             vector<double> photonPTtemp;
//             for (int ntower = 0; ntower < NinTower; ++ntower)
//             {
//              particle=(GenParticle*)tower->Particles.At(ntower);
//              int pid = particle->PID;
//              if (pid==22)
//              {
//                nPhoton++;
//                photonPTtemp.push_back(particle->PT);
//              }
//             }
//             if (nPhoton==1)
//             {
//               double pttemp=tower->ET;
//               if (pttemp > photonPT1)
//               {
//                  photonPT2 = photonPT1;
//                  photon2 = photon1;
//                  photonPT1 = pttemp;
//                  photon1 = tower->P4();

//               }
//               else if (pttemp < photonPT1 && pttemp > photonPT2)
//               {
//                 photonPT2 = pttemp;
//                 photon2 = tower->P4();
//               }
//             }
//             else if (nPhoton>=2)
//             {
//              photonBothQ=1;
//              double pttemp=tower->ET;
//              if (pttemp > photonBothPT)
//              {
//                photonBothPT = pttemp;
//                photonbothV=tower->P4();
//              }
//             }
//         }
//     }

//     if (ptPic<0||((photonPT1<0||photonPT2<0)&&photonBothPT<0))
//     {
//         Pic = TLorentzVector::TLorentzVector(0,0,0,0);
//         Pi0 = TLorentzVector::TLorentzVector(0,0,0,0);
//         VisP = Pic + Pi0;
//         charge = 0;
//         return false;
//     }
//     Pic = picMax;
//     if (photonBothQ==1)
//     {
//         Pi0 = photonbothV;
//     }
//     else
//     {
//         Pi0 = photon1 + photon2;
//     }
//     charge = pidpic1>0?1:-1;
//     VisP = Pic + Pi0;
//     return true;

// }