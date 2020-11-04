#include "cstdlib"
#include "iostream"
#include "TFile.h"
#include "cstring"
#include "TLegend.h"
#include "TGraph.h"
//#include "TROOT.h"
#include "TH3.h"
#include "TH2.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TMath.h"
#include "TText.h"
#include "TPaveText.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "TAttFill.h"
#include "TColor.h"
#include "string"

using namespace std;

int countLines(char *filelist);
ifstream& goToLine(ifstream& file, unsigned int num);


void SLOSS()
{
  
  ifstream input_pp;
  input_pp.open("PPSpecificData");
  int nLinesPP = countLines("PPSpecificData") - 1;
  const int nPPoints = countLines("PPSpecificData");
  cout << "nPPoints: " << nPPoints << endl;
  float px[nPPoints];
  float py[nPPoints];
  float ppx[nPPoints];
  float perr_x[nPPoints];
  float perr_y[nPPoints];

  float Sloss [nPPoints];

  ifstream input_pbpb;
  input_pbpb.open("PbPbSpecificData");
  int nLinesPbPb = countLines("PbPbSpecificData") - 1;
  const int nPbPboints = countLines("PbPbSpecificData");
  cout << "nPbPboints: " << nPbPboints << endl;
  float pbx[nPbPboints];
  float pby[nPbPboints];
  float pberr_x[nPbPboints];
  float pberr_y[nPbPboints];

  
  gStyle -> SetOptStat(1110);

  for(int i = 0; i < nPPoints ; i++)
    {
      goToLine(input_pp, i);
      string spx,spy,sperr_x,sperr_y,a,b;
      input_pp >> spx >> a >> b >> spy >> sperr_y ;
      cout << "sx: " << spx << " b: " << b << " a: " << a << " sy: " << spy << " serr_y: " << sperr_y << endl;
      px[i] = stof(spx);
      if(px[i] - stof(a) != stof(b)-px[i])
	{
	  cout << "F" << endl;
	  break;
	}
      else
	{
	  perr_x[i] = px[i]-stof(a);
	}
      py[i] = stof(spy);
      perr_y[i] = stof(sperr_y);
      cout << "x: " << px[i] << " ; y: " << py[i] << " ; err_x: " << perr_x[i]<< " err_y: " << perr_y[i] << endl;
    }

  cout << " finished file 1 " << endl;

  for(int i = 0; i < nPbPboints; i++)
    {
      goToLine(input_pbpb, i);
      string spbx,spby,spberr_x,spberr_y,a,b;
      input_pbpb >> spbx >> a >> b >> spby >> spberr_y ;
      cout << "sx: " << spbx << " b: " << b << " a: " << a << " sy: " << spby << " serr_y: " << spberr_y << endl;
      pbx[i] = stof(spbx);
      if(pbx[i] - stof(a) != stof(b)-pbx[i])
	{
	  cout << "F" << endl;
	  break;
	}
      else
	{
	  pberr_x[i] = pbx[i]-stof(a);
	}
      pby[i] = stof(spby);
      pberr_y[i] = stof(spberr_y);
      cout << "x: " << pbx[i] << " ; y: " << pby[i] << " ; err_x: " << pberr_x[i]<< " err_y: " << pberr_y[i] << endl;  
    }

  
  //TGraphErrors *spec_aa= new TGraphErrors(nPPoints,px,py,perr_x,perr_y);
  TGraphErrors *spec_aa= new TGraphErrors(nPPoints,px,py,0,perr_y);
  TCanvas *c1 = new TCanvas();
  spec_aa -> SetName("pp_spec");
  spec_aa-> SetTitle(" ; p_{T} ; Yield ");
  spec_aa-> SetMarkerStyle(20);
  spec_aa-> SetMarkerColor(4);
  spec_aa-> Draw("ap");
  gPad -> SetLogy();
  TF1* pow_fit1  = new TF1( "Fit" , "[0]*exp(-pow(x,[4])*[3]+[1])+[2]");
  //TF1 *pow_fit1 = new  TF1("Fit", "[0]*pow(x,-[1])+[2]");
  pow_fit1 -> SetParLimits(1,0,1000);
  pow_fit1 -> SetParLimits(2,0,1000);
  pow_fit1 -> SetParLimits(0,0,1000);
  pow_fit1 -> SetParLimits(4,0,1000);
  //spec_aa->GetYaxis()->SetRangeUser(0,0.15);
  spec_aa-> Fit(pow_fit1, "" , "+" , 0,1000);

    
  TGraphErrors *spec_ab= new TGraphErrors(nPbPboints,pbx,pby,0,pberr_y);
  //TCanvas *c2 = new TCanvas();
  spec_ab -> SetName("pbpb_spec");
  spec_ab-> SetTitle(" ; p_{T} ; Yield ");
  spec_ab-> SetMarkerStyle(20);
  spec_ab-> Draw("samep");
  gPad -> SetLogy();
  //TF1* pow_fit2  = new TF1( "Fit" , "[0]*exp(-pow(x,[4])*[3]+[1])+[2]");
  TF1 *pow_fit2 = new  TF1("Fit", "[0]*pow(x,0.0-[1])",113,800);
  pow_fit2 -> SetParLimits(1,0,100);
  pow_fit2 -> SetParLimits(0,0,10);
  pow_fit2 -> SetParLimits(2,0,100);
  pow_fit2 -> FixParameter(1,10);
  pow_fit2 -> SetParameter(0,1000);
  //pow_fit2 -> SetParameter(2,0);
  
  //pow_fit2 -> SetParLimits(0,0,1);
  //spec_ab->GetYaxis()->SetRangeUser(0,0.15);
  //spec_ab-> Fit(pow_fit2, "" , "+" ,113,800);

  //for(int i = 0; i < pby[i] ; i++)

    // ("136.11,43.62,13.749,4.051,1.1542,0.3131,0.08267,0.02019,0.004644,0.000961,0.000185,0.0000361,0.0000045,0.00000048")
  
      TF1 * PP_Fixed = new TF1("PP_Fixed","[0]*exp(-pow(x,[4])*[3]+[1])+[2]",0,950);
      PP_Fixed -> FixParameter(0,1.6e-03);
      PP_Fixed -> FixParameter(1,1.4e+01);
      PP_Fixed -> FixParameter(2,4.1e-07);
      PP_Fixed -> FixParameter(3,6.7e-01);
      PP_Fixed -> FixParameter(4,5.2e-01);

      for(int i = 0; i < sizeof(pby)/sizeof(pby[0]); i++)
	{
	  ppx[i] = PP_Fixed-> GetX(pby[i]);
	  cout << Form("py = %g; pbx = %g; ppx = %g",py[i],pbx[i],ppx[i]) << endl;
	  Sloss[i] = (ppx[i]-pbx[i])/ppx[i];
	  cout << Form("sLoss for point %d: %g",i,Sloss[i]) << endl;
      }
      TGraphErrors *GSloss= new TGraphErrors(nPbPboints,ppx,Sloss,0,0);
      TCanvas *c2 = new TCanvas("c2","c2");
       GSloss -> SetName("SLoss");
       GSloss-> SetTitle(" ; p_{T} ; S_{Loss}");
       GSloss-> SetMarkerStyle(20);
       GSloss-> SetMarkerColor(4);
       GSloss -> GetYaxis() -> SetRangeUser(-0.05,0.4);
       GSloss-> Draw("ap");
       //gPad -> SetLogy();
      // hSloss -> Fill(py,Sloss);
      //hSloss -> Draw();
}

ifstream& goToLine(ifstream& file, unsigned int num)
{
  file.seekg(0);
  for(int i = 0; i < num; i++)
    {
      file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    }
  return file;
}


int countLines(char *filelist) { 
    int number_of_lines = 0;
    std::string line;
    std::ifstream myfile(filelist);
    while (std::getline(myfile, line))++number_of_lines;
    myfile.seekg (0, ios::beg);
    return number_of_lines;

}
