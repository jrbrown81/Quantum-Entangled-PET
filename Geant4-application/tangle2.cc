// *******************************************************************
// * License and Disclaimer                                          *
// *                                                                 *
// * This software is copyright of Geant4 Associates International   *
// * Ltd (hereafter 'G4AI'). It is provided under the terms and      *
// * conditions described in the file 'LICENSE' included in the      *
// * software system.                                                *
// * Neither the authors of this software system nor G4AI make any   *
// * representation or warranty, express or implied, regarding this  *
// * software system or assume any liability for its use.            *
// * Please see the file 'LICENSE' for full disclaimer and the       *
// * limitation of liability.                                        *
// *******************************************************************
// $Id$
// John Allison  22nd May 2017

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif
#include "G4PhysListFactory.hh"
#include "Tangle2DetectorConstruction.hh"
#include "G4EmLivermorePolarizedPhysics.hh"
#include "G4DecayPhysics.hh"
#include "Tangle2ActionInitialization.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "Tangle2Data.hh"

#include <cstdio>

int main(int argc,char** argv)
{
  // Do this first to capture all output
  G4UIExecutive* ui = new G4UIExecutive(argc, argv);

  Tangle2::outFile.open("outFile.csv");

  // Choose the Random engine
  G4Random::setTheEngine(new CLHEP::RanecuEngine);

  int seed = time(0);
  seed |= 1;  // Make sure it's odd
//  seed = 1502758967;  // Activate this line with a fixed seed if desired
  CLHEP::HepRandom::setTheSeed(seed);
  G4cout << "Random engine seeded with " << seed << G4endl;

#ifdef G4MULTITHREADED
  G4MTRunManager* runManager = new G4MTRunManager;
#else
  G4RunManager* runManager = new G4RunManager;
#endif

  runManager->SetUserInitialization(new Tangle2DetectorConstruction);

  G4int verbose;
  G4PhysListFactory factory;
  G4VModularPhysicsList* physList = factory.GetReferencePhysList("FTFP_BERT");
  physList->SetVerboseLevel(verbose = 1);
  physList->ReplacePhysics(new G4EmLivermorePolarizedPhysics);
  physList->ReplacePhysics(new G4DecayPhysics);
  runManager->SetUserInitialization(physList);

  runManager->SetUserInitialization(new Tangle2ActionInitialization);

  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  UImanager->ApplyCommand("/control/execute vis.mac");

  ui->SessionStart();

  delete ui;
  delete visManager;
  delete runManager;
}
