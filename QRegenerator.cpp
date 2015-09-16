#include <iostream>
#include <TFile.h>
#include "QRegenerator.h"

using namespace std;

QRegenerator::QRegenerator {
  fFileName = file;
}

QRegenerator::~QRegenerator() {}

TTree* QRegenerator::ReadTree() {
  TFile* file = new TFile(fFileName.c_str());
  TTree* tree = (TTree*)file->Get("tree");

  fID = new int[3];
  tree->SetBranchAddress(  "id",    fID);
  tree->SetBranchAddress("time", &fTime);
  tree->SetBranchAddress("temp", &fTemp);

  return tree;
}

void QRegenerator::SetAddress(double time) {
  TTree* tree = ReadTree();

  for (int i=0; i<tree->GetEntries(); i++) {
    tree->GetEntry(i);
	if (fTime==time) {
      fX.push_back(fID[0]);
	  fY.push_back(fID[1]);
	  fZ.push_back(fID[2]);
	  cT.push_back(fTemp);
    }
  }
}

double QRegenerator::GetTemperature(int x, int y, int z) {
  int numOfVector = cT.size();
  if (numOfVector!=fX.size() || numOfVector!=fY.size() || numOfVector!=fZ.size())
    return 0.;

  double currentT;
  for (vector<int>::size_type i=0; i<numOfVector; i++) {
    if (fX[i]==x && fY[i]==y && fZ[i]==z)
      currentT = cT[i];
  }
  
  return currentT;
}

