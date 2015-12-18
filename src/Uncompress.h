#ifndef HUFFMAN_UNCOMPRESS_H
#define HUFFMAN_UNCOMPRESS_H

#include "huffman.h"
#include <unistd.h>
#include <fcntl.h>

/**
 * Write the uncompress code to std out
 * @param tree : array of node
 * @param treeSize : size of tree
 * @param textLength : original size of the compressed text
 * @param fSrc : compresses file
 */
void writeUncompressCode(Node* tree,uint16_t treeSize,size_t textLength,FILE* fSrc);

/**
 * Decompress a file
 * @param src : path to the compressed file
 */
void uncompress(char* src);

void uncompressOutput(char *src);


#endif //HUFFMAN_UNCOMPRESS_H
