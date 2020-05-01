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

#include "Tangle2EventAction.hh"

#include "Tangle2RunAction.hh"
#include "Tangle2VSteppingAction.hh"

Tangle2EventAction::Tangle2EventAction
(Tangle2VSteppingAction* onePhotonSteppingAction)
: fpTangle2VSteppingAction(onePhotonSteppingAction)
{}

Tangle2EventAction::~Tangle2EventAction()
{}

void Tangle2EventAction::BeginOfEventAction(const G4Event*)
{
  fpTangle2VSteppingAction->BeginOfEventAction();
}

void Tangle2EventAction::EndOfEventAction(const G4Event*)
{   
  fpTangle2VSteppingAction->EndOfEventAction();
}
