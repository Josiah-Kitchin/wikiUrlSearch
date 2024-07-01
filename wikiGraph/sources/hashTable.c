
#include "headers/wikiGraph.h"



LinkedList* createLinkedList(){
    LinkedList* ll = (LinkedList*)malloc(sizeof(ll)); 
    ll->start = NULL; 
    return ll; 
}

void appendNode(LinkedList* ll, char* inputUrl){
    Node* newNode = malloc(sizeof(Node)); 
    newNode->url = strdup(inputUrl); //need to strdup to prevent memory leaks   
    if (ll->start == NULL){
	ll->start = newNode;
	newNode->next = NULL; 
    } else {
	Node* tmp = ll->start; 
	ll->start = newNode; 
	newNode->next = tmp; 
    }
}


void freeLinkedList(LinkedList* list){
    Node* tmp = list->start; 
    while (tmp != NULL){
	tmp = tmp->next; 
	free(list->start->url); 
	free(list->start); 
	list->start = tmp; 
    }
    free(list); 
}

void printLinkedList(LinkedList* list){
    Node* currentNode = list->start; 
    while (currentNode != NULL){
	printf("%s\n", currentNode->url + 30);  //excludes the link
	currentNode = currentNode->next; 
    }
}

unsigned long hash(char* str){
    unsigned long hash = 5381; 
    unsigned long c; 
    while (c = *str++)
	hash = ((hash << 5) + hash + c); 
    return hash % MAX_HT_SIZE; 
}


HashTable* createHashTable(){
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));  
    for (int i = 0; i < MAX_HT_SIZE; i++){
	ht->nodes[i] = NULL; 
    }
    return ht; 
}

void storeUrl(HashTable* ht, char* inputUrl){
    unsigned long hashedIDX = hash(inputUrl); 
    if (ht->nodes[hashedIDX] == NULL){
	LinkedList* ll = createLinkedList(); 
	ht->nodes[hashedIDX] = ll; 
	appendNode(ll, inputUrl); 
    } else {
	appendNode(ht->nodes[hashedIDX], inputUrl); 
    }
}

bool isInHT(HashTable* ht, char* inputKey){
    unsigned long hashedIDX = hash(inputKey); 
    LinkedList* ll = ht->nodes[hashedIDX]; 
    if (ll != NULL){
	Node* currentNode = ll->start; 
	while (strcmp(currentNode->url, inputKey) != 0){
	    currentNode = currentNode->next; 
	    if (currentNode == NULL){
		return false; 
	    }
	}
	return true; 
    } else {
	return false; 
    }
}

void printHashTable(HashTable* ht){
    for (int i = 0; i < MAX_HT_SIZE; i++){
	if (ht->nodes[i]){
	    printLinkedList(ht->nodes[i]); 
	}
    }
}

void freeHashTable(HashTable* ht){
    for (int i = 0; i < MAX_HT_SIZE; i++){
	if (ht->nodes[i]){
	    freeLinkedList(ht->nodes[i]); 
	}
    }
    free(ht); 
}





