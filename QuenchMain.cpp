#include <iostream>
#include <cmath>
#include <fstream>
#include "QuenchMain.hh"
#include "FillData.hh"
#include "QRegenerator.h"

using namespace std;

int main(int argc, char** argv){

  for (int i=1; i<Mesh::Mz; i++) {
	for (int j=1; j<Mesh::Mphi; j++) {
	  for (int k=1; k<Mesh::Mr; k++) {
		RRR_Cdt[i][j][k] = Conductor::RRR;
	    RRR_Al [i][j][k] = Aluminium::RRR;
	  }
	}
  }
  
  QRegenerator* gen = new QRegenerator("input.root");
  gen->SetAddress(100.);

  for (int i=0;  i<=Mesh::Mz; i++){
	for (int j=0; j<=Mesh::Mphi; j++){
	  for (int k=0; k<=Mesh::Mr; k++){
		cout << gen->GetTemperature(i, j, k) << endl;
		T      [i][j][k] = 300.;
		preT   [i][j][k] = 300.;
	    Heat   [i][j][k] = 0.;
	    trigger[i][j][k] = false;
	  }
	}
  }
  
  double Qr, Qrr, Qp, Qpp, Qz, Qzz, Q;
  double kAl, kCdt, ktape, term1, term2;
  
  FillData* dataflow = new FillData();

  for (int t=1; t<=Mesh::Mt; t++){
    // current time:
    Time = Mesh::t0 + t * Mesh::dt;
    // initialization:
    TemperatureInitialization();
	
	//if (t%1000==0) printf("numOfQuench= %i ", qchCell);
    
	// Geometry and Parameter Initialization
	for (int i=1; i<Mesh::Mz; i++) {
      for (int j=1; j<Mesh::Mphi; j++) {
        for (int k=1; k<Mesh::Mr; k++) {
	      kAl   = Aluminium::mat->ThermalConduct(preT[i][j][k], RRR_Al [i][j][k], field);
		  kCdt  = Aluminium::mat->ThermalConduct(preT[i][j][k], RRR_Cdt[i][j][k], field);
		  ktape = Kapton::mat->ThermalConduct(preT[i][j][k], 0., 0.);
		  
		  C [i][j][k] = Aluminium::mat->SpecificHeat(preT[i][j][k]);
		  dz [i][j][k] = pow(Dimension::dz, 2);
		  dzz[i][j][k] = pow(Dimension::dz, 2);
          dp [i][j][k] = pow(Dimension::dp, 2);
		  dpp[i][j][k] = pow(Dimension::dp, 2);
          rho[i][j][k] = Aluminium::density;

		  // fill the thermal conductivity into array:
		  switch (k) {
            // 1: layer with shell
            case  1:
			  term1 = Kapton::lr + Conductor::lr/2 + Shell::lr/2;    // = dl from Shell cell to conductor cell
			  term2 = Kapton::lr/ktape + Conductor::lr/2/kCdt + Shell::lr/2/kAl;
			  kr [i][j][k] = term1 / term2;
		   	  kz [i][j][k] = kAl;
			  kp [i][j][k] = kAl;
			  dr [i][j][k] = term1 * Shell::lr;
			  drr[i][j][k] = Shell::lr * Shell::lr;
			  break;
		    // 19: inner aluminium strip:
		    case 19:
		      term1 = Aluminium::inner/2 + Kapton::lr + Conductor::lr/2;
			  term2 = Aluminium::inner/2/kAl + Kapton::lr/ktape + Conductor::lr/2/kCdt;
		      kr [i][j][k] = term1 / term2;
			  kz [i][j][k] = kAl;
			  kp [i][j][k] = kAl;
			  dr [i][j][k] = term1 * Aluminium::inner;
			  drr[i][j][k] = Aluminium::inner * Aluminium::inner;
			  if (i==Mesh::Mz-1) {
                dz [i][j][k] = (Shell::lHe + Dimension::dz/2) * Dimension::dz;
				dzz[i][j][k] = Dimension::dz * Dimension::dz;
			  }
			  else if (i==1) {
                dz [i][j][k] = Dimension::dz * Dimension::dz;
				dzz[i][j][k] = (Shell::lHe + Dimension::dz/2) * Dimension::dz;
			  }
			  break;
		    // conductor:
		    case  2:
		    case  4:
		    case  6:
		    case  8:
		    case 10:
		    case 12:
		    case 14:
		    case 16:
		    case 18:
	          term1 = Aluminium::strip/2 + Kapton::lr + Conductor::lr/2;
			  term2 = Aluminium::strip/2/kAl + Kapton::lr/ktape + Conductor::lr/2/kCdt;
			  kr [i][j][k] = term1 / term2;
			  kz [i][j][k] = (Kapton::lz + Conductor::lz) / (Kapton::lz/ktape + Conductor::lz/kCdt);
		      kp [i][j][k] = kCdt;
			  dr [i][j][k] = term1 * (Conductor::lr + Kapton::lr);
			  drr[i][j][k] = term1 * (Conductor::lr + Kapton::lr);
			  rho[i][j][k] = Conductor::density;
			  //SetQuenchSpot(i, j, k, 1, 1, 10);
			  break;
		    // aluminium:
		    case  3:
		    case  5:
		    case  7:
		    case  9:
		    case 11:
		    case 13:
		    case 15:
	  	    case 17:
			  term1 = Aluminium::strip/2 + Kapton::lr + Conductor::lr/2;
			  term2 = Aluminium::strip/2/kAl + Kapton::lr/ktape + Conductor::lr/2/kCdt;
			  kr [i][j][k] = term1 / term2;
              kz [i][j][k] = kAl;
			  kp [i][j][k] = ktape;
			  dr [i][j][k] = term1 * Aluminium::strip;
			  drr[i][j][k] = term1 * Aluminium::strip;
			  if (i==Mesh::Mz-1) {
                dz [i][j][k] = (Shell::lHe + Dimension::dz/2) * Dimension::dz;
				dzz[i][j][k] = Dimension::dz * Dimension::dz;
			  }
			  else if (i==1) {
                dz [i][j][k] = Dimension::dz * Dimension::dz;
				dzz[i][j][k] = (Shell::lHe + Dimension::dz/2) * Dimension::dz;
			  }
			  break;
		    default: break;
		  }
	    }
	  }
    }

	//SetGeometryParameter();

	for (int i=1;  i<Mesh::Mz; i++){
	  for (int j=1; j<Mesh::Mphi; j++){
		for (int k=1; k<Mesh::Mr; k++){	
		  
          Qz  = kz[i][j][k] * (preT  [i][j][k] - preT[i+1][j][k]) / (dz [i][j][k] * rho[i][j][k] * C[i][j][k]);
	      Qzz = kz[i][j][k] * (preT[i-1][j][k] - preT  [i][j][k]) / (dzz[i][j][k] * rho[i][j][k] * C[i][j][k]);
		  Qp  = kp[i][j][k] * (preT  [i][j][k] - preT[i][j+1][k]) / (dp [i][j][k] * rho[i][j][k] * C[i][j][k]);
		  Qpp = kp[i][j][k] * (preT[i][j-1][k] - preT  [i][j][k]) / (dpp[i][j][k] * rho[i][j][k] * C[i][j][k]);
		  Qr  = kr[i][j][k] * (preT  [i][j][k] - preT[i][j][k+1]) / (dr [i][j][k] * rho[i][j][k] * C[i][j][k]);
		  Qrr = kr[i][j][k] * (preT[i][j][k-1] - preT  [i][j][k]) / (drr[i][j][k] * rho[i][j][k] * C[i][j][k]);

		  Q = Qzz + Qpp + Qrr - Qz - Qp - Qr;
		  T[i][j][k] = preT[i][j][k] + Mesh::dt * Q;
          
		}
	  }
	}
	
	// setup boundary:
	SetBoundary(Time);
    
	if (t==1) WriteParameter("parameter.dat");

	double T1 = T[1][1][2];
	double T2 = T[Mesh::Mz-1][1][18];
	double T3 = T[2][1][1];
	double T4 = T[1][1][1];

	if (t%1000==0) {
	  printf("time= %.2f [h]  T1= %.4f  T2= %.4f  T3= %.4f  T4= %.4f [K] \n", Time/3600, T1, T2, T3, T4);
	  for (int i=1; i<Mesh::Mz; i++) {
        for (int j=1; j<Mesh::Mphi; j++) {
          for (int k=1; k<Mesh::Mr; k++) {
            dataflow->FillTree(Time, i, j, k, T[i][j][k]);
		  }
		}
	  }
	}
  
  }
    
  dataflow->Close();

  return 0;
}




