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
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
//#include "AdvancedCuts.h"
#include <sstream>
using namespace std;

double fac[11] = {1.,1.,2.,6.,24.,120.,720.,5040.,40320.,362880.,3628800.};
void createdir(char *dir);
int Basic_Analysis(string inputfile, string channel_name,TH1F *p2,int fakeQ,double sigma,int cate, char *Log_Dir)
{
    BasicCuts cuts_self;
    string CutsFile;
    //AdvancedCuts cuts_ad;
    if (cate == 10)
    {
        //cuts_self.ReadFile("./config/Cuts_card_VH.dat");
        CutsFile="./config/Cuts_card_VH.dat";
        //cuts_ad.ReadFile("./config/Cuts_card_VH.dat");
        cout<<"Analysing VH topology."<<endl;
    }
    else if (cate == 20)
    {
        //cuts_self.ReadFile("./config/Cuts_card_VBF_highPT.dat");
        CutsFile="./config/Cuts_card_VBF_highPT.dat";
        //cuts_ad.ReadFile("./config/Cuts_card_VBF_highPT.dat");
        cout<<"Analysing VBF High PT topology."<<endl;
    }
    else if (cate == 21)
    {
        //cuts_self.ReadFile("./config/Cuts_card_VBF_lowPTtight.dat");
        CutsFile="./config/Cuts_card_VBF_lowPTtight.dat";
        //cuts_ad.ReadFile("./config/Cuts_card_VBF_lowPTtight.dat");
        cout<<"Analysing VBF Low PT tight topology."<<endl;
    }
    else if (cate == 22)
    {
        //cuts_self.ReadFile("./config/Cuts_card_VBF_lowPTloose.dat");
        CutsFile="./config/Cuts_card_VBF_lowPTloose.dat";
        //cuts_ad.ReadFile("./config/Cuts_card_VBF_lowPTloose.dat");
        cout<<"Analysing VBF Low PT loose topology."<<endl;
    }
    else
    {
      cout<<"Can not recognize the ananlysis mode"<<endl;
      return -1;
    }
    cuts_self.ReadFile(CutsFile.c_str());
    char command_basic[500];
    sprintf(command_basic,"cp %s %s/BasicCuts_for_%s.dat",CutsFile.c_str(),Log_Dir,channel_name.c_str());
    int status = system(command_basic);

    
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
    double weight_cs;
    double fills_ge,fills_cs;
    int Njets;
    TH1F *h1 = new TH1F((channel_name+"_GE").c_str(),"Generated Events",30,0,30);
    TH1F *h2 = new TH1F((channel_name+"_CS").c_str(),"Cross Section",30,0,30);
    // int i = 8859;
    weight_cs = sigma/((double)allEntries);
    //weight = 1;
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
          outEvent->Weight = weight_cs;
          outEvent->PassQ = -1;
          t1->Fill();
          //outEvent->SetCuts(cuts_ad,PassQ,cate);
        }
        for (int ipass = 0; ipass < PassQ.size(); ++ipass)
        {
          if (ipass>0)
          {
            PassQ[ipass]*=PassQ[ipass-1];
          }
          fills_ge = PassQ[ipass];
          fills_cs = fills_ge * weight_cs;
          if (fakeQ > 0 && ipass>=5)
          {
            int njettotal = eachEvent->fJet->Ntotal;
            if (njettotal<fakeQ)
            {
              fills_ge = 0;
              fills_cs = 0;
            }
            else
            {
              fills_ge = fills_ge*pow(0.01,fakeQ)*pow(0.99,njettotal-fakeQ)*fac[njettotal]/(fac[njettotal-fakeQ]*fac[fakeQ]);
              fills_cs = fills_ge*weight_cs;
            }
          }
          h1->Fill(ipass+1.,fills_ge);
          h2->Fill(ipass+1.,fills_cs);
        }
        h1->Fill(0.,1.);
        h2->Fill(0.,weight_cs);   
    }
    //p1->SetLineColor(i+1);
    h1->SetLineWidth(2);
    h1->SetYTitle("Generated Event Counts");
    h1->SetTitleSize(0.05,"Y");
    h1->SetTitleOffset(1.1,"Y");
    h2->SetLineWidth(2);
    h2->SetYTitle("Cross Section [fb]");
    h2->SetTitleSize(0.05,"Y");
    h2->SetTitleOffset(1.1,"Y");
    t1->Write();
    h1->Write();
    h2->Write();
    cout<<channel_name<<" Basic Analysis Done!"<<endl;
    //h2->Write();
    // f1->Close();
    //h1->Scale(sigma/(double)allEntries);
    //cout<<count<<endl;
	  return 0;
}

double Advanced_Analysis(string inputfile, string channel_name,int cate, char *Log_Dir)
{
    AdvancedCuts cuts_ad;
    string CutsFile;
    if (cate == 10)
    {
        // cuts_self.ReadFile("./config/Cuts_card_VH.dat");
        // cuts_ad.ReadFile("./config/Cuts_card_VH.dat");
        CutsFile = "./config/Cuts_card_VH.dat";
        //cout<<"Analysing VH topology."<<endl;
    }
    else if (cate == 20)
    {
        // cuts_self.ReadFile("./config/Cuts_card_VBF_highPT.dat");
        // cuts_ad.ReadFile("./config/Cuts_card_VBF_highPT.dat");
        CutsFile = "./config/Cuts_card_VBF_highPT.dat";
        //cout<<"Analysing VBF High PT topology."<<endl;
    }
    else if (cate == 21)
    {
        // cuts_self.ReadFile("./config/Cuts_card_VBF_lowPTtight.dat");
        // cuts_ad.ReadFile("./config/Cuts_card_VBF_lowPTtight.dat");
        CutsFile = "./config/Cuts_card_VBF_lowPTtight.dat";
        //cout<<"Analysing VBF Low PT tight topology."<<endl;
    }
    else if (cate == 22)
    {
        // cuts_self.ReadFile("./config/Cuts_card_VBF_lowPTloose.dat");
        // cuts_ad.ReadFile("./config/Cuts_card_VBF_lowPTloose.dat");
        CutsFile = "./config/Cuts_card_VBF_lowPTloose.dat";
        //cout<<"Analysing VBF Low PT loose topology."<<endl;
    }
    else
    {
      cout<<"Can not recognize the ananlysis mode"<<endl;
      return -1;
    }
    cuts_ad.ReadFile(CutsFile.c_str());
    char command_advanced[500];
    sprintf(command_advanced,"cp %s %s/AdvancedCuts_for_%s.dat",CutsFile.c_str(),Log_Dir,channel_name.c_str());
    int status = system(command_advanced);
    TFile *f1 = new TFile(inputfile.c_str());
    TTree *t1 = (TTree*) f1->Get((channel_name+"_Af_Basic").c_str());
    Event_Variables *event = new Event_Variables();
    t1->SetBranchAddress("Event",&event);
    //stringstream selections;
    string cuts = cuts_ad.GetAllCuts();
    //AdvancedCuts::CutsCollection::iterator iteAd;
    //selections<<"Weight*(";
    //for (iteAd = cuts_ad.AdCuts.begin(); iteAd != cuts_ad.AdCuts.end(); ++iteAd)
    // {
    //    selections<<iteAd->first.first<<iteAd->first.second<<iteAd->second<<"&&";
    // }
    // selections<<"1==1)";
    // selections>>cuts;
    // cout<<"Cuts are: "<<cuts<<endl;
    //return 0;
    TCanvas *c1 = new TCanvas("c1","",800,600);
    t1->Draw("PassQ>>Count(12,-2,10)",cuts.c_str());
    gPad->Update();
    TH1F *htemp = (TH1F*) gPad->GetPrimitive("Count");
    double cs_left = htemp->Integral();

    delete c1;
    return cs_left;
}

int main(int argc, char const *argv[])
{
  if (argc < 4)
  {
      cout<<"Usage:  "<<endl;
      cout<<argv[0]<<"  Basic_or_Advanced  input_config  output_dir"<<endl;
      cout<<argv[0]<<"  0  input_config  Basic_output_dir"<<endl;
      cout<<argv[0]<<"  1  input_config  Advanced_output_dir"<<endl;
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


  int BorA = atoi(argv[1]);
  if (BorA==0)
  {
    
    ifstream input(argv[2]);
    int channel=0;
    int cate;
    string AnalysisName;
    input >> channel >> cate >> AnalysisName;
    input.ignore(999,'\n');
    char basic_output[500];
    sprintf(basic_output,"%s/%s_af_basic.root",argv[3],AnalysisName.c_str());
    TFile *f1 = new TFile(basic_output,"recreate");
    char Log_Dir[500];
    sprintf(Log_Dir,"%s/%s_Basic_Logs",argv[3],AnalysisName.c_str());
    createdir(Log_Dir);
    char command_basic[500];
    // sprintf(command_basic,"mkdir %s",Log_Dir);
    // system(command_basic);
    sprintf(command_basic,"cp %s %s/input_config_basic", argv[2], Log_Dir);
    int status = system(command_basic);
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
      Basic_Analysis(channel_path,channel_name,p2,fakeQ,sigma,cate,Log_Dir);
      p2->SetLineColor(i+1);
      p2->SetLineWidth(2);
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
  else if (BorA == 1)
  {
    ifstream input(argv[2]);
    int channel=0;
    int cate;
    string AnalysisName;
    input >> channel >> cate >> AnalysisName;
    input.ignore(999,'\n');
    char Advanced_output[500];
    sprintf(Advanced_output,"%s/%s_Af_Advanced.dat",argv[3],AnalysisName.c_str());
    ofstream output(Advanced_output);
    char Log_Dir[500];
    sprintf(Log_Dir,"%s/%s_Advanced_Logs",argv[3],AnalysisName.c_str());
    createdir(Log_Dir);
    char command_advanced[500];
    // sprintf(command_advanced,"mkdir %s",Log_Dir);
    // system(command_advanced);
    sprintf(command_advanced,"cp %s %s/input_config_advanced",argv[2],Log_Dir);
    int status = system(command_advanced);
    int fakeQ=0;
    double sigma;
    string channel_name;
    string channel_path;
    double cs_left;
    for (int i = 0; i < channel; ++i)
    {
      input>>fakeQ>>sigma>>channel_name>>channel_path;
      input.ignore(999,'\n');
      //TH1F *p2 = new TH1F((channel_name+"_PhiCP").c_str(),"",2,-3.15,3.15);
      cs_left=Advanced_Analysis(channel_path,channel_name,cate,Log_Dir);
      output<<channel_name<<"  CS_Left[fb]: "<<cs_left<<endl;
    }
    return 0;
  }
  else
  {
      cout<<"Usage:  "<<endl;
      cout<<argv[0]<<"  Basic_or_Advanced  input_config  output_files"<<endl;
      cout<<argv[0]<<"  0  input_config  Basic_output_root"<<endl;
      cout<<argv[0]<<"  1  input_config  Advanced_output_dat"<<endl;
      return 0;
  }
  
}

void createdir(char *dir)
{
    int exist;
    exist=access(dir,NULL);
    if (exist==0) return;
    mkdir(dir,S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH);//Mode: 775
}

