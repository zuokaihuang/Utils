#include "net/net.h"




#ifdef ENABLE_TESTCASE

#include "debug/testing.h"
#include <iostream>
using namespace std;

static int test_net (){
    cout << "Go Testing" << __FILE__ << endl;

    int rs;
    const size_t buffer_size = 128;
    char buffer[buffer_size]={0};
    const char* remote_ip ;
    int   remote_port;

    // tcp
    Socket socket_tcp_server(Socket::Socket_tcp);
    socket_tcp_server.bind (9090);

    Socket socket_tcp_client(Socket::Socket_tcp);
    socket_tcp_client.connect ("127.0.0.1", 9090);
    socket_tcp_client.send ("hello", 5);

    Socket* tcp_remote_handler = socket_tcp_server.accept ();
    memset (buffer, 0, buffer_size);
    rs = socket_tcp_server.recv (tcp_remote_handler, buffer, buffer_size);
    cout << "tcp server recv:" << buffer << "," << rs << endl;
    remote_ip = socket_tcp_server.remote_ip ();
    remote_port=socket_tcp_server.remote_port ();
    cout << "!!! tcp server:" << remote_ip << "," << remote_port << endl;

    tcp_remote_handler->send ("world", 5);

    memset (buffer, 0, buffer_size);
    rs = socket_tcp_client.recv (&socket_tcp_client, buffer, buffer_size);
    cout << "tcp client recv:" << buffer << "," << rs << endl;

    tcp_remote_handler->close ();
    delete tcp_remote_handler;
    socket_tcp_client.close ();
    socket_tcp_server.close ();

    // udp
    Socket socket_udp_server(Socket::Socket_udp);
    socket_udp_server.bind (9191);

    Socket socket_udp_client(Socket::Socket_udp);
    rs = socket_udp_client.sendto ("127.0.0.1", 9191, "hello", 5);

    memset (buffer, 0, buffer_size);
    rs = socket_udp_server.recvfrom (buffer, buffer_size);
    cout << "udp server recv:" << buffer << "," << rs << endl;
    remote_ip = socket_udp_server.remote_ip ();
    remote_port = socket_udp_server.remote_port ();
    cout << "!!! udp server:" << remote_ip << "," << remote_port << endl;

    rs = socket_udp_server.sendto (remote_ip, remote_port, "world", 5);
    cout << "udp server sent:" << rs << endl;
    memset (buffer, 0, buffer_size);
    rs = socket_udp_client.recvfrom (buffer, buffer_size);
    cout << "udp client recv:" << buffer << "," << rs << endl;

    socket_udp_client.close ();
    socket_udp_server.close ();


    // broadcast
    Socket broadcast_server1(Socket::Socket_udp);
    broadcast_server1.bind_broadcast (9999);

    Socket broadcast_client(Socket::Socket_udp);

    broadcast_client.broadcast (9999, "hello", 5);

    memset (buffer, 0, buffer_size);
    rs = broadcast_server1.recvbroadcast (buffer, buffer_size);
    remote_ip = broadcast_server1.remote_ip ();
    remote_port = broadcast_server1.remote_port ();
    cout << "!!! broadcast server 1:" << remote_ip << "," << remote_port << endl;
    cout << "!!! broadcast server 1 recv:" << buffer << "," << rs << endl;

    broadcast_client.close ();
    broadcast_server1.close ();

    return 0;
}

TC_Entry( net )

#endif // ENABLE_TESTCASE



