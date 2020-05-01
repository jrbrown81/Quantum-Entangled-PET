#include "Tangle2Data.hh"

std::ofstream Tangle2::outFile;
G4Mutex Tangle2::outFileMutex = G4MUTEX_INITIALIZER;
