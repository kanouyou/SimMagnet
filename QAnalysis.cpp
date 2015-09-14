#include <iostream>
#include <vector>
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
  
  if (scale==true) hist->GetZaxis()->SetRangeUser(204, 300);
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

double QAnalysis::GetMaximum(double fTime) {
  TTree* tree = ReadTree(file);
  
  vector<double> fTemperature;

  for (int i=0; i<tree->GetEntries(); i++) {
    tree->GetEntry(i);
    if (time==fTime)
      fTemperature.push_back(temp);
  }
  
  double fMaximum = fTemperature[0];

  for (vector<int>::size_type i=0; i<fTemperature.size(); i++) {
    if (fTemperature[i]>fMaximum)
      fMaximum = fTemperature[i];
  }

  return fMaximum;
}

double QAnalysis::GetMinimum(double fTime) {
  TTree* tree = (TTree*)ReadTree(file);

  vector<double> fTemperature;

  for (int i=0; i<tree->GetEntries(); i++) {
    tree->GetEntry(i);
    if (time==fTime)
      fTemperature.push_back(temp);
  }

  double fMinimum = fTemperature[0];

  for (vector<int>::size_type i=0; i<fTemperature.size(); i++) {
    if (fTemperature[i]<fMinimum)
      fMinimum = fTemperature[i];
  }

  return fMinimum;
}

vector<double> QAnalysis::GetTimeVector() {
  TTree* tree = (TTree*)ReadTree(file);

  vector<double> fTime;
  double tCounter = -999.;

  for (int i=0; i<tree->GetEntries(); i++) {
    tree->GetEntry(i);
    if (tCounter!=time) {
      fTime.push_back(time);
      tCounter=time;
    }
  }

  return fTime;
}

void QAnalysis::PlotDeltaTemperature() {
  vector<double> fTime = GetTimeVector();
  TGraph* gr = new TGraph();
  double fMin, fMax;

  for (vector<int>::size_type i=0; i<fTime.size(); i++) {
    fMax = GetMaximum(fTime[i]);
    fMin = GetMinimum(fTime[i]);
    cout << "Minimum: " << fMin << " [K], Maximum: " << fMax << " [K]" << endl;
    gr->SetPoint(i, fTime[i], fMax-fMin);
  }
  
  gr->Draw("al");
}

