#include <iostream>
#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <stdlib.h>
#include <math.h>

using namespace std;

    bool gameover = false;
    char aux[1024];

    class Robo {

public:
    void vira(int i);
    int getDir();
    Robo(float a, float b);
    float getX();
    float getY();
    void anda();
private:
    void incDir(int i);
    int dir = 270;
    float x, y;


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


float calc(int i, float a){

float t;
float ang;
float catX, catY;
float X[2], Y[2];

int otherP = 1;

if(i == 1){
    otherP = 0;
}

catX = 0;
catY = 0;

X[0] = ply[i]->getX();
Y[0] = ply[i]->getY();

X[1] = ply[otherP]->getX();
Y[1] = ply[otherP]->getY();

catX = X[0] - X[1];
catY = Y[0] - Y[1];


 t = sqrt( pow( catX ,  2)  +  pow( catY  ,  2) );

	if(Y[0] == Y[1] ){
		if( X[0] > X[1] ){
			ang = 270;
			t = X[0] - X[1];
		}
		else if(X[0] < X[1]){
			ang = 90;
			t = X[1] - X[0];
		}
	}
	else if(X[0] == X[1] ){
		if( Y[0] < Y[1] ){
			ang = 0;
		}
		else if(Y[0] > Y[1]){
			ang = 180;
		}
	}
	else{
		//sin(A) = X/T
		ang = asin( (catX / t) );


	ang = (ang * 180) / 3.14;
	if(ang < 0){
		ang = ang * -1;
	}


	if(catX < 0  && catY < 0){
		ang = 180 + ang;
	}
	else if  (catX > 0  && catY < 0){
		ang = 180 - ang;
	}

	else if  (catX > 0  && catY > 0){
		ang = 180 + ang;
	}

	else if  (catX < 0  && catY > 0){
		ang = 180 - ang;
	}
	}



	do{
	if(a >= 360){
		a-= 360;
	}
	else if(a < 0){
		a+= 360;
	}
	}while (a > 360  || a < 0);


	if (a > ( ang - 12 ) && a < ( ang + 12 )) {
		return t;
	}

	else {
		return 0;
	}
}

void leituraFre(int i){
    float d;
//aqui descobre a distância
    d = calc(i, ply[i]->getDir());
    cout<< "distancia de : " << d << endl;
    sprintf( aux, "%f", d);
}

char leituraEsq(int i){
    float d;
    //aqui descobre a distância
    d = calc(i, ply[i]->getDir() - 90);


    sprintf( aux, "%f", d);
}

char leituraDir(int i){
    float d;

    //aqui descobre a distância
    d = calc(i, ply[i]->getDir() + 90);

    sprintf( aux, "%f", d);
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
                leituraEsq(robot);
               strcpy(resp, aux);
            }
            else if(strcmp(buffer,"leituraDir") == 0){
                leituraDir(robot);
               strcpy(resp, aux);
            }
            else if(strcmp(buffer,"vira") == 0){
                recv(client, buffer, sizeof(buffer), 0);
                int dist;
                dist = atoi(buffer);
                ply[i]->vira(dist);
            }
            else if(strcmp(buffer,"anda") == 0){
                ply[i]->anda();
            }
            else{
                strcpy(resp,"não entendi!");
            }

            send(client, resp, sizeof(resp), 0);
        }
        closesocket(client);
        printf("Client disconnected.");
    }
}



DWORD WINAPI mythread( LPVOID lpParameter)
{

    comunica(0);

	return 0;
}


void criaT1(){

    HANDLE myhandle;
	DWORD mythreadid;
	myhandle = CreateThread(0, 0, mythread, 0, 0, &mythreadid);

}

void iniciaUsers(){
	system("SET path=%path%;MinGW64\bin");

    system("g++ usuario.cpp -o user1");
    Sleep(1000);
    system("user1");

    system("g++ usuario2.cpp -o user2");
    Sleep(1000);
    system("user2");
}



int main()
{
    //iniciaUsers();

    ply[0] = new Robo(15 , 30);
    ply[1] = new Robo(10, 30);

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

    do{
	if(this->dir >= 360){
		this->dir-= 360;
	}
	else if(this->dir < 0){
		this->dir+= 360;
	}
	}while (this->dir > 360  || this->dir < 0);

    }

int Robo::getDir(){
    return this->dir;
}

float Robo::getX(){
    return this->x;
}

float Robo::getY(){
    return this->y;
}

Robo::Robo(float a, float b){
    this->x = a;
    this->y = b;
}

void Robo::anda(){

    this->x = sin(this->getDir()) * 1;
    this->y = cos(this->getDir()) * 1;
    cout << "new position : x -> " << this->x << " Y -> " << this->y << endl;
}
