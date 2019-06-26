/* CSE100 WINTER 2017 PA4
 * Author: Shaoze Wang
 * Author: Youyuan Lu
 * Main program that builds the graph and finds strongly connected groups
 * Date: 3/13/2017
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include "TGraph.h"
#include "TNode.h"

using namespace std;

/* Main program that builds the graph and finds strongly connected groups */
int main(int argc, char* argv[]) {
  // check the number of arguments
  if (argc != 2) {
    cout << "Wrong number of arguments." << endl;
    return 0;
  }

  TGraph graph;
  bool isBuilt = graph.loadFromFile(argv[1]);
  if (isBuilt) {  // when the graph is loaded from file successfully
    // perform DFS on the original graph
    graph.dfs(graph.vertices, graph.finished, false);
    graph.reset();
    // perform DFS on the transpose of the original graph
    graph.dfs(graph.finished, graph.temp, true);
  }
  return 0;
}
