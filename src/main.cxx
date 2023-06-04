#include "config.h"

#include <cctype>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>

std::ifstream open_grid(const std::string &path, bool &check) {

  std::ifstream file(path);

  check = true;

  // Verify if file exists
  if (!file.is_open()) {
    std::cout << "Error: file '" << path << "' not found\n";
    check = false;
  }

  return file;
}

int **gen_grid(std::ifstream &file) {

  std::string line;
  int **grid = 0, i = 0, j = 0;

  grid = new int *[9];

  // Create grid
  while (std::getline(file, line)) {
    if (isdigit(line[0])) {

      grid[i] = new int[9];

      for (std::size_t k = 0; k < line.length(); ++k) {
        if (isdigit(line[k])) {
          grid[i][j] = line[k] - '0';
          ++j;
        }
      }
      j = 0;
      ++i;
    }
  }

  return grid;
}

bool verify_line(int **grid, int l) {

  bool res = true, verif_array[9] = {false};

  for (int i = 0; i < 9; ++i) {
    if (grid[l][i] != 0) {
      if (!verif_array[grid[l][i] - 1]) {
        verif_array[grid[l][i] - 1] = true;
      } else
        res = false;
    }
  }
  return res;
}

bool verify_column(int **grid, int c) {

  bool res = true, verif_array[9] = {false};

  for (int i = 0; i < 9; ++i) {
    if (grid[i][c] != 0) {
      if (!verif_array[grid[i][c] - 1]) {
        verif_array[grid[i][c] - 1] = true;
      } else
        res = false;
    }
  }

  return res;
}

bool verify_box(int **grid, int l, int c) {

  bool res = true, verif_array[9] = {false};

  for (int i = l; i < l + 3; ++i) {
    for (int j = c; j < c + 3; ++j) {
      if (grid[i][j] != 0) {
        if (!verif_array[grid[i][j] - 1]) {
          verif_array[grid[i][j] - 1] = true;
        } else
          res = false;
      }
    }
  }

  return res;
}

bool verify_grid(int **grid) {

  bool res = true;
  int i = 0, j = 0;

  while (res && i < 9) {
    res = verify_line(grid, i);
    ++i;
  }

  i = 0;
  while (res && i < 9) {
    res = verify_column(grid, i);
    ++i;
  }

  i = 0;
  while (res && i < 3) {
    while (res && j < 3) {
      res = verify_box(grid, i * 3, j * 3);
      ++j;
    }
    j = 0;
    ++i;
  }

  return res;
}

bool find_next_zero(int **grid, int &i, int &j) {

  bool res = false;

  i = 0, j = 0;

  while (!res && i < 9) {
    while (!res && j < 9) {
      if (grid[i][j] == 0) {
        res = true;
      } else {
        ++j;
      }
    }

    if (!res) {
      j = 0;
      ++i;
    }
  }

  return res;
}

bool solve(int **grid) {

  int i = 0, j = 0;

  if (find_next_zero(grid, i, j)) {

    for (int k = 1; k < 10; ++k) {
      grid[i][j] = k;

      if (verify_grid(grid) && solve(grid))
        return true;

      grid[i][j] = 0;
    }

    return false;
  }

  return true;
}

void save_grid(const std::string &path, int **grid) {

  std::ofstream file(path + ".result");

  for (int i = 0; i < 9; ++i) {
    for (int j = 0; j < 9; ++j) {
      file << grid[i][j];
      if (j == 2 || j == 5)
        file << " ";
    }

    file << "\n";
    if (i == 2 || i == 5)
      file << "\n";
  }

  file.close();
}

void print_grid(int **grid) {

  for (int k = 0; k < 9; ++k) {
    for (int l = 0; l < 9; ++l) {
      std::cout << grid[k][l];
      if (l == 2 || l == 5)
        std::cout << " ";
    }
    std::cout << "\n";
    if (k == 2 || k == 5)
      std::cout << "\n";
  }
}

void print_usage(char **argv) {
  std::cout << argv[0] << " version " << Solver_VERSION_MAJOR << "."
            << Solver_VERSION_MINOR << "\n";
  std::cout << "Usage: " << argv[0] << " <sudoku grid>"
            << "\n";
}

int main(int argc, char *argv[]) {

  // Verify arguments
  if (argc != 2) {
    print_usage(argv);
    return 1;
  }

  bool check;
  std::ifstream file = open_grid(argv[1], check);

  if (check) {
    int **grid = gen_grid(file);

    if (verify_grid(grid)) {
      std::cout << "Solving...\n";

      if (solve(grid))
        save_grid(argv[1], grid);
      else
        std::cout << "Grid not solvable\n";
    } else
      std::cout << "Errors in grid\n";

  } else {
    return 1;
  }

  return 0;
}
