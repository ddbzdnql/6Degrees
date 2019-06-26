/*
 * CSE100 WINTER 2017
 * Author: Shaoze Wang
 * Author: Youyuan Lu
 * Function: The uptree structure that takes in a file to build an uptree
 * out of it. The compression takes place when merge happens.
 */
#include "MoviesByYear.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
#include <stack>

#define YEAR_INIT 2020

/* Constructor using a file */
MoviesByYear :: MoviesByYear( const char * filename ){
  init_year =  YEAR_INIT - YEAR + 1 ;
  /* Open a file and sort all its items using hashtable by year */
  ifstream inFile ( filename );
  movies = vector<vector<pair<int, int>>>(120);
  for ( int i = 0; i < 120; i++ ){
    movies[i] = vector<pair<int, int>>(0);
  } 
  bool header = false;

  while( inFile ){
    string s;

    if ( !getline( inFile, s ) ) break;

    if ( header == false ){
       header = true;
       continue;
    }

    istringstream ss( s );
    vector<string> record;    

    while( ss ){
      string next;
      if ( !getline( ss, next, '\t' ) ) break;
      record.push_back( next );
    }

    if ( record.size() != 3 ){
      continue;
    }

    int actorIndex = nodes.insert( record[0] );

    int year = stoi(record[2]);

    Node * currActor = nodes.at( actorIndex );
    if ( currActor -> career_init > year ){
      currActor -> career_init = year;
    }

    string movie_title( record[1] );
    string full_title = movie_title + "#@" + record[2];

    int edgeIndex = edges.insert( full_title, movie_title, year );

    Edge * currEdge = edges.at( edgeIndex );
    if ( currEdge -> root == -1 )
    currEdge -> root = actorIndex;    

    int yearIndex = YEAR_INIT - year;

    movies[yearIndex].push_back( pair<int, int>(actorIndex, edgeIndex) );

  }
}

/* Reset the actors' parent field */
void
MoviesByYear :: reset(){
  for ( int i = 0; i < NSLOT; i++ ){
    Node * currNode = nodes.at(i);
    if ( currNode != nullptr ){
      currNode -> parent = -1;
    }
  }
  init_year = YEAR_INIT - YEAR + 1;
}

/* Load from the year hasbtable until some year */
void 
MoviesByYear :: loadTillYear( int year_limit ){
  for ( int i = init_year; i <= year_limit; i++ ){
    int index = YEAR_INIT - i;
    for ( int j = 0; j < movies[index].size(); j++ ){
      //cout << j << endl;
      int index1 = movies[index][j].first;
      int index2 = edges.at(movies[index][j].second) -> root;
      //cout << "Merge start" << endl;
      merge( index1, index2 );
      //cout << "Merge finished" << endl;
    }
  }
  init_year = year_limit;
}

/* Load one year more */
void 
MoviesByYear :: loadOneYear(){
  int index = YEAR_INIT - ++init_year; 
  for ( int j = 0; j < movies[index].size(); j++ ){
    int index1 = movies[index][j].first;
    int index2 = edges.at(movies[index][j].second) -> root;
    merge( index1, index2 );
  }
}

/* Merge the two uptrees' sentinel node */
int 
MoviesByYear :: merge( int index1, int index2 ){
  //cout << index1 << " " << index2 << endl;
  if ( index1 == index2 ){
    return index1;
  }
  if ( nodes.at(index1) -> parent == -1 && 
       nodes.at(index2) -> parent == -1 ){
    nodes.at(index2) -> parent = index1;
    return index1;
  }
  else{
    int dup1 = index1, dup2 = index2;
    if ( nodes.at(dup1) -> parent != -1 ){
      dup1 = nodes.at(dup1) -> parent;
    }
    if ( nodes.at(dup2) -> parent != -1 ){
      dup2 = nodes.at(dup2) -> parent;
    }
    int new_parent = merge( dup1, dup2 );
    if ( index1 != dup1 ){
      nodes.at(index1) -> parent = new_parent;
    }
    if ( index2 != dup2 ){
      nodes.at(index2) -> parent = new_parent;
    }
    return new_parent;
  }
}

/* Find the two node to see if they have the same sentinel node */
bool
MoviesByYear :: find( int index1, int index2 ){
  if ( nodes.at(index1) -> parent == -1 &&
       nodes.at(index2) -> parent == -1 ){
    return index1 == index2;
  }
  else{
    int dup1 = index1, dup2 = index2;
    if ( nodes.at(dup1) -> parent != -1 ){
      dup1 = nodes.at(dup1) -> parent;
    }
    if ( nodes.at(dup2) -> parent != -1 ){
      dup2 = nodes.at(dup2) -> parent;
    }
    return find( dup1, dup2 );
  }
}

/* Read from a file to look for pairs to find. And write the result to
 * another file
 */
void
MoviesByYear :: writeToFile( const char * filename, const char * outname ){
  ifstream inFile( filename );
  ofstream outFile( outname );
  outFile << "Actor1\tActor2\tYear" << endl;
  if ( inFile ){
    string s;
    getline( inFile, s );
  }
  else{
    return;
  }

  while( inFile ){
    string s;
    if ( !getline( inFile, s ) ) break;

    istringstream ss( s );
    vector<string> record;

    outFile << s << '\t';

    while( ss ){
      string next;
      if ( !getline( ss, next, '\t' ) ) {
        break;
      }
      record.push_back( next );
      
    }
    
    int index1 = nodes.find( record[0] );
    int index2 = nodes.find( record[1] );

    int common_init = nodes.at(index1) -> career_init;
    if ( common_init < nodes.at(index2) -> career_init ){
      common_init = nodes.at(index2) -> career_init;
    }

    loadTillYear( common_init );

    while( init_year < YEAR_INIT ){
      bool result = find( index1, index2 );
      if ( result == true ){
        outFile  << init_year << endl;
        break;
      }
      loadOneYear();
    }

    

    reset();
  }
}

  MoviesByYear::~MoviesByYear(){
    for ( int i = 0; i < NSLOT; i++ ){
      if ( nodes.table[i] != nullptr ){
        delete nodes.table[i] ;
      }
    }
    for ( int i = 0; i < ESLOT; i++ ){
      if ( edges.table[i] != nullptr ){
        delete edges.table[i] ;
      }
    }
  }
