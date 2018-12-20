#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <winsock2.h>

using namespace std;

bool gameover = false;


    WSADATA WSAData;
    SOCKET server;
    SOCKADDR_IN addr;



void conecta(){



    WSAStartup(MAKEWORD(2,0), &WSAData);
    server = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // replace the ip with your futur server ip address. If server AND client are running on the same computer, you can use the local ip 127.0.0.1
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5555);

    connect(server, (SOCKADDR *)&addr, sizeof(addr));
    printf("Connected to server!\n");



}


int main(){

    conecta();
    char resp[1024];
    char buffer[1024];

    while(strcmp(buffer,"end")!=0){
            cout << " o que vai fazer? " << endl;
            scanf("%[^\n]", buffer);
            setbuf(stdin, NULL);

            send(server, buffer, sizeof(buffer), 0);
            recv(server, resp, sizeof(resp), 0);

            cout << "o server respondeu " << resp << endl;

    }

    closesocket(server);
    WSACleanup();
    printf("Socket closed.");


    return 0;

}
