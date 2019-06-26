/* CSE100 WINTER 2017 PA4
 * Author: Shaoze Wang
 * Author: Youyuan Lu
 * Function definition of class TGraph
 * Date: 3/13/2017
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include "TGraph.h"
#include "TNode.h"

using namespace std;

/* constructor */
TGraph::TGraph(void) {
  // initialize arrays on heap
  storage = new TNode* [SLOT];
  stat = new int [SLOT];
  for (int i = 0; i < SLOT; i++) {
    storage[i] = nullptr;
    stat[i] = 0;
  }
}

/* load the graph from file */
bool TGraph::loadFromFile(const char* in_filename) {
  // Initialize the file stream
  ifstream infile(in_filename);

  // keep reading lines until the end of file is reached
  while (infile) {
    string s;

    // get the next line
    if (!getline( infile, s )) break;

    istringstream ss( s );
    vector <string> record;

    while (ss) {
      string next;

      // get the next string before hitting a space and put it in 'next'
      if (!getline( ss, next, ' ' ) || record.size() == 2) break;

      record.push_back( next );
    }

    if (record.size() != 2) {
      continue;
    }

    int first = stoi(record[0]);
    int second = stoi(record[1]);

    // insert a pair of nodes into storage
    if (storage[first] == nullptr) {
      storage[first] = new TNode;
      vertices.push(storage[first]);
    }
    if (storage[second] == nullptr) {
      storage[second] = new TNode;
      vertices.push(storage[second]);
    }

    // update the graph
    storage[first]->update(storage[second]);
  }

  if (!infile.eof()) {
    cerr << "Failed to read " << in_filename << "!\n";
    return false;
  }

  infile.close();

  return true;
}

/* perform DFS to find all nodes that are reachable from a given node */
void TGraph::dfs(stack<TNode*>& input, stack<TNode*>& output, bool isTranspose) {
  while (!input.empty()) {
    TNode* curr = input.top();
    input.pop();
    if (!curr->visited) {
      int size = 0;  // the size of a strongly connected group
      dfs_visit(size, curr, output, isTranspose);
      if (isTranspose)
        stat[size]++;  // update the statistics
    }
  }
  if (isTranspose) {  // output the statistics
    cout << "size of strongly connected group   ";
    cout << "# strongly connected group" << endl;
    for (int i = 0; i < SLOT; i++)
      if (stat[i] != 0) {
        cout.width(32);
        cout << right << i;
        cout.width(29);
        cout << right << stat[i] << endl;
      }
  }
}

/* helper method of dfs() */
void TGraph::dfs_visit(int& size, TNode* root, stack<TNode*>& output, bool isTranspose) {
  root->visited = true;
  // visit all nodes reachable from 'root'
  for (int i = 0; i < root->adjSize(isTranspose); i++)
    if (!root->getAdj(i, isTranspose)->visited)
      dfs_visit(size, root->getAdj(i, isTranspose), output, isTranspose);
  output.push(root);
  size++;
}

/* reset "visited" flag of every node in the graph */
void TGraph::reset() {
  for (int i = 0; i < SLOT; i++) {  // reset all nodes
    TNode* curr = storage[i];
    if (curr != nullptr)
      curr->visited = false;
  }
}

/* destructor */
TGraph::~TGraph() {
  for (int i = 0; i < SLOT; i++) {  // delete all nodes
    TNode* curr = storage[i];
    if (curr != nullptr)
      delete curr;
  }

  // delete arrays on heap
  delete [] storage;
  delete [] stat;
}
