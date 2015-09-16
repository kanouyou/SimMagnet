#include <iostream>
#include <string>
#include <TApplication.h>
#include <TMultiGraph.h>
#include <TGraph.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TStyle.h>
#include "QAnalysis.h"

using namespace std;

int main(int argc, char** argv) {
  
  TApplication* app = new TApplication("app", &argc, argv);
  
  gStyle->SetTitleSize(0.05, "xyz");
  gStyle->SetLabelSize(0.05, "xyz");

  TCanvas* c0 = new TCanvas("temp", "temp", 1500, 1000);
  c0->Divide(4,3);

  gStyle->SetOptStat(0);
  gStyle->SetNumberContours(99);

  double time = 3600.*24*9/12;

  QAnalysis* ana = new QAnalysis("output.root");
  
  cout << "Minimum Temperature: " << ana->GetMinimum("temp") << " [K]" << endl;
  
  for (int i=0; i<12; i++) {
    c0->cd(i+1);
    gPad->SetTicks(1,1);
    gPad->SetRightMargin(0.14);
    ana->PlotTemperatureDistribution(time*(i+1), 1, false);
  }
  
  // Plot time-temperature
  TCanvas* c1 = new TCanvas("timetemp", "timetemp", 500, 500);
  c1->SetTicks(1,1);

  TMultiGraph* mg = new TMultiGraph();
  TGraph* gr[4];
  
  gr[0] = ana->GetTemperatureGraph( 1, 1, 18);
  gr[1] = ana->GetTemperatureGraph(58, 1, 18);
  gr[2] = ana->GetTemperatureGraph(30, 1, 18);
  gr[3] = ana->GetTemperatureGraph(20, 1,  8);

  for (int i=0; i<4; i++) {
    gr[i]->SetLineColor(i+1);
	gr[i]->SetLineStyle(i+1);
	mg->Add(gr[i], "l");
  }
  
  mg->SetTitle(";Time [sec];Temperature [K]");
  mg->Draw("a");
  
  // Plot deta T
  /*TCanvas* c2 = new TCanvas("deltaT", "deltaT", 600, 500);
  c2->SetTicks(1,1);

  ana->PlotDeltaTemperature();
*/
  app->Run();
  
  return 0;
}

