/*
 * CSE100 WINTER 2017
 * Author: Shaoze Wang
 * Author: Youyuan Lu
 * Function: the main driver for the actor connection program.
 *           decides which method to use based on the flag passed in.
 */
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>
#include <queue>
#include <stack>
#include "ActorGraph.h"
#include "Node.h"
#include "Edge.h"
#include "NodeHash.h"
#include "EdgeHash.h"
#include "MoviesByYear.h"
#include <time.h>

using namespace std;

int main( int argc, char * argv[] ){
  clock_t start = clock();
  if ( argc != 5 ){
    cout << "Cannot parse arguments!" << endl;
  }

  if ( argv[4][0] == 'b' ){
    ActorGraph graph;
    graph.loadFromFile( argv[1], false );
    graph.connect(argv[2], argv[3]);
  }
  else{
    MoviesByYear upTree( argv[1] );
    upTree.writeToFile( argv[2], argv[3] );
  }
  start = clock() - start;
  double duration = (float)start / CLOCKS_PER_SEC;
  cout << duration << endl;
}
