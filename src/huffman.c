#include "huffman.h"

#define BUFFER_SIZE 2048

unsigned short int buildDistribution(FILE* fSrc,unsigned int*nbOccurrence,size_t* textLength) {

	if (! fSrc ) { printf("Error passing null File pointer");exit(-1); }
	size_t read = 0;
	uint16_t nbCar = 0;
	unsigned char* buffer = (unsigned char*) scrMalloc(sizeof(char) * BUFFER_SIZE); // buffer for read file

	while( (read = fread(buffer,sizeof(char),BUFFER_SIZE,fSrc)) ){ // read file into buffer
		for(size_t i=0 ; i<read ; i++)  // reading buffer
			nbOccurrence[ (int) buffer[i] ]++; // new character occurrence
		*textLength += read; // textLength increase
	}
	for(unsigned short int j=0; j < 256; j++) { // compute number of different characters
		if(nbOccurrence[j]) { // if occurrence of character -> increment nbCar
			//printf("[Char %c] [Occurrence %d] \n",j,nbOccurrence[j]);
			nbCar++;
		}
	}
	free(buffer);
	return nbCar;
}


Node* initArray(unsigned int* nbOccurrence,uint16_t treeSize) {

	Node* tree = (Node *) scrMalloc(treeSize * sizeof(Node));
	for(uint16_t i=0 ; i< treeSize ; i++) { // initialize the empty tree
		tree[i].parent = -1;
		tree[i].leftChild = -1;
		tree[i].rightChild = -1;
		tree[i].occurrence = 0;
		tree[i].name = '*';
		tree[i].code =" ";
	}
	unsigned short int currentOccurrence = 0; // index of the current character to add to the tree
	for(uint16_t i=0 ; i < 256 ; i++) { // add occurrence and name for each present characters
		if(nbOccurrence[i] > 0) {  // if a element if present then initialize the node
			tree[currentOccurrence].occurrence = nbOccurrence[i];
			tree[currentOccurrence].name = (unsigned char) i;
			currentOccurrence++;
		}
	}
	return tree;
}

void fillArray(Node* tree,uint16_t treeSize) {
	for(uint16_t nn = (treeSize)/2 +1 ; nn < treeSize ; nn++) {
		int16_t min1 = -1,min2 = -1;

		for (uint16_t i = 0; i < nn; i++)	{
			if(tree[i].parent == -1) {
				if(min1 == -1)
					min1 = i;
				else if(tree[i].occurrence < tree[min1].occurrence) {
					min2 = min1;
					min1 = i;
				}
				else if(min2 == -1)
					min2 = i;

				else if(tree[i].occurrence < tree[min2].occurrence)
					min2 = i;
			}
		}
		tree[nn].occurrence = tree[min1].occurrence + tree[min2].occurrence;
		tree[nn].leftChild = min1;
		tree[nn].rightChild = min2;
		tree[min1].parent = nn;
		tree[min2].parent= nn;
	}
}

void calculateCode(Node* tree,uint16_t root,char* code) {
	if(tree[root].leftChild != -1) { // not a leaf so there are two child
		char buff[strlen(code)+1]; // new buffer which contains old code + 0/1
		strcpy(buff,code);
		strcat(buff,"0");
		calculateCode(tree,tree[root].leftChild,buff); // left part of the tree

		strcpy(buff,code);
		strcat(buff,"1");
		calculateCode(tree,tree[root].rightChild,buff); // right part of the tree;
	}
	else { // leaf finded
		if( ! ( tree[root].code = (char*)  scrMalloc(strlen(code)*sizeof(char)) ) ) {perror("Error when allocate memory for node character \n");exit(-1); }
		strcpy(tree[root].code,code);
	}
}


void displayTree(Node* tree, uint16_t treeSize) {
	printf("\nDisplay Tree \n");
	printf("id\tcar\toccu\tpère\tfg\tfd\tcode\n");
	for (uint16_t i = 0; i < treeSize; ++i) {
		printf("%d\t",i);
		printf("%c\t",tree[i].name);
		printf("%d\t",(int)tree[i].occurrence);
		printf("%d\t",tree[i].parent);
		printf("%d\t",tree[i].leftChild);
		printf("%d\t",tree[i].rightChild);
		printf("%s",tree[i].code);
		printf("\n");
	}
}

void * scrMalloc(size_t size) {
	void * ptr = malloc(size);
	if( ! ptr) {
		fprintf(stderr,"Error when allocating memory");
		exit(-1);
	}
	return ptr;
}