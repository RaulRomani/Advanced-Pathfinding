
#ifndef H_SEARCH
#define H_SEARCH

#include <bits/stdc++.h>
#include "MapGraph.h"
#include "Location.h"


template<typename No, typename Ed>
struct EntryGreaterCompare {
  bool operator()(const pair<No, Ed>& l, const pair<No, Ed>& r) const {
    return l.second > r.second;
  }
};


class Search{
  private:
  typedef CGraph<Location, double>::E E;
  typedef CGraph<Location, double>::N N;
  typedef CGraph<Location, double>::Edge Edge;
  typedef EntryGreaterCompare<N, E> EntryCmp;
  typedef EntryGreaterCompare<int, E> EntryOCmp;
  typedef pair<N, E> Entry;
  typedef pair<int, E> EntryO;

  public:

    list<N>* reconstructPath(N start,N middle, N goal, map<N, N, LocationCompare> parentMap,
                            map<N, N, LocationCompare> rparentMap) {

    list<N>* path = new list<N>();

    N curr = middle;
    while (curr != start) {
//      cout<<curr<<" ";
      path->push_front(curr);
      curr = parentMap.at(curr);
    }

    path->push_front(start);
//    cout<<start<<" ";

    curr = rparentMap.at(middle);
    while (curr != goal) {
//      cout<<curr<<" ";
      path->push_back(curr);
      curr = rparentMap.at(curr);
    }
    path->push_back(goal);
//    cout<<goal<<" ";

    return path;
  }

  list<N>* reconstructPath(N start, N goal, map<N, N, LocationCompare> parentMap) {
    list<N>* path = new list<N>();
    N curr = goal;
    while (curr != start) {
      path->push_front(curr);
      curr = parentMap.at(curr);
    }
    path->push_front(start);
    return path;
  }
  list<N>* reconstructPathReverse(N start, N goal, map<N, N, LocationCompare> parentMap) {
    list<N>* path = new list<N>();
    N curr = goal;
    while (curr != start) {
      path->push_back(curr);
      curr = parentMap.at(curr);
    }
    path->push_back(start);
    return path;
  }

  list<N>* reconstructPathReverse(CGraph<Location, E>& graph, int idStart, int idGoal, vector<int> parentMap) {
    list<N>* path = new list<N>();
    N loc;
    int curr = idGoal;
    while (curr != idStart) {
      loc =  graph.getNodeFromId(curr);
      path->push_front(loc);
      curr = parentMap[curr];
    }
    loc =  graph.getNodeFromId(idStart);
    path->push_front(loc);
    return path;
  }

  list<N>* reconstructPath(CGraph<Location, E>& graph, int idStart, int idGoal, vector<int> parentMap) {
    list<N>* path = new list<N>();
    N loc;
    int curr = idGoal;
    while (curr != idStart) {
      loc =  graph.getNodeFromId(curr);
      path->push_front(loc);
      curr = parentMap[curr];
    }
    loc =  graph.getNodeFromId(idStart);
    path->push_front(loc);
    return path;
  }

  list<N>* reconstructPath(CGraph<Location, E>& graph, int idStart,int idMiddle, int idGoal,
                            vector<int> parentMap, vector<int> rparentMap) {

    list<N>* path = new list<N>();
    N loc;
    int curr = idMiddle;
    while (curr != idStart) {
      loc =  graph.getNodeFromId(curr);
      path->push_front(loc);
      curr = parentMap[curr];
    }
    loc =  graph.getNodeFromId(idStart);
    path->push_front(loc);


    curr = rparentMap[idMiddle];
    while (curr != idGoal) {
      loc =  graph.getNodeFromId(curr);
      path->push_back(loc);
      curr = rparentMap[curr];
    }
    loc =  graph.getNodeFromId(idGoal);
    path->push_back(loc);

    return path;
  }

  E distFromPath(list<N>* path){
    E dist=0.0;
    auto end = path->end();
    --end;
    for(auto i=path->begin(); i != end; ++i){
         dist += (*i).dist(*(++i)); i--;
    }
    return dist;
  }
  //Dijkstra return shortest dist
  E dijkstraDist(CGraph<Location, E>& graph,N start, N goal) {

    //      if (start == null || goal == null) {
    //			System.out.println("Start or goal node is null!  No path exists.");
    //			return null;
    //		}

    map<N, N, LocationCompare> parentMap;

    // find the goal with dijkstra
    bool found = dijkstraSearch(graph,start, goal, parentMap);

    if (!found) {
      cout << "No path exists" << endl;
      return 0.0;
    }

    list<N>* path = reconstructPath(start, goal, parentMap);
    return distFromPath(path);
  }

  list<N>* dijkstra(CGraph<Location, E>& graph,N start, N goal) {

    //      if (start == null || goal == null) {
    //			System.out.println("Start or goal node is null!  No path exists.");
    //			return null;
    //		}
    map<N, N, LocationCompare> parentMap;

    // find the goal with dijkstra
    bool found = dijkstraSearch(graph,start, goal, parentMap);

    if (!found) {
      cout << "No path exists" << endl;
      return NULL;
    }
    // reconstruct the path
    return reconstructPath(start, goal, parentMap);
  }

  bool dijkstraSearch(CGraph<Location, double>& graph, N start, N goal, map<Location, Location, LocationCompare>& parentMap) {

    // In order to add visited vertices
    set<N, LocationCompare> visited;
    // to keep track of the shortest path between src and dist[v]
    map<N, E, LocationCompare> dist;
    for (N node : graph.getVertices())
      dist.insert(make_pair(node, INF));

    // Mark the start node
    dist[start] = 0.0;

    priority_queue <Entry, vector<Entry>, EntryCmp> pQueue;
    priority_queue <Entry, vector<Entry>, EntryCmp> rpQueue;

    // Mark the start node as visited and enqueue it
    Entry entry;
    entry.first = start;
    entry.second = 0.0; //Warning: Assuming E is double
    pQueue.push(entry);

    bool found = false;
    Entry curr;
    int nroNodesVisited = 0;

    while (!pQueue.empty()) {

      // Dequeue a vertex from queue
      curr = pQueue.top();
      pQueue.pop();

      nroNodesVisited++;nroNodesVisited++;



      cout << "\nDijkstra visiting[NODE] at location " << curr.first << endl;
      cout << "Min Dist: " << curr.second << endl;


      if (visited.find(curr.first) == visited.end()) {
        visited.insert(curr.first);

        if (curr.first.isEquals(goal)) {
            found = true;
            break;
        }

        // Get all adjacent vertices (neighbors) of the dequeued vertex curr
        try {
          list<Edge*> neighbors = graph.getOutGoingEdges(curr.first);
          cout<<"Neighbors: " << neighbors.size()<<endl;
          for (Edge* neighbor : neighbors) {

            //USAR ALIAS

            N& next = neighbor->nodes[1]->data; // nodes[1] == dest

            //MODIFICATION FOR BIRECTIONAL(UNDIRECTED) GRAPH
            if (visited.find(next) == visited.end()) {

              double greedyScore = dist.at(curr.first) + neighbor->length;
              /*     *A  ***/
              double nextToGoal = Location(next.getX(),next.getY()).dist(goal);
              double heuristic = greedyScore+ nextToGoal;
              /* END *A  ***/

              cout<<"Vertex: " << next<<endl;
              if (dist.at(next) > greedyScore) { // || dist(Next) == INF
                cout<<"Dist before: " << to_string(dist.at(next))<<endl;
                cout<<"Dist after: " << to_string(greedyScore)<<endl;
                dist[next] = greedyScore;
                /*     *A  ***/
  //              pQueue.push(make_pair(next, heuristic));
                /* END *A  ***/
                /*     Dijkstra ***/
                pQueue.push(make_pair(next, greedyScore));
                /*     END Dijkstra ***/
                parentMap[next] = curr.first;
              }
            }
          }
        } catch (exception& e) {
          cout << "Out of range exception: " << e.what() << '\n';
        }

      }
    }
    cout << "# nodes visited: " << nroNodesVisited << endl;
    return found;
  }


  list<N>* biDijkstraO(CGraph<Location, E>& graph, N start, N goal) {

    //      if (start == null || goal == null) {
    //			cout<<"Start or goal node is null!  No path exists.";
    //			return NULL;
    //		}

    vector<int> parentMap(graph.nNodes);
    vector<int> rparentMap(graph.nNodes);

    int idMiddle;
    // find the goal with dijkstra
    bool found = biDijkstraSearchO(graph, start,idMiddle, goal, parentMap,rparentMap);

    if (!found) {
      cout << "No path exists" << endl;
      return NULL;
    }


    int idStart = graph.getIdFromNode(start);
    int idGoal = graph.getIdFromNode(goal);

    if (idMiddle == idStart){
      cout<<"middle = start"<<endl;
      return reconstructPathReverse(graph,idGoal,idStart,rparentMap);
    }else if (idMiddle == idGoal){
      cout<<"middle = goal"<<endl;
      return reconstructPath(graph, idStart,idGoal,parentMap);
    }

//    // reconstruct the path
    return reconstructPath(graph,idStart,idMiddle, idGoal, parentMap, rparentMap);
  }

  bool biDijkstraSearchO(CGraph<Location, double>& graph, N start,  int& idMiddle, N goal,
                        vector<int>& parentMap,
                        vector<int>& rparentMap) {


    int idStart = graph.getIdFromNode(start);
    int idGoal = graph.getIdFromNode(goal);

    // In order to add visited vertices
    bitset<16> visited;
    bitset<16> rVisited;

    // to keep track of the shortest path between src and dist[v]
    vector<E> dist(graph.nNodes), rdist(graph.nNodes);
    for (int i=0; i<graph.nNodes; ++i) {
      dist[i] = INF;
      rdist[i] = INF;
    }

    // Mark the start node
    dist[idStart] = 0.0;

    // Mark the goal node
    rdist[idGoal] = 0.0;

    priority_queue <EntryO, vector<EntryO>, EntryOCmp> pQueue;
    priority_queue <EntryO, vector<EntryO>, EntryOCmp> rpQueue;

    // Mark the start node as visited and enqueue it
    EntryO entry;
    entry.first = idStart;
    entry.second = 0.0; //Warning: Assuming E is double
    pQueue.push(entry);

    // Mark the end node as visited and enqueue it
    entry.first = idGoal;
    entry.second = 0.0;
    rpQueue.push(entry);


    bool found = false;
    EntryO curr, rcurr;
    int nroNodesVisited = 0;

    while (!pQueue.empty()&& !rpQueue.empty()) {

      // Dequeue a vertex from queue
      curr = pQueue.top();
      pQueue.pop();

      rcurr = rpQueue.top();
      rpQueue.pop();

      cout << "curr: " << curr.first << endl;
      cout << "rcurr: " << rcurr.first << endl;


      nroNodesVisited++;

      if (curr.first == rcurr.first) {
        found = true;
        idMiddle = curr.first;
        cout<<"curr == rcurr"<<endl;
        break;
      } else if (curr.first==idGoal) {
        found = true;
        cout<<"curr == goal"<<endl;
        idMiddle = idGoal;
        break;
      } else if (rcurr.first == idStart) {
        found = true;
        cout<<"curr == start"<<endl;
        idMiddle = idStart;
        break;
      }

      cout << "\nDijkstra visiting[NODE] at location " << curr.first << endl;
      cout << "Min Dist: " << curr.second << endl;


      if (visited[curr.first] == 0) { // si no ha sido visitado
        visited[curr.first] = 1;
        cout << "from start to goal" << endl;

        // Get all adjacent vertices (neighbors) of the dequeued vertex curr
        try {
          list<Edge*> neighbors = graph.getOutGoingEdges(curr.first);
          cout << "Neighbors: " << neighbors.size() << endl;
          for (Edge* neighbor : neighbors) {
            N next = neighbor->nodes[1]->data; // nodes[1] == dest
            int idNext = graph.getIdFromNode(next);
            double greedyScore = dist[curr.first] + neighbor->length;
            /*     *A  ***/
            double nextToGoal = Location(next.getX(), next.getY()).dist(goal);
            double heuristic = greedyScore + nextToGoal;
            /* END *A  ***/

            cout << "Vertex: " << next << endl;
            if (greedyScore < dist[idNext] ) { // || dist(Next) == INF
              cout << "Dist before: " << to_string(dist[idNext]) << endl;
              cout << "Dist after: " << to_string(greedyScore) << endl;
              cout << "Heuristic: " << to_string(heuristic) << endl;
              dist[idNext] = greedyScore;
              /*     *A  ***/
              pQueue.push(make_pair(idNext, heuristic));
              /* END *A  ***/
              /*     Dijkstra ***/
              //              pQueue.push(make_pair(next, greedyScore));
              /*     END Dijkstra ***/
              parentMap[idNext] = curr.first;
            }
          }
        } catch (exception& e) {
          cout << "Out of range exception: " << e.what() << '\n';
        }
      }

      nroNodesVisited++;
      curr = pQueue.top();

      if (curr.first == rcurr.first) {
        cout << "found: " << curr.first << " = " << rcurr.first << endl;
        idMiddle = curr.first;
        found = true;
        break;
      }

      cout << "\nReverse Dijkstra visiting[NODE] at location " << rcurr.first << endl;
      cout << "Min Dist: " << rcurr.second << endl;
      if (rVisited[rcurr.first] == 0) {
        rVisited[rcurr.first] = 1;

        // Get all adjacent vertices (neighbors) of the dequeued vertex rcurr
        try {
          list<Edge*> neighbors = graph.getOutGoingEdges(rcurr.first);
          cout << "Neighbors: " << neighbors.size() << endl;
          for (Edge* neighbor : neighbors) {
            N next = neighbor->nodes[1]->data; // nodes[1] == dest
            int idNext = graph.getIdFromNode(next);
            double greedyScore = rdist.at(rcurr.first) + neighbor->length;
            /*     *A  ***/
            double nextToGoal = Location(next.getX(), next.getY()).dist(start);
            double heuristic = greedyScore + nextToGoal;
            /* END *A  ***/

            cout << "Vertex: " << next << endl;
            if (greedyScore < rdist[idNext]) { // || dist(Next) == INF
              cout << "Dist before: " << to_string(rdist[idNext]) << endl;
              cout << "Dist after: " << to_string(greedyScore) << endl;
              cout << "Heuristic: " << to_string(heuristic) << endl;
              rdist[idNext] = greedyScore;
              /*     *A  ***/
              rpQueue.push(make_pair(idNext, heuristic));
              /* END *A  ***/
              /*     Dijkstra ***/
              //              rpQueue.push(make_pair(next, greedyScore));
              /*     END Dijkstra ***/
              rparentMap[idNext] = rcurr.first;
            }
          }
        } catch (exception& e) {
          cout << "Out of range exception: " << e.what() << '\n';
        }
      }
      cout << endl << "---------------------------------------------------" << endl;
    }
    cout << "# nodes visited: " << nroNodesVisited << endl;
    return found;
  }

  list<N>* biBFS(CGraph<Location, E>& graph, N start, N goal) {

    //      if (start == null || goal == null) {
    //			cout<<"Start or goal node is null!  No path exists.";
    //			return NULL;
    //		}

    vector<int> parentMap(graph.nNodes);
    vector<int> rparentMap(graph.nNodes);

    int idMiddle;
    // find the goal with dijkstra
    bool found = biBFSSearch(graph, start,idMiddle, goal, parentMap,rparentMap);

    if (!found) {
      cout << "No path exists" << endl;
      return NULL;
    }


    int idStart = graph.getIdFromNode(start);
    int idGoal = graph.getIdFromNode(goal);

    if (idMiddle == idStart){
      cout<<"middle = start"<<endl;
      return reconstructPathReverse(graph,idGoal,idStart,rparentMap);
    }else if (idMiddle == idGoal){
      cout<<"middle = goal"<<endl;
      return reconstructPath(graph, idStart,idGoal,parentMap);
    }

//    // reconstruct the path
    return reconstructPath(graph,idStart,idMiddle, idGoal, parentMap, rparentMap);
  }

  bool biBFSSearch(CGraph<Location, double>& graph, N start,  int& idMiddle, N goal,
                        vector<int>& parentMap,
                        vector<int>& rparentMap) {


    int idStart = graph.getIdFromNode(start);
    int idGoal = graph.getIdFromNode(goal);

    // In order to add visited vertices
    bitset<16> visited;
    bitset<16> rVisited;

    priority_queue <EntryO, vector<EntryO>, EntryOCmp> pQueue;
    priority_queue <EntryO, vector<EntryO>, EntryOCmp> rpQueue;

    // Mark the start node as visited and enqueue it
    EntryO entry;
    entry.first = idStart;
    entry.second = 0.0; //Warning: Assuming E is double
    pQueue.push(entry);

    // Mark the end node as visited and enqueue it
    entry.first = idGoal;
    entry.second = 0.0;
    rpQueue.push(entry);


    bool found = false;
    EntryO curr, rcurr;
    int nroNodesVisited = 0;

    while (!pQueue.empty()&& !rpQueue.empty()) {

      // Dequeue a vertex from queue
      curr = pQueue.top();
      pQueue.pop();

      rcurr = rpQueue.top();
      rpQueue.pop();

      cout << "curr: " << curr.first << endl;
      cout << "rcurr: " << rcurr.first << endl;


      nroNodesVisited++;

      if (curr.first == rcurr.first) {
        found = true;
        idMiddle = curr.first;
        cout<<"curr == rcurr"<<endl;
        break;
      } else if (curr.first==idGoal) {
        found = true;
        cout<<"curr == goal"<<endl;
        idMiddle = idGoal;
        break;
      } else if (rcurr.first == idStart) {
        found = true;
        cout<<"curr == start"<<endl;
        idMiddle = idStart;
        break;
      }

      cout << "\nDijkstra visiting[NODE] at location " << curr.first << endl;
      cout << "Min Dist: " << curr.second << endl;


      if (visited[curr.first] == 0) { // si no ha sido visitado
        visited[curr.first] = 1;
        cout << "from start to goal" << endl;

        // Get all adjacent vertices (neighbors) of the dequeued vertex curr
        try {
          list<Edge*> neighbors = graph.getOutGoingEdges(curr.first);
          cout << "Neighbors: " << neighbors.size() << endl;
          for (Edge* neighbor : neighbors) {
            N next = neighbor->nodes[1]->data; // nodes[1] == dest
            int idNext = graph.getIdFromNode(next);
//            double greedyScore = dist[curr.first] + neighbor->length;
            /*     *A  ***/
            double nextToGoal = Location(next.getX(), next.getY()).dist(goal);
            double heuristic = nextToGoal;
            /* END *A  ***/

            cout << "Vertex: " << next << endl;
            if (!visited[idNext]) {
              cout << "Heuristic: " << to_string(heuristic) << endl;
              /*     *A  ***/
              pQueue.push(make_pair(idNext, heuristic));
              /* END *A  ***/
              /*     Dijkstra ***/
              //              pQueue.push(make_pair(next, greedyScore));
              /*     END Dijkstra ***/
              parentMap[idNext] = curr.first;
            }
          }
        } catch (exception& e) {
          cout << "Out of range exception: " << e.what() << '\n';
        }
      }

      nroNodesVisited++;
      curr = pQueue.top();

      if (curr.first == rcurr.first) {
        cout << "found in middle: " << curr.first << " = " << rcurr.first << endl;
        idMiddle = curr.first;
        found = true;
        break;
      }

      cout << "\nReverse Dijkstra visiting[NODE] at location " << rcurr.first << endl;
      cout << "Min Dist: " << rcurr.second << endl;
      if (rVisited[rcurr.first] == 0) {
        rVisited[rcurr.first] = 1;

        // Get all adjacent vertices (neighbors) of the dequeued vertex rcurr
        try {
          list<Edge*> neighbors = graph.getOutGoingEdges(rcurr.first);
          cout << "Neighbors: " << neighbors.size() << endl;
          for (Edge* neighbor : neighbors) {
            N next = neighbor->nodes[1]->data; // nodes[1] == dest
            int idNext = graph.getIdFromNode(next);
            /*     *A  ***/
            double nextToGoal = Location(next.getX(), next.getY()).dist(start);
            double heuristic = nextToGoal;
            /* END *A  ***/

            cout << "Vertex: " << next << endl;
            if (!rVisited[idNext]) {
              cout << "Heuristic: " << to_string(heuristic) << endl;
              /*     *A  ***/
              rpQueue.push(make_pair(idNext, heuristic));
              /* END *A  ***/
              /*     Dijkstra ***/
              //              rpQueue.push(make_pair(next, greedyScore));
              /*     END Dijkstra ***/
              rparentMap[idNext] = rcurr.first;
            }
          }
        } catch (exception& e) {
          cout << "Out of range exception: " << e.what() << '\n';
        }
      }
      cout << endl << "---------------------------------------------------" << endl;
    }
    cout << "# nodes visited: " << nroNodesVisited << endl;
    return found;
  }

};


#endif
