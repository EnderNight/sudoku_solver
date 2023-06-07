#include "config.h"

#include <Magick++.h>
#include <Magick++/Color.h>
#include <Magick++/Drawable.h>
#include <Magick++/Functions.h>
#include <Magick++/Include.h>
#include <MagickCore/magick-type.h>
#include <cctype>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

int iterations = 0;
int knighX[8] = {2, 1, -1, -2, -2, -1, 1, 2};
int knighY[8] = {1, 2, 2, 1, -1, -2, -2, -1};

void save_image(int **grid, int **orig_grid, std::string filename) {

  Magick::Image image("1001x1001", "white");

  image.magick("PNG");

  std::vector<Magick::Drawable> borders, boxes, inner_boxes, orig_numbers,
      numbers;

  borders.push_back(Magick::DrawableStrokeColor("black"));
  borders.push_back(Magick::DrawableStrokeWidth(5));
  borders.push_back(Magick::DrawableStrokeAntialias(false));
  borders.push_back(Magick::DrawableLine(2, 0, 2, 1000));
  borders.push_back(Magick::DrawableLine(998, 0, 998, 1000));
  borders.push_back(Magick::DrawableLine(0, 2, 1000, 2));
  borders.push_back(Magick::DrawableLine(0, 998, 1000, 998));

  boxes.push_back(Magick::DrawableStrokeColor("black"));
  boxes.push_back(Magick::DrawableStrokeWidth(5));
  boxes.push_back(Magick::DrawableStrokeAntialias(false));
  // Vertical
  boxes.push_back(Magick::DrawableLine(334, 0, 334, 1000));
  boxes.push_back(Magick::DrawableLine(666, 0, 666, 1000));
  // Horizontal
  boxes.push_back(Magick::DrawableLine(0, 334, 1000, 334));
  boxes.push_back(Magick::DrawableLine(0, 666, 1000, 666));

  inner_boxes.push_back(Magick::DrawableStrokeColor("black"));
  inner_boxes.push_back(Magick::DrawableStrokeWidth(3));
  inner_boxes.push_back(Magick::DrawableStrokeAntialias(false));
  // Veritcal
  inner_boxes.push_back(Magick::DrawableLine(113, 0, 113, 1000));
  inner_boxes.push_back(Magick::DrawableLine(223, 0, 223, 1000));
  inner_boxes.push_back(Magick::DrawableLine(445, 0, 445, 1000));
  inner_boxes.push_back(Magick::DrawableLine(555, 0, 555, 1000));
  inner_boxes.push_back(Magick::DrawableLine(777, 0, 777, 1000));
  inner_boxes.push_back(Magick::DrawableLine(887, 0, 887, 1000));
  // Horizontal
  inner_boxes.push_back(Magick::DrawableLine(0, 113, 1000, 113));
  inner_boxes.push_back(Magick::DrawableLine(0, 223, 1000, 223));
  inner_boxes.push_back(Magick::DrawableLine(0, 445, 1000, 445));
  inner_boxes.push_back(Magick::DrawableLine(0, 555, 1000, 555));
  inner_boxes.push_back(Magick::DrawableLine(0, 777, 1000, 777));
  inner_boxes.push_back(Magick::DrawableLine(0, 887, 1000, 887));

  int x = 30, y = 95;

  orig_numbers.push_back(Magick::DrawableStrokeColor("black"));
  orig_numbers.push_back(Magick::DrawableFont("Calibri"));
  orig_numbers.push_back(Magick::DrawablePointSize(120));
  orig_numbers.push_back(Magick::DrawableFillColor("black"));

  numbers.push_back(Magick::DrawableStrokeColor("red"));
  numbers.push_back(Magick::DrawableFont("Calibri"));
  numbers.push_back(Magick::DrawablePointSize(120));
  numbers.push_back(Magick::DrawableFillColor("red"));

  for (int i = 0; i < 9; ++i) {
    for (int j = 0; j < 9; ++j) {
      if (orig_grid[i][j] == 0) {
        if (grid[i][j] != 0)
          numbers.push_back(
              Magick::DrawableText(x, y, std::to_string(grid[i][j])));
      } else
        orig_numbers.push_back(
            Magick::DrawableText(x, y, std::to_string(grid[i][j])));
      x += 107;

      if (j == 2 || j == 5)
        x += 5;
      else
        x += 3;
    }
    y += 107;
    x = 30;

    if (i == 2 || i == 5)
      y += 5;
    else
      y += 3;
  }

  image.draw(borders);
  image.draw(boxes);
  image.draw(inner_boxes);
  image.draw(orig_numbers);
  image.draw(numbers);

  image.write(filename + ".png");
}

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

int **copy_grid(int **grid) {

  int **res = 0;

  res = new int *[9];

  for (int i = 0; i < 9; ++i) {
    res[i] = new int[9];
    for (int j = 0; j < 9; ++j) {
      res[i][j] = grid[i][j];
    }
  }

  return res;
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

bool verify_consec(int **grid, int i, int j) {

  int num = grid[i][j];

  if (i > 0 && (grid[i - 1][j] != 0 &&
                (grid[i - 1][j] == num - 1 || grid[i - 1][j] == num + 1)))
    return false;

  if (i < 8 && (grid[i + 1][j] != 0 &&
                (grid[i + 1][j] == num - 1 || grid[i + 1][j] == num + 1)))
    return false;

  if (j > 0 && (grid[i][j - 1] != 0 &&
                (grid[i][j - 1] == num - 1 || grid[i][j - 1] == num + 1)))
    return false;

  if (j < 8 && (grid[i][j + 1] != 0 &&
                (grid[i][j + 1] == num - 1 || grid[i][j + 1] == num + 1)))
    return false;

  return true;
}

bool rule_consec(int **grid) {

  bool res = true;
  int i = 0, j = 0;

  while (res && i < 9) {
    while (res && j < 9) {
      // std::cout << "Box: " << i << ", " << j << "\n";
      if (grid[i][j] != 0)
        res = verify_consec(grid, i, j);
      ++j;
    }
    j = 0;
    ++i;
  }

  return res;
}

bool verify_king(int **grid, int i, int j) {

  bool res = true;

  for (int k = i - 1; k < i + 2; ++k) {
    if (i > 0 && i < 8) {
      for (int l = j - 1; l < j; ++l) {
        if (j > 0 && j < 8)
          res &= grid[k][l] != grid[i][j];
      }
    }
  }

  return res;
}

bool rule_king(int **grid) {

  bool res = true;
  int i = 0, j = 0;

  while (res && i < 9) {
    while (res && j < 9) {
      if (grid[i][j] != 0)
        res = verify_king(grid, i, j);
      ++j;
    }
    j = 0;
    ++i;
  }

  return res;
}

bool verify_knight(int **grid, int x, int y) {

  int i = 0, n_x, n_y;
  bool res = true;

  while (res && i < 8) {
    n_x = x + knighX[i];
    n_y = y + knighY[i];

    if (n_x >= 0 && n_y >= 0 && n_x < 9 && n_y < 9)
      res = grid[n_x][n_y] != grid[x][y];
    ++i;
  }

  return res;
}

bool rule_knight(int **grid) {

  bool res = true;
  int i = 0, j;

  while (res && i < 9) {
    j = 0;
    while (res && j < 9) {
      if (grid[i][j] != 0)
        res = verify_knight(grid, i, j);
      ++j;
    }
    ++i;
  }

  return res;
}

bool verify_grid(int **grid, bool consec, bool king, bool knight) {

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

  if (res && consec)
    res = rule_consec(grid);

  if (res && king)
    res = rule_king(grid);

  if (res && knight)
    res = rule_knight(grid);

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

bool solve(int **grid, bool consec, bool king, bool knight) {

  int i = 0, j = 0;

  if (find_next_zero(grid, i, j)) {

    for (int k = 1; k < 10; ++k) {
      grid[i][j] = k;
      ++iterations;

      if (iterations % 10 == 0)
        std::cout << "\riterations: " << iterations;

      if (verify_grid(grid, consec, king, knight) &&
          solve(grid, consec, king, knight))
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

void print_usage(const char **argv) {
  std::cout << argv[0] << " version " << Solver_VERSION_MAJOR << "."
            << Solver_VERSION_MINOR << "\n";
  std::cout
      << "Usage: " << argv[0]
      << " <sudoku grid> <consec_rule> <king_rule> <knight_rule>"
      << "\n"
      << "A not recognized rule option (not 1 or 0) will count as 0 was passed."
      << std::endl;
}

uint64_t time_ms(void) {
  return duration_cast<std::chrono::milliseconds>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
}

int main(int argc, const char *argv[]) {

  // Verify arguments
  if (argc != 5) {
    print_usage(argv);
    return 1;
  }

  try {
    // Init ImageMagick
    Magick::InitializeMagick(argv[0]);

    // Check if source grid exists
    bool check;
    std::ifstream file = open_grid(argv[1], check);
    uint64_t save_time = time_ms(), start_time = time_ms(),
           solve_time = time_ms();

    if (check) {
      // Generate a int[][] grid
      int **grid = gen_grid(file), **orig_grid = copy_grid(grid);
      std::string filename = argv[1], consec = argv[2], king = argv[3],
                  knight = argv[4];

      // Generate original grid
      save_image(orig_grid, orig_grid, filename + "_original");

      // if (verify_grid(grid, true, true)) {
      if (verify_grid(grid, consec.compare("1") == 0, king.compare("1") == 0,
                      knight.compare("1") == 0)) {
        std::cout << "Solving...\n";
        start_time = time_ms();

        // Solve the grid using backtracking
        if (solve(grid, consec.compare("1") == 0, king.compare("1") == 0,
                  knight.compare("1") == 0)) {
          solve_time = time_ms() - start_time;

          start_time = time_ms();
          std::cout << std::endl;

          // Write the grid in a '.result' file
          std::cout << "Saving solved grid in '" << filename << ".result'"
                    << std::endl;
          save_grid(filename, grid);

          // Generate an image containing the solved grid
          std::cout << "Generating solved grid image '" << filename
                    << "_result.png'" << std::endl;
          save_image(grid, orig_grid, filename + "_result");
          save_time = time_ms() - start_time;
        } else {
          std::cout << std::endl;
          std::cout << "Grid not solvable\n";
        }
      } else {
        std::cout << "Errors in grid\n";
      }
      std::cout << "\nTotal iterations: " << iterations << "\n"
                << "Solve time: " << solve_time << "ms"
                << ", save time: " << save_time << "ms"
                << std::endl;

    } else {
      return 1;
    }

    return 0;

  } catch (std::exception &error_) {
    std::cout << "Caught exception: " << error_.what() << std::endl;
    return 1;
  }

  return 0;
}
