#ifndef NET_H
#define NET_H

#include <unistd.h>

class  Socket{
public:
    enum Socket_type{
        Socket_tcp,
        Socket_udp
    };
public:
    Socket(Socket_type m_type);
    ~Socket();

    void close();
private:
    void create();
public:
    int bind(const int port);
    int connect(const char *ip, const int port); // only tcp
    Socket* accept();
    int send(const void* buffer, const size_t size);
    ssize_t recv(const Socket *Socket, void* buffer, const size_t size);

    int sendto(const char* ip, const int port, const void* buffer, const size_t size); // only udp
    ssize_t recvfrom(void* buffer, const size_t size);
    const char* remote_ip();
    int   remote_port();

    // broadcast
    int bind_broadcast(const int port);
    int broadcast(const int port, const void* buffer, const size_t size);
    ssize_t recvbroadcast(void* buffer, const size_t size);

    int m_master_Socket;
    Socket_type m_type;

private:
    char* m_last_remote_ip;
    int   m_last_remote_port;
};

char* net_get_ip(const char* host);

#endif // NET_H
