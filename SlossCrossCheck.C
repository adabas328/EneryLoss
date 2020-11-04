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


void SLOSS(char *ppin, char* hiin, float TAA, int system)
{
  
  ifstream input_pp;
  input_pp.open(ppin);
  int nLinesPP = countLines(ppin) - 1;
  const int nPPoints = countLines(ppin);
  cout << "nPPoints: " << nPPoints << endl;
  float px[nPPoints];
  float py[nPPoints];
  float ppx[nPPoints];
  float perr_x[nPPoints];
  float perr_y[nPPoints];

  float Sloss [nPPoints];

  ifstream input_pbpb;
  cout << "opening pbpb data: " << hiin <<endl;
  input_pbpb.open(hiin);
  int nLinesPbPb = countLines(hiin) - 1;
  const int nPbPboints = countLines(hiin);
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
      float wlow=px[i] - stof(a);
      float whi=stof(b)-px[i];
      //if(px[i] - stof(a) != stof(b)-px[i])
      if(wlow != whi)
	{
	  cout << "width low " << px[i] - stof(a) << " width high " << stof(b)-px[i]<<endl;
	  //cout << "F" << endl;
	  //break;
	  perr_x[i] = px[i]-stof(a);
	}
      else
	{
	  perr_x[i] = px[i]-stof(a);
	}
      py[i] = stof(spy) * TAA;
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
	  //cout << "F" << endl;
	   //break;
	  pberr_x[i] = pbx[i]-stof(a);
	}
      else
	{
	  pberr_x[i] = pbx[i]-stof(a);
	}
      pby[i] = stof(spby);
      pberr_y[i] = stof(spberr_y);
      cout << "x: " << pbx[i] << " ; y: " << pby[i] << " ; err_x: " << pberr_x[i]<< " err_y: " << pberr_y[i] << endl;  
    }

  
  //TGraphErrors *spec_pp= new TGraphErrors(nPPoints,px,py,perr_x,perr_y);
  TGraphErrors *spec_pp= new TGraphErrors(nPPoints,px,py,0,perr_y);
  TCanvas *c1 = new TCanvas();
  spec_pp -> SetName("pp_spec");
  spec_pp-> SetTitle(" ; p_{T} ; Yield ");
  spec_pp-> SetMarkerStyle(20);
  spec_pp-> SetMarkerColor(4);
  spec_pp-> Draw("ap");
  gPad -> SetLogy();
  TF1* pow_fit1  = new TF1( "Fit" , "[0]*exp(-pow(x,[4])*[3]+[1])+[2]");
  //TF1 *pow_fit1 = new  TF1("Fit", "[0]*pow(x,-[1])+[2]");
  pow_fit1 -> SetParLimits(1,0,1000);
  pow_fit1 -> SetParLimits(2,0,1000);
  pow_fit1 -> SetParLimits(0,0,1000);
  pow_fit1 -> SetParLimits(4,0,1000);
  //spec_pp->GetYaxis()->SetRangeUser(0,0.15);
  // spec_pp-> Fit(pow_fit1, "" , "+" , 0,1000);

    
  TGraphErrors *spec_aa= new TGraphErrors(nPbPboints,pbx,pby,0,pberr_y);
  //TCanvas *c2 = new TCanvas();
  spec_aa -> SetName("pbpb_spec");
  spec_aa-> SetTitle(" ; p_{T} ; Yield ");
  spec_aa-> SetMarkerStyle(20);
  spec_aa-> Draw("samep");
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
  //spec_aa->GetYaxis()->SetRangeUser(0,0.15);
  //spec_aa-> Fit(pow_fit2, "" , "+" ,113,800);

  //for(int i = 0; i < pby[i] ; i++)

    // ("136.11,43.62,13.749,4.051,1.1542,0.3131,0.08267,0.02019,0.004644,0.000961,0.000185,0.0000361,0.0000045,0.00000048")
  
  TF1 * PP_Fixed;




  if (system == 0){
    PP_Fixed = new TF1("PP_Fixed","[0]*exp(-pow(x,[4])*[3]+[1])+[2]",0,950);
      PP_Fixed -> FixParameter(0,1.6e-03);
      PP_Fixed -> FixParameter(1,1.4e+01);
      PP_Fixed -> FixParameter(2,4.1e-07);
      PP_Fixed -> FixParameter(3,6.7e-01);
      PP_Fixed -> FixParameter(4,5.2e-01);
  }


  if (system == 1){
    //PP_Fixed = new TF1("PP_Fixed","[0]*exp(-pow(x,[4])*[3]+[1])+[2]",0,30);
    //PP_Fixed = new TF1("PP_Fixed","[0]*pow([1]*x+[2],[3])",0,30);
    //PP_Fixed = new TF1("PP_Fixed","[0]*exp(-pow(x,2)+[1])+[2];
    PP_Fixed = new TF1("PP_Fixed","[0]*pow(x*(1+[1]/x)),-8.1",0,31);    //PHENIX paper 
    //PP_Fixed = new TF1("PP_Fixed","[0]*pow([1]*x+[2],[3])+[4]",0,31); //Current
    //PP_Fixed -> FixParameter(0,10);        //current
    //PP_Fixed -> FixParameter(1,-0.48);     //current
    //PP_Fixed -> FixParameter(2,0.1);       //current 
    //PP_Fixed -> FixParameter(3,-6.);      //current
    //PP_Fixed -> FixParameter(4,0.00);     //current
    //PP_Fixed -> FixParameter(2,-8.1);
      
  }

  PP_Fixed -> SetNpx(100000);

  
  
 
  TSpline3 *pp_spline = new TSpline3("pp_spline",spec_pp,"",ppx[0],ppx[sizeof(ppx)/sizeof(ppx[0])]);
  pp_spline -> SetNpx(100000);
  pp_spline->Draw("same");
  auto lambda = [&](double *x, double *){return pp_spline->Eval(x[0]);};
  auto pp_spline_F = new TF1("oof",lambda,ppx[0],ppx[sizeof(ppx)/sizeof(ppx[0])-1],0);
  pp_spline_F -> SetNpx(100000);
  
  //cout << "X for " << pby[4] << " at: " << pp_spline_F -> GetX(pby[4],ppx[0],30) << endl;
  //TLine *line = new TLine(0,pby[4],30,pby[4]);
  //TLine *line2 = new TLine(pp_spline_F -> GetX(pby[4],ppx[0],30),pby[0],pp_spline_F -> GetX(pby[4],ppx[0],30),pby[sizeof(pby)/sizeof(pby[0])-1]);
  //line2 -> Draw("same");
  //line -> Draw("same");
  
  for(int i = 0; i < sizeof(pby)/sizeof(pby[0]); i++)
    {
      //ppx[i] = PP_Fixed-> GetX(pby[i]);
      //ppx[i] = pp_spline_F -> GetX(pby[i],(Double_t)ppx[0],(Double_t)ppx[sizeof(ppx)/sizeof(ppx[0])-1]);
      ppx[i] = pp_spline_F -> GetX(pby[i],ppx[0],30);
      cout << Form("py = %g; pbx = %g; ppx = %g",py[i],pbx[i],ppx[i]) << endl;
  
      Sloss[i] = (ppx[i]-pbx[i])/ppx[i];
      cout << Form("sLoss for point %d: %g",i,Sloss[i]) << endl;
    }

  


  
  


  spec_pp -> Fit(PP_Fixed, "" , "R+");
  TF1 *PP_Fit = (TF1*)spec_pp -> GetFunction("PP_Fixed");
  TF1 *pp_spline_f = (TF1*)spec_pp -> GetFunction("pp_spline_F");
  
  float ratio[nPPoints];
  // new ratio variable
  for(int i = 0; i < sizeof(px)/sizeof(px[0]);i++)
    {
      if (py[i] > 0){	
	ratio[i] = py[i]/PP_Fit -> Eval(px[i]);
	//ratio[i] = py[i]/pp_spline_F -> Eval(px[i]);
      //fill new ratio array. 
      cout << Form("py: %g; PP_Fit: %g; ratio: %g",py[i], PP_Fit->Eval(px[i]), ratio[i]) << endl;
      //cout << Form("py: %g; pp_spline_f: %g; ratio: %g",py[i], pp_spline_f->Eval(ppx[i]), ratio[i]) << endl;
      }
      else {ratio[i] = 0;}
    }
 
  //Rewrite code above for TSpline
  
  int nPoints = sizeof(ppx)/sizeof(ppx[0]);
  TGraph *DFit = new TGraph(nPoints,ppx,ratio);
  TCanvas *c3 = new TCanvas();
  DFit -> SetName("Ratio");
  DFit -> SetTitle(" ; ");
  DFit -> SetMarkerStyle(20);
  DFit -> SetMarkerColor(4);
  DFit -> Draw("ap");
 	     	     
  //change the marker style
  //create a new tcanvas
  //draw graph draw("ap")




  

  
  TGraphErrors *GSloss= new TGraphErrors(nPbPboints,ppx,Sloss,0,0);
  GSloss -> GetXaxis() -> SetRangeUser(4,20);
  TCanvas *c2 = new TCanvas("c2","c2");
  GSloss -> SetName("SLoss");
  GSloss-> SetTitle(" ; p_{T}^{pp} ;s_{Loss} = #delta p_{T}/p_{T}");
  GSloss -> GetYaxis() -> SetTitleOffset(1.2);
  GSloss-> SetMarkerStyle(20);
  GSloss-> SetMarkerColor(4);
  GSloss -> GetYaxis() -> SetRangeUser(-0.05,0.4);
  GSloss-> Draw("ap");
  //gPad -> SetLogy();
  // hSloss -> Fill(py,Sloss);
  //hSloss -> Draw();


  float paperSlossX[6] = {5.,6.,7.,10.,12,15};
  float paperSlossY[6] = {0.241,0.270,0.293,0.316,0.303,0.282};
  float errorStat[6] = {0.001,0.001,0.001,0.001,0.001,0.002};

  TGraphErrors *papSloss = new TGraphErrors(6,paperSlossX,paperSlossY,0,errorStat);
  papSloss -> SetMarkerStyle(20);
  papSloss -> SetMarkerColor(2);
  papSloss -> Draw("samep");
  
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
