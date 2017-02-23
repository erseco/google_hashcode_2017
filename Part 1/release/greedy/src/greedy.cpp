#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <vector>
#include <fstream>
#include <string>
#include "functions.h"
#include <sstream>

template <class T>
inline std::string to_string (const T& t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

int main(int argc, char const *argv[]) {
  if (argc < 2) {
    printf ("usage: %s <path_to_datafile>\n", argv[0]);
    exit (1);
  }
  struct timeval tp;
  gettimeofday(&tp, NULL);
  long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
  srand (ms);

  bool **pizza, **used;
  int rows, columns, min_ingredients, max_size;
  int numberOfCombinations;

  std::vector<combination> combinations;
  std::vector<slice> slices;

  readAndFill(argv[1], pizza, rows, columns, min_ingredients, max_size);
  initializeUsedMatrix(used, rows, columns);

  slices.reserve((rows*columns)/(min_ingredients*2));

  combination comb;
  for (int i = 1; i <= max_size; ++i){
    for (int j = 1; j <= max_size; ++j){
      if (j*i >= min_ingredients*2 && j*i <= max_size){
        comb.r = i;
        comb.c = j;
        combinations.push_back(comb);
      }
    }
  }

  numberOfCombinations = combinations.size();
  std::vector<combination> possibleCombinations;
  for (int i = 0; i < rows; ++i){
    for (int j = 0; j < columns; ++j){
      possibleCombinations.clear();
      // Check if cell is already used
      if (!used[i][j]){
        // Check each combination, and if possible, add to possibleCombinations
        for (int k = 0; k < numberOfCombinations; ++k){
          int c_row = combinations[k].r, c_col = combinations[k].c;
          if (i + c_row - 1 < rows && j + c_col - 1 < columns){ // Slice fits in the pizza
            int t = 0, m = 0;
            bool allAvailable = true;
            // Count mush and toms in the potencial slide, and check if all cells are available
            for (int l = i; l < i+c_row; ++l){
              for (int n = j; n < j+c_col; ++n){
                if (pizza[l][n]) ++m;
                else ++t;
                if(used[l][n]) allAvailable = false;
              }
            }
            if (m >= min_ingredients && t >= min_ingredients && allAvailable){ // Slide could be taken
              possibleCombinations.push_back(combinations[k]);
            }
          }
        }
        // If there is any possible slide, select one randomly
        if (!possibleCombinations.empty()){
          int sel = rand() % possibleCombinations.size();
          int sel_row = possibleCombinations[sel].r, sel_col = possibleCombinations[sel].c;
          // Create the new slice
          slice newSlice;
          newSlice.r1 = i;
          newSlice.r2 = i+sel_row-1;
          newSlice.c1 = j;
          newSlice.c2 = j+sel_col-1;
          // Set cells as used
          for (int k = newSlice.r1; k <= newSlice.r2; ++k){
            for (int l = newSlice.c1; l <= newSlice.c2; ++l){
              used[k][l] = true;
            }
          }
          // Add the new slice to the slices vector
          slices.push_back(newSlice);
        }
      }
    }
  }

  int score = 0;
  for (int i = 0; i < rows; ++i){
    for (int j = 0; j < columns; ++j){
      if (used[i][j]) ++score;
    }
  }

  std::string name(argv[1]);
  name = name.substr(9,name.length()-12);
  std::string s = "../sol/" + name + "/" + to_string(score) + " - " + to_string(ms);
  const char* path = s.c_str();
  std::ofstream ofs;
  ofs.open(path);
  ofs << slices.size() << std::endl;
  for (unsigned int i = 0; i < slices.size(); ++i){
    ofs << slices[i].r1 << " " << slices[i].c1 << " " << slices[i].r2 << " " << slices[i].c2 << std::endl;
  }
  ofs.close();

  printf("Score: %i", score);
  if (score == rows*columns) printf ("  --> OPTIMAL SOLUTION!!!!!!!\n");
  else printf("\n");

  freeSpace(pizza, used, rows);

  return 0;
}
