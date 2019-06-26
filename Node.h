/* CSE100 WINTER 2017 PA4
 * Author: Shaoze Wang
 * Author: Youyuan Lu
 * Function declaration and definition of class Node
 * Date: 3/3/2017
 */

#ifndef NODE_H
#define NODE_H

#include <vector>
#include <string>
#include <utility>
#define INFINITY 0x7FFFFFFF  // the initial dist

using namespace std;

/* A class whose instances are nodes (actors) in the graph */
class Node {

  public:

    string name;  // the name of the actor
    pair<string, Node*> prev;  // previous node and the corresponding movie
    int dist;  // the distance from the starting point
    vector<int> adj;  // adjacency list
    bool done;
    int career_init;
    int parent;

    /* constructor */
    Node(string input):name(input), dist(INFINITY){
      prev = make_pair("", nullptr);
      done = false;
      //visit = false;
      career_init = INFINITY;
      parent = -1;
    }

    /* update prev and dist to record the path */
    void update(string movie, Node* actor, int newDist) {
      prev.first = movie;
      prev.second = actor;
      dist = newDist;
    }

    /* Add a new edge (movie) into the adjacency list */
    void addAdj(int MovieIndex) {
      adj.push_back(MovieIndex);
    }

    /* get the size of the adjacency list */
    int adjSize() {
      return adj.size();
    }

    /* get the index of an edge (movie) in edges */
    int edgeIndex(int index) {
      return adj[index];
    }

    bool operator< ( const Node& rhs );
};

#endif
