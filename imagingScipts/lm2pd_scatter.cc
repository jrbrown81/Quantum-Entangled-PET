#include "lm2pd_scatter.hh"

int main(int argc, char *argv[])
{
  if (argc == 1)
  {
     cout << " -------------------------- \n"
    " Description : \n"
    "     List-mode file to projection data (sino 3D). \n"
    "     Input data: Global X Y Z (Arce format) \n"
    " \n"

    // ARGUMENTS (original from GAMOS):
    " Arguments convention: \n"
    "     -a Axial FOV (mm), <d_axial = 29.0> \n"
    "     -d Diameter Transaxial FOV (mm), <d_FOV = 60.0> \n"
    "     -i Type of the input file (by default: 0 = Arce_binary), <typeINfile = 0> \n"
    "     -j Offset X direction (mm), <offsetX = 0.0> \n"
    " \n"
    "     -m Maximum ring difference (by default: -1 = n_planes), <maxRingDiff> \n"
    "     -n Name of output file, <fileOUTname> \n"
    "     -p Axial number of planes, <n_planes> \n"
    "     -r Number of bins, \"distancias\", <n_bin> \n"
    "     -t Number of angular views, \"direcciones\", <n_ang> \n"
    "     -v Verbosity (by default: 0 = silent, 3 = debug), <verbos> \n"
    "     -x Maximum number of coincidences to be stored (by default: -1 = no limit), <Max_Coinci> \n"
    "     -o Output type (by default: 0 = mcc Interfile, 1 = STIR Interfile), <oType> \n"
    // Following arguments were implemented for QE-PET
    "     -b Type of coincidence: -1 = true + scatter (by default), 0 = true, 1 = scatter, <scatteringOutOfCZT> \n"
    "     -c Lower endpoint of the theta window, in degrees (67 by default), <thetaMin> \n"
    "     -e Upper endpoint of the theta window, in degrees (97 by default), <thetaMax> \n"
    "     -f Lower endpoint of the |deltaPhi| window, in degrees (0 by default), <deltaPhiMin> \n"
    "     -g Upper endpoint of the |deltaPhi| window, in degrees (180 by default), <deltaPhiMax> \n"
    "     -h Lower endpoint of the energy window, in keV (0 by default), <ElostMin> \n"
    "     -k Upper endpoint of the energy window, in keV (511 by default), <ElostMax> \n"
    " \n"
    " GAMOS Collaboration. CIEMAT 2009-11 \n"
    " mario.canadas@ciemat.es \n"
    " -------------------------- \n";
    exit(1);
  }

  for (int i = 1; i < argc; i++)
  {
  // Check for a switch (leading "-").
    if (argv[i][0] == '-')
    {
      switch (argv[i][1])
      {
        case 'a':
          d_axial = atof(argv[++i]);
          break;
        case 'd':
          d_FOV = atof(argv[++i]);
          break;
        case 'i':
          typeINfile = atoi(argv[++i]);
          break;
        case 'j':
          offsetX = atof(argv[++i]);
          break;
        case 'm':
          maxRingDiff = atoi(argv[++i]);
          break;
        case 'n':
          fileOUTname = argv[++i];
          break;
        case 'p':
          n_planes = atoi(argv[++i]); //rings or "direct planes"
          break;
        case 'r':
          n_bin = atoi(argv[++i]);
          break;
        case 't':
          n_ang = atoi(argv[++i]);
          break;
        case 'v':
          verbos = atoi(argv[++i]);
          break;
        case 'x':
          MAX_Coinci = (long unsigned int) atof(argv[++i]);
          break;
        case 'o':
          oType = atoi(argv[++i]);
          break;
        case 'b':
          scatteringOutOfCZT = atoi(argv[++i]);
          break;
        case 'c':
          thetaMin = atof(argv[++i]);
          break;
        case 'e':
          thetaMax = atof(argv[++i]);
          break;
        case 'f':
          deltaPhiMin = atof(argv[++i]);
          break;
        case 'g':
          deltaPhiMax = atof(argv[++i]);
          break;
        case 'h':
          ElostMin = atof(argv[++i]);
          break;
        case 'k':
          ElostMax = atof(argv[++i]);
          break;
        break;
      }
    }

    else
    {
      fileINname = argv[i];
    }
  }

  cout << "Input file: " << fileINname << endl;
  cout << "Parameters: planes = " << n_planes << "; bins = " << n_bin
   << "; ang_views = " << n_ang << endl;
  cout << "Dimensions: Transaxial FOV = " << d_FOV << "; Axial FOV = "
   << d_axial << " ; Transaxial_pix (mm) = " << d_FOV / (n_bin - 1)
   <<"; Plane_width (mm) = " << d_axial / (n_planes - 1) << endl;
  
  if (scatteringOutOfCZT == -1)
   cout << "True and scatter coincidences are considered" << endl;
  
  if (scatteringOutOfCZT == 0)
   cout << "Only true coincidences are considered (no scatter)" << endl;
  
  if (scatteringOutOfCZT == 1)
   cout << "Only scatter coincidences are considered (no true)" << endl;
  
  cout << "Interval of theta = [" << thetaMin << ", " << thetaMax
   << "] degrees" << endl;
  cout << "Interval of |deltaPhi| = [" << deltaPhiMin
   << ", " << deltaPhiMax << "] degrees" << endl;
  cout << "Energy window = [" << ElostMin
   << ", " << ElostMax << "] keV" << endl;  
  cout << "... " << endl;

  int i, j, k;

  if (maxRingDiff == -1) maxRingDiff = n_planes - 1;

  nbTotAxialPlanes = n_planes * n_planes;

  // total number of Axial planes (segments*planes) in STIR format
  if (oType == 1) nbTotAxialPlanes = (2*n_planes - 1 - maxRingDiff)
   * maxRingDiff + n_planes;

  // Initialize sino3D
  sino3D = new SINO_TYPE**[n_bin];

  for (i = 0; i < n_bin; i++)
  {
    sino3D[i] = new SINO_TYPE*[n_ang];

    for (j = 0; j < n_ang; j++)
    {
      sino3D[i][j] = new SINO_TYPE[nbTotAxialPlanes];

      for (k = 0; k < nbTotAxialPlanes; k++)
      {
        sino3D[i][j][k] = 0;
      }
    }
  }

  _PI = 2*asin(1);
  lor theLOR;

  // Protection to avoid incoherent values for the upper and lower endpoints
  //  of the windows (theta, |deltaPhi| and energy)
  if (thetaMin >= thetaMax)
  {
    cout << "Theta min is greater than or equal to theta max" << endl;
    exit(1);
  }

  if (deltaPhiMin >= deltaPhiMax)
  {
    cout << "deltaPhi min is greater than or equal to deltaPhi max" << endl;
    exit(1);
  }

  if (deltaPhiMin < 0 || deltaPhiMax < 0)
  {
    cout <<
     "deltaPhi min and deltaPhi max must be greater than or equal to 0 (absolute values)"
     << endl;
    exit(1);
  }

  if (ElostMin >= ElostMax)
  {
    cout << "Elost min is greater than or equal to Elost max" << endl;
    exit(1);
  }

  // open binary file "theFile"
  theINfile = fopen(fileINname, "r");

  if (theINfile == NULL)
  {
    printf("Error opening Geant4 file\n");
    exit (1);
  }

  theLOR = ReadData();

  while (!feof(theINfile) && !ferror(theINfile))
  // while (!feof(theINfile) && !ferror(theINfile) && nbTotCoincidences < 10000000)
  {
    if (verbos == 0 && nbTotCoincidences % 20000000 == 0)
    {
      cout << nbTotCoincidences / 1e6 << " million coincidences" << endl;
    }
    
    // For debugging - check if the Geant4 file is correctly read
    if (verbos >= 3)
    {
      cout << nbTotCoincidences << " LoR_p1: " << theLOR.p1.theta
       << " " << theLOR.p1.X_a << " " << theLOR.p1.Y_a
       << " " << theLOR.p1.Z_a << " " << theLOR.p1.E_a
       << " " << theLOR.p1.X_b << " " << theLOR.p1.Y_b
       << " " << theLOR.p1.Z_b << " " << theLOR.p1.E_b
       << "; LoR_p2: " << theLOR.p2.theta  << " " << theLOR.p2.X_a
       << " " << theLOR.p2.Y_a << " " << theLOR.p2.Z_a
       << " " << theLOR.p2.E_a << " " << theLOR.p2.X_b
       << " " << theLOR.p2.Y_b << " " << theLOR.p2.Z_b
       << " " << theLOR.p2.E_b << " " << theLOR.p2.scatterCoincidence
       << " " << theLOR.p2.deltaPhi << endl;

      if (nbTotCoincidences >= 10) exit(1);
    }

    // theLOR --> sino3D
    Load_sino3D(theLOR);

    theLOR = ReadData();
  }

  cout << "Total coincidences: " << nbTotCoincidences << endl;
  cout << "Accepted coincidences for the sinogram: " << nCoinciVal << endl;
  cout << "True coincidences: " << nbTrueTot << endl;
  cout << "Scatter coincidences: " << nbScatterTot << endl;
  cout << "Scatter fraction: " << (double) (100 * nbScatterTot /
   (nbScatterTot + nbTrueTot)) << "%" << endl;

  write_sino3D();

  // close file "theFile"
  fclose (theINfile);
}

void Load_sino3D(lor inpLOR)
{
  //for discretization on the crystal: int x1_i, x2_i, y1_i, y2_i;
  int z1_i, z2_i;

  float z1_abs = inpLOR.p1.Z_a+d_axial/2;
  float z2_abs = inpLOR.p2.Z_a+d_axial/2;
  double a, b, phi, dis;
  int phi_i, dis_i;
  int ring_diff;

  z1_i = (int) (n_planes * z1_abs / d_axial); //round --> n_planes+1 ...h
  z2_i = (int) (n_planes * z2_abs / d_axial);

  // control; if z_i out of range: return
  if ((z1_i < 0) || (z2_i < 0) || (z1_i >= n_planes) || (z2_i >= n_planes))
  return;
  
  // control; if x1, y1 == x2, y2: return
  if ((inpLOR.p1.X_a == inpLOR.p2.X_a) && (inpLOR.p1.Y_a == inpLOR.p2.Y_a))
   return;

  ring_diff = (int) fabs(z1_i - z2_i);

  // max ring difference; control:92
  if (ring_diff > maxRingDiff) return;

  a = (double) (inpLOR.p2.Y_a - inpLOR.p1.Y_a);
  b = (double) (inpLOR.p2.X_a - inpLOR.p1.X_a);

  if (a == 0.0)
  {
    phi = _PI*0.5;
  }

  else
  {
    phi = atan(b/a);
  }

  if (phi < 0) phi = phi + _PI;

  dis = inpLOR.p1.X_a * cos(phi) - inpLOR.p1.Y_a * sin(phi);

  // control; transaxial FOV
  if (fabs(dis) > d_FOV*0.5) return;

  dis = dis + d_FOV*0.5;

  // discret values:
  phi_i = (int) round((double) (n_ang - 1) * phi / _PI);
  dis_i = (int) round((double) (n_bin - 1) * dis / (double) d_FOV);

  // only possible "=" because 'round' check it..
  if ((phi_i >= n_ang) || (dis_i >= n_bin)) return;

  int zPos;

  if (oType == 0)
  {
    zPos = (z1_i*n_planes + z2_i);
  }

  else
  {
    if (z1_i >= z2_i)
    {
      zPos = (((2*n_planes - 1 - maxRingDiff - ring_diff)
       * (maxRingDiff - ring_diff))/2 + z2_i);
    }

    else
    {
      zPos = ((nbTotAxialPlanes) -
       ((2*n_planes - 1 - maxRingDiff - ring_diff + 1)
       * (maxRingDiff - ring_diff + 1))/ 2 + z1_i);
    }
  }

  // To select only true or only scatter coincidences
  if (scatteringOutOfCZT != -1
   && scatteringOutOfCZT != inpLOR.p2.scatterCoincidence) return;

  // To apply the theta window
  if (inpLOR.p1.theta < thetaMin || inpLOR.p1.theta > thetaMax
   || inpLOR.p2.theta < thetaMin || inpLOR.p2.theta > thetaMax) return;

  // To apply the |deltaPhi| window
  if (abs(inpLOR.p2.deltaPhi) < deltaPhiMin
   || abs(inpLOR.p2.deltaPhi) > deltaPhiMax) return;

  if (inpLOR.p2.scatterCoincidence == 0) nbTrueTot++;
  if (inpLOR.p2.scatterCoincidence == 1) nbScatterTot++;

  sino3D[dis_i][phi_i][zPos]++;
  nCoinciVal++;
}

lor ReadData()
{
  lor lorOUT;

  if (typeINfile == 0)
  {
    // %*c allows to read csv files 
    fscanf(theINfile,
     "%lf%*c %lf%*c %lf%*c %lf%*c %lf%*c %lf%*c %lf%*c %lf%*c %lf%*c %lf%*c %lf%*c %lf%*c %lf%*c %lf%*c %lf%*c %lf%*c %lf%*c %lf%*c %d%*c %lf%*c",
     &theta1, &X1_a, &Y1_a, &Z1_a, &E1_a, &X1_b, &Y1_b, &Z1_b, &E1_b,
     &theta2, &X2_a, &Y2_a, &Z2_a, &E2_a, &X2_b, &Y2_b, &Z2_b, &E2_b,
     &scatterCoincidence, &deltaPhi);

    // Data for the 1st annihilation photon
    lorOUT.p1.theta = theta1;
    lorOUT.p1.X_a = X1_a + offsetX;
    lorOUT.p1.Y_a = Y1_a;
    lorOUT.p1.Z_a = Z1_a;
    lorOUT.p1.E_a = E1_a;
    lorOUT.p1.X_b = X1_b + offsetX;
    lorOUT.p1.Y_b = Y1_b;
    lorOUT.p1.Z_b = Z1_b;
    lorOUT.p1.E_b = E1_b;
    lorOUT.p1.scatterCoincidence = scatterCoincidence;
    lorOUT.p1.deltaPhi = deltaPhi;
    
    // Data for the 2nd annihilation photon
    lorOUT.p2.theta = theta2;
    lorOUT.p2.X_a = X2_a + offsetX;
    lorOUT.p2.Y_a = Y2_a;
    lorOUT.p2.Z_a = Z2_a;
    lorOUT.p2.E_a = E2_a;
    lorOUT.p2.X_b = X2_b + offsetX;
    lorOUT.p2.Y_b = Y2_b;
    lorOUT.p2.Z_b = Z2_b;
    lorOUT.p2.E_b = E2_b;

    lorOUT.p2.scatterCoincidence = scatterCoincidence;

    // Difference of phi between the two annihilation photons
    lorOUT.p2.deltaPhi = deltaPhi;
    
    if (!feof(theINfile) && !ferror(theINfile)) nbTotCoincidences++;
    if (lorOUT.p2.scatterCoincidence == 0) nbTrueFile++;
    if (lorOUT.p2.scatterCoincidence == 1) nbScatterFile++;
  }

  return lorOUT;
}

void write_sino3D()
{
  char name_hv[512];
  char name_v[512];

  if (oType == 0)
  {
    strcpy(name_hv, fileOUTname);
    strcpy(name_v, fileOUTname);

    strcat(name_hv, ".hv");
    strcat(name_v, ".v");

    theOUTfile = fopen(name_hv, "w");

    fprintf(theOUTfile, "!INTERFILE := \n");
    fprintf(theOUTfile, "name of data file := %s.v\n", fileOUTname);
    fprintf(theOUTfile, "!GENERAL DATA := \n");
    fprintf(theOUTfile, "!GENERAL IMAGE DATA :=\n");
    fprintf(theOUTfile, "!type of data := tomographic\n");
    fprintf(theOUTfile, "!version of keys := 3.3\n");
    fprintf(theOUTfile, "!data offset in bytes := 0\n");
    fprintf(theOUTfile, "imagedata byte order := littleendian\n");
    fprintf(theOUTfile, "!PET STUDY (General) :=\n");
    fprintf(theOUTfile, "!PET data type := 3D-Sinogram\n");
    fprintf(theOUTfile, "process status := Reconstructed\n");
    fprintf(theOUTfile, "!number format := unsigned short\n");
    fprintf(theOUTfile, "!number of bytes per pixel := 2\n");
    fprintf(theOUTfile, "number of dimensions := 3\n");
    fprintf(theOUTfile, "matrix axis label [1] := x\n");
    fprintf(theOUTfile, "!matrix size [1] := %i\n", n_bin);
    fprintf(theOUTfile, "scaling factor (mm/pixel) [1] := %f\n",
     (float) (d_FOV / (n_bin - 1)));
    fprintf(theOUTfile, "matrix axis label [2] := y\n");
    fprintf(theOUTfile, "!matrix size [2] := %i\n", n_ang);
    fprintf(theOUTfile, "scaling factor (degree/pixel) [2] := %f\n",
     (float) (360. / (n_ang - 1)));
    fprintf(theOUTfile, "matrix axis label [3] := z\n");
    fprintf(theOUTfile, "!matrix size [3] := %i\n", n_planes*n_planes);
    fprintf(theOUTfile, "scaling factor (mm/pixel) [3] := %f\n",
     (float) (d_axial / (n_planes - 1)));
    fprintf(theOUTfile, "number of slices := %i\n", n_planes*n_planes);
    fprintf(theOUTfile, "number of time frames := 1\n");
    fprintf(theOUTfile, "image scaling factor[1] := 1\n");
    fprintf(theOUTfile, "data offset in bytes[1] := 0\n");
    fprintf(theOUTfile, "quantification units := 1\n");
    fprintf(theOUTfile, "!END OF INTERFILE := \n");

    fclose(theOUTfile);
  }

  else
  {
    strcpy(name_hv, fileOUTname);
    strcpy(name_v, fileOUTname);

    // STIR extension: .hs .s
    strcat(name_hv, ".hs");
    strcat(name_v, ".s");

    theOUTfile = fopen(name_hv, "w");

    fprintf(theOUTfile, "!INTERFILE := \n");
    fprintf(theOUTfile, "name of data file := %s.s\n", fileOUTname);
    fprintf(theOUTfile, "!GENERAL DATA := \n");
    fprintf(theOUTfile, "!GENERAL IMAGE DATA :=\n");
    fprintf(theOUTfile, "!type of data := PET\n");
    fprintf(theOUTfile, "imagedata byte order := littleendian\n");
    fprintf(theOUTfile, "!PET STUDY (General) :=\n");
    fprintf(theOUTfile, "!PET data type := Emission\n");
    fprintf(theOUTfile, "applied corrections := {arc correction}\n");
    fprintf(theOUTfile, "!number format := unsigned integer\n");
    fprintf(theOUTfile, "!number of bytes per pixel := 2\n");

    fprintf(theOUTfile, "number of dimensions := 4\n");
    fprintf(theOUTfile, "matrix axis label [4] := segment\n");
    fprintf(theOUTfile, "!matrix size [4] := %i\n", maxRingDiff*2 + 1);
    fprintf(theOUTfile, "matrix axis label [3] := axial coordinate\n");
    fprintf(theOUTfile, "!matrix size [3] := {");

    if (maxRingDiff == 0)
    {
      fprintf(theOUTfile, "%i}\n", n_planes);
    }

    else
    {
      for (int m = n_planes - maxRingDiff; m <= n_planes; m++)
      fprintf (theOUTfile, "%i,", m);

      for (int m = n_planes - 1; m > n_planes - maxRingDiff; m--)
      fprintf (theOUTfile, "%i,", m);
      fprintf (theOUTfile, "%i}\n", n_planes - maxRingDiff);
    }

    fprintf(theOUTfile, "matrix axis label [2] := view\n");
    fprintf(theOUTfile, "!matrix size [2] := %i\n", n_ang);
    fprintf(theOUTfile, "matrix axis label [1] := tangential coordinate\n");
    fprintf(theOUTfile, "!matrix size [1] := %i\n", n_bin);
    fprintf(theOUTfile, "minimum ring difference per segment := {");
    fprintf(theOUTfile, "%i", -maxRingDiff);

    for (int m =- maxRingDiff + 1; m <= maxRingDiff; m++)
     fprintf (theOUTfile, ",%i", m);

    fprintf (theOUTfile, "}\n");
    fprintf (theOUTfile, "maximum ring difference per segment := {");
    fprintf (theOUTfile, "%i", -maxRingDiff);

    for (int m = -maxRingDiff + 1; m <= maxRingDiff; m++) fprintf(theOUTfile,
     ",%i", m);

    fprintf(theOUTfile, "}\n");
    fprintf(theOUTfile, "inner ring diameter (cm) := %f\n", d_FOV/10);
    fprintf(theOUTfile, "average depth of interaction (cm) := 0.0001\n");
    fprintf(theOUTfile, "default bin size (cm) := %f\n",
     0.1*((float) d_FOV / ((float) n_bin - 1.)));
    fprintf(theOUTfile, "number of rings := %i\n", n_planes);
    fprintf(theOUTfile, "distance between rings (cm) := %f\n",
     0.1*(float) (d_axial / (n_planes - 1.))); // Axial pixel dimension
    fprintf(theOUTfile, "number of detectors per ring := %i\n", n_ang*2);
    fprintf(theOUTfile, "number of time frames := 1\n");
    fprintf(theOUTfile, "image scaling factor[1] := 1\n");
    fprintf(theOUTfile, "data offset in bytes[1] := 0\n");
    fprintf(theOUTfile, "quantification units := 1\n");
    fprintf(theOUTfile, "!END OF INTERFILE := \n");

    fclose(theOUTfile);
  }

  sino_line = (SINO_TYPE*) malloc(n_bin * n_ang * nbTotAxialPlanes *
   sizeof(SINO_TYPE));

  long unsigned int cont = 0;
  int i, j, k;

  for (k = 0; k < nbTotAxialPlanes; k++)
  {
    for (j = 0; j < n_ang; j++)
    {
      for (i = 0; i < n_bin; i++)
      {
        if (sino3D[i][j][k] >= 0) sino_line[cont] = sino3D[i][j][k];
        else sino_line[cont] = 0;
        cont++;
      }
    }
  }

  theOUTfile = fopen(name_v, "w");

  fwrite(sino_line, 1, n_bin * n_ang * nbTotAxialPlanes * sizeof(SINO_TYPE),
   theOUTfile);
  fclose(theOUTfile);
  free(sino_line);
}

// Free memory
void free_sino3D()
{
  int i, j;

  for (i = 0; i < n_bin; i++)
  {
    for (j = 0; j < n_ang; j++)
    {
      free(sino3D[i][j]);
    }

    free(sino3D[i]);
  }

  free(sino3D);
}
