#ifndef HTTPC_H
#define HTTPC_H

#include "net.h"
#include <string>
#include "URI.h"

#define USERAGENT "HTTPC 0.1"

class HttpC
{
public:
    HttpC();
    ~HttpC();

    void setUserAgent(const char* ua);

    void teardown();

    int head(std::string& url);
    int get(std::string& url);

    void onRead(const void* buffer, const size_t size);
    void onReadDone();
    char *getContent();
    size_t getContentLength();
    int  getStateCode();

    void setWriteToFileTag(const char* tag);
    void setWriteToFile(bool enable, bool auto_delete);
    const char* getWritedFilePath();
    const char* getHeader();

private:
    void parseHeader();
private:
    Socket* m_pSocket;
    URI m_url;
    char* m_userAgent;
    size_t m_httpOnReadSize;
    size_t m_httpReadedSize;

    size_t m_BufferInitSize;
    size_t m_HeaderOffset;
    size_t m_BufferSize;
    char* m_pBuffer;
    char* m_pHeader;
    char* m_pContent;

    char* m_WriteFileName;
    bool  m_bWriteToFile;
    int   m_iWriteToFileFd;
    bool  m_bAutoDelete;
    char* m_WriteToFileTag;
    //
    int m_contentLength;
    int m_stateCode;
};

#endif // HTTPC_H
