
tangle2/README.txt  John Allison  May 2017

An example of the use of quantum entanglement of positron
annihilation photons

- A simple cube of water of side 4 m - see Tangle2DetectorContruction.cc.
- Positrons of zero energy are generated at the centre of the cube - see gps.mac.
- G4EmLivermorePolarizedPhysics is selected - see tangle2.cc.

For a description of the implementation of entanglement see G4VEntanglementClipBoard.hh. It uses a C++ shared pointer to create a "clip board" so that the two photons can communicate. What Geant4 decides to do to one influences what should happen to the other. The specific clip board is G4eplusAnnihilationEntanglementClipBoard.hh.

To run the application:

- Create a working directory:

mkdir build

- Go to the working directory

cd build

- Build the Geant4 application:

cmake ..

- Compile the application:

make -j8

- Run the simulation:

./tangle2 vis.mac

Data are recorded in a csv file:

theta1,phi1,theta2,phi2

where theta1/2 and phi1/2 are the polar and azimuthal scattering angles of annihilation gamma1/2, respectively.
