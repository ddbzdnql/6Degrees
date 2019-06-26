/* CSE100 WINTER 2017 PA4
 * Author: Shaoze Wang
 * Author: Youyuan Lu
 * The main program that builds the graph, finds paths and prints them
 * Date: 3/3/2017
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

using namespace std;
/* The main program that builds the graph, finds paths and prints them */
int main(int argc, char* argv[]) {
  // check the number of arguments
  if (argc != 5) {
    cout << "Wrong number of arguments." << endl;
    return 0;
  }

  // check if argv[2] (specifying the edge type) is valid
  string type(argv[2]);
  if (type != "u" && type != "w") {
    cout << type << " is not a valid edge type." << endl;
    return 0;
  }
  ActorGraph graph;
  bool u = (type == "u" ? true : false);  // get the edge type
  bool isBuilt = graph.loadFromFile(argv[1], u);
  if (isBuilt)  // when the graph is loaded from file successfully
    graph.writeToFile(argv[3], argv[4],u);
  return 0;
}
