#include "lector.h"

int tamanio(const char *ruta){
  int tam;
  FILE *fichero;
  fichero = fopen(ruta, "r");
  if (fichero == NULL){
    printf("No se encontró el fichero\n");
    exit(1);
  }
  if(fscanf (fichero, "%i", &tam) != 1){
    printf("Error al leer archivo\n");
    exit(1);
  }
  fclose(fichero);
  return tam;
}

void leer(const char *ruta, int *numberofvideos, int *numberofendpoints,
          int *numberofrequests, int *numberofcacheservers, int *maxsizeofcache,
          int *videos, int *endpoints, int *caches, int *requests){


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






  int tam;
  int i, j;
  FILE *fichero;
  fichero = fopen(ruta, "r");
  if (fichero == NULL){
    printf("No se encontró el fichero\n");
    exit(1);
  }
  if(fscanf (fichero, "%i", &tam) != 1){
    printf("Error al leer archivo\n");
    exit(1);
  }


  for(i = 0; i < tam; ++i){
    for (j = 0; j < tam; ++j){
      if(fscanf(fichero, " %i", &flux[i][j]) != 1){
        printf("Error al leer archivo\n");
        exit(1);
      }
    }
  }

  for(i = 0; i < tam; ++i){
    for (j = 0; j < tam; ++j){
      if(fscanf(fichero, " %i", &dist[i][j]) != 1){
        printf("Error al leer archivo\n");
        exit(1);
      }
    }
  }

  fclose(fichero);
}
