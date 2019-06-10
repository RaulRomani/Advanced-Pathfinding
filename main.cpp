
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   main.cpp
 * Author: raul
 *
 * Created on June 22, 2017, 6:35 PM
 */

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <stack>
#include <random>
#include <string>
#include <math.h>
#include <queue>
#include<time.h>
#include <set>
#include <ostream>
#include <SFML/Graphics.hpp>


#include "MapGraph.h"
#include "Grafico.h"
#include "Search.h"
#include "Precalculo.h"
#include "DelaunayTriangulization.h"

#define N 50
#define WIDTH 1200
#define HEIGHT 700
using namespace std;


void loadAlexTestCase(char* fileName, CGraph<Location, double>& graph){

  ifstream infile(fileName);

  float x, y;
  int nodes;
  std::string junk;
  infile >> junk >> nodes;
  std::vector<Location> points;
  for (int i = 0; i < nodes ; ++i) {
    infile >> x >> y;
    points.push_back(Location(x,y));
    graph.insertNode(Location(x,y));
  }

  int u, v, edges;
  infile >> junk >> edges;
  for (int i = 0; i < edges; ++i) {
    infile >> u >> v;
    cout<<"edge "<<i<<": "<<points[u]<<" "<<points[v]<<endl;
    graph.insertEdge(points[u], points[v]);
  }

  infile.close();
}

void uploadTestCase(char* fileName, CGraph<Location, double>& graph){

  Location location;
  ifstream infile(fileName);
  float x, y;
  int nodes;
  infile >> nodes;
  std::vector<Location> points;
  for (int i = 1; infile >> x >> y; ++i) {
    location.setX(x);
    location.setY(y);
    points.push_back(Location(location.getX(), location.getY()));
    graph.insertNode(location);
  }
  infile.close();

  DelaunayTriangulization dt;
  dt.generateEdges(points,WIDTH,HEIGHT);
  dt.loadEdgesIntoGraph(graph);



  //cout << "Source: " << source << " Destination: " << target << endl;


  // Load edges
//  ifstream in("test2Edges");
//
//  int numEdges;
//  in >> numEdges;
//  //  cout<<"num edeges;;"<<numEdges<<endl;
//  Edge<float> e;
//  std::vector<Edge<float>> edges;
//  while (in >> e.p1.x >> e.p1.y >> e.p2.x >> e.p2.y) {
//    edges.push_back(e);
//  }
//  in.close();


}

/*
* Generate test case and load graph
*/
void generateTestCase(char* fileName, CGraph<Location, double>& graph){


  std::random_device rd;
	std::default_random_engine seed(rd());
	int padding = 50;
	std::uniform_real_distribution<double> x(padding,WIDTH-padding);
	std::uniform_real_distribution<double> y(padding,HEIGHT-padding);


  Location location;

  ofstream outfile(fileName);

  outfile<<N<<endl;
  std::vector<Location> points;
  for (int i = 0; i < N; i++) {

      location.setX(rand() % (WIDTH-2*padding) + padding);
      location.setY(rand() % (HEIGHT-2*padding) + padding);

//      location.setX(x(seed));
//      location.setY(y(seed));

      outfile<<location.getX()<<"\t"<<location.getY()<<"\n";

      points.push_back(Location(location.getX(), location.getY()));

      graph.insertNode(location);
    }
    outfile.close();

    DelaunayTriangulization dt;
    dt.generateEdges(points,WIDTH,HEIGHT);
    dt.loadEdgesIntoGraph(graph);

}


void deleteNodes(CGraph<Location, double>& graph){
  cout<<"Deleting "<< graph.adjList.size()<<endl;

  vector<Location> vertices = graph.getVertices();
  for (Location u: vertices) {

//      Location u = *it;

      //first rectangle
      int xMin = 230, xMax = 350;
      int yMin = 150, yMax = 480;
      if ( xMin < u.getX() && u.getX() < xMax &&
           yMin < u.getY() && u.getY() < yMax){
        graph.removeNode(u);
      }

      //second rectangle
      xMin = 790, xMax = 860;
      yMin = 200, yMax = 580;
      if ( xMin < u.getX() && u.getX() < xMax &&
           yMin < u.getY() && u.getY() < yMax)
        graph.removeNode(u);
  }

  cout<<"Deleting "<< graph.adjList.size()<<endl;
}


void generateSourcesAndTargets(CGraph<Location, double>& graph,std::vector<std::pair<Location,Location>>& sourcesTargets, int n ){

  std::vector<Location> vertices = graph.getVertices();


  for(int i=0; i<n ;i++){
    std::pair<Location,Location> sourceTarget;

    for (Location location: vertices) {
        //source

        int xMin = 230, xMax = 350;
        int yMin = 150, yMax = 480;
        if(sourceTarget.first.isEmpty()){
          if ( xMin < location.getX() && location.getX() < xMax &&
               yMin < location.getY() && location.getY() < yMax){
            sourceTarget.first = location;

          }
        }


        //target
        xMin = 790, xMax = 860;
        yMin = 200, yMax = 580;
        if(sourceTarget.second.isEmpty()){
          if ( xMin < location.getX() && location.getX() < xMax &&
               yMin < location.getY() && location.getY() < yMax)
            sourceTarget.second = location;
        }


        if(!sourceTarget.first.isEmpty() && !sourceTarget.second.isEmpty())
          break;
    }
    sourcesTargets.push_back(sourceTarget);
  }
}

/*
 *
 */
int main(int argc, char** argv) {

  srand(time(NULL)); // randomize seed
  CGraph<Location, double> graph;
  std::vector<std::pair<Location,Location>> sourcesTargets;


//generateTestCase("test10", graph);
  uploadTestCase("test10", graph);
//  deleteNodes(graph);


//  loadAlexTestCase("data/100points.data",graph);

  generateSourcesAndTargets(graph, sourcesTargets, 1);
  std::cout<<"source: "<<sourcesTargets[0].first<<"target: "<<sourcesTargets[0].second<<std::endl;



  /***** FIND SHORTEST PATH *************/
  Search busqueda;
  list<Location>* pathList = busqueda.dijkstra(graph, sourcesTargets[0].first, sourcesTargets[0].second);
//  list<Location>* pathList = busqueda.biDijkstraO(graph, source, target);

  //print Path
  if(pathList != NULL){
    cout << "Path: " << endl;
    for (Location path : *pathList) {
      cout << path << " ";
    }
    cout << endl;
  }

//    while(true); //PAUSE

  /***** END FIND SHORTEST PATH *************/


  if(pathList != NULL){
    Grafico graphics;
    graphics.graficar(graph,*pathList);
  }



  return 0;
}

