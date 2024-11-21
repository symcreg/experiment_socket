#include <iostream>
#include <winsock2.h>
#include "server.h"

int protocol = 1;

void Lowercase2Uppercase(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] -= 32;
        }
    }
}

int main() {
    Server server(8080, protocol);
    server.Start();
    int msg_size = 0;
    while (true){
        if(protocol == 0){
            server.Accept();
        }
        msg_size = server.Receive();
        if(msg_size){
            if(server.GetBuffer()[0] == 'q' && server.GetBuffer()[1] == '\0'){
                break;
            }
            std::cout << "Client: " << server.GetBuffer() << std::endl;
            Lowercase2Uppercase(server.GetBuffer());
            server.Send(server.GetBuffer());
            std::cout << "Server: " << server.GetBuffer() << std::endl;
        }
    }
    server.Close();
    return 0;
}
