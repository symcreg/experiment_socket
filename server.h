//
// Created by symc on 2024/11/21.
//

#ifndef NET_SOCKET_SERVER_H
#define NET_SOCKET_SERVER_H

#include <winsock2.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

class Server {
private:
    int protocol_;
    int port_; // server port_
    int server_fd_; // server file descriptor
    int client_fd_; // client file descriptor
    struct sockaddr_in server_addr_;
    struct sockaddr_in client_addr_;
    int client_addr_len_;
    char buffer_[1024];
    int buffer_size_ = 1024;

public:
    Server(int port = 8080, int protocal = 0);
    void Start();
    void Accept();
    void Send(const char *message);
    int Receive();
    char* GetBuffer();
    void Close();
    ~Server(){
        Close();
    }
};

#endif //NET_SOCKET_SERVER_H
