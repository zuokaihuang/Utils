#ifndef URI_H
#define URI_H

#include <string>
/*
 * RFC 2396
 * https://www.ietf.org/rfc/rfc2396.txt
*/
// <scheme>://<authority><path>?<query>
class URI
{
public:
    URI();
    ~URI();

    void parse(const char* url);
    void parse(std::string &url);
    bool valid();
    bool isHttp();
    bool isFile();

    void dump();
private:
    void url_get_schema();
    void url_get_host();
    void url_get_port();
    void url_get_path();

    void url_get_file_path();
public:
    std::string m_origin;
    char*   schema;
    char*   host;
    int     port;
    char*   path;
    char*   query;
};

std::ostream& operator<<(std::ostream& out, const URI& url);

#endif // URI_H
