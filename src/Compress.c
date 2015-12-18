#include "Compress.h"

/**
 * change the index ième bit of var to bit value
 * @param var : variable to change the bit
 * @param bit : value to write to the bit 0 or 1
 * @param index : bit number index to write
 */
#define changeBit(var,bit,index) ( var ^= (-bit ^ var) & (1 << index) )

#define BUFFER_SIZE 2048

void toBinCode(char* code,unsigned char* c,uint16_t strIndex , uint16_t index,uint16_t length) {
    for(uint16_t k = strIndex; k < length ; k++) {
        if(code[k] == '0')
            changeBit(*c,0, (7-index) );
        else if(code[k] == '1')
            changeBit(*c,1, (7-index) );
        index++;
    }
}

size_t writeHeader(Node* tree, uint16_t treeSize,FILE *fDest){
    if (! fDest ) { printf("Error passing null File pointer");exit(-1); }

    uint16_t nbCar = (treeSize / 2) + 1;
    size_t textLength = 0; // unknown maximal text length  --> size_t
    for ( uint16_t i = 0; i < nbCar; i++) // compute of the total text length
        textLength += tree[i].occurrence;
    fwrite(&textLength,sizeof(size_t),1,fDest) ; // write text length

    fwrite(&nbCar,sizeof( uint16_t),1,fDest); // write 2 octets for number of different characters
    for ( uint16_t i = 0; i < nbCar; i++) {
        fwrite(&tree[i].name,sizeof(char),1,fDest); // write each different characters into a char
        fwrite(&tree[i].occurrence,sizeof(size_t),1,fDest); // write occurrence of each characters into short int -> should be sizeof(size_t) for production usage
    }
    return (sizeof(size_t)) + sizeof( uint16_t) + (sizeof(char) * nbCar) + ( sizeof(size_t) * nbCar ); //  textLength + each characters + each characters occurrence
}


size_t writeCode(Node* tree, int16_t* indexInTree,FILE* fSrc,FILE* fDest)  {
    if (! fSrc || ! fDest) { printf("Error passing null File pointer for write code");exit(-1); }

    unsigned char* readBuffer = (unsigned char*)  scrMalloc(sizeof(char) * BUFFER_SIZE); // buffer for read the file
    unsigned char* writeBuffer = (unsigned char*)  scrMalloc(sizeof(char) * BUFFER_SIZE); // buffer for write compressed file

    uint16_t treeIndex = 0; // index in the tree of the current read character
    uint16_t bufferBitIndex = 0; // index of the current bit of a char
    size_t writeIndex = 0; // current index of writeBuffer
    size_t nbRead = 0;

    while( (nbRead = fread(readBuffer,sizeof(char),BUFFER_SIZE,fSrc)) ) { // read file into readBuffer
        for(size_t i=0 ; i< nbRead ; i++) { // loop readBuffer

            treeIndex = indexInTree[(int) readBuffer[i] ];
            uint16_t codeLength = strlen(tree[treeIndex].code);

            if(codeLength + bufferBitIndex <= 8) { // end of current binary character will not be reached so we continue
                toBinCode(tree[treeIndex].code, &writeBuffer[writeIndex], 0, bufferBitIndex, codeLength);
                bufferBitIndex += codeLength; // move the current bit to write
            }
            else{ // we could'nt write all the char huffman code
                uint16_t diff = (8-bufferBitIndex); // compute possible which bits are writable into the current char
                if(diff)  // if there are possible bits
                    toBinCode(tree[treeIndex].code, &writeBuffer[writeIndex], 0, bufferBitIndex, diff); // we complete current char
                writeIndex++; // new writeBuffer index
                if(writeIndex == BUFFER_SIZE) { // end of writeBuffer -> write buffer into file
                    fwrite(writeBuffer, sizeof(char), writeIndex, fDest);
                    writeIndex = 0; // return to begin
                }
                bufferBitIndex = 0;
                toBinCode(tree[treeIndex].code, &writeBuffer[writeIndex], diff, bufferBitIndex, codeLength); // write rest of bits
                bufferBitIndex += (codeLength-diff);  // new bit index for new char
            }
        }
    }
    fwrite(writeBuffer, sizeof(char), writeIndex+1, fDest); // last write pb with 1 or 2 lastest char nedd to gdb
    /*if(bufferBitIndex == 8) { // exception when last bit of last buffer character isn't whiten
        fwrite(&writeBuffer[writeIndex+2],sizeof(char),1,fDest);
    }*/
    free(writeBuffer);
    free(readBuffer);
    return writeIndex;
}


void compress(char* src, char* dest) {
    printf("Compress in progress... \n");
    clock_t t1, t2;
    t1 = clock();

    FILE *fSrc = NULL, *fDest = NULL;
    if( ! (fSrc = fopen(src,"r") ) ) { printf("Error could'nt open the source file : %s",src); exit(1); }
    if( ! (fDest = fopen(dest,"w") ) )  {printf("Error could'nt open the destination file : %s",dest); exit(1); }

    unsigned int nbOccurrence[256] = {0}; // array which contains all character occurrence
    size_t textLength = 0;
    size_t compressedLength = 0;
    uint16_t nbCar = buildDistribution(fSrc, nbOccurrence,&textLength); // make probabilistic distribution

    if(nbCar > 0) {
         uint16_t treeSize = (2*nbCar) -1;
        Node* tree = initArray(nbOccurrence,treeSize);  // initialize the array of node
        fillArray(tree,treeSize); // fill the array
        if(nbCar == 1)
           tree[0].code = "0";
        else
           calculateCode(tree,treeSize-1,""); // calculate each huffman code

        double avgSize = 0;
        for(unsigned int i=0 ; i < nbCar ; i++) // compute average size with huffman coding
            avgSize += strlen(tree[i].code);
        avgSize = avgSize / nbCar;
        compressedLength += writeHeader(tree,treeSize,fDest); // write the header
        fseek(fSrc,0,SEEK_SET); // return to begin for write body

        int16_t indexInTree[256] = {-1};
        for( uint16_t i=0; i<nbCar ; i++) // to better complexity array which point directly to tree character
            // ex if 'a' if at index 8 in Tree indexInTree[97] = 8; ascii(a)= 97
            indexInTree[ (int) tree[i].name ] = i;
        compressedLength += writeCode(tree,indexInTree,fSrc,fDest); // write the code
        t2 = clock();
        //displayTree(tree,treeSize);
        if(nbCar > 1) {
            for( uint16_t i=0 ; i<nbCar ; i++)
                free(tree[i].code);
        }
        free(tree);
        printf("\nAvg code size :  %lf bits instead of 8 \n",avgSize);
        printf("Before Compression size : %d octets \n",(int) textLength);
        printf("After Compression size : %d octets \n",(int)compressedLength);
        printf("Benefit of %lf %% \n", (((double) textLength - compressedLength ) / (double) compressedLength) * 100);
        printf("Ellapsed time for compress file : %f ms \n",(double) (t2-t1)/(CLOCKS_PER_SEC/1E3));
    }
    else
        printf("There is no caracter in the file\n");
    fclose(fSrc);
    fclose(fDest);
}

int main(int argc,char** argv) {
    if(argc < 2) {
		printf("Error no argument specified :\n Run ./Huffman -h for print help\n"); exit(-1);
	}
	if(! strcmp(argv[1],"-h") )
		printf("Compress : ./huf file_to_compress file_compressed.huf \n");
    else {
        if(argc != 3) {
            printf("Error bad number of argument \n Run ./Huffman -h for print help\n"); exit(-1);
        }
        compress(argv[1],argv[2]);
    }
    return 0;
}



