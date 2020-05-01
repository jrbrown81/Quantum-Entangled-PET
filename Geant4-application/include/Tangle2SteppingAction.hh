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

#ifndef AnnihilationPhotonsSteppingAction_hh
#define AnnihilationPhotonsSteppingAction_hh

#include "Tangle2VSteppingAction.hh"

#include "G4ThreeVector.hh"

class Tangle2SteppingAction: public Tangle2VSteppingAction
{
public:
  Tangle2SteppingAction();
  virtual void BeginOfEventAction();
  virtual void UserSteppingAction(const G4Step*);
  virtual void EndOfEventAction();

private:

  // These are used to remember quantities from call to call of UserSteppingAction
  G4bool fComptonScatteringAnnihilationPhotonFound1;
  G4bool fComptonScatteringAnnihilationPhotonFound2;
  G4int fTrackID1;
  G4int fTrackID2;
  G4int fParentID1;
  G4ThreeVector fPhoton1_z_axis;
  G4double fTheta1;
  G4double fPhi1;
};

#endif
