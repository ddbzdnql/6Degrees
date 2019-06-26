/* CSE100 WINTER 2017 PA4
 * Author: Shaoze Wang
 * Author: Youyuan Lu
 * Function declaration of class ActorGraph
 * Date: 3/3/2017
 */

#ifndef ACTORGRAPH_H
#define ACTORGRAPH_H

#include <iostream>
#include <queue>
#include <vector>
#include <stack>
#include "Node.h"
#include "Edge.h"
#include "NodeHash.h"
#include "EdgeHash.h"

#define VECTOR_SIZE 120

using namespace std;

/* A class whose instances are graphs */
class ActorGraph {

  public:

    NodeHash nodes;  // a hash table used to store nodes (actors)
    EdgeHash edges;  // a hash table used to store edges (movies)

    vector<vector<pair<int, int>>> yearHash;

    /* constructor */
    ActorGraph(void);

    /* load the graph from file */
    bool loadFromFile(const char* in_filename, bool use_weighted_edges);

    /* perform BFS to find the shortest path connecting two actors */
    Node* bfs(string start, string end);

    Node * bfsByYear( string, string, int );

    Node* dijkstra( string, string );

    /* find the shortest path for every pair in the file and print them */
    bool writeToFile(const char* in_pairname, const char* out_filename,
                     bool unweighted);

    /* reset prev and dist of every node in the graph */
    void reset();

    bool connect( const char*, const char* );

    //void sortByYear( const char *&);

    //void buildByYear( int );

    /* destructor */
    ~ActorGraph();
};

#endif // ACTORGRAPH_H
