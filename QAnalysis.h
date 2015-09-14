#ifndef QAnalysis_HH
#define QAnalysis_HH

#include <string>
#include <TFile.h>
#include <TTree.h>

class QAnalysis {
  public:
    QAnalysis(std::string fileName);
    ~QAnalysis() {}
    void PlotTemperatureDistribution(double qtime, int numPhi, bool scale=false);
    void PlotSpotTemperature(int z, int phi, int r);
    TGraph* GetTemperatureGraph(int z, int phi, int r);
    double GetMinimum(std::string name);
    double GetMaximum(std::string name);
    double GetMaximum(double fTime);
    double GetMinimum(double fTime);
    void   PlotDeltaTemperature();
    std::vector<double> GetTimeVector();

  private:
    TTree* ReadTree(TFile* file);
    
  private:
    TFile* file;
    int*   posID;
    double time;
    double temp;
};

#endif
