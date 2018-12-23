#include <iostream>
#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <stdlib.h>
#include <math.h>

using namespace std;

    bool gameover = false;
    bool start = false;
    char aux[1024];

    class Robo {

public:
    void vira(int i);
    int getDir();
    Robo(float a, float b);
    float getX();
    float getY();
    void andaX(float X);
    void andaY(float Y);

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

void anda(int i){
    int otherP = 1;

    if(i == 1){
    otherP = 0;
    }

    float X, Y;
    X = ply[i]->getDir();
    Y = ply[i]->getDir();

    X = sin(X* 3.14159265 /180);
    Y = cos(Y* 3.14159265 /180);

    if((ply[i]->getX() + X) == ply[otherP]->getX() && (ply[i]->getY() + Y) == ply[otherP]->getY()){
        if(ply[i]->getDir() == (ply[otherP]->getDir() + 180)){
            cout<< "estão travados" << endl;
        }
        else {
            ply[i]->andaX(X);
            ply[i]->andaX(Y);

            ply[otherP]->andaX(-X);
            ply[otherP]->andaY(-Y);
        }
    }
    else {
    ply[i]->andaX(X);
    ply[i]->andaX(Y);
}
}

void comunica(int i){

    int robot = i;
    char buffer[1024];
    char resp[1024];
    strcpy(resp, "logged");

    bind(server, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
    listen(server, 0);
    while(!start){
        Sleep(1);
    }

    printf("lobby \n");
    int clientAddrSize = sizeof(clientAddr);
    if((client = accept(server, (SOCKADDR *)&clientAddr, &clientAddrSize)) != INVALID_SOCKET)
    {

        printf("Client  connected!\n");

        while (!gameover){
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
                anda(robot);
            }
            else{
                strcpy(resp,"noo");
            }

            send(client, resp, sizeof(resp), 0);
        }

        while(strcmp(buffer,"endGame") == 0){
            recv(client, buffer, sizeof(buffer), 0);
        }
        strcpy(resp, "yes");
        send(client, resp, sizeof(resp), 0);

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

DWORD WINAPI player2( LPVOID lpParameter)
{

    comunica(1);

	return 0;
}

void criaT2(){

    HANDLE myhandle;
	DWORD mythreadid;
	myhandle = CreateThread(0, 0, player2, 0, 0, &mythreadid);

}

void iniciaUsers(){
	system("SET path=%path%;MinGW64\bin");
    /*
    system("g++ user.cpp -o user1");
    Sleep(1000);
    system("g++ user2.cpp -o user2");
    Sleep(1000);
*/
    system("start user");
    cout<< "iniciando o segundo ! "<< endl;
    system("start user");
    cout<< "terminei" << endl;

    }

bool checkP1(int i){
    if (ply[i]->getX() < (-15) || ply[i]->getX() > 15){
        return true;
    }
    else if(ply[i]->getY() < (-15) || ply[i]->getY() > 15){
        return true;
    }
    else{
        return false;
    }
}

void printGame(){

cout<< "Player 1 x \t y \t dir" << " || Player 2 x \t y \t dir" << endl;
        cout<< " \t "<< ply[0]->getX() << " \t " << ply[0]->getY() << " \t " << ply[0]->getDir()
        << " ||\t " << ply[1]->getX() << " \t " << ply[1]->getY() << " \t " << ply[1]->getDir() << endl;

}



int main()
{


    ply[0] = new Robo(5 , 10);
    ply[1] = new Robo(10, 5);

    criaServer();
    Sleep(10);
    criaT1();
    Sleep(100);
    criaT2();
    Sleep(1000);
    iniciaUsers();
    system("cls");
    start = true;
    while(!gameover){

        printGame();



        if(checkP1(0)){
            gameover = true;
        }
        else if(checkP1(1)){
            gameover = true;
        }
        Sleep(100);
        system("cls");
    }
    if(checkP1(0)){
        cout << "player 2 venceu" << endl;
    }
    else {
        cout << "player 1 venceu" << endl;
    }

    Sleep(1000);

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

void Robo::andaX(float X){
    this->x += X;
}

void Robo::andaY(float Y){
    this->y += Y;
}
