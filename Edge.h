/* CSE100 WINTER 2017 PA4
 * Author: Shaoze Wang
 * Author: Youyuan Lu
 * Function declaration and definition of class Edge
 * Date: 3/3/2017
 */

#ifndef EDGE_H
#define EDGE_H

#include <vector>
#include <string>
#include <utility>

using namespace std;

/* A class whose instances are edges (movies) in the graph */
class Edge {

  public:

    string fullTitle; // the string <title>#@<year>
    string title;  // the movie title
    int year;  // the year the movie was made
    vector<int> cast;  // the list of cast
    int root;

    /* constructor */
    Edge(string newFull, string newTitle, int newYear):
      fullTitle(newFull), title(newTitle), year(newYear) {
      root = -1;
    }

    /* add a new node (actor) into the list of cast */
    void addCast(int actorIndex) {
      cast.push_back(actorIndex);
    }

    /* get the size of the list of cast */
    int castSize() {
      return cast.size();
    }

    /* get the index of a node (actor) in nodes */
    int nodeIndex(int index) {
      return cast[index];
    }
};

#endif
