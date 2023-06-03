#include "config.h"

#include <iostream>

int main(int argc, char *argv[]) {

  if (argc != 2) {
    std::cout << argv[0] << " version " << Solver_VERSION_MAJOR << "."
              << Solver_VERSION_MINOR << "\n";
    std::cout << "Usage: " << argv[0] << " <sudoku grid>"
              << "\n";
    return 1;
  }

  return 0;
}
