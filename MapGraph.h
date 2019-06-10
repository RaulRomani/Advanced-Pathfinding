#ifndef H_MAPGRAPH
#define H_MAPGRAPH


#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <stack>
#include <math.h>
#include <queue>
#include<time.h>
#include <set>
#include <ostream>

#include "Location.h"

#define NIL -1
const double INF = 9999999.0;
using namespace std;

struct ooops : std::exception {
  const char* what() const noexcept {
    return "Ooops!\n";
  }
};



template<class G>
class MapNode {
public:
  typedef typename G::N N;
  typedef typename G::Edge Edge;

  N data;
  int id;
  list<Edge*> outgoingEdges;
  list<Edge*> incommingEdges;
};

template<class G>
class MapEdge {
public:
  typedef typename G::Node Node;
  typedef typename G::E E;
  E length;
  string roadName;
  string roadType;
  Node* nodes[2];
};

template<typename No, typename Ed>
class CGraph {
public:
  typedef CGraph<No, Ed> self;
  typedef MapNode<self> Node;
  typedef MapEdge<self> Edge;

  typedef No N;
  typedef Ed E;
  typedef pair<N, E> Entry;
  typedef typename vector<Node*>::iterator vectorIt;

  //    vector<Node*> adjList;
  size_t nNodes=0;
  map<N, Node*, LocationCompare> adjList;
  vector<Node*> nodes;


  bool insertNode(N n);


  bool removeNode(N n) {

    //CHECAR SI EL NODO EXISTE
    Node* node = adjList[n];

    for(auto i = node->outgoingEdges.begin(); i != node->outgoingEdges.end();++i)
      removeEdge(*i);

    for(auto i = node->incommingEdges.begin(); i != node->incommingEdges.end();++i)
      removeEdge(*i);

    nodes[node->id] = NULL;

    delete node;
    adjList.erase (n);
    return true;
  }

  void removeEdge(Edge*& edge){
     Node* tail = edge->nodes[0];
     Node* head = edge->nodes[1];

     auto it = find (tail->outgoingEdges.begin(), tail->outgoingEdges.end(), edge);
     tail->outgoingEdges.erase(it);
     auto jt = find (head->incommingEdges.begin(), head->incommingEdges.end(), edge);
     head->incommingEdges.erase(jt);

     delete edge;
  }

  Node findNode(N node) {
    auto n = adjList.find(node);
    return *(*n).second; // warning: if it does not find it: segmentation fault
  }

  bool insertEdge(N u, N v) {

  //TODO Comprobar que el edge existe
    //buscar si los nodos existen en el grafo
    auto src = adjList.find(u);
    auto dest = adjList.find(v);

    if (src != adjList.end() && dest != adjList.end()) {
      // found
      Edge* edge = new Edge();
//      edge->length = 1.0;
      edge->length = u.dist(v);
      edge->nodes[0] = (*src).second;
      edge->nodes[1] = (*dest).second;

      (*src).second->outgoingEdges.push_back(edge);
      (*dest).second->incommingEdges.push_back(edge);

      edge = new Edge();
//      edge->length = 1.0;
      edge->length = u.dist(v);
      edge->nodes[0] = (*dest).second;
      edge->nodes[1] = (*src).second;

      (*src).second->incommingEdges.push_back(edge);
      (*dest).second->outgoingEdges.push_back(edge);

      return 1;
    } else {
      return 0;
    }
  }


  vector<N> getVertices() {
    vector<N> retval;
    for (auto const& element : adjList)
      retval.push_back(element.first);
    return retval;
  }

  N getNodeFromId(int verexId){
    return nodes[verexId]->data;
  }
  int getIdFromNode(N location){
    return adjList[location]->id;
  }

  list<Edge*> getOutGoingEdges(Location vertex) {
    return adjList.at(vertex)->outgoingEdges;
  }

  list<Edge*> getOutGoingEdges(int vertexId) {
    return nodes[vertexId]->outgoingEdges;
  }


  void SCCUtil(N u, map<N, int, LocationCompare> &disc, map<N, int, LocationCompare> &low,
          stack<N> *st, map<N, bool, LocationCompare> &stackMember, vector<list<N>>& connectedComponents) {
    // A static variable is used for simplicity, we can avoid use
    // of static variable by passing a pointer.
    static int time = 0;
    static list<N> connCompList;

    // Initialize discovery time and low value
    disc[u] = low[u] = ++time;
    st->push(u);
    stackMember[u] = true;

    // Go through all vertices adjacent to this
    list<int>::iterator i;
    list<Edge*> neighbors = adjList.at(u)->outgoingEdges;
    for (Edge* neighbor : neighbors) {
      N v = neighbor->nodes[1]->data; // v is current adjacent of 'u'

      // If v is not visited yet, then recur for it
      if (disc.at(v) == NIL) {
        SCCUtil(v, disc, low, st, stackMember,connectedComponents);

        // Check if the subtree rooted with 'v' has a
        // connection to one of the ancestors of 'u'
        // Case 1 (per above discussion on Disc and Low value)
        low[u] = min(low.at(u), low.at(v));
      }
        // Update low value of 'u' only of 'v' is still in stack
        // (i.e. it's a back edge, not cross edge).
        // Case 2 (per above discussion on Disc and Low value)
      else if (stackMember.at(v) == true)
        low[u] = min(low.at(u), disc.at(v));
    }

    // head node found, pop the stack and print an SCC
    N w; // To store stack extracted vertices
    if (low.at(u) == disc.at(u)) {
      while (!st->top().isEquals(u)) //because Ive overriten the != opertator on Location class
      {
        w = st->top();

        cout << w << " ";
        connCompList.push_back(w);
        stackMember[w] = false;
        st->pop();
      }
      w = st->top();
      cout << w << "\n";
      connCompList.push_back(w);
      connectedComponents.push_back(connCompList);
      connCompList.clear();
      stackMember[w] = false;
      st->pop();
    }
  }

  vector<N> calculateRepresentant(vector<list<N>> connectedComponents){

    vector<N> SCCRepresentant;

    for( auto it = connectedComponents.begin(); it != connectedComponents.end(); it++){

      double minX = INF, minY = INF,maxX = -1,maxY = -1;
//      cout<<"test"<<endl;
      for( auto jt = it->begin() ; jt != it->end(); jt++){

//          cout<<*jt<<endl;
          minX = min(minX,jt->getX());
          minY = min(minY,jt->getY());

          maxX = max(maxX,jt->getX());
          maxY = max(maxY,jt->getY());
      }

//      cout<<minX<<" "<< minY <<"\t"<<maxX<<" "<<maxY<<endl;

      Location A(minX,minY);
      Location B(minX,maxY);
      Location C(maxX,maxY);

      Location halfPointAB(A.getX(), A.getY() + (B.getY()-A.getY())/2);
      Location halfPointBC(B.getX() + (C.getX()-B.getX())/2, A.getY());
      Location centroid(halfPointBC.getX(), halfPointAB.getY());

      double minDist = INF;
      Location representant;
      for( auto jt = it->begin() ; jt != it->end(); jt++){
        if(centroid.dist(*jt) <minDist){  //Optimizar
          minDist = centroid.dist(*jt);
          representant = *jt;
        }
      }
      SCCRepresentant.push_back(representant);
//      SCCRepresentant.push_back(centroid);
    }
    return SCCRepresentant;
  }

  // The function to do DFS traversal. It uses SCCUtil()
  vector<N> TarjanSCCs() {

    map<N, int, LocationCompare> disc;
    map<N, int, LocationCompare> low;
    map<N, bool, LocationCompare> stackMember;
    stack<N> *st = new stack<N>();

    // Initialize disc and low, and stackMember arrays
    for (N node : getVertices()) {
      disc.insert(make_pair(node, NIL));
      low.insert(make_pair(node, NIL));
      stackMember.insert(make_pair(node, false));
    }

    vector<list<N>> connectedComponents;
    // Call the recursive helper function to find strongly
    // connected components in DFS tree with vertex 'i'
    for (N node : getVertices()) {
      if (disc.at(node) == NIL)
        SCCUtil(node, disc, low, st, stackMember, connectedComponents);
    }

    return calculateRepresentant(connectedComponents);

  }
};

template<typename No, typename Ed>
bool CGraph<No, Ed>::insertNode(N n) {

  //CHECAR SI EL NODO EXISTE
  Node* node = new Node;
  node->data = n;
  list<Edge*> edgesOut;
  list<Edge*> edgesIn;
  node->outgoingEdges = edgesOut;
  node->incommingEdges = edgesIn;
  node->id = nNodes++;
  nodes.push_back(node);
  adjList.insert(make_pair(n, node));
  return true;
}


#endif


