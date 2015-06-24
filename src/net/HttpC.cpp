#include <net/HttpC.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>
#include <assert.h>
#include <string.h>
#include <fcntl.h>


HttpC::HttpC()
{
    m_userAgent = NULL;
        m_pSocket = new Socket(Socket::Socket_tcp);
        m_httpOnReadSize = 8192;
        m_BufferInitSize = 16*1024*1024;
        m_BufferSize = m_BufferInitSize;
        m_bWriteToFile = false;

        m_pHeader = NULL;
        m_pContent = NULL;
        m_pBuffer = (char*)malloc(m_BufferSize);
}

HttpC::~HttpC()
{
    if ( m_userAgent != NULL ){
            free(m_userAgent); m_userAgent = NULL;
        }
}


int HttpC::head (std::string& url){
    m_httpReadedSize = 0;
    int rs = 0;

    m_url.parse (url);
    if ( !m_url.valid () )
        return -1;

    const char *get_fmt_template = "HEAD %s HTTP/1.0\r\nHost: %s\r\nUser-Agent: %s\r\n\r\n";
    int len =
            strlen( m_url.host ) +
            strlen ( m_url.path ) +
            strlen (USERAGENT) +
            strlen(get_fmt_template);
    char* query_header = (char*) calloc (1, len);
    snprintf (query_header, len, get_fmt_template, m_url.path, m_url.host, USERAGENT);


    char* ip = NULL;
    if ( (ip = net_get_ip ( m_url.host )) == NULL ){
        return -1;
    }

    rs = m_pSocket->connect (ip, m_url.port);
    free(ip);

    if ( rs != 0){
        ELog("connect error");
        return -1;
    }

    m_pSocket->send (query_header, strlen (query_header));
    if (rs == -1){
        DLog("http send get error %s\n", strerror (errno));
        return -1;
    }


    char *buffer = new char[m_httpOnReadSize];

    while ((rs = m_pSocket->recv (m_pSocket, buffer, m_httpOnReadSize)) > 0) {
        onRead(buffer, rs);
        memset (buffer, 0x0, m_httpOnReadSize);
    }
    delete[] buffer;buffer = NULL;

    onReadDone ();

    return m_httpReadedSize;
}

int HttpC::get (std::string& url){
    m_httpReadedSize = 0;
    int rs = 0;

    m_url.parse (url);
    if ( !m_url.valid () )
        return -1;

    const char *get_fmt_template = "GET %s HTTP/1.0\r\nHost: %s\r\nUser-Agent: %s\r\n\r\n";
    int len =
            strlen( m_url.host ) +
            strlen ( m_url.path ) +
            strlen (USERAGENT) +
            strlen(get_fmt_template);
    char* query_header = (char*) calloc (1, len);
    snprintf (query_header, len, get_fmt_template, m_url.path, m_url.host, USERAGENT);

    char* ip = NULL;
    if ( (ip = net_get_ip ( m_url.host )) == NULL ){
        return -1;
    }

    rs = m_pSocket->connect (ip, m_url.port);
    free(ip);

    if ( rs != 0){
        ELog("connect error");
        return -1;
    }

    m_pSocket->send (query_header, strlen (query_header));
    if (rs == -1){
        DLog("http send get error %s\n", strerror (errno));
        return -1;
    }

    char *buffer = new char[m_httpOnReadSize];

    while ((rs = m_pSocket->recv (m_pSocket, buffer, m_httpOnReadSize)) > 0) {
        onRead(buffer, rs);
        memset (buffer, 0x0, m_httpOnReadSize);
    }
    delete[] buffer;buffer = NULL;

    onReadDone ();

    return m_httpReadedSize;
}

void HttpC::onRead (const void *buffer, const size_t size){

    // get header ok
    if ( m_pHeader != NULL && m_bWriteToFile == true){
        int rs = ::write (m_iWriteToFileFd, buffer, size);
        assert ( rs == size && "http write file error");
        m_httpReadedSize += size;
        return;
    }


    // need realloc
    if ( m_httpReadedSize + size > m_BufferSize ){
        m_BufferSize = m_BufferSize + 4*1024*1014;
        char* tmp = (char*)malloc( m_BufferSize );
        if ( tmp ){
            memcpy(tmp, m_pBuffer, m_httpReadedSize);
            free(m_pBuffer);
            m_pBuffer = tmp;
        }
    }
    memcpy ((char*)m_pBuffer+m_httpReadedSize, buffer, size);
    m_httpReadedSize += size;

    if (m_pHeader == NULL){
        char* tmp = strstr ((char*)m_pBuffer, "\r\n\r\n");
        if ( tmp ){
            m_HeaderOffset = tmp - (char*)m_pBuffer + 4;
            m_pHeader = (char*) calloc(1, m_HeaderOffset+1);
            strncpy (m_pHeader, (char*)m_pBuffer, m_HeaderOffset);
            parseHeader();

            // 需要写入文件
            if ( m_bWriteToFile == true){
                if (m_iWriteToFileFd){ ::close(m_iWriteToFileFd);m_iWriteToFileFd=-1;}
                if (m_WriteFileName) {m_WriteFileName = NULL;}
                char temp[] = "/tmp/__httpc__file__XXXXXX";
                m_WriteFileName = mktemp (temp);
                m_iWriteToFileFd = open(m_WriteFileName, O_RDWR|O_CREAT, 0777);

                unlink (m_WriteFileName); // auto delete when app exit

                write(m_iWriteToFileFd, (char*)m_pBuffer+ m_HeaderOffset, m_httpReadedSize - m_HeaderOffset );
                memset (m_pBuffer, 0x0, m_httpReadedSize - m_HeaderOffset);
            }
        }
    }


}

void HttpC::parseHeader (){
    char httpVersion[512];
    sscanf (m_pHeader, "%s %d", httpVersion, &m_stateCode);

    typedef struct ParseLine{
        const char* key;
        void* value;
        const char* type;
    }ParseLine;
    ParseLine lines[] = {
        {"Content-Length:", &(this->m_contentLength), "i"},
        {NULL, NULL, NULL}
    };

    int i=0;
    ParseLine* line = NULL;
    do{
        line = &(lines[i]);
        if (line->key == NULL) break;
        char* pos = strstr(m_pHeader, line->key);
        if ( pos ) { // found
            if ( strcmp (line->type, "i") == 0 ){
                *((int*)line->value) = 0;
                pos += strlen(line->key);
                sscanf (pos , " %d", (int*)line->value );
            }
        }
        i++;
    }while(1);
}

void HttpC::onReadDone (){
    if ( m_bWriteToFile == true){
        ::close (m_iWriteToFileFd);
    } else {
        m_pContent = (char*)m_pBuffer + m_HeaderOffset;
    }
}

char* HttpC::getContent (){
    return m_pContent;
}

size_t HttpC::getContentLength (){
    return m_contentLength;
}

void HttpC::setUserAgent (const char *ua){
    m_userAgent = strdup(ua);
}

int HttpC::getStateCode (){
    return m_stateCode;
}

void HttpC::setWriteToFile (bool enable){
    m_bWriteToFile = enable;
}

#ifdef ENABLE_TESTCASE

#include <testing.h>
#include <iostream>
using namespace std;
TESTCASE_START
{
    void* arg = NULL;
    TestCase& testcase = Sigleton<TestCase>();
    testcase.addTestCase ("HttpC", [](void* arg){
        cout << "Go Testing" << __FILE__ << endl;

        HttpC http;
        std::string url = "http://m.baidu.com/?from=1002296a";

        http.setWriteToFile (true);
        http.get (url);

        cout << "http code:" << http.getStateCode () << endl;
        cout << "http content length:" << http.getContentLength () << endl;
        if (http.getContent ())
            cout << "content:" << (char*)http.getContent () << endl;

        return 0;
    }, arg );
}

TESTCASE_END

#endif // ENABLE_TESTCASE
