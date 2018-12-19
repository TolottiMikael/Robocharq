#include <iostream>
#include <stdio.h>
#include <winsock2.h>
#include <windows.h>

using namespace std;

    bool gameover = false;
    bool receivable2 = false;

    WSADATA WSAData;

    SOCKET server, client , client2;

    SOCKADDR_IN serverAddr,clientAddr, clientAddr2;


void criaServer(){
    WSAStartup(MAKEWORD(2,0), &WSAData);
    server = socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5555);

}


void comunica(){



    char buffer[1024];
    char resp[1024];

    bind(server, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
    listen(server, 0);

    printf("Listening for incoming connections...\n");
    int clientAddrSize = sizeof(clientAddr);
    if((client = accept(server, (SOCKADDR *)&clientAddr, &clientAddrSize)) != INVALID_SOCKET)
    {

        printf("Client  connected!\n");
        while (strcmp(buffer,"end")!=0){

            recv(client, buffer, sizeof(buffer), 0);


            if(strcmp(buffer,"dois mais dois") == 0){
                strcpy(resp,"é quatro porra ");
                cout << " enviei que é quatro!" << endl;
            }
            else{
                strcpy(resp,"pergunte algo ");
                cout << " enviei que é pergunta" << endl;
            }
            send(client, resp, sizeof(resp), 0);

        }

        closesocket(client);
        printf("Client disconnected.");
    }
}


void comunica2(){


    char buffer[1024];
    char resp[1024];

    bind(server, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
    listen(server, 0);

    printf("Listening for incoming connections...\n");
    int clientAddrSize2 = sizeof(clientAddr2);
    if((client2 = accept(server, (SOCKADDR *)&clientAddr2, &clientAddrSize2)) != INVALID_SOCKET)
    {

        printf("Client 2 connected!\n");
        while (strcmp(buffer,"end")!=0){

            recv(client, buffer, sizeof(buffer), 0);


            if(strcmp(buffer,"dois mais dois") == 0){
                strcpy(resp,"é quatro porra ");
            }
            else{
                strcpy(resp,"pergunte algo ");
            }

            send(client, resp, sizeof(resp), 0);

        }

        closesocket(client);
        printf("Client disconnected.");
    }
}


DWORD WINAPI mythread( LPVOID lpParameter)
{
    cout<< "estou na thread 2" << endl;

    comunica();

	return 0;
}


void criaT1(){

    HANDLE myhandle;
	DWORD mythreadid;
	myhandle = CreateThread(0, 0, mythread, 0, 0, &mythreadid);

}


int main()
{
    criaServer();
    Sleep(10);
    criaT1();
    Sleep(100);
    comunica();

    while (1){
        Sleep(500000);
    }

}
