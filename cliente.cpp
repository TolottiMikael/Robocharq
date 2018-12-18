#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <winsock2.h>

using namespace std;

bool gameover = false;

char resp[1024];
char buffer[1024];


void comunica(){

    WSADATA WSAData;
    SOCKET server;
    SOCKADDR_IN addr;

    WSAStartup(MAKEWORD(2,0), &WSAData);
    server = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // replace the ip with your futur server ip address. If server AND client are running on the same computer, you can use the local ip 127.0.0.1
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5555);

    connect(server, (SOCKADDR *)&addr, sizeof(addr));
    printf("Connected to server!\n");


    while (resp != "end" && !gameover){
        send(server, buffer, sizeof(buffer), 0);
        recv(server, resp , sizeof(resp), 0);
    }

    closesocket(server);
    WSACleanup();
    printf("Socket closed.");
}



DWORD WINAPI mythread( LPVOID lpParameter)
{
    cout<< "estou na thread" << endl;

    while (!gameover){

        cout << "o que você deseja fazer? " << endl;
        scanf("%[^\n]s", buffer);
        setbuf(stdin, NULL);
        cout << "o server respondeu com  -->  " << resp << endl;
    }

	return 0;
}


void criaT1(){

    HANDLE myhandle;
	DWORD mythreadid;
	myhandle = CreateThread(0, 0, mythread, 0, 0, &mythreadid);

}

void prog(){

    cout<< "server disse que: " << resp << endl;

}


int main(){

    criaT1();
    comunica();


    return 0;

}
