/* CSE100 WINTER 2017 PA4
 * Author: Shaoze Wang
 * Author: Youyuan Lu
 * Function declaration and definition of class NodeHash
 * Date: 3/3/2017
 */

#ifndef NODEHASH_H
#define NODEHASH_H

#include <vector>
#include <string>
#include <utility>
#include "Node.h"
#define NSLOT 30000  // the size of the hash table
#define NLSHIFT 5  // the number of left shift used in the hash function
#define NRSHIFT 27  // the number of right shift used in the hash function

using namespace std;

/* A class whose instances are hash tables used to store nodes (actors) */
class NodeHash {

  public:

    Node* table[NSLOT];  // the storage
    int size;  // the number of elements in the hash table

    /* constructor */
    NodeHash() {
      size = 0;  // the initial number of elements is zero

      // initialize all slots to be null
      for (int i = 0; i < NSLOT; i++)
        table[i] = nullptr;
    }

    /* get the hash value of an actor name */
    unsigned int hash(string key) {
      unsigned int value = 0;  // the initial hash value
      unsigned int left;
      unsigned int right;
      for (unsigned int i = 0; i < key.length(); i++) {
        // rotate the current hash value and XOR it with a character
        left = value << NLSHIFT;
        right = value >> NRSHIFT;
        value = (left | right) ^ key[i];
      }
      return value % NSLOT;
    }

    /* insert an actor into the hash table */
    int insert(string actorName) {
      int hashValue = hash(actorName);  // get the hash value

      // linear probing
      while (table[hashValue] != nullptr) {
        if (table[hashValue]->name == actorName)
          return hashValue;  // when the actor is already in the hash table
        hashValue = (hashValue + 1) % NSLOT;
      }

      // insert a new actor into the hash table
      size++;
      Node* newNode = new Node(actorName);
      table[hashValue] = newNode;
      return hashValue;
    }

    /* find an actor in the hash table */
    int find(string actorName) {
      int hashValue = hash(actorName); //  get the hash value

      // linear probing
      while (table[hashValue] != nullptr) {
        if (table[hashValue]->name == actorName)
          return hashValue;  // when the actor is found
        hashValue= (hashValue + 1) % NSLOT;
      }
      return -1;  // when the actor is not in the hash table
    }

    /* get the actor at a given index */
    Node* at(int index) {
      return table[index];
    }
};

#endif
