#ifndef TANGLE2DATA_HH
#define TANGLE2DATA_HH

#include <fstream>
#include "G4Threading.hh"

namespace Tangle2 {
  extern std::ofstream outFile;
  extern G4Mutex outFileMutex;
}

#endif
