#include "net/URI.h"
#include <string.h>
#include <stdlib.h>
#include <sstream>
#include <vector>
#include <iostream>
#include <assert.h>
using namespace std;

typedef
struct _tagSchema{
    const char* prefix;
    const size_t size;
}Schema_t;

static
Schema_t schema_list[]={
    {"http://", 7},
    {"https://", 8},
    {NULL, 0}
};


static
int url_start_with(const char*a, const char* b){
    if( a!=NULL && b!=NULL && strncasecmp (a, b, strlen(a) ) == 0) {
        return 0;
    }
    return 1;
}

static
char* url_remove_schema_prefix(const char* url){
    if (!url) return NULL;

    int i = 0;
    Schema_t* sch = &schema_list[i];
    do{
        sch = &schema_list[i];
        if ( NULL == sch->prefix ) break;
        if ( url_start_with (sch->prefix, url) == 0 ){
            return strdup ( url + sch->size );
        }
        i++;
    }while( NULL != sch->prefix );

    return NULL;
}


URI::URI()
{
}

URI::~URI()
{

}

void URI::parse (const char *url){
    std::string uri(url);
    parse (uri);
}

void URI::parse (std::string& uri){
    if (uri.size () == 0){
        assert(!"URI is empty");
    }
    m_origin = uri;

    schema = nullptr;
    host   = nullptr;
    port   = 0;
    path   = nullptr;
    query  = nullptr;

    url_get_schema ();
    if ( isHttp () ){
        url_get_host ();
        url_get_port ();
        url_get_path ();
    }else if ( isFile () ){
        url_get_file_path();
    }else{
        assert(!"Not support such uri");
    }
}


void URI::url_get_schema (){
    const char* url = m_origin.c_str ();
    char* sch = strchr (url, ':');
    if (sch && sch[1] == '/' && sch[2] == '/'){
        size_t sz = sch - url;
        char* schema = (char*) calloc(1, sz);
        strncpy (schema, url, sz);
        this->schema = schema;
        return;
    }
    this->schema = NULL;
}
void URI::url_get_host (){
    const char* url = m_origin.c_str ();
    char* no_http_prefix = url_remove_schema_prefix (url);
    if (!no_http_prefix) return;
    char* offset_slash = NULL;
    char* offset_colon = NULL;
    int len = 0;

    char* the_host = NULL;

    offset_slash = strstr (no_http_prefix, "/");
    offset_colon = strstr (no_http_prefix, ":");

    // no slash and no colon
    if ( !offset_slash && !offset_colon ){
        the_host = strdup (no_http_prefix);
    }

    if (offset_colon){
        // with colon
        len = offset_colon - no_http_prefix;
    }else if (offset_slash ){
        // with slash and without colon
        len = offset_slash - no_http_prefix;
    }
    if (len != 0){
        the_host = (char*) calloc(1, len+1);
        memcpy( the_host, no_http_prefix , len);
        the_host[len] = '\0';
    }
    free( no_http_prefix);
    no_http_prefix = NULL;

    this->host = the_host;
}
void URI::url_get_port (){
    const char* url = m_origin.c_str ();
    char* no_http_prefix = url_remove_schema_prefix (url);
    if (!no_http_prefix) return;
    char* offset_slash = NULL;
    char* offset_colon = NULL;
    int len = 0;
    if ( (offset_colon = strchr (no_http_prefix, ':') ) != NULL){
        offset_slash = strchr (no_http_prefix, '/') ;
        if (offset_slash){
            len = offset_slash - offset_colon;
        }else{
            len = no_http_prefix + strlen(no_http_prefix) - offset_colon;
        }

        char* tmp = (char*) calloc(1, len);
        memcpy( tmp, offset_colon+1, len-1); // exclude ":" & "/"
        tmp[len-1] = '\0';

        int port = atoi(tmp);
        free( tmp);
        free( no_http_prefix);
        this->port = port;// == 0 ? 80 : port;
        return;
    }
    free( no_http_prefix);

    this->port = 80;
}
void URI::url_get_path (){
    const char* url = m_origin.c_str ();
    size_t offset = 0;

    offset += this->schema != NULL? strlen ( this->schema ) + 3 : 0;
    offset += this->host != NULL? strlen ( this->host ) : 0;

    char* chr = strchr ((url+offset), '/');
    if (chr){
        char* tmp_query = strchr(chr, '?');
        if ( !tmp_query ){ // not exit query
            this->path = strdup(chr);
        }else{
            offset = tmp_query - chr;
            this->path = (char*) calloc(1, offset+1);
            strncpy(this->path, chr, offset);

            this->query = strdup (tmp_query+1);
        }
    } else {
        this->path = strdup("/");
    }
}

void URI::url_get_file_path(){
    const char* url = m_origin.c_str ();
    size_t offset = 0;

    offset += this->schema != NULL? strlen ( this->schema ) + 3 : 0;
    char* chr = strchr ((url+offset), '/');
    if (chr)
        this->path = strdup(chr);
    else
        this->path = strdup("/");
}

void URI::dump (){
    cout << "schema:" << (schema != NULL ? schema : "<nullptr>") << "\n";
    cout << "host:" << (host != NULL ? host : "<nullptr>") << "\n";
    cout << "port:" << (port                          ) << "\n";
    cout << "path:" << (path != NULL ? path : "<nullptr>") << "\n";
    cout << "query:" << (query != NULL ? query : "<nullptr>") << "\n";
    cout << endl;
}

bool URI::isHttp(){
    if (schema != nullptr && strstr(schema, "http") != 0 ){
        return true;
    }
    return false;
}
bool URI::isFile (){
    if (schema != nullptr && strstr(schema, "file") != 0 ){
        return true;
    }
    return false;
}

bool URI::valid (){
    return isHttp() || isFile ();
}



