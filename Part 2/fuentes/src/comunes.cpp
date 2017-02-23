#include "comunes.h"

/*
  Reserva memoria para las matrices de flujos, distancias
  y solución. Inicializa las matrices de flujo y
  distancias con sus valores leídos desde archivo
*/

void inicializar_matrices(const char *ruta){
  tam = tamanio(ruta);
  int i;

  solucion = new int[tam];

  flux = new int*[tam];
  for(i = 0; i < tam; ++i){
    flux[i] = new int[tam];
  }

  dist = new int*[tam];
  for(i = 0; i < tam; ++i){
    dist[i] = new int[tam];
  }
  leer(ruta, flux, dist);
}

/*
  Libera la memoria de las matrices de flujos,
  distancias y solución.
  PRE: Las matrices deben haber sido reservadas en memoria.
*/

void liberar(){
  for(int i = 0; i < tam; ++i){
    delete[] flux[i];
  }
  delete[] flux;
  for(int i = 0; i < tam; ++i){
    delete[] dist[i];
  }
  delete[] dist;
  delete[] solucion;
}

/*
  Calcula el costo de una solución dada.
  PRE: Las matrices deben haber sido inicializadas.
  NOTA: No comprueba la validez de la solución
*/

int costo(){
  int total = 0;
  for (int i = 0; i < tam; ++i){
    for (int j = 0; j < tam; ++j){
      total += flux[i][j] * dist[solucion[i]][solucion[j]];
    }
  }
  return total;
}

/*
  Genera una solución inicial de forma aleatoria
  PRE: Las matrices deben haber sido inicializadas.
*/

void solucion_inicial(){
  auxiliar *valores = new auxiliar[tam];
  for(int i = 0; i < tam; ++i){
    valores[i].i = i;
    valores[i].valor = Randint(0,1000000);
  }

  mergesort(valores, tam);

  for(int i = 0; i < tam; ++i){
    solucion[i] = valores[i].i;
  }
}

/*
  Calcula, mediante factorización, la diferencia de
  coste del vecino resultante de intercambiar r y s.
  Si el resultado es negativo, el vecino es mejor que
  la solución actual.
  PRE: Las matrices deben haber sido inicializadas.
  PRE: r y s deben ser menores que tam
*/

int calcula_vecino(int r, int s){
  int dif = 0;
  for (int i = 0; i < tam; ++i){
    if (i != r && i != s){
      dif += flux[r][i] * (dist[solucion[s]][solucion[i]] - dist[solucion[r]][solucion[i]]) +
        flux[s][i] * (dist[solucion[r]][solucion[i]] - dist[solucion[s]][solucion[i]]) +
        flux[i][r] * (dist[solucion[i]][solucion[s]] - dist[solucion[i]][solucion[r]]) +
        flux[i][s] * (dist[solucion[i]][solucion[r]] - dist[solucion[i]][solucion[s]]);
    }
  }
  return dif;
}

/*
  Intercambia dos localizaciones de la solución actual
  PRE: Las matrices deben haber sido inicializadas.
  PRE: r y s deben ser menores que tam
*/

void acepta_vecino(int r, int s){
  int aux = solucion[r];
  solucion[r] = solucion[s];
  solucion[s] = aux;
}

/*
  Algoritmo de ordenación. Toma como entrada un array
  de estructuras auxiliar, y las ordena en orden descendente
  en función del valor de su campo "valor".
  PRE: La matriz de entrada debe haber sido reservada en memoria.
*/

/**************************************************
* El código del mergesort ha sido obtenido de
* http://www.cprogramming.com/tutorial/computersciencetheory/merge.html
* y después adaptado a mi programa
**************************************************/

/* Helper function for finding the max of two numbers */
int max(int x, int y)
{
    if(x > y)
    {
        return x;
    }
    else
    {
        return y;
    }
}

/* left is the index of the leftmost element of the subarray; right is one
 * past the index of the rightmost element */
void merge_helper(auxiliar *input, int left, int right, auxiliar *scratch)
{
    /* base case: one element */
    if(right == left + 1)
    {
        return;
    }
    else
    {
        int i = 0;
        int length = right - left;
        int midpoint_distance = length/2;
        /* l and r are to the positions in the left and right subarrays */
        int l = left, r = left + midpoint_distance;

        /* sort each subarray */
        merge_helper(input, left, left + midpoint_distance, scratch);
        merge_helper(input, left + midpoint_distance, right, scratch);

        /* merge the arrays together using scratch for temporary storage */
        for(i = 0; i < length; i++)
        {
            /* Check to see if any elements remain in the left array; if so,
             * we check if there are any elements left in the right array; if
             * so, we compare them.  Otherwise, we know that the merge must
             * use take the element from the left array */
            if(l < left + midpoint_distance &&
                    (r == right || max(input[l].valor, input[r].valor) == input[l].valor))
            {
                scratch[i].valor = input[l].valor;
                scratch[i].i = input[l].i;
                l++;
            }
            else
            {
                scratch[i].valor = input[r].valor;
                scratch[i].i = input[r].i;
                r++;
            }
        }
        /* Copy the sorted subarray back to the input */
        for(i = left; i < right; i++)
        {
            input[i].valor = scratch[i - left].valor;
            input[i].i = scratch[i - left].i;
        }
    }
}

/* mergesort returns true on success.  Note that in C++, you could also
 * replace malloc with new and if memory allocation fails, an exception will
 * be thrown.  If we don't allocate a scratch array here, what happens?
 *
 * Elements are sorted in reverse order -- greatest to least */

int mergesort(auxiliar *input, int size)
{
    auxiliar *scratch = new auxiliar[size];
    if(scratch != NULL)
    {
        merge_helper(input, 0, size, scratch);
        delete [] scratch;
        return 1;
    }
    else
    {
        printf("Error al reservar memoria en el mergesort\n");
        return 0;
    }
}
