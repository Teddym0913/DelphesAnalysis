#include "TROOT.h"
#include "TH1F.h"
#include "TreeReader.h"
//#include "BasicCuts.h"
//#include "EachEvent.h"
#include "CutSystem.h"
#include "Event_Self.h"
#include "Event_Variables.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TLatex.h"
#include "modules/Delphes.h"
#include "classes/DelphesClasses.h"
#include "classes/DelphesFactory.h"
//#include "AdvancedCuts.h"
#include <sstream>
using namespace std;

double fac[11] = {1.,1.,2.,6.,24.,120.,720.,5040.,40320.,362880.,3628800.};

// TLorentzVector BoostSelf(TLorentzVector v1, TLorentzVector v2) // Boost v1 to the rest frame of v2
// {
//   double beta = v2.Beta();
//   double gamma = v2.Gamma();

//   TVector3 unit = v2.Vect();
//   unit = (1.0/unit.Mag())*unit;

//   double Erest = gamma*(v1.Energy()-beta*(unit.Dot(v1.Vect())));
//   TVector3 prest = v1.Vect() + (gamma-1)*(unit.Dot(v1.Vect()))*unit - ( gamma * beta * v1.Energy())*unit;

//   return TLorentzVector(prest,Erest);

// }

double CPPhi(TauJet tau1, TauJet tau2)
{
    TLorentzVector pic1 = tau1.Pic;
    TLorentzVector pic2 = tau2.Pic;
    TLorentzVector pi01 = tau1.Pi0;
    TLorentzVector pi02 = tau2.Pi0;
    double selection = (pic1.Energy()-pi01.Energy())*(pic2.Energy()-pi02.Energy());
   TLorentzVector pictotal = pic1 + pic2;
   //boost = pictotal.BoostVector();
   pic1=BoostSelf(pic1,pictotal);
   pic2=BoostSelf(pic2,pictotal);
   pi01=BoostSelf(pi01,pictotal);
   pi02=BoostSelf(pi02,pictotal);

   //TLorentzVector check = pic1+pic2;
   //cout<<check.Px()<<" "<<check.Py()<<"  "<<check.Pz()<<endl;

   
   TVector3 z = pic1.Vect();
   TVector3 pi013 = pi01.Vect();
   TVector3 pi023 = pi02.Vect();

   TVector3 plane1 = (z.Cross(pi013)).Cross(z);
   TVector3 plane2 = (z.Cross(pi023)).Cross(z);

   double angle = acos(plane1.Dot(plane2)/(plane1.Mag()*plane2.Mag()));
   double sign = (plane1.Cross(plane2)).Dot(z);
   if (sign < 0)
   {
     angle = -angle;
   }
   if (selection < 0)
   {
      if (angle > 0)
      {
        angle = angle - TMath::Pi();
      }
      else
      {
        angle = angle + TMath::Pi();
      }
   }
   return angle;
}

int Analysis(string inputfile, string channel_name,TH1F *h2,int fakeQ,double sigma,int cate)
{
    BasicCuts cuts_self;
    AdvancedCuts cuts_ad;
    if (cate == 10)
    {
        cuts_self.ReadFile("./config/Cuts_card_VH.dat");
        cuts_ad.ReadFile("./config/Cuts_card_VH.dat");
        cout<<"Analysing VH topology."<<endl;
    }
    else if (cate == 20)
    {
        cuts_self.ReadFile("./config/Cuts_card_VBF_highPT.dat");
        cuts_ad.ReadFile("./config/Cuts_card_VBF_highPT.dat");
        cout<<"Analysing VBF High PT topology."<<endl;
    }
    else if (cate == 21)
    {
        cuts_self.ReadFile("./config/Cuts_card_VBF_lowPTtight.dat");
        cuts_ad.ReadFile("./config/Cuts_card_VBF_lowPTtight.dat");
        cout<<"Analysing VBF Low PT tight topology."<<endl;
    }
    else if (cate == 22)
    {
        cuts_self.ReadFile("./config/Cuts_card_VBF_lowPTloose.dat");
        cuts_ad.ReadFile("./config/Cuts_card_VBF_lowPTloose.dat");
        cout<<"Analysing VBF Low PT loose topology."<<endl;
    }
    else
    {
      cout<<"Can not recognize the ananlysis mode"<<endl;
      return -1;
    }
    
    TreeReader *reader = new TreeReader("./config/delphes_card.dat");
    TChain *chain = new TChain("Delphes");
    chain->Add(inputfile.c_str());
    reader->SetTreeChain(chain);
    Long64_t allEntries = reader->GetEntries();
    cout<<"Channel name: "<<channel_name<<endl;
    cout<<"Total Events: "<<allEntries<<endl;
    cout<<"Analysing ..."<<endl;
    Event_Self *eachEvent = new Event_Self(reader,fakeQ);
    Event_Variables *outEvent = new Event_Variables(eachEvent);
    // TFile *f1 = new TFile("test.root","recreate");
    TTree *t1 = new TTree((channel_name+"_Af_Basic").c_str(),"");
    t1->Branch("Event","Event_Variables",&outEvent);
    int good;
    int count=0;
    double weight;
    double fills;
    int Njets;
    TH1F *h1 = new TH1F(channel_name.c_str(),"",30,0,30);
    //TH1F *h2 = new TH1F("h2","",50,50,200);
    // int i = 8859;
    weight = sigma/((double)allEntries);
    weight = 1;
    for (int i = 0; i < allEntries; ++i)
    {
        //int i = 31;
        if((i+1)%10000==0){cout<<i+1<<"\r"; cout.flush();}
        //cout<<"No. "<<i<<endl;
        //int PassQ[10]={0,0,0,0,0,0,0,0,0,0};
        vector<int> PassQ;
        PassQ.clear();
        reader->GetEntry(i);
        //cout<<"good2"<<endl;
        eachEvent->UpdateEvent(cuts_self);
        // cout<<"good1"<<endl;
        good = eachEvent->BasicCutsOnEvent(cuts_self,PassQ);
        // cout<<"good?"<<endl;
        if (good)
        {
          outEvent->SetVariables();
          //t1->Fill();
          outEvent->SetCuts(cuts_ad,PassQ,cate);
        }
        for (int ipass = 0; ipass < PassQ.size(); ++ipass)
        {
          if (ipass>0)
          {
            PassQ[ipass]*=PassQ[ipass-1];
          }
          fills = PassQ[ipass]*weight;
          if (fakeQ > 0 && ipass>=5)
          {
            int njettotal = eachEvent->fJet->Ntotal;
            if (njettotal<fakeQ)
            {
              fills = 0;
            }
            else
            {
              fills = fills*pow(0.01,fakeQ)*pow(0.99,njettotal-fakeQ)*fac[njettotal]/(fac[njettotal-fakeQ]*fac[fakeQ]);
            }
          }
          if (good)
          {
            outEvent->PassQ = PassQ[PassQ.size()-1];
            t1->Fill();
          }
          h1->Fill(ipass+1.,fills);
        }
        h1->Fill(0.,weight);   
    }
    //p1->SetLineColor(i+1);
    h1->SetLineWidth(2);
    h1->SetYTitle("Event Counts");
    h1->SetTitleSize(0.05,"Y");
    h1->SetTitleOffset(1.1,"Y");
    t1->Write();
    h1->Write();
    cout<<channel_name<<" Done!"<<endl;
    //h2->Write();
    // f1->Close();
    //h1->Scale(sigma/(double)allEntries);
    //cout<<count<<endl;
	  return 0;
}

int main(int argc, char const *argv[])
{
  if (argc < 3)
  {
      cout<<"Usage:  "<<endl;
      cout<<argv[0]<<"  input_config  output_root_files"<<endl;
      return 0;
  }
  gStyle->SetOptStat("100000");
  //gStyle->SetOptTitle(0);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetStatBorderSize(1);
  gStyle->SetStatFont(42);
  gStyle->SetPadTopMargin(0.10);
  gStyle->SetPadRightMargin(0.14);
  gStyle->SetPadBottomMargin(0.16);
  gStyle->SetPadLeftMargin(0.18);
  
  TFile *f1 = new TFile(argv[2],"recreate");
  ifstream input(argv[1]);
  int channel=0;
  int cate;
  input >> channel >> cate;
  input.ignore(999,'\n');
  int fakeQ=0;
  double sigma;
  string channel_name;
  string channel_path;
  //vector<TH1F *> histo;
  TCanvas *c1 = new TCanvas("c1","",800,600);
  TLegend *legs = new TLegend(0.6206897,0.5318143,0.8778736,0.9023207,NULL,"brNDC");
  legs->SetBorderSize(1);
  legs->SetTextFont(32);
  legs->SetTextSize(0.05);
  legs->SetLineColor(0);
  legs->SetLineStyle(1);
  legs->SetLineWidth(1);
  legs->SetFillColor(0);
  legs->SetFillStyle(0);
  for (int i = 0; i < channel; ++i)
  {
      input>>fakeQ>>sigma>>channel_name>>channel_path;
      input.ignore(999,'\n');
      TH1F *p2 = new TH1F((channel_name+"_PhiCP").c_str(),"",2,-3.15,3.15);
      Analysis(channel_path,channel_name,p2,fakeQ,sigma,cate);
      p2->SetLineColor(i+1);
      p2->SetLineWidth(2);
      // p1->SetYTitle("Event Counts");
      // p1->SetTitleSize(0.05,"Y");
      // p1->SetTitleOffset(1.1,"Y");
      // f1->cd();
      // p1->Write();
      p2->Write();
      p2->Draw("SAME");
      legs->AddEntry(p2,channel_name.c_str(),"l");
  }
  legs->Draw();
  TLatex T1;
  T1.SetTextSize(0.04);
  T1.DrawLatex(0,100,"Including Detector Effects");
  T1.DrawLatex(0,0,"Unrenormalized According to #sigma");
  f1->cd();
  c1->Write();
  f1->Close();


  return 0;
}


