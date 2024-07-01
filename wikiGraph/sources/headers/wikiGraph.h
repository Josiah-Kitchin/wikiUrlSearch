
#define MAX_URLS 1000 //the max urls to look for in a single page  
#define MAX_TOTAL_URLS 1000000 // the max urls to look for in total   

#define MAX_HT_SIZE MAX_TOTAL_URLS * 2


#include <stdio.h> 
#include <string.h> 
#include <stdbool.h> 
#include <stdlib.h> 
#include <curl/curl.h> 
#include <stdbool.h> 
#include <regex.h>
#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>



//HashTable.c 
typedef struct Node{
	struct Node* next; 
	char* url; 
} Node; 
typedef struct{
	Node* start; 
} LinkedList; 
typedef struct{
    LinkedList* nodes[MAX_HT_SIZE]; 
} HashTable; 

void initalizeLL(LinkedList*);
LinkedList* createLinkedList();
void appendNode(LinkedList*, char*);
void freeLinkedList(LinkedList*);
void printLinkedList(LinkedList*);
unsigned long hash(char*);
HashTable* createHashTable();
void storeUrl(HashTable*, char*);
bool isInHT(HashTable*, char*);
void printHashTable(HashTable*); 
void freeHashTable(HashTable*);

//Queue.c
typedef struct{ 
    char* urls[MAX_TOTAL_URLS];
    unsigned long start; 
    unsigned long end; 
    unsigned long size; 
} Queue; 

Queue* createQueue();
void enqueue(Queue*, char*);
char* dequeue(Queue* q);
void freeQueue(Queue*); 

//urlScraper.c
struct CURLResponse{
    char* html; 
    size_t size; 
}; 

static size_t WriteHTMLCallback(void*, size_t, size_t, void*);
struct CURLResponse getRequest(CURL*, const char*); 
struct CURLResponse getHTML(const char*);
char** getUrls(char*);
bool startsWithWiki(char*); 
bool hasExcludedPrefix(char*); 
void filterUrls(char**);
void catHTTPSToUrl(char**); 
void freeUrls(char**);

//urlSearch.c
HashTable* searchUrls(char*, char*); 


