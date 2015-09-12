#include <iostream>
#include <TH2F.h>
#include <TGraph.h>
#include "QAnalysis.h"

using namespace std;

QAnalysis::QAnalysis(string fileName) {
  file = new TFile(fileName.c_str());
}

TTree* QAnalysis::ReadTree(TFile* file) {
  TTree* tree = (TTree*)file->Get("tree");
  
  posID = new int[3];
  tree->SetBranchAddress(         "id",    posID);
  tree->SetBranchAddress(       "time",    &time);
  tree->SetBranchAddress("temperature",    &temp);

  return tree;
}

void QAnalysis::PlotTemperatureDistribution(double qtime, int numPhi, bool scale) {
  int zbin = 60;
  int rbin = 19;
  TH2F*  hist = new TH2F(Form("tempDis%.1f", qtime), Form("tempDis%.1f", qtime), zbin, 0, 60, rbin, 0, 19);
  TTree* tree = ReadTree(file);
  
  for (int i=0; i<tree->GetEntries(); i++) {
    tree->GetEntry(i);
	if ( time==qtime && posID[1]==numPhi ) 
      hist->Fill(posID[0], posID[2], temp);
  }
  
  if (scale==true) hist->GetZaxis()->SetRangeUser(298, 300);
  hist->SetTitle(Form("time = %.1f [sec]; Z; R; Temperature [K]", qtime));
  hist->Draw("colz");
}

void QAnalysis::PlotSpotTemperature(int z, int phi, int r) {
  TTree* tree = ReadTree(file);
  TGraph* gr  = new TGraph();
  
  int cnt = 0;
  for (int i=0; i<tree->GetEntries(); i++) {
    tree->GetEntry(i);
	if ( posID[0]==z && posID[1]==phi && posID[2]==r ) {
	  gr->SetPoint(cnt, time, temp);
	  cnt += 1;
    }
  }

  gr->SetTitle(Form("Position: [%i, %i, %i]; Time [sec]; Temperature [K]", z, phi, r));
  gr->SetLineColor(kRed);
  gr->SetLineWidth(2);
  gr->Draw("al");

  cout << "Finished PlotSpotTemperature()" << endl;
}

TGraph* QAnalysis::GetTemperatureGraph(int z, int phi, int r) {
  TTree* tree = ReadTree(file);
  TGraph* gr  = new TGraph();
  
  int cnt = 0;
  for (int i=0; i<tree->GetEntries(); i++) {
    tree->GetEntry(i);
	if ( posID[0]==z && posID[1]==phi && posID[2]==r ) {
	  gr->SetPoint(cnt, time, temp);
	  cnt += 1;
    }
  }

  gr->SetLineColor(kRed);
  gr->SetLineWidth(2);
  return gr;
}

double QAnalysis::GetMaximum(string name) {
  TTree* tree = ReadTree(file);
  double max  = tree->GetMaximum(name.c_str());
  return max;
}

double QAnalysis::GetMinimum(string name) {
  TTree* tree = ReadTree(file);
  double min  = tree->GetMinimum(name.c_str());
  return min;
}

