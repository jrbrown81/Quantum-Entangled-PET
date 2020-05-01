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

#include "Tangle2PrimaryGeneratorAction.hh"

#include "G4GeneralParticleSource.hh"

Tangle2PrimaryGeneratorAction::Tangle2PrimaryGeneratorAction()
{
  fpParticleGun  = new G4GeneralParticleSource;
}

Tangle2PrimaryGeneratorAction::~Tangle2PrimaryGeneratorAction()
{
  delete fpParticleGun;
}

void Tangle2PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  fpParticleGun->GeneratePrimaryVertex(anEvent);
}
