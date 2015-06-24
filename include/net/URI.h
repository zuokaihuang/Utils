#ifndef URI_H
#define URI_H

#include <testing.h>

#include <string>
/*
 * RFC 2396
 * https://www.ietf.org/rfc/rfc2396.txt
*/
// <scheme>://<authority><path>?<query>
class URI
{
public:
    URI(std::string uri);
    ~URI();

    std::string origin;
    std::string scheme;
    std::string path;
    std::string query;

private:
    void parse();
    static char* checkScheme(const char *uri, const size_t size);
    static char* checkPath(const char *uri, const size_t size);
};

std::ostream& operator<<(std::ostream& out, const URI& url);

#endif // URI_H
