#include "headers/wikiGraph.h"

#define LOG  

void printNeighbors(char** neighbors, long size){
    for (int i = 0; i < size; i++){
	if (!neighbors[i]){
	    continue; 
	}
	printf("%s\n", neighbors[i]); 
    }
}

HashTable* searchUrls(char* url, char* endUrl){
    /*  BFS algorithm
     *  Takes an url and searches until the end url is found or it reaches MAX_TOTAL_URLS
     *  Prints the depth of endUrl (or endURL not found)
     *  Returns a hash table of all the urls searched
    */
    HashTable* urlHt = createHashTable(); 
    Queue* urlQ = createQueue(); 

    storeUrl(urlHt, url); 
    enqueue(urlQ, url); 

    int currentDepth = 1, 
	elementsToDepthIncrease = 1, 
	nextElementsToDepthIncrease = 0;

    while (urlQ->size && urlQ->end < MAX_TOTAL_URLS){

	char* currentUrl = dequeue(urlQ); 
	char** urlNeighbors = getUrls(currentUrl); 
	if (!urlNeighbors){
	    free(currentUrl); 
	    continue; 
	}
	filterUrls(urlNeighbors); 
	catHTTPSToUrl(urlNeighbors); 
	char* currentNeighbor; 

	#ifdef LOG
	    printf("Searching the links of %s...\n", currentUrl); 
	#endif

	int neighborSize = 0; 
	for (int i = 0; i < MAX_URLS; i++){
	    if (urlQ->end >= MAX_TOTAL_URLS){ 
		break; 
	    }
	    currentNeighbor = urlNeighbors[i]; 

	    
	    if (!currentNeighbor || isInHT(urlHt, currentNeighbor)){
		continue; 
	    }

	    //endUrl found case
	    if (strcmp(currentNeighbor, endUrl) == 0){
		printf("%s Wiki found from %s Wiki in %d page(s)!\n", endUrl, url, currentDepth); 
		freeUrls(urlNeighbors); 
		free(currentUrl); 
		freeQueue(urlQ); 
		return urlHt; 
	    }

	    storeUrl(urlHt, currentNeighbor); 
	    enqueue(urlQ, currentNeighbor); 
	    neighborSize++; 
	}
	
	//Calculate depth 
	nextElementsToDepthIncrease += neighborSize;
	if (elementsToDepthIncrease-- == 0) {
	     currentDepth++; 
	     elementsToDepthIncrease = nextElementsToDepthIncrease;
	     nextElementsToDepthIncrease = 0;
	}


	//store url and enqueue use strdup, so we can safely free these
	freeUrls(urlNeighbors); 
	free(currentUrl); 
    }
    freeQueue(urlQ); //frees whats left in the queue 
    
    printf("%s wiki could not be found from %s wiki with a limit of %d searches\n", endUrl, url, MAX_TOTAL_URLS); 
    return urlHt;
}







 















