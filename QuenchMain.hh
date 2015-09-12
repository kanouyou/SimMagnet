#ifndef QuenchMain__HH
#define QuenchMain__HH

#include <iostream>
#include <string>
#include <fstream>
#include "QSuperconduct.h"
#include "MParameter.hh"
#include "QMaterial.h"

// meshing
namespace Mesh {
  const int    Mz   = 60 + 1;
  const int    Mphi = 4 + 1;
  const int    Mr   = 19 + 1;
  // time mesh:
  const double t0   = 0.;
  const double tf   = 100000.;
  const int    Mt   = 100000;
  const double dt   = (tf - t0) / Mt;
}

// unit: [m]
namespace Kapton {
  const double l_Resin   = 0.5 * 1e-3;
  const double k_Resin   = 0.05;
  const double thinkness = 0.075 * 1e-3;
  const double lr = thinkness * 2 + l_Resin;
  const double lz = thinkness * 2;
  QMaterial* mat = new QMaterial("Kapton");
}

namespace Aluminium {
  const double RRR     = 2000.;
  const double density = 2700.;
  const double strip   = 1. * 1e-3;
  const double inner   = 3. * 1e-3;
  QMaterial* mat = new QMaterial("Al");
}

namespace Shell {
  const double lr  = 50. * 1e-3;
  const double lHe = 50. * 1e-3;
}

namespace Solenoid {
  const int    turn     = 270;
  const int    layer    = 9;
  const double current  = 0.;    // [A]
  const double resistor = 0.185;    // [Ohm]
  const double induct   = 12.69;    // [H]
  const double field    = 0.;      // [T]
  const int    operTime = 90;       // [days]
}

namespace Conductor {
  const double RRR     = 400.;
  const double density = 4000.;
  const double lz      = 4.73 * 1e-3;
  const double lr      = 15. * 1e-3;
  const double rAl     = 7.3 / 9.3;
  const double rNbTi   = 1. / 9.3;
  const double rCu     = 1. / 9.3;
  const double rArea   = static_cast<double>(Solenoid::turn) / (Mesh::Mz - 1);
  const double aAl     = lz * lr * rAl;
  const double areaAl  = lz * lr * rAl * rArea;
  const double cdtvol  = lz * lr * (2*M_PI*0.8);
  const double volume  = rArea * lz * lr * (2*M_PI*0.8);
  const double current = Solenoid::current;
  const double RRR_Cu  = 100.;
}

namespace Dimension {
  const double lz = Solenoid::turn  * (Conductor::lz + Kapton::lz * 2);
  const double lr = Solenoid::layer * (Conductor::lr + Kapton::lr * 2) + (Solenoid::layer-1) * Aluminium::strip
                    + Aluminium::inner;
  const double lp = 2 * M_PI * 0.80;
  const double dz = lz / (Mesh::Mz   - 1);
  const double dr = lr / (Mesh::Mr   - 1);
  const double dp = lp / (Mesh::Mphi - 1);
}

namespace Radiation {
  const int nz = 5;
  const int np = 4;
  const int nr = 9;
  // PHITS data (flux[n/m2/sec]):
  double neutron[5][4][9] = {{{2.7e+20, 4.0e+20, 5.3e+20, 6.8e+20, 8.5e+20, 1.0e+21, 1.3e+21, 1.5e+21, 1.9e+21},
	   						  {2.8e+20, 4.1e+20, 5.4e+20, 6.9e+20, 8.4e+20, 1.0e+21, 1.3e+21, 1.5e+21, 1.8e+21},
		   					  {2.6e+20, 3.8e+20, 5.0e+20, 6.3e+20, 7.9e+20, 9.6e+20, 1.2e+21, 1.4e+21, 1.7e+21},
		   					  {2.7e+20, 4.0e+20, 5.3e+20, 6.7e+20, 8.3e+20, 1.0e+21, 1.2e+21, 1.5e+21, 1.8e+21}},
		   					 {{5.0e+20, 7.4e+20, 9.8e+20, 1.3e+21, 1.5e+21, 1.9e+21, 2.3e+21, 2.8e+21, 3.3e+21},
		   					  {5.0e+20, 7.4e+20, 9.7e+20, 1.2e+21, 1.5e+21, 1.9e+21, 2.2e+21, 2.7e+21, 3.2e+21},
		   					  {4.4e+20, 6.4e+20, 8.5e+20, 1.1e+21, 1.3e+21, 1.6e+21, 1.9e+21, 2.3e+21, 2.8e+21},
		   					  {4.9e+20, 7.2e+20, 9.8e+20, 1.2e+21, 1.5e+21, 1.9e+21, 2.2e+21, 2.7e+21, 3.2e+21}},
		   					 {{6.3e+20, 9.4e+20, 1.2e+21, 1.6e+21, 2.0e+21, 2.4e+21, 2.8e+21, 3.4e+21, 4.0e+21},
		   					  {6.6e+20, 9.8e+20, 1.3e+21, 1.6e+21, 2.0e+21, 2.5e+21, 3.0e+21, 3.6e+21, 4.2e+21},
		   					  {6.7e+20, 9.8e+20, 1.3e+21, 1.6e+21, 2.0e+21, 2.4e+21, 2.9e+21, 3.5e+21, 4.2e+21},
		   					  {6.7e+20, 9.9e+20, 1.3e+21, 1.7e+21, 2.0e+21, 2.5e+21, 3.0e+21, 3.6e+21, 4.3e+21}},
		   					 {{5.6e+20, 8.2e+20, 1.1e+21, 1.4e+21, 1.7e+21, 2.0e+21, 2.5e+21, 3.0e+21, 3.5e+21},
		   					  {6.5e+20, 9.4e+20, 1.2e+21, 1.5e+21, 1.9e+21, 2.3e+21, 2.8e+21, 3.3e+21, 3.9e+21},
		   					  {7.4e+20, 1.1e+21, 1.4e+21, 1.8e+21, 2.2e+21, 2.6e+21, 3.2e+21, 3.8e+21, 4.5e+21},
		   					  {6.5e+20, 9.5e+20, 1.2e+21, 1.6e+21, 1.9e+21, 2.3e+21, 2.8e+21, 3.4e+21, 4.0e+21}},
		   					 {{4.3e+20, 6.2e+20, 8.0e+20, 1.0e+21, 1.3e+21, 1.5e+21, 1.8e+21, 2.2e+21, 2.7e+21},
		   					  {4.7e+20, 6.8e+20, 8.9e+20, 1.1e+21, 1.4e+21, 1.7e+21, 2.0e+21, 2.4e+21, 2.9e+21},
		   					  {6.3e+20, 9.0e+20, 1.2e+21, 1.5e+21, 1.8e+21, 2.2e+21, 2.6e+21, 3.2e+21, 3.8e+21},
		   					  {4.9e+20, 7.1e+20, 9.4e+20, 1.2e+21, 1.4e+21, 1.7e+21, 2.1e+21, 2.5e+21, 3.0e+21}}};
  
}

static double T      [Mesh::Mz+1][Mesh::Mphi+1][Mesh::Mr+1];
static double preT   [Mesh::Mz+1][Mesh::Mphi+1][Mesh::Mr+1];
static double RRR_Al [Mesh::Mz+1][Mesh::Mphi+1][Mesh::Mr+1];
static double RRR_Cdt[Mesh::Mz+1][Mesh::Mphi+1][Mesh::Mr+1];
static double kr     [Mesh::Mz+1][Mesh::Mphi+1][Mesh::Mr+1];
static double kz     [Mesh::Mz+1][Mesh::Mphi+1][Mesh::Mr+1];
static double kp     [Mesh::Mz+1][Mesh::Mphi+1][Mesh::Mr+1];
static double C      [Mesh::Mz+1][Mesh::Mphi+1][Mesh::Mr+1];
static double Heat   [Mesh::Mz+1][Mesh::Mphi+1][Mesh::Mr+1];
static double dz     [Mesh::Mz+1][Mesh::Mphi+1][Mesh::Mr+1];
static double dzz    [Mesh::Mz+1][Mesh::Mphi+1][Mesh::Mr+1];
static double dr     [Mesh::Mz+1][Mesh::Mphi+1][Mesh::Mr+1];
static double drr    [Mesh::Mz+1][Mesh::Mphi+1][Mesh::Mr+1];
static double dp     [Mesh::Mz+1][Mesh::Mphi+1][Mesh::Mr+1];
static double dpp    [Mesh::Mz+1][Mesh::Mphi+1][Mesh::Mr+1];
static double rho    [Mesh::Mz+1][Mesh::Mphi+1][Mesh::Mr+1];
static double qchTime[Mesh::Mz+1][Mesh::Mphi+1][Mesh::Mr+1];
static bool   trigger[Mesh::Mz+1][Mesh::Mphi+1][Mesh::Mr+1];

static double totRes  = 0.;
static int    qchCell = 0;
static double Time;
static double field   = Solenoid::field;
static double current = Solenoid::current;

double RTok(double rho){
	double T   = 4.5;
	double Lwf = 2.44e-8;

	// Resistivity of conduct at room temperature
	double k;
	k = Lwf * T / rho;
	return k;
}

double CriticalTemperature(double B){
	double T;
	T = 9.35*pow(1-B/14.25,1/1.7);
	return T;
}


void GetConductorResistance(double B) {
  double res = 0.;
  int    n   = 0;

  for (int i=1; i<Mesh::Mz; i++) {
    for (int j=1; j<Mesh::Mphi; j++) {
      for (int k=1; k<Mesh::Mr; k++) {
        if (k%2==0 && T[i][j][k]>=CriticalTemperature(B)) {
          n += 1;
		  res += AlResistivity(preT[i][j][k], RRR_Al[i][j][k], B) * Dimension::lp / Conductor::areaAl;
		}
	  }
	}
  }
  
  qchCell = n;
  totRes  = res;
}


double GetRRRFromData(std::string mat, int ix, int iy, int iz) {
  QSuperconduct* rad = new QSuperconduct();
  int zfactor = (Mesh::Mz-1) / Radiation::nz;
  int pfactor = (Mesh::Mphi-1) / Radiation::np;
  int rfactor = (Mesh::Mr-1) / Radiation::nr;
  
  int cz = (ix-1) / zfactor;
  int cp = (iy-1) / pfactor;
  int cr = (iz-1) / rfactor;

  if (cr>=Radiation::nr)
    cr = cr - 1;
  
  // flip the z and r direction data
  //cz = Radiation::nz - 1 - cz;
  //cr = Radiation::nr - 1 - cr;
  double RRR = rad->GetRadiationEffect(mat, Solenoid::operTime, Radiation::neutron[cz][cp][cr]);

  //std::cout << cz << " " << cp << " " << cr << " " << " " << ix << " " << iy << " " << iz << " " << RRR << " " << Radiation::neutron[cz][cp][cr] << std::endl;
  return RRR;
}

void TemperatureInitialization() {
  for (int i=0; i<=Mesh::Mz; i++) {
    for (int j=0; j<=Mesh::Mphi; j++) {
      for (int k=0; k<=Mesh::Mr; k++)
	    preT[i][j][k] = T[i][j][k];
	}
  }
}

void SetGeometryParameter() {
  double k_Al, k_Cdt, k_tape;
  double term1, term2;
  //double C_Al;
  
  //std::cout << preT[1][1][8] << RRR_Al [1][1][8] << std::endl;
  for (int i=1; i<Mesh::Mz; i++) {
    for (int j=1; j<Mesh::Mphi; j++) {
      for (int k=1; k<Mesh::Mr; k++) {
		k_Al   = Aluminium::mat->ThermalConduct(preT[i][j][k], RRR_Al [i][j][k], field);
		k_Cdt  = Aluminium::mat->ThermalConduct(preT[i][j][k], RRR_Cdt[i][j][k], field);
		k_tape = Kapton::mat->ThermalConduct(preT[i][j][k], 0., 0.);
		
		C [i][j][k] = Aluminium::mat->SpecificHeat(preT[i][j][k]);
		
		// fill the thermal conductivity into array:
		switch (k) {
          // 1: layer with shell
          case  1:
			term1 = Kapton::lr + Conductor::lr/2 + Shell::lr/2;
			term2 = Kapton::lr/k_tape + Conductor::lr/2/k_Cdt + Shell::lr/2/k_Al;
			kr[i][j][k] = term1 / term2;
			kz[i][j][k] = k_Al;
			break;
		  // 19: inner aluminium strip:
		  case 19:
		    term1 = Aluminium::inner/2 + Kapton::lr + Conductor::lr/2;
			term2 = Aluminium::inner/2/k_Al + Kapton::lr/k_tape + Conductor::lr/2/k_Cdt;
		    kr[i][j][k] = term1 / term2;
			kz[i][j][k] = k_Al;
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
			term2 = Aluminium::strip/2/k_Al + Kapton::lr/k_tape + Conductor::lr/2/k_Cdt;
			kr[i][j][k] = term1 / term2;
			kz[i][j][k] = (Kapton::lz + Conductor::lz) / (Kapton::lz/k_tape + Conductor::lz/k_Cdt);
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
			term2 = Aluminium::strip/2/k_Al + Kapton::lr/k_tape + Conductor::lr/2/k_Cdt;
			kr[i][j][k] = term1 / term2;
            kz[i][j][k] = k_Al;
			break;
		  default: break;
		}
	  }
	}
  }
  
  //std::cout << k_Al << " " << k_Cdt << std::endl;
}

void SetQuenchSpot(int i, int j, int k, int qz, int qphi, int qr) {
  double heat;

  //if ( i==qz && j==qphi && k==qr ) {
  if ( (i==1 || i==2) && j==1 && k==18 ) {
	heat = pow(current,2) * AlResistivity(preT[i][j][k], RRR_Al[i][j][k], field) * Dimension::dp / Conductor::aAl;
	Heat[i][j][k] = heat * Conductor::rArea / (Conductor::density * C[i][j][k] * Conductor::cdtvol);
  }
  
  if ( preT[i][j][k]>=CriticalTemperature(field) ) {
    heat = pow(current,2) * AlResistivity(preT[i][j][k], RRR_Al[i][j][k], field) * Dimension::dp / Conductor::aAl;
	Heat[i][j][k] = heat * Conductor::rArea / (Conductor::density * C[i][j][k] * Conductor::cdtvol);
  }
}

void SetBoundary(double time) {
  double coolVelocity = 1./3600.;    // K/hour
  double coolTemp     = 300 - coolVelocity * time;
  
  if (coolTemp<4.2) coolTemp = 4.2;

  // r direction:
  for (int i=0;  i<=Mesh::Mz; i++){
    for (int j=0;  j<=Mesh::Mphi; j++){
	  T[i][j][0]        = T[i][j][1];
	  T[i][j][Mesh::Mr] = T[i][j][Mesh::Mr-1];
	  T[i][j][0]        = coolTemp;
	}
  }
  // phi direction:
  for (int i=0;  i<=Mesh::Mz; i++){
	for (int k=0;  k<=Mesh::Mr; k++){
	  T[i][0][k]          = T[i][Mesh::Mphi-1][k];
	  T[i][Mesh::Mphi][k] = T[i][1][k];
	}
  }
  // z direction:	
  for (int j=0;  j<=Mesh::Mphi; j++){
	for (int k=0;  k<=Mesh::Mr; k++){
	  T[Mesh::Mz][j][k] = T[Mesh::Mz-1][j][k];
	  T[0][j][k]        = T[1][j][k];
	}
  }
  // cooling:
  for (int j=0; j<=Mesh::Mphi; j++) {
    T[0][j][19]        = coolTemp;
	T[Mesh::Mz][j][19] = coolTemp;
	T[0][j][17]        = coolTemp;
	T[Mesh::Mz][j][15] = coolTemp;
    T[0][j][13]        = coolTemp;
	T[Mesh::Mz][j][11] = coolTemp;
	T[0][j][9]         = coolTemp;
	T[Mesh::Mz][j][7]  = coolTemp;
	T[0][j][5]         = coolTemp;
	T[Mesh::Mz][j][3]  = coolTemp;
	T[0][j][1]         = coolTemp;
  }
}

void WriteQuenchVelocity() {
  // write time when temperature exceed to the critical temperature
  for (int i=1; i<Mesh::Mz; i++) {
    for (int j=1; j<Mesh::Mphi; j++) {
      for (int k=1; k<Mesh::Mr; k++) {
        if ( trigger[i][j][k]==false && T[i][j][k]>=CriticalTemperature(field) ) {
          qchTime[i][j][k] = Time;
		  trigger[i][j][k] = true;
		}
	  }
	}
  }

}

void WriteParameter(std::string fName) {
  std::ofstream output(fName.c_str());

  if (output.fail())
    std::cout << " Cannot open this file." << std::endl;
  else {
    output << " z    phi    r    T [K]    preT [K]   kz [W/m/K]    kp [W/m/K]    kr [W/m/K]    rho [g/cm3]    C [J/kg/K]    dz [m2]    dzz [m2]\n";
	for (int i=0; i<=Mesh::Mz; i++) {
      for (int j=0; j<=Mesh::Mphi; j++) {
        for (int k=0; k<=Mesh::Mr; k++) {
          output << i << " " << j << " " << k << " " << T[i][j][k] << " " << preT[i][j][k] << " " << kz[i][j][k] << " " << kp[i][j][k]
		         << " " << kr[i][j][k] << " " << rho[i][j][k] << " " << C[i][j][k] << " " << dz[i][j][k] << " " << dzz[i][j][k] << "\n";
		}
	  }
	}
  }
}

#endif 
