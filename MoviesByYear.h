/*
 * CSE100 WINTER 2017
 * Author: Shaoze Wang
 * Author: Youyuan Lu
 * Function: the class for building the uptree structure.
 */
#ifndef MOVIESBYYEAR_H
#define MOVIESBYYEAR_H

#define YEAR 120

#include <iostream>
#include <vector>
#include "NodeHash.h"
#include "EdgeHash.h"

class MoviesByYear{
  public:
    /* The starting point of loeading movies */
    int init_year;
    /* Vector that stores the movies by year */
    vector<vector<pair<int, int>>> movies;
    /* The hashtable that stores the nodes read in */
    NodeHash nodes;
    /* The hashtable that sotres the edges read in */
    EdgeHash edges;
    /* Constructor that takes in a file and bulds the uptree */
    MoviesByYear( const char * filename );
    /* Reset the uptree */
    void reset();
    /* Load until a specific year */
    void loadTillYear( int );
    /* Load one year forward */
    void loadOneYear();
    /* Merge the two nodes' sentinel nodes */
    int merge(int, int);
    /* Check if the two nodes have the same sentinel nodes */
    bool find(int, int);
    void writeToFile( const char *, const char * );
    /* Descturctor */
    ~MoviesByYear();
};

#endif
