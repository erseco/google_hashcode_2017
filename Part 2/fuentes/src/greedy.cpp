#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <sys/time.h>
// #include <sstream>

using namespace std;

int numVideos, numEndpoints, numRequests, numCaches,  capacidadCache;
vector<int> videos; //no se ordena

struct CacheReal{
  int capacidadRestante;
  vector<int> videosEnCache;;
};

struct Cache{
  int id;
  int latencia;
};

struct CacheConectada{
  int id;
  int latencia;

  bool operator < (CacheConectada & c1){
    return latencia < c1.latencia;
  }
};

struct VideoPedido{
  int idVideo;
  int peso;

  bool operator < (VideoPedido & v1){
    return peso > v1.peso;
  }
};

struct EndPoint{
  int latenciaAlDatacenter;
  int numCaches;
  vector<CacheConectada> caches;
  vector<VideoPedido> videoPedido;

  bool operator < (EndPoint & v1){
    return latenciaAlDatacenter > v1.latenciaAlDatacenter;
  }};

struct Request{
  int idVideo;
  int idEndPoint;
  int numRequestsVideo;
};



vector<EndPoint> endPoints;
vector<Request> requests;
vector<Request> requestsComprimido;
vector<CacheReal> cachesReales;


bool readAndFill(const char* path){
    FILE *f;
    f = fopen(path, "r");
    if (f == NULL){
        printf("File was not found\n");
        return false;
    }
    if(fscanf (f, "%i", &numVideos) != 1){
        printf("Error reading file 1\n");
        return false;
    }
    if(fscanf (f, "%i", &numEndpoints) != 1){
        printf("Error reading file 2\n");
        return false;
    }
    if(fscanf (f, "%i", &numRequests) != 1){
        printf("Error reading file 3\n");
        return false;
    }
    if(fscanf (f, "%i", &numCaches) != 1){
        printf("Error reading file 4\n");
        return false;
    }
    if(fscanf (f, "%i", &capacidadCache) != 1){
        printf("Error reading file 5\n");
        return false;
    }

    videos = vector<int>(numVideos);

    for (int i = 0; i < numVideos; ++i){
        if(fscanf (f, "%i", &videos[i]) != 1){
            printf("Error reading file 6\n");
            return false;
        }
    }

    endPoints = vector<EndPoint>(numEndpoints);
    for (int i = 0; i < numEndpoints; ++i){
        if(fscanf (f, "%i", &endPoints[i].latenciaAlDatacenter) != 1){
            printf("Error reading file 7\n");
            return false;
        }
        if(fscanf (f, "%i", &endPoints[i].numCaches) != 1){
            printf("Error reading file 8\n");
            return false;
        }

        endPoints[i].caches = vector<CacheConectada>(endPoints[i].numCaches);

        for (int j = 0; j < endPoints[i].numCaches; ++j){

            if(fscanf (f, "%i", &endPoints[i].caches[j].id) != 1){
                printf("Error reading file 9\n");

                return false;
            }
            if(fscanf (f, "%i", &endPoints[i].caches[j].latencia) != 1){
                printf("Error reading file 10\n");
                return false;
            }

        }
    }

    requests = vector<Request>(numRequests);


    for (int i = 0; i < numRequests; ++i)
    {
        if(fscanf (f, "%i", &requests[i].idVideo) != 1)
        {
            printf("Error reading file 11\n");
            return false;
        }
        if(fscanf (f, "%i", &requests[i].idEndPoint) != 1)
        {
            printf("Error reading file 12\n");
            return false;
        }
        if(fscanf (f, "%i", &requests[i].numRequestsVideo) != 1)
        {
            printf("Error reading file 13\n");
            return false;
        }
    }

    return true;

}

void ordenarTodo(){
 sort(endPoints.begin(), endPoints.end());
 for(unsigned int i = 0; i < endPoints.size(); ++i){
   sort(endPoints[i].caches.begin(), endPoints[i].caches.end());
   sort(endPoints[i].videoPedido.begin(), endPoints[i].videoPedido.end());
 }
}

void rellenaPedidosEnEndPoints(){
  //cout << requestsComprimido.size() << endl;
  for (unsigned int i = 0; i < requestsComprimido.size(); ++i){
    VideoPedido vp;
    vp.idVideo = requestsComprimido[i].idVideo;
    vp.peso = requestsComprimido[i].numRequestsVideo * (videos[vp.idVideo] * 0.75);
    endPoints[requestsComprimido[i].idEndPoint].videoPedido.push_back(vp);
  }
}

void CrearCaches(){
  CacheReal cr;
  cr.capacidadRestante = capacidadCache;
  //vector<CacheReal> cachesReales(numCaches, cr);

  for(int i = 0; i < numCaches; ++i){
    cachesReales.push_back(cr);
  }
}

void comprimeRequests(){
  int idVideo, idEndPoint, numRequestsLocal;
  while (!requests.empty()){
    idVideo = requests[0].idVideo;
    idEndPoint = requests[0].idEndPoint;
    numRequestsLocal = requests[0].numRequestsVideo;
    requests.erase(requests.begin());
    unsigned int i = 0;
    while (i < requests.size()){
      if (requests[i].idVideo == idVideo && requests[i].idEndPoint == idEndPoint){
        numRequestsLocal += requests[i].numRequestsVideo;
        requests.erase(requests.begin() + i);
      } else{
        ++i;
      }
    }
    Request r;
    r.idVideo = idVideo;
    r.idEndPoint = idEndPoint;
    r.numRequestsVideo = numRequestsLocal;
    requestsComprimido.push_back(r);
  }
}


bool estaEnCache(int videoActual, int endPointIndex){
  for (int k = 0; k < endPoints[endPointIndex].caches.size(); ++k){
    for (int h = 0; h < cachesReales[endPoints[endPointIndex].caches[k].id].videosEnCache.size(); ++h){
      if (cachesReales[endPoints[endPointIndex].caches[k].id].videosEnCache[h] == videoActual){
        return true;
      }
    }
  }
  return false;
}


void greedy(){
  bool seguir = true;
  int ultimoEnMeter = -1;
  int i = 0, j = 0;
  while (seguir){
    if (i == ultimoEnMeter){
      seguir = false;
      while(j < endPoints[i].videoPedido.size()){
        int videoActual = endPoints[i].videoPedido[j].idVideo;
        bool metido = false;
        int k = 0;
        while (!metido && k < endPoints[i].caches.size()){
          if (cachesReales[endPoints[i].caches[k].id].capacidadRestante > videos[videoActual]){
            cachesReales[endPoints[i].caches[k].id].videosEnCache.push_back(videoActual);
            cachesReales[endPoints[i].caches[k].id].capacidadRestante -= videos[videoActual];
            metido = true;
          }
          ++k;
        }
        ++j;
      }
    }
    else if (j < endPoints[i].videoPedido.size()){
      int videoActual = endPoints[i].videoPedido[j].idVideo;
      if (!estaEnCache(videoActual, i)){
        bool metido = false;
        int k = 0;
        while (!metido && k < endPoints[i].caches.size()){
          if (cachesReales[endPoints[i].caches[k].id].capacidadRestante > videos[videoActual]){
            cachesReales[endPoints[i].caches[k].id].videosEnCache.push_back(videoActual);
            cachesReales[endPoints[i].caches[k].id].capacidadRestante -= videos[videoActual];
            metido = true;
            ultimoEnMeter = i;
          }
          ++k;
        }
      }
    }
    ++i;
    if (i == numEndpoints-1){
      i = 0;
      ++j;
    }
  }
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

  readAndFill(argv[1]);
  comprimeRequests();
  rellenaPedidosEnEndPoints();
  CrearCaches();
  ordenarTodo();
  greedy();

  cout << cachesReales.size() << endl;
  for (int i = 0; i < cachesReales.size(); ++i){
    cout << i << " ";
    for (int j = 0; j < cachesReales[i].videosEnCache.size(); ++j){
      cout << cachesReales[i].videosEnCache[j] << " ";
    }
    cout << endl;
  }

  return 0;
}
