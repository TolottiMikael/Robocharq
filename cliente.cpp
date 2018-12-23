#ifndef USUARIO
#define USUARIO


#include "user.cpp"

#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <winsock2.h>
#include <cstdlib>



using namespace std;

bool gameover = false;
char env[1024];
char response[1024];
void setup();
void loop();


    WSADATA WSAData;
    SOCKET server;
    SOCKADDR_IN addr;

float leituraFre(){
    float dist;
    strcpy(env, "leituraFre");
    send(server, env, sizeof(env), 0);
    recv(server, response, sizeof(response), 0);

    dist = atof(response);
    return dist;
}

float leituraEsq(){

    float dist;
    strcpy(env, "leituraEsq");
    send(server, env, sizeof(env), 0);
    recv(server, response, sizeof(response), 0);

    dist = atof(response);

    return dist;
}

float leituraDir(){
    float dist;
    strcpy(env, "leituraDir");
    send(server, env, sizeof(env), 0);
    recv(server, response, sizeof(response), 0);

    dist = atof(response);

    return dist;
}

void vira(int g){
    strcpy(env, "vira");
    send(server, env, sizeof(env), 0);
    sprintf( env, "%d", g);
    send(server, env, sizeof(env), 0);
}

void anda(){
    strcpy(env, "anda");
    send(server, env, sizeof(env), 0);
}




void conecta(){



    WSAStartup(MAKEWORD(2,0), &WSAData);
    server = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // replace the ip with your futur server ip address. If server AND client are running on the same computer, you can use the local ip 127.0.0.1
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5555);

    connect(server, (SOCKADDR *)&addr, sizeof(addr));
    printf("Connected to server!\n");



}

bool checkGame(){
    strcpy(env, "endGame");
    send(server, env, sizeof(env), 0);
    recv(server, response, sizeof(response), 0);

    if(strcmp(response,"yes") == 0){

        return true;
    }
    else {
         return false;
    }
}

int main(){


    conecta();
    char resp[1024];
    char buffer[1024];

    strcpy(env, "llobby");
    while(!gameover){
        loop();
        if(checkGame){
            gameover = true;
        }

        system("cls");
    }

    closesocket(server);
    WSACleanup();
    printf("Socket closed.");

    return 0;

}



#endif //USUARIO
