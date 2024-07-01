
#include "../urlScraper.c" 

//gcc urlScraperTest.c -lcurl -lxml2

bool testStartsWithWiki(){
    bool trueTest1 = startsWithWiki("/wiki/testing1"); 
    bool falseTest1 = startsWithWiki("flksdjklfjs"); 
    bool falseTest2 = startsWithWiki("fjdkls/wikijdfskl"); 
    bool falseTest3 = startsWithWiki("fjkdsljfsl/wiki"); 

    return trueTest1 && !falseTest1 && !falseTest2 && !falseTest3; 
}

bool testHasExcludedPrefix(){
    bool trueTest1 = hasExcludedPrefix("/wiki/Template"); 
    bool falseTest1 = hasExcludedPrefix("/wiki/Yakuza"); 
    bool trueTest2 = hasExcludedPrefix("/wiki/Help"); 
    bool trueTest3 = hasExcludedPrefix("/wiki/File"); 

    return trueTest1 && !falseTest1 && trueTest2 && trueTest3; 
}

bool testGetUrls(){
    char inputUrl[] = "https://en.wikipedia.org/wiki/Yakuza"; 
    char** urls = getUrls(inputUrl); 
    if (!urls){
	return false;
    }
    for (int i = 0; i < MAX_URLS; i++){
	if (urls[i]){
	    xmlFree(urls[i]); 
	}
    }
    free(urls); 
    return true; 
}

bool testFilterUrls(){
    char inputUrl[] = "https://en.wikipedia.org/wiki/Yakuza"; 
    char** urls = getUrls(inputUrl); 
    filterUrls(urls); 
    for (int i = 0; i < MAX_URLS; i++){
	if (urls[i]){
	    if (urls[i][0] != '/' && urls[i][1] != 'w'){
		return false; 
	    }
	}
    }
    freeUrls(urls); 
    return true; 

}

bool testCatHTTPSToUrl(){
    char inputUrl[] = "https://en.wikipedia.org/wiki/Yakuza"; 
    char** urls = getUrls(inputUrl); 
    filterUrls(urls); 
    catHTTPSToUrl(urls); 
    for (int i = 0; i < MAX_URLS; i++){
	if (urls[i]){
	    if (urls[i][0] != 'h' && urls[i][1] != 't'){
		return false; 
	    }
	}
    }
    freeUrls(urls); 
    return true; 
}


int main(){
    if(!testStartsWithWiki())
	printf("startWithWiki Failed\n"); 
    if(!testGetUrls())
	printf("getUrls Failed\n"); 
    if(!testFilterUrls())
	printf("filterUrls Failed\n"); 
    if(!testCatHTTPSToUrl())
	printf("catHTTPSToUrl Failed\n"); 
    if(!testHasExcludedPrefix()){
	printf("hasExcludedPrefix Failed\n"); 
    }

    if (testStartsWithWiki() && testGetUrls() && testFilterUrls() && testCatHTTPSToUrl() &&
	testHasExcludedPrefix()
	){
	printf("All Tests Passed\n");
    }
    return 0; 
}
