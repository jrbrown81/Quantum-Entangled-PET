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

#include "Tangle2DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Orb.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4VisAttributes.hh"

Tangle2DetectorConstruction::Tangle2DetectorConstruction()
: fpWater_log(nullptr)
{ }

Tangle2DetectorConstruction::~Tangle2DetectorConstruction()
{ }

G4VPhysicalVolume* Tangle2DetectorConstruction::Construct()
{
  // Useful names
  G4String name, symbol;
  G4bool checkOverlaps = true;

  // Materials
  G4NistManager* nist = G4NistManager::Instance();
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  G4Material* water_mat = nist->FindOrBuildMaterial("G4_WATER");

  name = "World";
  G4VSolid* world = new G4Box(name,2.*m,2.*m,2.*m);
  G4LogicalVolume* world_log = new G4LogicalVolume(world,world_mat,name);
  world_log->SetVisAttributes(G4VisAttributes::GetInvisible());
  G4VPhysicalVolume* physWorld = new G4PVPlacement(G4Transform3D(),world_log,name,0,false,0);

  name = "Water";
  G4VSolid* water = new G4Box(name, 1.*m,1.*m,1.*m);
  fpWater_log = new G4LogicalVolume(water,water_mat,name);
  new G4PVPlacement(G4Transform3D(),fpWater_log,name,world_log,false,0,checkOverlaps);

  return physWorld;
}
