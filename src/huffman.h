#ifndef HUFFMAN_DEFINE_H
#define HUFFMAN_DEFINE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

typedef struct {
	unsigned char name; // char value
	size_t occurrence; // occurrence of the caracter
	short int parent;
	short int rightChild;
	short int leftChild;
	char* code; // Huffman code
} Node;


/**
 * Read a file and store into an array the occurrence of each character
 * O(n) n : size of the file
 * @return the number of different character
 * @param path : path of the file to read
 * @param nbOccurrence: store the occurrence of each character
 *
*/

unsigned short int buildDistribution(FILE* fSrc,unsigned int*nbOccurrence,size_t * textLength);

/**
 * create at array of node,initialize it and return it
 * O(n) complexity : n : number of ascii characters
 * @param nbOccurrence : array which store the number of currency of each character
 * @param treeSize : number of Node
 *
 */

Node* initArray(unsigned int*nbOccurrence,uint16_t treeSize);

/**
 * fill the array by building the huffman tree
 * O(n²) complexity : n: treeSize
 * @param tree : array of node to fill
 * @param treeSize : size of the tree
 *
 */
void fillArray(Node* tree,uint16_t treeSize);

/**
 * write the huffman code of each tree node
 * O(n) complexity : n : root index
 * @param tree : array of node
 * @param root : root index of the tree from where to start
 * @param code : code which store the huffman codes
 *
 */
void calculateCode(Node* tree,uint16_t root,char* code);

/**
 * display the tree on the std out
 * O(n) complexity n = treeSize
 * @param Tree : array of node to print
 * @param treeSize : number of element of tree to print
 *
 */
void displayTree(Node* tree,uint16_t treeSize);

/**
 * wrap the malloc function with error verification
 * return a pointer alloyed by malloc function
 * @param size : size of the pointer
 */
void * scrMalloc(size_t size);

#endif