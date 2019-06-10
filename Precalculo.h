#ifndef H_PRECALCULO
#define H_PRECALCULO
#include <map>
#include <array>
#include "Location.h"
#include "MapGraph.h"
#include "Search.h"

#define MAX_SIZE 30

using namespace std;
class Precalculo{

  typedef CGraph<Location, double>::E E;
  typedef CGraph<Location, double>::N N;


    public:
      map<Location,map<Location,double,LocationCompare>,LocationCompare> precalculo;

      Search busqueda;

      Precalculo(CGraph<N, E>& graph, vector<Location>& SCCRepresentants){

        for(int i = 0; i < SCCRepresentants.size(); ++i){
          Location src = SCCRepresentants[i];
          for(int j = 0; j < SCCRepresentants.size(); ++j){
              if(i != j){
                  Location dest = SCCRepresentants[j];
                  precalculo[src][dest] = busqueda.dijkstraDist(graph,src,dest);
                  cout<<"PRECALCULO: " << precalculo[src][dest] <<endl;
              }
          }
        }
      }

    Precalculo(){
        Location a(114.0,256.0);
        Location b(270.0,560.0);
        Location c(270.0,560.0);
        precalculo[a][b] = 4.0;
        precalculo[a][c] = 8.0;
//        Location(710,652) c;
//        Location(666,240) d;
//
//        precalculo.insert(make_pair(a,make_pair(b, 60.0)));
//        precalculo.insert(make_pair(b,make_pair(a, 30.0)));

    }
};
#endif
