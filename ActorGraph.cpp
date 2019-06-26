/* CSE100 WINTER 2017 PA4
 * Author: Shaoze Wang
 * Author: Youyuan Lu
 * Function definition of class ActorGraph
 * Date: 3/3/2017
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include "ActorGraph.h"
#include "Node.h"
#include "Edge.h"
#include "NodeHash.h"
#include "EdgeHash.h"
#define INIT_YEAR 2020

using namespace std;

/* 
 * Overloads the operator for comparing between two nodes in the priority 
 * queue, to be used by the comparator class. 
 */
bool
Node :: operator< ( const Node& rhs ){
  return (dist == rhs.dist) ? (name > rhs.name) : ( dist > rhs.dist );
}

/*
 * The comparator class to be passed in to the priority queue
 */
class
ActorNodeComp{
  public:
    /* Call on the overloaded comparator to do the comparison */
    bool operator()( Node*& lhs, Node*& rhs ) const{
      return *lhs < *rhs;
    }
};

/* constructor */
ActorGraph::ActorGraph(void) {
  yearHash = vector<vector<pair<int, int>>>(VECTOR_SIZE);
  for ( int i = 0; i < VECTOR_SIZE; i++ ){
    yearHash[i] = vector<pair<int, int>>(0);
  }
}


/* load the graph from file */
bool ActorGraph::loadFromFile(const char* in_filename, bool use_weighted_edges) {
  // Initialize the file stream
  ifstream infile(in_filename);

  bool have_header = false;

  // keep reading lines until the end of file is reached
  while (infile) {
    string s;

    // get the next line
    if (!getline( infile, s )) break;

    if (!have_header) {
      // skip the header
      have_header = true;
      continue;
    }

    istringstream ss( s );
    vector <string> record;

    while (ss) {
      string next;

      // get the next string before hitting a tab and put it in 'next'
      if (!getline( ss, next, '\t' )) break;

      record.push_back( next );
    }

    if (record.size() != 3) {
      // we should have exactly 3 columns
      continue;
    }

    string actor_name(record[0]);
    string movie_title(record[1]);
    int movie_year = stoi(record[2]);

    // insert the actor into nodes
    int nodeIndex = nodes.insert(actor_name);

    Node * currActor = nodes.at( nodeIndex );

    if ( currActor -> career_init > movie_year ){
      currActor -> career_init = movie_year;
    }

    // get the fullTitle of the movie and insert the movie into edges
    string full_title = movie_title + "#@" + record[2]; 
    int edgeIndex = edges.insert(full_title, movie_title, movie_year);

    // update the graph
    (nodes.at(nodeIndex))->addAdj(edgeIndex);
    (edges.at(edgeIndex))->addCast(nodeIndex);
  }

  if (!infile.eof()) {
    cerr << "Failed to read " << in_filename << "!\n";
    return false;
  }
  infile.close();

  return true;
}

/* 
 * Apply the same bfs algorithm only to movies before the year specified.
 * The only difference is that when going through the adjacency list of 
 * the pooped node, only check the cast of those movies made before the 
 * specified year.
 */
Node* ActorGraph::bfsByYear(string start, string end, int year_limit) {
  // check if two actors are both in the graph
  int sIndex = nodes.find(start);
  if (sIndex == -1) {  // when 'start' is not in the graph
    cout << "(" << start << ") doesn't exist." << endl;
    return nullptr;
  }
  int eIndex = nodes.find(end);
  if (eIndex == -1) {  // when 'end' is not in the graph
    cout << "(" << end << ") doesn't exist." << endl;
    return nullptr;
  }

  queue<Node*> temp;  // the queue used by BFS
  Node* sNode = nodes.at(sIndex);  // get the starting node
  sNode->update("", nullptr, 0);  // initialize the starting node

  if (sIndex == eIndex)  // when 'start' and 'end' are the same
    return nodes.at(sIndex);

  temp.push(sNode);  // push the starting node into the queue

  // perform BFS
  while (!temp.empty()) {
    Node* currNode = temp.front();
    temp.pop();
    // access each movie currNode played in
    for (unsigned int i = 0; i < currNode->adjSize(); i++) {
      Edge* currEdge = edges.at(currNode->edgeIndex(i));
      // visit all cast of the movie before the year specified in the list
      if ( currEdge -> year <= year_limit ){
        for (unsigned int j = 0; j < currEdge->castSize(); j++) {
          Node* neighbor = nodes.at(currEdge->nodeIndex(j));
          int newDist = currNode->dist + 1;
          // when the neighbor hasn't been visited before
          if (newDist < neighbor->dist) {
            // update the neighbor
            neighbor->update(currEdge->fullTitle, currNode, newDist);
            temp.push(neighbor);
            // when 'end' is found
            if (neighbor->name == end)
              return neighbor;
          }
        }
      }
    }
  }
  // when all nodes reachable from 'start' have been visited
  return nullptr;
}

/* 
 * Perform the breadth first search on the graph.
 * First push the start actor node into the queue.
 * Then keep popping the queue until it is empty.
 * For every node popped, examine its adjacency list and go through all
 * the movies' cast.
 *   For every actor visited, if the dist property is larger than the dist
 *   of the popped node plus 1, update the dist property and set the prev 
 *   property of the visited node to the popped node,then push the visited
 *   node into the queue.
 *   If the end node is pushed, return the end node.
 * Return nullptr.
 */
Node* ActorGraph::bfs(string start, string end) {
  // check if two actors are both in the graph
  int sIndex = nodes.find(start);
  if (sIndex == -1) {  // when 'start' is not in the graph
    cout << "(" << start << ") doesn't exist." << endl;
    return nullptr;
  }
  int eIndex = nodes.find(end);
  if (eIndex == -1) {  // when 'end' is not in the graph
    cout << "(" << end << ") doesn't exist." << endl;
    return nullptr;
  }

  queue<Node*> temp;  // the queue used by BFS
  Node* sNode = nodes.at(sIndex);  // get the starting node
  sNode->update("", nullptr, 0);  // initialize the starting node

  if (sIndex == eIndex)  // when 'start' and 'end' are the same
    return nodes.at(sIndex);

  temp.push(sNode);  // push the starting node into the queue

  // perform BFS
  while (!temp.empty()) {
    Node* currNode = temp.front();
    temp.pop();
    // access each movie currNode played in
    for (unsigned int i = 0; i < currNode->adjSize(); i++) {
      Edge* currEdge = edges.at(currNode->edgeIndex(i));
      // visit all cast of a movie
      for (unsigned int j = 0; j < currEdge->castSize(); j++) {
        Node* neighbor = nodes.at(currEdge->nodeIndex(j));
        int newDist = currNode->dist + 1;
        // when the neighbor hasn't been visited before
        if (newDist < neighbor->dist) {
          // update the neighbor
          neighbor->update(currEdge->fullTitle, currNode, newDist);
          temp.push(neighbor);
          // when 'end' is found
          if (neighbor->name == end){
            return neighbor;
          }
        }
      }
    }
  }
  // when all nodes reachable from 'start' have been visited
  return nullptr;
}

/*
 * Perform the SSSP on a weighted graph.
 * First push the start actor node into the priority queue. 
 * Next keep poping the priority queue until it is empty.
 * For every node popped, set it to done and then check all its related 
 * actor nodes by going into the casts of all the movies in the popped 
 * node's adjacency list.
 *   For every related actor node visited, if it is not done, check if its
 *   dist is larger than the popped node's dist + the weight of that movie.
 *     if so, update the dist to the latter, the prev to the popped node, 
 *     and push that node into the queue.
 * If the node popped is the end actor node, return.
 * Return nullptr.
 */
Node * ActorGraph :: dijkstra( string start, string end ){

  /* Check and then get the index of the two actors passed in */
  int iIndex = nodes.find( start );
  if ( iIndex == -1 ){
    cout << "(" << start << ") doesn't exist." << endl;
  }
  int eIndex = nodes.find( end );
  if ( eIndex == -1 ){
    cout << "(" << end << ") doesn't exist." << endl;
  }  

  /* Get the pointer to the nodes using the indexes */
  Node * iNode = nodes.at( iIndex );
  Node * eNode = nodes.at( eIndex );

  /* Create the container for the algorithm */
  priority_queue<Node*,vector<Node*>,ActorNodeComp> queue;

  /* Set the start actor's done as first */
  iNode -> done = false;
  iNode -> dist = 0;

  /* Push the start actor node into the priority queue */
  queue.push( iNode );

  /* Keep popping the queue until it is empty */
  while( queue.size() != 0 ){
    Node * toCheck = queue.top();
    queue.pop();
    if ( toCheck -> done == false ){
      /* Set the popped node to done */
      toCheck -> done = true;
      /* If the popped node is the end actor node, break */
      if ( toCheck == eNode ){
        break;
      }

      /* 
       * Go thorugh all the casts of all the movies in the adjacency 
       * list of the popped node.
       */
      int currDist = toCheck -> dist;
      int adjSize = toCheck -> adj.size();
      for ( int i = 0; i < adjSize; i++ ){
        Edge * currChild = edges.at( toCheck -> adj[i] );
        int castSize = currChild -> cast.size();
        for ( int j = 0; j < castSize; j++ ){
          Node * currActor = nodes.at( currChild -> cast[j] );
          if ( currActor != toCheck ){
            /* 
             * Check if the distance of the popped node plus the distance 
             * between the visited node and the popped node is smaller than
             * the distance property of the visited node.
             * If so, update the dist, prev property of the visited node
             * and then push the visited node into the priority queue.
             */
            int newDist = currDist + 2015 - currChild -> year + 1;
            if ( newDist < currActor -> dist ){
              currActor -> dist = newDist;
              currActor -> prev = pair<string, Node*>( 
                                  currChild -> fullTitle, toCheck );
              queue.push( currActor );
            }
          }
        }
      }
    }
  }

  /* If the end node is not found, return false. */
  if ( eNode -> done == false ){
    return nullptr;
  }
  
  /* Return the end node. */
  return eNode;
} 

/*
 * The method used to run bfs year by year to find out the earliset year 
 * two actors became connected.
 * Calls the bfsByYear first passing in the earlist year where both actors
 * had appeared in a movie.
 * Then increase that common year by 1 and call bfsByYear again.
 * Repeat the above process until the year exceeeds the limit.
 *   During the process, if the two actor became connected, return the year
 */
bool
ActorGraph :: connect( const char * inName, const char * outName ){
  ifstream inFile ( inName );
  ofstream outFile ( outName );
  outFile << "Actor1\tActor2\tYear" << endl;
  bool have_header = false;
 
  /* 
   * Read in all the actor pairs and perform the algorithms stated above
   * for all of them.
   */
  while( inFile ){
    string s;
 
    if ( !getline( inFile, s ) ) break;

    if (!have_header) {
      have_header = true;
      continue;
    }

    istringstream ss( s );
    vector<string> record;

    while( ss ){
      string next;
      if ( !getline( ss, next, '\t' ) ) break;

      record.push_back( next );
    }

    if ( record.size() != 2 ){
      continue;
    }

    /* Extract the two actors' name */
    string actorName1( record[0] );
    string actorName2( record[1] );

    /* Get the nodes using the indexes */
    Node * actor1 = nodes.at( nodes.find(actorName1) );
    Node * actor2 = nodes.at( nodes.find(actorName2) );

    /* 
     * Determine the common year using the larger career_init of the two 
     * actors.
     */
    int start_year = actor1 -> career_init;
    if ( start_year < actor2 -> career_init ) {
      start_year = actor2 -> career_init;
    }   
  
    /*
     * Increase the year and perform bfs until the uear exceeds the limit 
     * or the connection is found.
     */ 
    while( 1 ){
      if ( start_year <= 2020 ){
        if ( this -> bfsByYear( actorName1, actorName2, start_year ) 
             != nullptr ){
          outFile<< actorName1 << "\t" << actorName2 << "\t" << start_year << endl;
        break;
        }
        start_year++;
        reset();
      }
      else{
        cout << actorName1 << " " << actorName2 << "Not found" << endl;
        break;
      }
    }
    this -> reset();
  }

  return true;
}

/* find the shortest path for every pair in the file and print them */
bool ActorGraph::writeToFile(const char* in_pair, const char* out_file,
                             bool unweighted) {
  ifstream inpair(in_pair);
  ofstream outfile(out_file);
  outfile << "(actor)--[movie#@year]-->(actor)--..." << endl;

  bool have_header = false;

  // keep reading lines until the end of file is reached
  while (inpair) {
    string s;

    // get the next line
    if (!getline( inpair, s )) break;

    if (!have_header) {
      // skip the header
      have_header = true;
      continue;
    }

    istringstream ss( s );
    vector <string> record;

    while (ss) {
      string next;

      // get the next string before hitting a tab and put it in 'next'
      if (!getline( ss, next, '\t' )) break;

      record.push_back( next );
    }

    if (record.size() != 2) {
      // we should have exactly 2 columns
      continue;
    }

    string actor1(record[0]);
    string actor2(record[1]);

    Node * end;

    // call bfs() to find the shortest path connecting actor1 and actor2
    if ( unweighted ){
      end = bfs(actor1, actor2);
    }
    else{
      end = dijkstra( actor1, actor2 );
    }

    // when no such path exists
    if (end == nullptr) {
      outfile << "No valid path between ("; 
      outfile << actor1 << ") and (" << actor2 << ")." << endl;
      reset(); // reset all nodes in the graph for the next bfs()
      continue;
    }

    // use a stack to print the path in the correct order
    stack<string> temp;
    // push the whole path into the stack
    temp.push("(" + end->name + ")");
    while (end->dist != 0) {
      string str="("+end->prev.second->name+")--["+end->prev.first+"]-->";
      temp.push(str);
      end = end->prev.second;
    }
    // pop the path out of the stack
    while (!temp.empty()) {
      outfile << temp.top();
      temp.pop();
    }
    outfile << endl;
    reset();  // reset all nodes in the graph for the next bfs()
  }

  if (!inpair.eof()) {
    cerr << "Failed to read " << in_pair << "!\n";
    outfile.close();
    return false;
  }
  inpair.close();
  outfile.close();

  return true;
}

/* reset prev and dist of every node in the graph */
void ActorGraph::reset() {
  for (int i = 0; i < NSLOT; i++) {
    Node* currNode = nodes.at(i);
    if (currNode != nullptr){
      currNode->update("", nullptr, INFINITY);  // reset prev and dist
      currNode -> done = false;
      //currNode -> visit = false;
    }
  }
}

/* destructor */
ActorGraph::~ActorGraph() {
  for (int i = 0; i < NSLOT; i++) {
    // delete all nodes
    Node* currNode = nodes.at(i);
    if (currNode != nullptr)
      delete currNode;
    // delete all edges
    Edge* currEdge = edges.at(i);
    if (currEdge != nullptr)
      delete currEdge;
  }
}
