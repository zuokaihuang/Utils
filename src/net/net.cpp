#include "net/net.h"

#include <string.h>
#include <arpa/inet.h>
#include <sys/Socket.h>
#include <unistd.h>

#include <netdb.h>
#include <arpa/inet.h>

#include <debug/Debug.h>
#include <iostream>
using namespace std;

static int
unix_read(int  fd, void*  buff, int  len)
{
    int  ret;
    do { ret = read(fd, buff, len); } while (ret < 0 && errno == EINTR);
    return ret;
}
static int
unix_write(int  fd, const void*  buff, int  len)
{
    int  ret;
    do { ret = write(fd, buff, len); } while (ret < 0 && errno == EINTR);
    return ret;
}

Socket::Socket(Socket_type type){
    this->m_type = type;
    this->m_last_remote_ip = nullptr;
    this->m_last_remote_ip = 0;
    create();
}

Socket::~Socket(){
    close();
}

void Socket::create (){
    if (m_type == Socket_udp){
        if ((m_master_socket = ::socket(AF_INET, SOCK_DGRAM, 0)) == -1){
            DLog("Socket error\n");
        }
    } else if ( m_type == Socket_tcp ){
        if ((m_master_socket = ::socket(AF_INET, SOCK_STREAM, 0)) == -1){
            DLog("Socket error\n");
        }
    }else{
        m_master_socket = -1;
    }
}

void Socket::close (){
    if (m_master_socket != -1){
        ::close(m_master_socket);
    }
    m_master_socket = -1;

    if (m_last_remote_ip) free(m_last_remote_ip);
    m_last_remote_ip = nullptr;

    m_last_remote_port = 0;
}

int Socket::bind (const int port){
    socklen_t socklen = sizeof(struct sockaddr_in);
    struct sockaddr_in address;
    int opt = 1;

    memset(&address, 0, socklen);

    address.sin_family = AF_INET;
    address.sin_port   = htons(port);
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    if ( setsockopt (m_master_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0){
//        ELog("[%s +%d]setsockopt:(%d)%s\n", __FILE__, __LINE__, m_master_socket, strerror (errno), m_master_socket; return -1));
    }

    if (::bind(m_master_socket, (struct sockaddr*)&address, socklen ) == -1){
//        ELog("[%s +%d]bind error:(%d)%s\n", __FILE__, __LINE__, m_master_socket, strerror (errno); return -1));
    }

    if (m_type == Socket_tcp && listen (m_master_socket, 10) < 0){
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
    if(::connect(m_master_socket, (struct sockaddr *)&remote, socklen) < 0){
        ELog("connect:%s\n", strerror (errno));
        return -1;
    }
    return 0;
}

bool Socket::isDataAvailable (unsigned int iTimeout){
    if (m_master_socket == 0){
        return false;
    }

    struct timeval tv;
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(m_master_socket, &fds);

    if (iTimeout!=0){
        tv.tv_sec = iTimeout / 1000;
        tv.tv_usec = iTimeout%1000;
    }
    int rs = select (m_master_socket + 1, &fds, NULL, NULL, &tv);

    if (rs <= 0){
        return false;
    }else{
        return true;
    }
}

Socket* Socket::accept (){
    socklen_t socklen = sizeof(struct sockaddr_in);
    struct sockaddr_in remote;

    memset(&remote, 0, socklen);
    int socket;
    if ((socket = ::accept (m_master_socket, (struct sockaddr*)&remote, (socklen_t*)&socklen )) < 0){
        DLog("accept:%s\n", strerror (errno) );
        return NULL;
    }

    m_last_remote_ip = strdup ( inet_ntoa (remote.sin_addr) );
    m_last_remote_port=ntohs(remote.sin_port);

    Socket* new_Socket = new Socket(Socket_tcp);
    new_Socket->close (); // no need this
    new_Socket->m_master_socket = socket;
    return new_Socket;
}

int Socket::send (const void *buffer, const size_t size){
    return ::send(m_master_socket, buffer, size, 0);
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
    return ::sendto(m_master_socket, buffer, size, 0, (struct sockaddr*)&remote, socklen );
}

ssize_t Socket::recv (void *buffer, const size_t size){
    return ::recv( m_master_socket, buffer, size, 0);
}

ssize_t Socket::recvfrom (void *buffer, const size_t size){
    socklen_t socklen = sizeof(struct sockaddr_in);
    struct sockaddr_in remote;

    memset(&remote, 0, socklen);
    int rs = ::recvfrom(m_master_socket, buffer, size, 0, (struct sockaddr*)&remote, &socklen);

    m_last_remote_ip = strdup( inet_ntoa (remote.sin_addr) );
    m_last_remote_port=ntohs(remote.sin_port);
    return rs;
}

const char* Socket::remote_ip (){
    return m_last_remote_ip == nullptr ? "<unknow>" : m_last_remote_ip ;
}

int  Socket::remote_port (){
    return m_last_remote_port;
}

int Socket::bind_broadcast(const int port){
    return this->bind (port);
}

int Socket::broadcast (const int port, const void *buffer, const size_t size){
    const int opt = 1;
    int rs;
    rs = setsockopt(m_master_socket, SOL_SOCKET, SO_BROADCAST, (char *)&opt, sizeof(opt));
    if ( rs == -1){
        ELog("setsock opt:%s\n", strerror (errno));
        return -1;
    }
    socklen_t socklen = sizeof(struct sockaddr_in);
    struct sockaddr_in remote;

    memset(&remote, 0, socklen);

    remote.sin_family = AF_INET;
    remote.sin_port   = htons(port);
    remote.sin_addr.s_addr = htonl(INADDR_BROADCAST);


    return ::sendto (m_master_socket, buffer, size, 0, (struct sockaddr*)&remote, socklen);
}

ssize_t Socket::recvbroadcast (void *buffer, const size_t size){
    const int opt = 1;
    int rs;
    rs = setsockopt(m_master_socket, SOL_SOCKET, SO_BROADCAST, (char *)&opt, sizeof(opt));
    if ( rs == -1){
        ELog("setsock opt:%s\n", strerror (errno));
        return -1;
    }
    return this->recvfrom (buffer, size);
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







