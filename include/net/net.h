#ifndef NET_H
#define NET_H

#include <unistd.h>

#ifndef DLog
#define DLog( ... ) { fprintf( stdout, __VA_ARGS__ ); printf("\n"); fflush(stdout); }
#endif

#ifndef ELog
#define ELog( ... ) { fprintf( stderr, __VA_ARGS__ ); printf("\n");fflush(stderr); }
#endif

#ifndef DAssert
#define DAssert(cond, msg) { if(!cond){ ELog("%s", msg); assert( !cond && msg); } }
#endif


class  Socket{
public:
    enum Socket_type{
        Socket_tcp,
        Socket_udp
    };
public:
    Socket(Socket_type m_type);
    ~Socket();

    int bind(int port);
    int connect(const char *ip, const int port); // only tcp
    Socket* accept();
    int send(const void* buffer, const size_t size);
    ssize_t recv(const Socket *Socket, void* buffer, const size_t size);

    int sendto(const char* ip, const int port, const void* buffer, const size_t size); // only udp
    ssize_t recvfrom(void* buffer, const size_t size);

    int m_master_Socket;
    Socket_type m_type;
};

char* net_get_ip(const char* host);

#endif // NET_H
