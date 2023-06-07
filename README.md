# sudoku_solver
A sudoku solver using backtracking and an image generator using Magick++

# Installation

Make sure that you have gcc installed on your machine as well as Magick++ and CMake with version 3.26 or higher.

Then run:
```command
mkdir build && pushd build && PARALLEL_LEVEL=4 cmake -GNinja .. && popd && ninja -C build && make install
```

# Usage

```sudoku_solver <sudoku_grid> <consec> <king> <knight>```

Where sudoku_grid is the input grid in a text file and consec, king and knight are boolean values indicating if the related rule must be used when solving the algorithm.

# Issues

- Only basic error checking when reading the input grid file (no format error checking)
- Hard-coded image generation
- Really slow solving algorithm (recursive backtracking instead of iterative)

# TODO

This is written in C++ but it doesn't anything from it (I do not count strings and primitive stuff).
- [ ] Use classes and C++ stuff
