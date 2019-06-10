#ifndef DELAUNAYTRIANGULIZATION_H_INCLUDED
#define DELAUNAYTRIANGULIZATION_H_INCLUDED


#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "vector2.h"
#include "MapGraph.h"


class DelaunayTriangulization {

  typedef CGraph<Location, double>::E E;
  typedef CGraph<Location, double>::N N;
  typedef CGraph<N, E>::Edge Edge;

public:

  std::vector<cv::Vec4f> edges;


  void generateEdges(std::vector<Location> &points, int WIDTH, int HEIGHT ){


    cv::Rect rect(0, 0, WIDTH, HEIGHT);
    cv::Subdiv2D subdiv(rect);

    for(Location point : points)
      subdiv.insert(cv::Point2f( point.getX() , point.getY()));

    subdiv.getEdgeList(edges);
  }

  void loadEdgesIntoGraph(CGraph<Location, double>& graph){
    std::cout<<"Edges opencv: "<<edges.size()<<std::endl;
    for (cv::Vec4f edge : edges) {
      Location u(edge[0], edge[1]);
      Location v(edge[2], edge[3]);
      graph.insertEdge(u, v);
    }
  }




};



#endif // DELAUNAYTRIANGULIZATION_H_INCLUDED
