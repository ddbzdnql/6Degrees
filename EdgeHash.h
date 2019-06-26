/* CSE100 WINTER 2017 PA4
 * Author: Shaoze Wang
 * Author: Youyuan Lu
 * Function declaration and definition of class EdgeHash
 * Date: 3/3/2017
 */

#ifndef EDGEHASH_H
#define EDGEHASH_H

#include <vector>
#include <string>
#include <utility>
#include "Edge.h"
#define ESLOT 30000  // the size of the hash table
#define ELSHIFT 5  // the number of left shift used in the hash function
#define ERSHIFT 27  // the number of right shift used in the hash function

using namespace std;

/* A class whose instances are hash tables used to store edges (movies) */
class EdgeHash {

  public:

    Edge* table[ESLOT];  // the storage
    int size;  // the number of elements in the hash table

    /* constructor */
    EdgeHash() {
      size = 0;  // the initial number of elements is zero

      // initialize all slots to be null
      for (int i = 0; i < ESLOT; i++)
        table[i] = nullptr;
    }

    /* get the hash value of the fullTitle of a movie */
    unsigned int hash(string key) {
      unsigned int value = 0;  // the initial hash value
      unsigned int left;
      unsigned int right;
      for (unsigned int i = 0; i < key.length(); i++) {
        // rotate the current hash value and XOR it with a character
        left = value << ELSHIFT;
        right = value >> ERSHIFT;
        value = (left | right) ^ key[i];
      }
      return value % ESLOT;
    }

    /* insert a movie into the hash table */
    int insert(string full, string title, int year) {
      int hashValue = hash(full);  // get the hash value

      // linear probing
      while (table[hashValue] != nullptr) {
        if (table[hashValue]->fullTitle == full)
          return hashValue;  // when the movie is found
        hashValue = (hashValue + 1) % ESLOT;
      }

      // insert a new movie into the hash table
      size++;
      Edge* newEdge = new Edge(full, title, year);
      table[hashValue] = newEdge;
      return hashValue;
    }

    /* get the movie at a given index */
    Edge* at(int index) {
      return table[index];
    }
};

#endif
