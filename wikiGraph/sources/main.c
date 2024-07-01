#include "headers/wikiGraph.h"
//#define LOG

int main(int argc, char** argv){

    if (argc != 3){
	fprintf(stderr, "Input Error, input in the format: <wikipedia-link> <wikipedia-link>\n"); 
	exit(EXIT_FAILURE); 
    }

    char* startUrl = argv[1]; 
    char* endUrl = argv[2]; 
    HashTable* urlHt = searchUrls(startUrl, endUrl); 
    #ifdef LOG
	printHashTable(urlHt); 
    #endif
    freeHashTable(urlHt); 
    return 0; 
}
