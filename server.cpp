#include <iostream>
#include <stdio.h>
#include <winsock2.h>
#include <windows.h>

using namespace std;


    char buffer[1024];
    char buffer1[1024] ;
    char resp[1024];
    char resp1[1024];

    bool gameover = false;


void comunica(){
    WSADATA WSAData;

    SOCKET server, client, client2;

    SOCKADDR_IN serverAddr, clientAddr, clientAddr2;

    WSAStartup(MAKEWORD(2,0), &WSAData);
    server = socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5555);

    bind(server, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
    listen(server, 0);

    printf("Listening for incoming connections...\n");
    bool a = true;
    int clientAddrSize = sizeof(clientAddr);
    int clientAddrSize2 = sizeof(clientAddr2);
    if((client = accept(server, (SOCKADDR *)&clientAddr, &clientAddrSize)) != INVALID_SOCKET)
    {

        printf("Client 1 connected!\n");
        do{
        if((client2 = accept(server, (SOCKADDR *)&clientAddr2, &clientAddrSize2)) != INVALID_SOCKET){
           printf("Client 2 connected!\n");
           a = false;
        }
        } while(a);
        cout<< "cliente 1 \t cliente 2" << endl;
        while (strcmp(buffer,"end")!=0 || strcmp(buffer1,"end")!=0 ){

            recv(client, buffer, sizeof(buffer), 0);
            send(client, resp, sizeof(resp), 0);


            recv(client2, buffer1, sizeof(buffer1), 0);
            send(client2, resp1, sizeof(resp1), 0);


        }

        closesocket(client);
        printf("Client disconnected.");
    }
}


DWORD WINAPI mythread( LPVOID lpParameter)
{
    cout<< "estou na thread" << endl;
     strncpy(resp, "testes", sizeof(resp));
     strncpy(resp1, "teste", sizeof(resp1));

    while (1){


        if(strcmp(buffer,"dois mais dois")==0) {
            strncpy(resp, "eh quatro !!!", sizeof(resp));
            cout << "agora resp é " << resp << endl;
        }
        else {
            strncpy(resp, "pergunte algo!", sizeof(resp));
        }

        if(strcmp(buffer1,"ola")==0){
            strncpy(resp1, "oiii, eu sou o goku!", sizeof(resp1));
            cout << " agora resp1 é " << resp1 << endl;
        }
        else {
            strncpy(resp1, "alo", sizeof(resp1));
        }

    }

	return 0;
}


void criaT1(){

    HANDLE myhandle;
	DWORD mythreadid;
	myhandle = CreateThread(0, 0, mythread, 0, 0, &mythreadid);

}


int main()
{
    criaT1();
    comunica();

}
