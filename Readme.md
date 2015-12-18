
Programme de compression de Huffman d'un fichier

Auteurs :
COLIN Renaud
PASCUAL Florian


Pour compiler :
1) Avec make
Dans le makefile
 - Compiler le compresseur :
        -rajouter src/Compress.c dans la variable SOURCES
        - Attribuer la valeur huf à la variable PROGRAMM
  - Compiler le decompresseur :
        -rajouter src/Uncompress.c dans la variable SOURCES
        - Attribuer la valeur dehuf à la variable PROGRAMM

    Puis lancer make

2) À la main
 gcc -Wall -ansi -pedantic -std=c99 -c  src/huffman.c
  - Compiler le compresseur :
        gcc -Wall -ansi -pedantic -std=c99 -c src/Compress.c
        gcc huffman.o Compress.o -o huf
  - Compiler le décompresseur :
        gcc -Wall -ansi -pedantic -std=c99 -c src/Uncompress.c
        gcc huffman.o Uncompress.o -o dehuf

Pour lancer le programme :
 - Compresser : Huf fichier_a_compresser  fichier_compresse
 - Décompresser : Dehuf fichier_compresse

Etat d'avancement

L'occurence d'un charactere est codé par un short int ce qui pose problème pour de trés gros fichiers
Problème pour gérer les caractères spéciaux ASCII à partir du code 128 et ce malgré un codage en unsigned char

-> Compression fonctionelle
-> Décompression fonctionelle
