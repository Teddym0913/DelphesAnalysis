#include "CutSystem.h"

BasicCuts::BasicCuts():fJetCuts(),fLepCuts(),fVBFCuts()
{
    METmin = 0;
}

BasicCuts::BasicCuts(const char *file):fJetCuts(file),fLepCuts(file),fVBFCuts(file)
{
    ifstream infile(file,ios::in);
    string temp;
    while(infile)
    {
        infile>>temp;
        if (temp == "Basic")
        {
            infile.ignore(999,'\n');
            string name;
            while(infile)
            {
                infile>>name;
                if(name == "MET")
                {
                    infile>>METmin;
                    infile.ignore(999,'\n');
                }
                else if (name == "{" || name == "#" || name == "##")  
                {
                    infile.ignore(999,'\n');
                }
                else if (name =="}")
                {
                    infile.ignore(999,'\n');
                    break;
                }
                else
                {
                    infile.ignore(999,'\n');
                }
            }
        }
        else
        {
            infile.ignore(999,'\n');
        }
    }
    sprintf(inputfilename,"%s",file);
    //infile.close();

}

void BasicCuts::ReadFile(const char *file)
{
    Clear();
    fJetCuts.ReadFile(file);
    fLepCuts.ReadFile(file);
    fVBFCuts.ReadFile(file);
    ifstream infile(file,ios::in);
    string temp;
    while(infile)
    {
        infile>>temp;
        if (temp == "Basic")
        {
            infile.ignore(999,'\n');
            string name;
            while(infile)
            {
                infile>>name;
                if(name == "MET")
                {
                    infile>>METmin;
                    infile.ignore(999,'\n');
                }
                else if (name == "{" || name == "#" || name == "##")  
                {
                    infile.ignore(999,'\n');
                }
                else if (name =="}")
                {
                    infile.ignore(999,'\n');
                    break;
                }
                else
                {
                    infile.ignore(999,'\n');
                }
            }
        }
        else
        {
            infile.ignore(999,'\n');
        }
    }
    sprintf(inputfilename,"%s",file);
}

AdvancedCuts::AdvancedCuts()
{
    NCuts=0;
    PtSumTauMin=0;
    dRtautau[0] = -999;
    dRtautau[1] = 999;
    dEtatautau=999;
    PtHiggsMin=-999;
    METInTransQ = -9;
    EtaTauInVBFQ = -9;
}

void AdvancedCuts::Clear()
{
    NCuts=0;
    PtSumTauMin=0;
    dRtautau[0] = -999;
    dRtautau[1] = 999;
    dEtatautau=999;
    PtHiggsMin=-999;
    METInTransQ = -9;
    EtaTauInVBFQ = -9;
}

AdvancedCuts::AdvancedCuts(const char *file)
{
    ReadFile(file);
}

void AdvancedCuts::ReadFile(const char *file)
{
    Clear();
    ifstream infile(file,ios::in);
    string temp;
    while(infile)
    {
        infile>>temp;
        if (temp == "Advanced")
        {
            infile.ignore(999,'\n');
            string name;
            string signs;
            double value;
            int nflows;
            while(infile)
            {
                infile>>name;
                // if(name == "PtSumTauMin")
                // {
                //     infile>>PtSumTauMin;
                //     infile.ignore(999,'\n');
                // }
                // else if (name == "dRtautau")
                // {
                //     infile>>dRtautau[0]>>dRtautau[1];
                //     infile.ignore(999,'\n');
                // }
                // else if (name == "dEtatautau")
                // {
                //     infile>>dEtatautau;
                //     infile.ignore(999,'\n');
                // }
                // else if (name == "PtHiggsMin")
                // {
                //     infile>>PtHiggsMin;
                //     infile.ignore(999,'\n');
                // }
                // else if (name == "METInTransQ")
                // {
                //     infile>>METInTransQ;
                //     infile.ignore(999,'\n');
                // }
                // else if (name == "EtaTauInVBFQ")
                // {
                //     infile>>EtaTauInVBFQ;
                //     infile.ignore(999,'\n');
                // }
                // else if (name == "TightorLooseQ")
                // {
                //     infile>>TightorLooseQ;
                //     infile.ignore(999,'\n');
                // }
                // else 
                if (name == "NCuts")
                {
                    infile>>NCuts;
                    infile.ignore(999,'\n');
                }
                else if (name == "{" || name == "#" || name == "##")  
                {
                    infile.ignore(999,'\n');
                }
                else if (name =="}")
                {
                    infile.ignore(999,'\n');
                    break;
                }
                else
                {
                    infile>>nflows;
                    for (int iflows = 0; iflows < nflows; ++iflows)
                    {
                        infile>>signs>>value;
                        AdCuts.insert(make_pair(make_pair(name,signs),value));
                    }
                    infile.ignore(999,'\n');
                }
            }
        }
        else
        {
            infile.ignore(999,'\n');
        }
    }
}

string AdvancedCuts::GetAllCuts()
{
    stringstream selections;
    string cuts;
    CutsCollection::iterator ite;
    selections<<"Weight*(";
    for (ite = AdCuts.begin(); ite != AdCuts.end(); ++ite)
    {
        selections<<ite->first.first<<ite->first.second<<ite->second<<"&&";
    }
    selections<<"1==1)";
    selections>>cuts;
    return cuts;
}

int BasicCuts::Output(const char *file)
{
    char command[300];
    sprintf(command,"cat %s >> %s",inputfilename,file);
    int status=system(command);
    return status;
}

JetCuts::JetCuts():Cuts()
{
    NofBJets[0]=0;
    NofBJets[1]=-1;

    NofTauJets[0]=0;
    NofTauJets[1]=-1;

    NofNormalJets[0]=0;
    NofNormalJets[1]=-1;

    dPhiJMET[0] = 0;
    dPhiJMET[1] = 0;

    TauComponentQ = 0;

    LeadingPTBJets.clear();
    LeadingPTTauJets.clear();
    LeadingPTNormalJets.clear();

}

void JetCuts::Clear()
{
    NofBJets[0]=0;
    NofBJets[1]=-1;

    NofTauJets[0]=0;
    NofTauJets[1]=-1;

    NofNormalJets[0]=0;
    NofNormalJets[1]=-1;

    dPhiJMET[0] = 0;
    dPhiJMET[1] = 0;

    TauComponentQ = 0;

    LeadingPTBJets.clear();
    LeadingPTTauJets.clear();
    LeadingPTNormalJets.clear();

    PTmin=-9;
    for (int i = 0; i < 4; ++i)
    {
        Eta[i] = -9;
    }
    dRSelf=-9;
    for (int i = 0; i < 2; ++i)
    {
        NofObject[i]=-9;
    }
    LeadingPTmin.clear();
}

JetCuts::JetCuts(const char *file)
{
    ifstream infile(file,ios::in);
    string temp;
    while(infile)
    {
        infile>>temp;
        if(temp == "Basic")
        {
            infile.ignore(999,'\n');
            string name,cuts;
            while(infile)
            {
                infile >> name;
                if (name == "Jets")
                {
                    infile.ignore(999,'\n');
                    while(infile)
                    {
                        infile >> cuts;
                        if (cuts == "{{" || cuts == "#" || cuts == "##")
                        {
                            infile.ignore(999,'\n');
                        }
                        else if (cuts == "}}")
                        {
                            infile.ignore(999,'\n');
                            break;
                        }
                        else
                        {
                            if (cuts == "NofObject")
                            {
                                infile >> NofObject[0] >> NofObject[1];
                                infile.ignore(999,'\n');
                            }
                            else if (cuts == "NofBJets")
                            {
                                infile >> NofBJets[0] >> NofBJets[1];
                                infile.ignore(999,'\n');
                            }
                            else if (cuts == "NofTauJets")
                            {
                                infile >> NofTauJets[0] >> NofTauJets[1];
                                infile.ignore(999,'\n');
                            }
                            else if (cuts == "NofNormalJets")
                            {
                                infile >> NofNormalJets[0] >> NofNormalJets[1];
                                infile.ignore(999,'\n');
                            }
                            else if (cuts == "PTmin")
                            {
                                infile >> PTmin;
                                infile.ignore(999,'\n');
                            }
                            else if (cuts == "Eta")
                            {
                                infile >> Eta[0] >> Eta[1] >> Eta[2] >> Eta[3];
                                infile.ignore(999,'\n');
                            }
                            else if (cuts == "LeadingPT")
                            {
                                int nfollowing;
                                double leadPT; 
                                infile >> nfollowing;
                                for (int ifollow = 0; ifollow < nfollowing; ++ifollow)
                                {
                                    infile >> leadPT;
                                    LeadingPTmin.push_back(leadPT);
                                }
                                infile.ignore(999,'\n');
                            }
                            else if (cuts == "LeadingBPT")
                            {
                                int nfollowing;
                                double leadPT; 
                                infile >> nfollowing;
                                for (int ifollow = 0; ifollow < nfollowing; ++ifollow)
                                {
                                    infile >> leadPT;
                                    LeadingPTBJets.push_back(leadPT);
                                }
                                infile.ignore(999,'\n');
                            }
                            else if (cuts == "LeadingTauPT")
                            {
                                int nfollowing;
                                double leadPT; 
                                infile >> nfollowing;
                                for (int ifollow = 0; ifollow < nfollowing; ++ifollow)
                                {
                                    infile >> leadPT;
                                    LeadingPTTauJets.push_back(leadPT);
                                }
                                infile.ignore(999,'\n');
                            }
                            else if (cuts == "LeadingNormalPT")
                            {
                                int nfollowing;
                                double leadPT; 
                                infile >> nfollowing;
                                for (int ifollow = 0; ifollow < nfollowing; ++ifollow)
                                {
                                    infile >> leadPT;
                                    LeadingPTNormalJets.push_back(leadPT);
                                }
                                infile.ignore(999,'\n');
                            }
                            else if (cuts == "dRJJ")
                            {
                                infile >> dRSelf;
                                infile.ignore(999,'\n');
                            }
                            else if (cuts == "dPhiJMET")
                            {
                                infile >> dPhiJMET[0] >> dPhiJMET[1];
                                infile.ignore(999,'\n');
                            }
                            else if (cuts == "TauComponentQ")
                            {
                                infile >> TauComponentQ >> PhiGranu >> EtaGranu;
                                infile.ignore(999,'\n');
                            }
                            else
                            {
                                cout<<"Warning: Some Cuts We don't Recognise."<<endl;
                                infile.ignore(999,'\n');
                            }

                        }
                    }
                }
                else if (name=="{" || name=="#" || name=="##")
                {
                    infile.ignore(999,'\n');
                }
                else if (name=="}")
                {
                    infile.ignore(999,'\n');
                    break;
                }
                else
                {
                    infile.ignore(999,'\n');
                }
            }
        }
        else
        {
            infile.ignore(999,'\n');
        }
    }
}

void JetCuts::ReadFile(const char *file)
{
    Clear();
    ifstream infile(file,ios::in);
    string temp;
    while(infile)
    {
        infile>>temp;
        if(temp == "Basic")
        {
            infile.ignore(999,'\n');
            string name,cuts;
            while(infile)
            {
                infile >> name;
                if (name == "Jets")
                {
                    infile.ignore(999,'\n');
                    while(infile)
                    {
                        infile >> cuts;
                        if (cuts == "{{" || cuts == "#" || cuts == "##")
                        {
                            infile.ignore(999,'\n');
                        }
                        else if (cuts == "}}")
                        {
                            infile.ignore(999,'\n');
                            break;
                        }
                        else
                        {
                            if (cuts == "NofObject")
                            {
                                infile >> NofObject[0] >> NofObject[1];
                                infile.ignore(999,'\n');
                            }
                            else if (cuts == "NofBJets")
                            {
                                infile >> NofBJets[0] >> NofBJets[1];
                                infile.ignore(999,'\n');
                            }
                            else if (cuts == "NofTauJets")
                            {
                                infile >> NofTauJets[0] >> NofTauJets[1];
                                infile.ignore(999,'\n');
                            }
                            else if (cuts == "NofNormalJets")
                            {
                                infile >> NofNormalJets[0] >> NofNormalJets[1];
                                infile.ignore(999,'\n');
                            }
                            else if (cuts == "PTmin")
                            {
                                infile >> PTmin;
                                infile.ignore(999,'\n');
                            }
                            else if (cuts == "Eta")
                            {
                                infile >> Eta[0] >> Eta[1] >> Eta[2] >> Eta[3];
                                infile.ignore(999,'\n');
                            }
                            else if (cuts == "LeadingPT")
                            {
                                int nfollowing;
                                double leadPT; 
                                infile >> nfollowing;
                                for (int ifollow = 0; ifollow < nfollowing; ++ifollow)
                                {
                                    infile >> leadPT;
                                    LeadingPTmin.push_back(leadPT);
                                }
                                infile.ignore(999,'\n');
                            }
                            else if (cuts == "LeadingBPT")
                            {
                                int nfollowing;
                                double leadPT; 
                                infile >> nfollowing;
                                for (int ifollow = 0; ifollow < nfollowing; ++ifollow)
                                {
                                    infile >> leadPT;
                                    LeadingPTBJets.push_back(leadPT);
                                }
                                infile.ignore(999,'\n');
                            }
                            else if (cuts == "LeadingTauPT")
                            {
                                int nfollowing;
                                double leadPT; 
                                infile >> nfollowing;
                                for (int ifollow = 0; ifollow < nfollowing; ++ifollow)
                                {
                                    infile >> leadPT;
                                    LeadingPTTauJets.push_back(leadPT);
                                }
                                infile.ignore(999,'\n');
                            }
                            else if (cuts == "LeadingNormalPT")
                            {
                                int nfollowing;
                                double leadPT; 
                                infile >> nfollowing;
                                for (int ifollow = 0; ifollow < nfollowing; ++ifollow)
                                {
                                    infile >> leadPT;
                                    LeadingPTNormalJets.push_back(leadPT);
                                }
                                infile.ignore(999,'\n');
                            }
                            else if (cuts == "dRJJ")
                            {
                                infile >> dRSelf;
                                infile.ignore(999,'\n');
                            }
                            else if (cuts == "dPhiJMET")
                            {
                                infile >> dPhiJMET[0] >> dPhiJMET[1];
                                infile.ignore(999,'\n');
                            }
                            else if (cuts == "TauComponentQ")
                            {
                                infile >> TauComponentQ >> PhiGranu >> EtaGranu;
                                infile.ignore(999,'\n');
                            }
                            else
                            {
                                cout<<"Warning: Some Cuts We don't Recognise."<<endl;
                                infile.ignore(999,'\n');
                            }

                        }
                    }
                }
                else if (name=="{" || name=="#" || name=="##")
                {
                    infile.ignore(999,'\n');
                }
                else if (name=="}")
                {
                    infile.ignore(999,'\n');
                    break;
                }
                else
                {
                    infile.ignore(999,'\n');
                }
            }
        }
        else
        {
            infile.ignore(999,'\n');
        }
    }
}

LepCuts::LepCuts():Cuts()
{
    dRwithJet = 0;
    NofZBoson = -1;
    MinvMin = 0;
    MinvMax = 999;
}

void LepCuts::Clear()
{
    dRwithJet = 0;
    NofZBoson = -1;
    MinvMin = 0;
    MinvMax = 999;

    PTmin=-9;
    for (int i = 0; i < 4; ++i)
    {
        Eta[i] = -9;
    }
    dRSelf=-9;
    for (int i = 0; i < 2; ++i)
    {
        NofObject[i]=-9;
    }
    LeadingPTmin.clear();

}

LepCuts::LepCuts(const char *file)
{
    ifstream infile(file,ios::in);
    string temp;
    while(infile)
    {
        infile>>temp;
        if(temp == "Basic")
        {
            infile.ignore(999,'\n');
            string name,cuts;
            while(infile)
            {
                infile >> name;
                if (name == "Leptons")
                {
                    infile.ignore(999,'\n');
                    while(infile)
                    {
                        infile >> cuts;
                        if (cuts == "{{" || cuts == "#" || cuts == "##")
                        {
                            infile.ignore(999,'\n');
                        }
                        else if (cuts == "}}")
                        {
                            infile.ignore(999,'\n');
                            break;
                        }
                        else
                        {
                            if (cuts == "NofObject")
                            {
                                infile >> NofObject[0] >> NofObject[1];
                                infile.ignore(999,'\n');
                            }
                            else if (cuts == "NofZBoson")
                            {
                                infile >> NofZBoson >> MinvMin >> MinvMax;
                                infile.ignore(999,'\n');
                            }
                            else if (cuts == "PTmin")
                            {
                                infile >> PTmin;
                                infile.ignore(999,'\n');
                            }
                            else if (cuts == "Eta")
                            {
                                infile >> Eta[0] >> Eta[1] >> Eta[2] >> Eta[3];
                                infile.ignore(999,'\n');
                            }
                            else if (cuts == "LeadingPT")
                            {
                                int nfollowing;
                                double leadPT; 
                                infile >> nfollowing;
                                for (int ifollow = 0; ifollow < nfollowing; ++ifollow)
                                {
                                    infile >> leadPT;
                                    LeadingPTmin.push_back(leadPT);
                                }
                                infile.ignore(999,'\n');
                            }
                            else if (cuts == "dRLL")
                            {
                                infile >> dRSelf;
                                infile.ignore(999,'\n');
                            }
                            else if (cuts == "dRLJ")
                            {
                                infile >> dRwithJet;
                                infile.ignore(999,'\n');
                            }
                            else
                            {
                                cout<<"Warning: Some Cuts We don't Recognise."<<endl;
                                infile.ignore(999,'\n');
                            }

                        }
                    }
                }
                else if (name=="{" || name=="#" || name=="##")
                {
                    infile.ignore(999,'\n');
                }
                else if (name=="}")
                {
                    infile.ignore(999,'\n');
                    break;
                }
                else
                {
                    infile.ignore(999,'\n');
                }
            }
        }
        else
        {
            infile.ignore(999,'\n');
        }
    }
}

void LepCuts::ReadFile(const char *file)
{
    Clear();
    ifstream infile(file,ios::in);
    string temp;
    while(infile)
    {
        infile>>temp;
        if(temp == "Basic")
        {
            infile.ignore(999,'\n');
            string name,cuts;
            while(infile)
            {
                infile >> name;
                if (name == "Leptons")
                {
                    infile.ignore(999,'\n');
                    while(infile)
                    {
                        infile >> cuts;
                        if (cuts == "{{" || cuts == "#" || cuts == "##")
                        {
                            infile.ignore(999,'\n');
                        }
                        else if (cuts == "}}")
                        {
                            infile.ignore(999,'\n');
                            break;
                        }
                        else
                        {
                            if (cuts == "NofObject")
                            {
                                infile >> NofObject[0] >> NofObject[1];
                                infile.ignore(999,'\n');
                            }
                            else if (cuts == "NofZBoson")
                            {
                                infile >> NofZBoson >> MinvMin >> MinvMax;
                                infile.ignore(999,'\n');
                            }
                            else if (cuts == "PTmin")
                            {
                                infile >> PTmin;
                                infile.ignore(999,'\n');
                            }
                            else if (cuts == "Eta")
                            {
                                infile >> Eta[0] >> Eta[1] >> Eta[2] >> Eta[3];
                                infile.ignore(999,'\n');
                            }
                            else if (cuts == "LeadingPT")
                            {
                                int nfollowing;
                                double leadPT; 
                                infile >> nfollowing;
                                for (int ifollow = 0; ifollow < nfollowing; ++ifollow)
                                {
                                    infile >> leadPT;
                                    LeadingPTmin.push_back(leadPT);
                                }
                                infile.ignore(999,'\n');
                            }
                            else if (cuts == "dRLL")
                            {
                                infile >> dRSelf;
                                infile.ignore(999,'\n');
                            }
                            else if (cuts == "dRLJ")
                            {
                                infile >> dRwithJet;
                                infile.ignore(999,'\n');
                            }
                            else
                            {
                                cout<<"Warning: Some Cuts We don't Recognise."<<endl;
                                infile.ignore(999,'\n');
                            }

                        }
                    }
                }
                else if (name=="{" || name=="#" || name=="##")
                {
                    infile.ignore(999,'\n');
                }
                else if (name=="}")
                {
                    infile.ignore(999,'\n');
                    break;
                }
                else
                {
                    infile.ignore(999,'\n');
                }
            }
        }
        else
        {
            infile.ignore(999,'\n');
        }
    }
}

VBFCuts::VBFCuts()
{
    NofForwardObj = 0;
    PTofForwardObj.clear();
    deltaEta = 0;
    mjjmin = 0;
    VBFQ = 0;
}

void VBFCuts::Clear()
{
    NofForwardObj = 0;
    PTofForwardObj.clear();
    deltaEta = 0;
    mjjmin = 0;
    VBFQ = 0;
}

VBFCuts::VBFCuts(const char *file)
{
    ifstream infile(file,ios::in);
    string temp;
    while(infile)
    {
        infile>>temp;
        if(temp == "Basic")
        {
            infile.ignore(999,'\n');
            string name,cuts;
            while(infile)
            {
                infile >> name;
                if (name == "VBF")
                {
                    infile.ignore(999,'\n');
                    while(infile)
                    {
                        infile >> cuts;
                        if (cuts == "{{" || cuts == "#" || cuts == "##")
                        {
                            infile.ignore(999,'\n');
                        }
                        else if (cuts == "}}")
                        {
                            infile.ignore(999,'\n');
                            break;
                        }
                        else
                        {
                            if (cuts == "NofForwardObj")
                            {
                                infile >> NofForwardObj;
                                NofObject[0]=NofForwardObj;
                                NofObject[1]=NofForwardObj;
                                infile.ignore(999,'\n');
                            }
                            else if (cuts == "deltaEta")
                            {
                                infile >> deltaEta;
                                infile.ignore(999,'\n');
                            }
                            else if (cuts == "PTofForwardObj")
                            {
                                int nfollowing;
                                double leadPT; 
                                infile >> nfollowing;
                                for (int ifollow = 0; ifollow < nfollowing; ++ifollow)
                                {
                                    infile >> leadPT;
                                    PTofForwardObj.push_back(leadPT);
                                }
                                infile.ignore(999,'\n');
                            }
                            else if (cuts == "mjjmin")
                            {
                                infile >> mjjmin;
                                infile.ignore(999,'\n');
                            }
                            else if (cuts == "ApplyQ")
                            {
                                infile >> VBFQ;
                                infile.ignore(999,'\n');
                            }
                            else
                            {
                                cout<<"Warning: Some Cuts We don't Recognise."<<endl;
                                infile.ignore(999,'\n');
                            }

                        }
                    }
                }
                else if (name=="{" || name=="#" || name=="##")
                {
                    infile.ignore(999,'\n');
                }
                else if (name=="}")
                {
                    infile.ignore(999,'\n');
                    break;
                }
                else
                {
                    infile.ignore(999,'\n');
                }
            }
        }
        else
        {
            infile.ignore(999,'\n');
        }
    }

}

void VBFCuts::ReadFile(const char *file)
{
    ifstream infile(file,ios::in);
    string temp;
    while(infile)
    {
        infile>>temp;
        if(temp == "Basic")
        {
            infile.ignore(999,'\n');
            string name,cuts;
            while(infile)
            {
                infile >> name;
                if (name == "VBF")
                {
                    infile.ignore(999,'\n');
                    while(infile)
                    {
                        infile >> cuts;
                        if (cuts == "{{" || cuts == "#" || cuts == "##")
                        {
                            infile.ignore(999,'\n');
                        }
                        else if (cuts == "}}")
                        {
                            infile.ignore(999,'\n');
                            break;
                        }
                        else
                        {
                            if (cuts == "NofForwardObj")
                            {
                                infile >> NofForwardObj;
                                NofObject[0]=NofForwardObj;
                                NofObject[1]=NofForwardObj;
                                infile.ignore(999,'\n');
                            }
                            else if (cuts == "deltaEta")
                            {
                                infile >> deltaEta;
                                infile.ignore(999,'\n');
                            }
                            else if (cuts == "PTofForwardObj")
                            {
                                int nfollowing;
                                double leadPT; 
                                infile >> nfollowing;
                                for (int ifollow = 0; ifollow < nfollowing; ++ifollow)
                                {
                                    infile >> leadPT;
                                    PTofForwardObj.push_back(leadPT);
                                }
                                infile.ignore(999,'\n');
                            }
                            else if (cuts == "mjjmin")
                            {
                                infile >> mjjmin;
                                infile.ignore(999,'\n');
                            }
                            else if (cuts == "ApplyQ")
                            {
                                infile >> VBFQ;
                                infile.ignore(999,'\n');
                            }
                            else
                            {
                                cout<<"Warning: Some Cuts We don't Recognise."<<endl;
                                infile.ignore(999,'\n');
                            }

                        }
                    }
                }
                else if (name=="{" || name=="#" || name=="##")
                {
                    infile.ignore(999,'\n');
                }
                else if (name=="}")
                {
                    infile.ignore(999,'\n');
                    break;
                }
                else
                {
                    infile.ignore(999,'\n');
                }
            }
        }
        else
        {
            infile.ignore(999,'\n');
        }
    }
}

Cuts::Cuts()
{
    PTmin=-9;
    for (int i = 0; i < 4; ++i)
    {
        Eta[i] = -9;
    }
    dRSelf=-9;
    for (int i = 0; i < 2; ++i)
    {
        NofObject[i]=-9;
    }
    LeadingPTmin.clear();
}

void Cuts::Clear()
{
    PTmin=-9;
    for (int i = 0; i < 4; ++i)
    {
        Eta[i] = -9;
    }
    dRSelf=-9;
    for (int i = 0; i < 2; ++i)
    {
        NofObject[i]=-9;
    }
    LeadingPTmin.clear();
}
