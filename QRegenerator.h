#ifndef QRegenerator_HH
#define QRegenerator_HH

#include <TTree.h>

class QRegenerator {
  public:
    QRegenerator(std::string file);
	~QRegenerator();
    TTree* ReadTree();
    void   SetAddress(double time);
    double GetTemperature(int x, int y, int z);

  private:
    std::string fFileName;
    int*   fID;
	double fTime;
	double fTemp;
    std::vector<int>    fX;
	std::vector<int>    fY;
	std::vector<int>    fZ;
    std::vector<double> cT;
};

#endif
