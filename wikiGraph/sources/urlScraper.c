
#include "headers/wikiGraph.h"

//define LOG

bool startsWithWiki(char *address) {
    regex_t regex;
    int reti;

    char *pattern = "^\\/wiki";

    reti = regcomp(&regex, pattern, REG_EXTENDED);
    if (reti) {
	#ifdef LOG
	    fprintf("Regex failed to compile\n"); 
	#endif
	regfree(&regex); 
        return false;
    }

    reti = regexec(&regex, address, 0, NULL, 0);
    if (!reti) {
        regfree(&regex);
        return true;
    } 

    regfree(&regex);
    return false;
}

bool hasExcludedPrefix(char* address){
    const char *excluded_prefixes[] = {"^File", "^Wikipedia", "^Special", 
                                       "^User", "^Category", "^Template", "^Help",
					"^Main", "^Portal" };
    if (strlen(address) < 6){
	return false;
    }
    char* prefix = address + 6; //to start after /wiki 
    regex_t regex; 
    int reti; 

    int excludeLen = sizeof(excluded_prefixes) / sizeof(excluded_prefixes[0]); 
    for (int i = 0; i < excludeLen; i++){
	reti = regcomp(&regex, excluded_prefixes[i], REG_EXTENDED); 
	if (reti){
	    #ifdef LOG
		fprintf("Regex failed to compile\n"); 
	    #endif
	    regfree(&regex); 
	    return false; 
	}
	reti = regexec(&regex, prefix, 0, NULL, 0); 
	if (!reti){
	    regfree(&regex); 
	    return true; 
	}
	regfree(&regex); 
    }
    regfree(&regex); 
    return false; 
}



//Response

static size_t WriteHTMLCallback(void* contents, size_t size, size_t nmemb, void* userp){
    //Write function for CURL 
    //Writes the html in the CURLResponse html member 
    size_t realsize = size * nmemb; 
    struct CURLResponse *mem = (struct CURLResponse *)userp; 
    char *ptr = realloc(mem->html, mem->size + realsize + 1); 

    if (!ptr){
	#ifdef LOG
	    fprintf(stderr, "Failed to allcate memory for HTML\n"); 
	#endif
	return 0; 
    }
    mem->html = ptr; 
    memcpy(&(mem->html[mem->size]), contents, realsize); 
    mem->size += realsize; 

    mem->html[mem->size] = '\0'; 
    return realsize;
}


struct CURLResponse getRequest(CURL *curl_handle, const char *url){ 
    //Gets the html response from curl 
    CURLcode res; 
    struct CURLResponse response; response.html = malloc(1); response.size = 0; 
    curl_easy_setopt(curl_handle, CURLOPT_URL, url); 
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteHTMLCallback); 
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&response); 
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, 
	    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/117.0.0.0 Safari/537.36");
    res = curl_easy_perform(curl_handle);
    if (res != CURLE_OK){
	#ifdef LOG
	    fprintf(stderr, "GET request failed: %s\n", curl_easy_strerror(res)); 
	#endif
	free(response.html); 
	response.html = NULL;
    }
    return response;
}

struct CURLResponse getHTML(const char *url){
    //sets up curl and returns the response 
    curl_global_init(CURL_GLOBAL_ALL); 
    CURL* curl_handle = curl_easy_init(); 
    struct CURLResponse response = getRequest(curl_handle, url);
    curl_easy_cleanup(curl_handle); 
    curl_global_cleanup(); 
    return response; 
}


char** getUrls(char* inputUrl){
    //scrapes all of the urls from a webpage (given by inputUrl)
    //returns an array of urls 
    struct CURLResponse response = getHTML(inputUrl);
    
    if (!response.html){
	return NULL; 
    }

    htmlDocPtr doc = htmlReadMemory(response.html, (unsigned long)response.size, NULL, NULL, HTML_PARSE_NOERROR); 

    free(response.html); 

    if (!doc){
	#ifdef LOG
	    fprintf(stderr, "HtmlDocPtr returned NULL\n"); 
	#endif
	xmlFreeDoc(doc); 
	xmlCleanupParser(); 
	return NULL; 
    }

    xmlXPathContextPtr context = xmlXPathNewContext(doc);
    if (!context){
	#ifdef LOG
	    fprintf(stderr, "xmlXPathContext returned NULL\n"); 
	#endif
	xmlXPathFreeContext(context); 
	xmlFreeDoc(doc); 
	xmlCleanupParser(); 
	return NULL; 
    }

    xmlXPathObjectPtr linkTagElements = xmlXPathEvalExpression((xmlChar *)"//a", context);

    if (!linkTagElements){
	#ifdef LOG
	    fprintf(stderr, "xmlXPathObjectPtr returned NULL\n"); 
	#endif
	xmlXPathFreeContext(context); 
	xmlXPathFreeObject(linkTagElements); 
	xmlFreeDoc(doc); 
	xmlCleanupParser(); 
	return NULL; 
    }

    char** urls = (char**)malloc(sizeof(char*) * MAX_URLS); 

    if (!urls){
	#ifdef LOG
	    fprintf(stderr, "Failed to allocate memory for urls\n"); 
	#endif
	xmlXPathFreeContext(context); 
	xmlXPathFreeObject(linkTagElements); 
	xmlFreeDoc(doc); 
	xmlCleanupParser(); 
	return NULL; 
    }

    for (int i = 0; i < MAX_URLS; i++){
	//Creating the array of urls from the doc
	if (i >= linkTagElements->nodesetval->nodeNr){
	    urls[i] = NULL; 
	    continue; 
	}
	
	xmlNodePtr currentLinkElement = linkTagElements->nodesetval->nodeTab[i];

        xmlChar* url = (xmlChar *)xmlGetProp(currentLinkElement, (xmlChar *)"href");

	if (url){
	    urls[i] = strdup(url); 
	    xmlFree(url); 
	} else { 
	    urls[i] = NULL; 
	}
    }
    
    xmlXPathFreeContext(context); 
    xmlXPathFreeObject(linkTagElements); 
    xmlFreeDoc(doc); 
    xmlCleanupParser(); 
    return urls; 
}

void filterUrls(char** inputUrls){
    //turns every url that does not start with wiki/ or has an excluded prefix into NULL
    for (int i = 0; i < MAX_URLS; i++){
	if (inputUrls[i] && (!startsWithWiki(inputUrls[i]) || hasExcludedPrefix(inputUrls[i]))){
	    free(inputUrls[i]); 
	    inputUrls[i] = NULL; 
	}
    }
}

void catHTTPSToUrl(char** inputUrls){
    //adds https://wikipedia.org to the front of each url 

    char* urlFront = "https://en.wikipedia.org"; 
    int urlFrontLen = strlen(urlFront); 
    for (int i = 0; i < MAX_URLS; i++){
	if (!inputUrls[i]){
	    continue; 
	}
	int urlLen = strlen(inputUrls[i]); 
	char* resultUrl = (char*)malloc(sizeof(char) * (urlFrontLen + urlLen + 1)); 
	strcpy(resultUrl, urlFront); 
	strcat(resultUrl, inputUrls[i]); 
	free(inputUrls[i]); 
	inputUrls[i] = resultUrl; 
    }
}

void freeUrls(char** urls){
    for(int i = 0; i < MAX_URLS; i++){
	if (urls[i]){
	    free(urls[i]); 
	}
    }
    free(urls); 
}




