/* CSE100 WINTER 2017 PA4
 * Author: Shaoze Wang
 * Author: Youyuan Lu
 * Function declaration of class TGraph
 * Date: 3/13/2017
 */

#ifndef TGRAPH_H
#define TGRAPH_H

#include <iostream>
#include <vector>
#include <stack>
#include "TNode.h"
#define SLOT 500000

using namespace std;

/* A class whose instances are graphs */
class TGraph {

  public:

    TNode** storage;
    int* stat;
    stack<TNode*> vertices;
    stack<TNode*> finished;
    stack<TNode*> temp;

    /* constructor */
    TGraph(void);

    /* load the graph from file */
    bool loadFromFile(const char* in_filename);

    /* perform DFS to find all nodes that are reachable from a given node */
    void dfs(stack<TNode*>& input, stack<TNode*>& output, bool isTranspose);

    /* helper method of dfs() */
    void dfs_visit(int& size, TNode* root, stack<TNode*>& output, bool isTranspose);

    /* reset "visited" flag of every node in the graph */
    void reset();

    /* destructor */
    ~TGraph();
};

#endif
