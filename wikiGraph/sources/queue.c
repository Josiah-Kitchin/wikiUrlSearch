
#include "headers/wikiGraph.h"

Queue* createQueue(){
    Queue* q = (Queue*)(malloc(sizeof(Queue))); 
    q->start = 0; 
    q->end = 0; 
    q->size = 0; 
    return q; 
}

void enqueue(Queue* q, char* url){
    if (q->size >= MAX_TOTAL_URLS){
	fprintf(stderr, "Queue overflow\n");
	exit(EXIT_FAILURE); 
    }
    q->urls[q->end] = strdup(url); //must use strdup to free strings from url neighbors
    q->end++; 
    q->size++; 
}

char* dequeue(Queue* q){
    if (q->size == 0){
	fprintf(stderr, "Queue underflow\n"); 
	exit(EXIT_FAILURE); 
    }
    char* url = q->urls[q->start]; 
    q->start++;
    q->size--; 
    //url must be free'd later. 
    return url; 
}

void freeQueue(Queue* q){
    //Frees any strings that still remain in the queue. 
    for (unsigned long i = q->start; i < q->end; i++){
	free(q->urls[i]); 
    }
    free(q); 
}

