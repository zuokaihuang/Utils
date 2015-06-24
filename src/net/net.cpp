#include "net/net.h"

#include <string.h>
#include <arpa/inet.h>
#include <sys/Socket.h>
#include <unistd.h>

#include <netdb.h>
#include <arpa/inet.h>


#include <iostream>
using namespace std;


Socket::Socket(Socket_type type){
    this->m_type = type;
}

Socket::~Socket(){
    close();
}

void Socket::create (){
    if (m_type == Socket_udp){
        if ((m_master_Socket = ::socket(AF_INET, SOCK_DGRAM, 0)) == -1){
            DLog("Socket error\n");
        }
    } else if ( m_type == Socket_tcp ){
        if ((m_master_Socket = ::socket(AF_INET, SOCK_STREAM, 0)) == -1){
            DLog("Socket error\n");
        }
    }else{
        m_master_Socket = -1;
    }
}

void Socket::close (){
    if (m_master_Socket != -1){
        ::close(m_master_Socket);
    }
    m_master_Socket = -1;
}

int Socket::bind (int port){
    socklen_t socklen = sizeof(struct sockaddr_in);
    struct sockaddr_in address;
    int opt = 1;

    memset(&address, 0, socklen);

    address.sin_family = AF_INET;
    address.sin_port   = htons(port);
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    if ( setsockopt (m_master_Socket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0){
        ELog("setsockopt:%s\n", strerror (errno));
        return -1;
    }

    if (::bind(m_master_Socket, (struct sockaddr*)&address, socklen ) == -1){
        ELog("bind error:%s\n", strerror (errno));
        return -1;
    }

    if (m_type == Socket_tcp && listen (m_master_Socket, 10) < 0){
        ELog("listen:%s\n", strerror (errno) );
        return -1;
    }
    return 0;
}

int Socket::connect (const char *ip, const int port){
    close();
    create();

    socklen_t socklen = sizeof(struct sockaddr_in);
    struct sockaddr_in remote;

    memset(&remote, 0, socklen);
    remote.sin_family = AF_INET;
    remote.sin_port   = htons(port);
    remote.sin_addr.s_addr = inet_addr(ip);
    if(::connect(m_master_Socket, (struct sockaddr *)&remote, socklen) < 0){
        ELog("connect:%s\n", strerror (errno));
        return -1;
    }
    return 0;
}

Socket* Socket::accept (){
    socklen_t socklen = sizeof(struct sockaddr_in);
    struct sockaddr_in remote;

    memset(&remote, 0, socklen);
    int socket;
    if ((socket = ::accept (m_master_Socket, (struct sockaddr*)&remote, (socklen_t*)&socklen )) < 0){
        DLog("accept:%s\n", strerror (errno) );
        return NULL;
    }
    Socket* new_Socket = new Socket(Socket_tcp);
    ::close(new_Socket->m_master_Socket);
    new_Socket->m_master_Socket = socket;
    return new_Socket;
}

int Socket::send (const void *buffer, const size_t size){
    return ::send(m_master_Socket, buffer, size, 0);
}

int Socket::sendto(const char *ip, const int port, const void *buffer, const size_t size){
    socklen_t socklen = sizeof(struct sockaddr_in);
    struct sockaddr_in remote;

    memset(&remote, 0, socklen);

    remote.sin_family = AF_INET;
    remote.sin_port   = htons(port);
    if ( inet_aton(ip, &remote.sin_addr) == 0){
        DLog("inet_aton\n");
    }
    return ::sendto(m_master_Socket, buffer, size, 0, (struct sockaddr*)&remote, socklen );
}

ssize_t Socket::recv ( const Socket* Socket, void *buffer, const size_t size){
    return ::recv( Socket->m_master_Socket, buffer, size, 0);
}

ssize_t Socket::recvfrom (void *buffer, const size_t size){
    socklen_t socklen = sizeof(struct sockaddr_in);
    return ::recvfrom(m_master_Socket, buffer, size, 0, (struct sockaddr*)NULL, &socklen);
}

// net
char* net_get_ip(const char* host){
    struct hostent *hent = NULL;
    int ip_len = 15;
    char* ip = NULL;

    if ( NULL != host ){
        if ( (hent = gethostbyname (host)) == NULL){
            herror ("hostbyname error");
            return NULL;
        }
    }
    ip = (char*) calloc(1, ip_len + 1);
    if(inet_ntop(AF_INET, (void *)hent->h_addr_list[0], ip, ip_len) == NULL){
        perror ("resolve host error");
        free( ip );
        return NULL;
    }
    return ip;
}
