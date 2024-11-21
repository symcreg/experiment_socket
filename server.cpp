//
// Created by symc on 2024/11/21.
//

#include "server.h"
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")


Server::Server(int port, int protocal) {
    this->protocol_ = protocal;
    WORD version = MAKEWORD(2, 2);
    WSADATA data;
    if (WSAStartup(version, &data) != 0) {
        std::cerr << "Failed to start WSA" << std::endl;
        exit(1);
    }
    this->port_ = port;
    //protocol: 0 means TCP, 1 means UDP
    if(this->protocol_ == 0){
        this->server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    }else if(this->protocol_ == 1){
        this->server_fd_ = socket(AF_INET, SOCK_DGRAM, 0);
    }
    if (this->server_fd_ == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        exit(1);
    }

    this->server_addr_.sin_family = AF_INET; //sin means socket internet
    this->server_addr_.sin_addr.s_addr = INADDR_ANY;
    this->server_addr_.sin_port = htons(this->port_);

    if (bind(this->server_fd_, (struct sockaddr *) &this->server_addr_, sizeof(this->server_addr_)) < 0) {
        std::cerr << "Failed to bind socket" << std::endl;
        exit(1);
    }

    this->client_addr_len_ = sizeof(this->client_addr_);

}
void Server::Start() {
    if(this->protocol_ == 0) {
        std::cout << "[TCP]" << std::endl;
        if (listen(this->server_fd_, 10) < 0) {
            std::cerr << "Failed to listen" << std::endl;
            exit(1);
        }
    }else if(this->protocol_ == 1){
        std::cout << "[UDP]" << std::endl;
    }
    std::cout << "Server started on port " << this->port_ << std::endl;
}

void Server::Accept() {
    if(this->protocol_ == 1){
        std::cerr << "UDP does not support accept" << std::endl;
        exit(1);
    }
    this->client_fd_ = ::accept(this->server_fd_, (struct sockaddr *) &this->client_addr_, &this->client_addr_len_);
    if (this->client_fd_ < 0) {
        std::cerr << "Failed to accept client" << std::endl;
        exit(1);
    }
    std::cout << "Client connected" << std::endl;
}

void Server::Send(const char *message) {
    if(this->protocol_ == 0) {
        if (::send(this->client_fd_, message, strlen(message), 0) < 0) {
            std::cerr << "Failed to send message" << std::endl;
            exit(1);
        }
    }else if(this->protocol_ == 1){
        if (sendto(this->server_fd_, message, strlen(message), 0, (struct sockaddr *) &this->client_addr_, sizeof(this->client_addr_)) < 0) {
            std::cerr << "Failed to send message" << std::endl;
            exit(1);
        }
    }
}

int Server::Receive() {
    memset(this->buffer_, 0, sizeof(this->buffer_));
    if(this->protocol_ == 0){
        if (recv(this->client_fd_, this->buffer_, sizeof(this->buffer_), 0) < 0) {
            std::cerr << "Failed to receive message" << std::endl;
            exit(1);
        }
    }else if(this->protocol_ == 1){
        if (recvfrom(this->server_fd_, this->buffer_, sizeof(this->buffer_), 0, (struct sockaddr *) &this->client_addr_, &this->client_addr_len_) < 0) {
            std::cerr << "Failed to receive message" << std::endl;
            exit(1);
        }
    }
    this->buffer_size_ = strlen(this->buffer_);
    return buffer_size_;
}
char* Server::GetBuffer() {
    return this->buffer_;
}
void Server::Close() {
    closesocket(this->client_fd_);
    closesocket(this->server_fd_);
    WSACleanup();
}