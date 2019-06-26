/* CSE100 WINTER 2017 PA4
 * Author: Shaoze Wang
 * Author: Youyuan Lu
 * Function declaration and definition of class TNode
 * Date: 3/13/2017
 */

#ifndef TNODE_H
#define TNODE_H

#include <vector>

using namespace std;

/* A class whose instances are nodes in the graph */
class TNode {

  public:

    vector<TNode*> following;  // list of followings
    vector<TNode*> follower;  // list of followers
    bool visited;

    /* constructor */
    TNode(void){
      visited = false;
    }

    /* Add a new neighbor into two adjacency lists */
    void update(TNode* newFollowing) {
      // update the list of followings of "this"
      this->following.push_back(newFollowing);
      // update the list of followers of "newFollowing"
      newFollowing->follower.push_back(this);
    }

    /* get the size of the adjacency list */
    int adjSize(bool isTranspose) {
      if (isTranspose)  // when the transpose of the original graph is used
        return follower.size();
      else  // when the original grpah is used
        return following.size();
    }

    /* get the index of a neighbor in */
    TNode* getAdj(int index, bool isTranspose) {
      if (isTranspose)  // when the transpose of the original graph is used
        return follower[index];
      else  // when the original graph is used
        return following[index];
    }

};

#endif
