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

#include "Tangle2RunAction.hh"

#include "G4Run.hh"
#include "G4Threading.hh"

Tangle2RunAction::Tangle2RunAction()
{}

Tangle2RunAction::~Tangle2RunAction()
{}

void Tangle2RunAction::BeginOfRunAction(const G4Run*)
{}

void Tangle2RunAction::EndOfRunAction(const G4Run* run)
{
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;

  G4String runType;
  if (G4Threading::IsMasterThread()) {
    runType = "Global Run";
  } else {
    runType = "Local Run-";
  }

  G4cout
  << "\n----------------------End of " << runType << "------------------------"
  << "\n The run consists of " << nofEvents << " events."
  << "\n------------------------------------------------------------"
  << G4endl;
}
