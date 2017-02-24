#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

#include "stdio.h"
#include "stdlib.h"

struct combination{
  int r, c;
};

struct slice{
  int r1, r2, c1, c2;
};

// Reads an data file and fills all the variables
bool readAndFill(const char* path, bool** &pizza, int &rows, int &columns, int &min_ingredients, int &max_size);

// Frees dynamic memory
void freeSpace(bool** pizza, bool** used, int rows);

// Allocates and initializes de "used" matrix
void initializeUsedMatrix(bool** &used, int rows, int columns);

#endif
