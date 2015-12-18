#ifndef HUFFMAN_COMPRESS_H
#define HUFFMAN_COMPRESS_H

#include "huffman.h"
#include <unistd.h>
#include <time.h>
#include <stdint.h>

/**
 * Convert a char * to binary code into c
 * @param code : huffman code of a character
 * @param c : character which will store bit by bit the huffman code
 * @param strIndex : begin index of code
 * @param index : index th bit of c to write
 * @param length : number of huffman character to write into c
 * ex :
 * code = "101011"(10) code ="00000000"(2) strIndex = 2 , length = 3 , index = 3
 * code[2;2+length] = "101"(10)
 * write 001(2) to code from index
 * c[3;3+length] = "000"(2) -> "101"
 * So now c = "00010100"(2)
 */

void toBinCode(char* code ,unsigned char* c,uint16_t strIndex , uint16_t index, uint16_t length);

/**
 *  write the huffman header to the file fDest
 *  @param tree : array of node
 *  @param treeSize : size of tree
 *  @param fSrc : File where to write the header
 */
size_t writeHeader(Node* tree, uint16_t treeSize,FILE *fDest);

/**
 * write the compressed code  body to the file fDest
 * @param tree : array of node
 * @param treeSize : size of tree
 * @param src : path to the source file to read
 * @param dest : path to the destination file to write
 */
size_t writeCode(Node* tree, int16_t* indexInTree,FILE* fSrc,FILE* fDest) ;

/**
 * Compress a file
 * @param src : path of the file to compress
 * @param dest : path of the compress file
 */
void compress(char* src, char* dest);

#endif //HUFFMAN_COMPRESS_H
