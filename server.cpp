#include <iostream>
#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <stdlib.h>

using namespace std;

    bool gameover = false;
    bool receivable2 = false;
    char aux[1024];

    class Robo {

public:
    void vira(int i);
    int getDir();
    Robo(int a, int b);
    int getX();
    int getY();
private:
    void incDir(int i);
    int dir = 1;
    int x, y;


};


Robo *ply[2];



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

void leituraFre(int i){
    int d;
    int otherP;

    if(i == 1 ){
        otherP = 0;
    }
    else if( i == 0 ){
        otherP = 1;
    }


    //aqui descobre a distância
    if(ply[i]->getY() == ply[otherP]->getY()){
        //estão na mesma linha
        if(ply[i]->getX() >= ply[otherP]->getX()){
           d = ply[i]->getX() - ply[otherP]->getX();
        }
        else {
           d = 0;
        }
    }
    else{
        d = 0;
    }


    sprintf( aux, "%d", d);
}

char leituraEsq(int i){
    return '0';
}

char leituraDir(int i){
    return '0';
}



void comunica(int i){


    int robot = i;
    char buffer[1024];
    char resp[1024];
    strcpy(resp, "logged");

    bind(server, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
    listen(server, 0);

    printf("Listening for incoming connections...\n");
    int clientAddrSize = sizeof(clientAddr);
    if((client = accept(server, (SOCKADDR *)&clientAddr, &clientAddrSize)) != INVALID_SOCKET)
    {

        printf("Client  connected!\n");

        while (strcmp(buffer,"end")!=0){

            recv(client, buffer, sizeof(buffer), 0);

            if(strcmp(buffer,"leituraFre") == 0){
                leituraFre(robot);
               strcpy(resp, aux);
            }
            else if(strcmp(buffer,"leituraEsq") == 0){
                strcpy(resp, "100");
            }
            else if(strcmp(buffer,"leituraDir") == 0){
                strcpy(resp, "100");
            }
            else{
                strcpy(resp,"não entendi!");
            }
            cout<< "respondi com "<< resp << endl;

            send(client, resp, sizeof(resp), 0);
        }
        closesocket(client);
        printf("Client disconnected.");
    }
}



DWORD WINAPI mythread( LPVOID lpParameter)
{
    cout<< "estou na thread 2" << endl;

    comunica(0);

	return 0;
}


void criaT1(){

    HANDLE myhandle;
	DWORD mythreadid;
	myhandle = CreateThread(0, 0, mythread, 0, 0, &mythreadid);

}




int main()
{

    ply[0] = new Robo(10 , 60);
ply[1] = new Robo(5, 60);
    criaServer();
    Sleep(10);
    criaT1();
    Sleep(100);
    comunica(1);



}

void Robo::vira(int i){
    this->incDir(i);
}

void Robo::incDir(int i){
    this->dir += i;

    while(this->dir > 4 || this->dir < 1) {
        if (this->dir > 4){
            this->dir -= 4;
        }
        else {
            this-> dir += 4;
        }

    }

}

int Robo::getDir(){
    return this->dir;
}

int Robo::getX(){
    return this->x;
}

int Robo::getY(){
    return this->y;
}

Robo::Robo(int a, int b){
    this->x = a;
    this->y = b;
}
