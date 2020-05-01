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
  float theta, X_a, Y_a, Z_a, E_a, X_b, Y_b, Z_b, E_b, scatterCoincidence,
   deltaPhi;
};

struct lor
{
  _3Dvector p1, p2;
};

double theta1, X1_a, Y1_a, Z1_a, E1_a, X1_b, Y1_b, Z1_b, E1_b;
double theta2, X2_a, Y2_a, Z2_a, E2_a, X2_b, Y2_b, Z2_b, E2_b;
int scatterCoincidence;
double deltaPhi;

lor ReadData(void);
void Load_sino3D(lor);
void free_sino3D(void);
void write_sino3D(void);

// Global variables:
double _PI;

FILE *theINfile;
const char* fileINname =
 "/shared/storage/physhad/Medical/SimulationData/april2020Sims/NEMA-1trill-scattfix_posSmear.csv";

FILE *theOUTfile;
const char* fileOUTname = "NEMA_100pixels_onlyScatter_80-100_sino3D";

unsigned long int nbTrue(0), nbTrueFile(0), nbScatterFile(0), nbTrueTot(0),
 nbScatterTot(0);
float d_axial(29.), d_FOV(60.);
int typeINfile(0);
float offsetX(0.);
int maxRingDiff(-1), n_planes(1), n_bin(100), n_ang(100), verbos(0);
unsigned long int MAX_Coinci(0);
int oType(0), scatteringOutOfCZT(-1);
float thetaMin(67.), thetaMax(97.), deltaPhiMin(0.), deltaPhiMax(180.),
 ElostMin(0.), ElostMax(511.);

int nbTotAxialPlanes;

SINO_TYPE ***sino3D;
SINO_TYPE *sino_line;

unsigned long int nbTotCoincidences(0);
unsigned long int nCoinciVal(0);
;
