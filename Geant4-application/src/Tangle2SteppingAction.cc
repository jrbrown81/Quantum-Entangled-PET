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

#include "Tangle2SteppingAction.hh"

#include "Tangle2Data.hh"

#include "G4Step.hh"
#include "G4VProcess.hh"
#include "G4MTRunManager.hh"
#include "G4EventManager.hh"
#include "G4TrackingManager.hh"
#include "G4SteppingManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4Gamma.hh"
#include "G4Threading.hh"

Tangle2SteppingAction::Tangle2SteppingAction()
: fComptonScatteringAnnihilationPhotonFound1(false)
, fComptonScatteringAnnihilationPhotonFound2(false)
, fTrackID1(0)
, fTrackID2(0)
, fParentID1(0)
, fTheta1(0.)
, fPhi1(0.)
{}

void Tangle2SteppingAction::BeginOfEventAction()
{
  fComptonScatteringAnnihilationPhotonFound1 = false;
  fComptonScatteringAnnihilationPhotonFound2 = false;
  fTrackID1 = -1;
  fTrackID2 = -1;
}

void Tangle2SteppingAction::EndOfEventAction()
{
}

namespace {
  void CalculateThetaPhi
  (const G4ThreeVector& v,
   const G4ThreeVector& z_axis,
   // Output quantities
   G4ThreeVector& y_axis,
   G4ThreeVector& x_axis,
   G4double& cosTheta,
   G4double& theta,
   G4double& phi)
  {
    cosTheta = v*z_axis;
    theta = std::acos(cosTheta);
    // Make y' perpendicular to global x-axis.
    y_axis = (z_axis.cross(G4ThreeVector(1,0,0))).unit();
    x_axis = y_axis.cross(z_axis);
    const G4ThreeVector ontoXYPlane = v.cross(z_axis);
    // ontoXYPlane is a vector in the xy-plane, but perpendicular to the
    // projection of the scattered photon, so
    const G4double projection_x = -ontoXYPlane*y_axis;
    const G4double projection_y = ontoXYPlane*x_axis;
    phi = std::atan2(projection_y,projection_x);
  }
}

//#define Tangle2SteppingActionPrinting
#define Tangle2SteppingActionConsistencyCheck

void Tangle2SteppingAction::UserSteppingAction(const G4Step* step)
{
  G4Track* track = step->GetTrack();

  if (track->GetDefinition() != G4Gamma::Gamma()) return;

  const G4VProcess* creatorProcess = track->GetCreatorProcess();
  if (creatorProcess == nullptr) return;
  if (creatorProcess->GetProcessName() != "annihil") return;

  G4StepPoint* preStepPoint = step->GetPreStepPoint();

  G4StepPoint* postStepPoint = step->GetPostStepPoint();
  const G4VProcess* postProcessDefinedStep = postStepPoint->GetProcessDefinedStep();
  if (postProcessDefinedStep == nullptr) return;

  if (postProcessDefinedStep->GetProcessName() != "compt") return;

  // This is an annihilation photon undergoing Compton scattering.

  ///////////////////////////////////////////////////////////////////////////
  if (!fComptonScatteringAnnihilationPhotonFound1) {

    // Only accept new photons
    if (track->GetTrackID() == fTrackID1 ||
        track->GetTrackID() == fTrackID2) return;

    // This the first annihilation photon undergoing its first Compton scattering.
  first_photon:

#ifdef Tangle2SteppingActionPrinting
    const G4VProcess* preProcessDefinedStep = preStepPoint->GetProcessDefinedStep();
    G4cout
    << "creator/pre/post process 1: "
    << creatorProcess->GetProcessName()
    << '/' << (preProcessDefinedStep? preProcessDefinedStep->GetProcessName(): "0")
    << '/' << postProcessDefinedStep->GetProcessName()
    << G4endl;
#endif

    fComptonScatteringAnnihilationPhotonFound1 = true;
    fTrackID1 = track->GetTrackID();

    fParentID1 = track->GetParentID();
    fPhoton1_z_axis = preStepPoint->GetMomentumDirection();

    const G4ThreeVector photon1_z_axis = preStepPoint->GetMomentumDirection();
    G4ThreeVector photon1_y_axis;
    G4ThreeVector photon1_x_axis;
    G4double cosTheta1;
    CalculateThetaPhi
    (postStepPoint->GetMomentumDirection(),
     fPhoton1_z_axis,
     photon1_y_axis,
     photon1_x_axis,
     cosTheta1,
     fTheta1,
     fPhi1);

#ifdef Tangle2SteppingActionPrinting
    G4cout
    << "\n  1st photon found: track ID: " << track->GetTrackID()
    << "\n  parent ID: " << track->GetParentID()
    << "\n  preStepPointPosition: " << preStepPoint->GetPosition()
    << "\n  postStepPointPosition: " << postStepPoint->GetPosition()
    << "\n  preStepPointMomentum: " << preStepPoint->GetMomentum()
    << "\n  postStepPointMomentum: " << postStepPoint->GetMomentum()
    << "\n  preStepPolarisation: " << preStepPoint->GetPolarization()
    << "\n  postStepPolarisation: " << postStepPoint->GetPolarization()
    << "\n  theta1: " << theta1
    << "\n  phi1: " << phi1
    << G4endl;
#endif  // Tangle2SteppingActionPrinting

    ///////////////////////////////////////////////////////////////////////////
  } else if (!fComptonScatteringAnnihilationPhotonFound2) {
    ///////////////////////////////////////////////////////////////////////////

    // One Compton scattering annihilation photon has already been found. We
    // need this Compton scattering annihilation photon to have a different
    // track Id - then we can assume it's the second annihilation photon.

    if (track->GetTrackID() != fTrackID1) {

      // Unless it has a different parent ID. That would mean we didn't pick up
      // the second photon (perhaps it left the world without scattering). So
      // treat this as a first photon of another annihilation.
      if (track->GetParentID() != fParentID1) {
        goto first_photon;
      }

#ifdef Tangle2SteppingActionPrinting
      const G4VProcess* preProcessDefinedStep = preStepPoint->GetProcessDefinedStep();
      G4cout
      << "creator/pre/post process 2: "
      << creatorProcess->GetProcessName()
      << '/' << (preProcessDefinedStep? preProcessDefinedStep->GetProcessName(): "0")
      << '/' << postProcessDefinedStep->GetProcessName()
      << G4endl;
#endif

      // This the second annihilation photon undergoing its first Compton scattering.
      fComptonScatteringAnnihilationPhotonFound2 = true;
      fTrackID2 = track->GetTrackID();

      const G4ThreeVector photon2_z_axis = preStepPoint->GetMomentumDirection();
      G4ThreeVector photon2_y_axis;
      G4ThreeVector photon2_x_axis;
      G4double cosTheta2;
      G4double theta2;
      G4double phi2;
      CalculateThetaPhi
      (postStepPoint->GetMomentumDirection(),
       photon2_z_axis,
       photon2_y_axis,
       photon2_x_axis,
       cosTheta2,
       theta2,
       phi2);

#ifdef Tangle2SteppingActionPrinting
      G4cout
      << "\n  2nd photon found: track ID: " << track->GetTrackID()
      << "\n  parent ID: " << track->GetParentID()
      << "\n  preStepPointPosition: " << preStepPoint->GetPosition()
      << "\n  postStepPointPosition: " << postStepPoint->GetPosition()
      << "\n  preStepPointMomentum: " << preStepPoint->GetMomentum()
      << "\n  postStepPointMomentum: " << postStepPoint->GetMomentum()
      << "\n  preStepPolarisation: " << preStepPoint->GetPolarization()
      << "\n  postStepPolarisation: " << postStepPoint->GetPolarization()
      << "\n  theta2: " << theta2
      << "\n  phi2: " << phi2
      << G4endl;
#endif  // Tangle2SteppingActionPrinting

#ifdef Tangle2SteppingActionConsistencyCheck
      if (track->GetParentID() != fParentID1) {
        G4cout
        << "\n  Annihilation photons do not have the same parent ID"
        << "\n  track/parent IDs: " << fTrackID1 << '/' << fParentID1
        << ',' << track->GetTrackID() << '/' << track->GetParentID()
        << G4endl;
        //Reset for further possible annihilations in this event.
        fComptonScatteringAnnihilationPhotonFound1 = false;
        fComptonScatteringAnnihilationPhotonFound2 = false;
        return;
      }
      const G4double dotProduct = preStepPoint->GetMomentumDirection().unit()*fPhoton1_z_axis;
      if (dotProduct > -0.99) {
        G4cout <<
        "\n  Annihilation photons not in opposite directions: dot product: "
        << dotProduct
        << G4endl;
        //Reset for further possible annihilations in this event.
        fComptonScatteringAnnihilationPhotonFound1 = false;
        fComptonScatteringAnnihilationPhotonFound2 = false;
        return;
      }
#endif // Tangle2SteppingActionConsistencyCheck

      G4MUTEXLOCK(&Tangle2::outFileMutex);  // Lock while writing
      static G4bool first = true;
      if (first) {
        first = false;
        Tangle2::outFile << "#,theta1,phi1,theta2,phi2\n";
      }
      Tangle2::outFile
      << ',' << fTheta1
      << ',' << fPhi1
      << ',' << theta2
      << ',' << phi2
      << std::endl;
      G4MUTEXUNLOCK(&Tangle2::outFileMutex);

      //Reset for further possible annihilations in this event.
      fComptonScatteringAnnihilationPhotonFound1 = false;
      fComptonScatteringAnnihilationPhotonFound2 = false;
    }
  }
  ///////////////////////////////////////////////////////////////////////////

  return;
}
