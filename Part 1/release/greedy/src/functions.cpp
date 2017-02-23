#include "functions.h"

bool readAndFill(const char* path, bool** &pizza, int &rows, int &columns, int &min_ingredients, int &max_size){
  FILE *f;
  f = fopen(path, "r");
  if (f == NULL){
    printf("File was not found\n");
    return false;
  }
  if(fscanf (f, "%i", &rows) != 1){
    printf("Error reading file\n");
    return false;
  }
  if(fscanf (f, "%i", &columns) != 1){
    printf("Error reading file\n");
    return false;
  }
  if(fscanf (f, "%i", &min_ingredients) != 1){
    printf("Error reading file\n");
    return false;
  }
  if(fscanf (f, "%i", &max_size) != 1){
    printf("Error reading file\n");
    return false;
  }
  pizza = new bool*[rows];
  for(int i = 0; i < rows; ++i){
    pizza[i] = new bool[columns];
  }

  char c;
  for (int i = 0; i < rows; ++i){
    for (int j = 0; j < columns; ++j){
      if(fscanf (f, " %c", &c) != 1){
        printf("Error reading file\n");
        return false;
      }
      if (c == 'M') pizza[i][j] = true;
      else if (c == 'T') pizza[i][j] = false;
      else{
        printf("Error reading file\n");
        return false;
      }
    }
  }
  return true;
}

void initializeUsedMatrix(bool** &used, int rows, int columns) {
  used = new bool*[rows];
  for(int i = 0; i < rows; ++i){
    used[i] = new bool[columns];
  }

  for (int i = 0; i < rows; ++i){
    for (int j = 0; j < columns; ++j){
      used[i][j] = false;
    }
  }
}

void freeSpace(bool** pizza, bool** used, int rows){
  for(int i = 0; i < rows; ++i){
    delete[] pizza[i];
  }
  delete[] pizza;

  for(int i = 0; i < rows; ++i){
    delete[] used[i];
  }
  delete[] used;
}
