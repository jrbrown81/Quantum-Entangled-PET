using namespace std;
#include <math.h>
#include <iostream>
#include <map>
#include <list>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned short SINO_TYPE;

struct _3Dvector
{
  int windowID;
  float theta, X_a, Y_a, Z_a, E_a, X_b, Y_b, Z_b, E_b; 
  int coincidence; 
  float deltaPhi;
};

struct lor
{
  _3Dvector p1, p2;
};

int window;
double theta1, X1_a, Y1_a, Z1_a, E1_a, X1_b, Y1_b, Z1_b, E1_b;
double theta2, X2_a, Y2_a, Z2_a, E2_a, X2_b, Y2_b, Z2_b, E2_b;
int type;
double deltaPhi;

lor ReadData(void);
void Load_sino3D(lor);
void free_sino3D(void);
void write_sino3D(void);

// Global variables:
double _PI;

FILE *theINfile;
const char* fileINname =
 "/shared/storage/physhad/Medical/SimulationData/NEMA-edge-fix/hotrods/randoms/addRandoms_out_20to1_posSmear_3.2bill.csv";

FILE *theOUTfile;
const char* fileOUTname = "NEMA_100pixels_random_onlyTheta_3.2BillLORs_sino";

unsigned long int nbTrueTot(0), nbScatterTot(0), nbRandomTot(0);
float d_axial(29.), d_FOV(60.);
int typeINfile(0);
float offsetX(0.);
int maxRingDiff(-1), n_planes(1), n_bin(100), n_ang(100), verbos(0);
unsigned long int MAX_Coinci(0);
int oType(0);
unsigned long int coincidenceType(-1);
float thetaMin(67.), thetaMax(97.), deltaPhiMin(0.), deltaPhiMax(180.),
 ElostMin(0.), ElostMax(511.), deltaPhi90Min(9999.);
int dis_i_window(0), phi_i_window(0), zPos_window(0);
bool coinciVal(false);

int nbTotAxialPlanes;

SINO_TYPE ***sino3D;
SINO_TYPE *sino_line;

unsigned long int nbTotCoincidences(0);
unsigned long int nbCoinciVal(0);
;
