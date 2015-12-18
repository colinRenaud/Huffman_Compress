#include "Uncompress.h"
#define BUFFER_SIZE 2048

void writeUncompressCode(Node* tree,uint16_t treeSize,size_t textLength,FILE* fSrc) {
    if (! fSrc) {  printf("Error passing null File pointer for decompress");exit(-1); }

    unsigned char* writeBuffer = (unsigned char *)  scrMalloc(sizeof(char) * BUFFER_SIZE); // write buffer
    unsigned char* readBuffer = (unsigned char *)  scrMalloc(sizeof(char) * BUFFER_SIZE); // read buffer

    int16_t treeIndex = treeSize - 1; // begin on the root of the tree
    size_t nbRead = 0;
    size_t nbWrite = 0; // total number of character written
    size_t writeIndex = 0; // current writeBuffer index

    while ( (nbRead = fread(readBuffer, sizeof(char),BUFFER_SIZE, fSrc)) ) { // read the compressed file
        size_t i=0;
        while(i < nbRead && nbWrite <= textLength  ) { // loop the buffer
            int16_t j = 7;  // read the bit from left to right ex : 1000 0000  begin at 1
            while(j >= 0 ) { // loop 8 bits of the characters
                if (tree[treeIndex].leftChild == -1 ) { // no children -> leaf -> character find
                    writeBuffer[writeIndex] = tree[treeIndex].name; // write the character
                    writeIndex++; // new index on buffer
                    treeIndex = treeSize - 1; // return to the root of the tree
                    nbWrite ++;
                }
                if(writeIndex == BUFFER_SIZE) { // end of write buffer
                    for (size_t k = 0; k < writeIndex; k++) // flush the buffer : print characters
                        printf("%c", writeBuffer[k]);
                    writeIndex = 0; // just return at begin to continue to write
                }
                if ((readBuffer[i] >> j) & 1) //  bit is 1
                    treeIndex = tree[treeIndex].rightChild; // continue to right part of the tree
                else // bit is 0
                    treeIndex = tree[treeIndex].leftChild; // continue to left part of the tree
                j--;
                if(treeIndex < 0) // when only one character
                    treeIndex = 0;
            }
            i++;
        }
    }
    for (size_t k = 0; k < writeIndex; k++) // flush the buffer : print characters
        printf("%c",writeBuffer[k]);
    free(readBuffer);
    free(writeBuffer);
}

void uncompress(char* src) {
    FILE *fSrc = NULL;
    if( !  (fSrc = fopen(src,"r") ) ) { printf("Error could'nt open the file to uncrompress : %s ",src); exit(1); }
    uint16_t nbCar = 0;
    unsigned int nbOccurence[256] = {0};
    unsigned char c = 0;
    size_t textLength = 0;
    if( ! fread(&textLength,sizeof(size_t),1,fSrc) )  { printf("Error No header present"); exit(1); } // read the text length
    if ( ! fread(&nbCar,sizeof(uint16_t),1,fSrc) ) { printf("Error header misformed : couldn't read number of différent char "); exit(1); } // read the number of different characters
    if(nbCar > 0) {
        uint16_t treeSize = (2 * nbCar) - 1;
        for (uint16_t i = 0; i < nbCar; i++) { // read the header
            if( ! fread(&c, sizeof(char), 1, fSrc)) { printf("Error header misformed : couldn't read présent character"); exit(1); }
            if( ! fread(&nbOccurence[(int) c], sizeof(size_t), 1, fSrc) ) { printf("Error header misformed : couldn't read occurence of character"); exit(1); }
        }

        Node *tree = initArray(nbOccurence, treeSize); // create Huffman Tree from occurrence array
        fillArray(tree, treeSize); // just fill the array
        if(nbCar == 1)
            tree[0].code = "0";
        else
            calculateCode(tree, treeSize - 1,""); // calculate the huffman codes of characters
        writeUncompressCode(tree, treeSize,textLength,fSrc); // uncompress the code
        //printf("\nEllapsed time for uncompress file : %f ms \n",(double) (t2-t1)/(CLOCKS_PER_SEC/1E3));
        if(nbCar > 1) {
            for(uint16_t i=0 ; i<nbCar ; i++)
                free(tree[i].code);
        }
        free(tree);
    }
    else
        printf("There is no caracter in the Compressed file\n");
    fclose(fSrc);
}

int main(int argc,char** argv) {
    if(argc < 2) {
		printf("Error no argument specified :\n Run ./Huffman -h for print help\n"); exit(-1);
	}
	if(! strcmp(argv[1],"-h") )  // help
		printf("Decompress : ./Dehuf file_to_decompress.huf \n");
	else {
        if(argc != 2) {
            printf("Error bad number of argument \n Run ./Huffman -h for print help\n"); exit(-1);
        }
        uncompress(argv[1]);
	}
    return 0;
}
