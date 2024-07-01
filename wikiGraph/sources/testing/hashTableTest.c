

#include "../hashTable.c"


bool testHashTable(){
    HashTable* ht = createHashTable(); 
    storeUrl(ht, "milo"); 
    storeUrl(ht, "tito"); 
    storeUrl(ht, "jager");
    
    bool inHT = isInHT(ht, "milo") && isInHT(ht, "tito") && isInHT(ht, "jager"); 
    bool notInHT = isInHT(ht, "faith") || isInHT(ht, "oliver"); 
    freeHashTable(ht); 

    return inHT && !notInHT; 
}


void testFreeHashTable(){
    char tmp[] = "milo"; 
    char* link1 = malloc(sizeof(strlen(tmp) + 1)); 
    strcpy(link1, tmp); 

    HashTable* ht = createHashTable(); 
    storeUrl(ht, link1); 
    freeHashTable(ht); 
}

int main(){
    //if (!testHashTable())
	//printf("HashTable Failed\n"); 
    
    testFreeHashTable(); 

    //if (testHashTable())
	//printf("All Non-Memory Leak Tests Passed\n"); 
    

    return 0; 
}
