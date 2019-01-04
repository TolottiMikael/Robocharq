#include <iostream>
#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <stdlib.h>
#include <math.h>


#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>



#define FPS 60.0
#define LARGURA_TELA 640
#define ALTURA_TELA 480

    ALLEGRO_DISPLAY *janela = NULL;
    ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_BITMAP *quadrado = NULL;
    ALLEGRO_BITMAP *quadrado2 = NULL;
    ALLEGRO_BITMAP *seta1 = NULL;
    ALLEGRO_BITMAP *seta2 = NULL;


using namespace std;

    bool gameover = false;
    bool start = false;
    char aux[1024];
    float roboX[2][10000];
    float roboY[2][10000];
    int turno = 0;
    bool sTurno = false;

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
            system("exit");
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
            roboX[robot][turno] = ply[i]->getX();
            roboY[robot][turno] = ply[i]->getY();

            if(sTurno == true){
                turno++;
                sTurno = false;
            }
            else{
                sTurno = true;
            }
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

    system("MinGW64\bin\g++ user.cpp -o user1 -lws_32s");
    Sleep(1000);
    system("g++ user.cpp -o user2 -lws_32s");
    Sleep(1000);

    system("start user");
    cout<< "iniciando o segundo ! "<< endl;
   // system("start user");
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



void error_msg(char *text){
    	al_show_native_message_box(NULL,"ERRO",
    		"Ocorreu o seguinte erro e o programa sera finalizado:",
    		text,NULL,ALLEGRO_MESSAGEBOX_ERROR);
}

int inicializar(){
    if (!al_init()){
        error_msg("Falha ao inicializar a Allegro");
        return 0;
    }

    //cria o timer com o intervalo de tempo que ele ira disparar
    timer = al_create_timer(1.0 / FPS);
    if(!timer) {
        error_msg("Falha ao criar temporizador");
        return 0;
    }

    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if(!janela) {
        error_msg("Falha ao criar janela");
        al_destroy_timer(timer);
        return 0;
    }

    al_set_window_title(janela, "Mikael Simulations");

    quadrado = al_create_bitmap(50, 50);

    if (!quadrado){
        error_msg("Falha ao criar bitmap");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        return 0;
    }

    quadrado2 = al_create_bitmap(50, 50);
    if(!quadrado2){
        error_msg("Falha ao criar bitmap");
        al_destroy_timer(timer);
        al_destroy_bitmap(quadrado);
        al_destroy_display(janela);
        return 0;
    }

    seta1 = al_create_bitmap(10, 50);
    if(!seta1){
        error_msg("Falha ao criar bitmap");
        al_destroy_timer(timer);
        al_destroy_bitmap(quadrado);
        al_destroy_bitmap(quadrado2);
        al_destroy_display(janela);
        return 0;
    }
    seta2 = al_create_bitmap(50, 10);
    if(!seta2){
        error_msg("Falha ao criar bitmap");
        al_destroy_timer(timer);
        al_destroy_bitmap(quadrado);
        al_destroy_bitmap(quadrado2);
        al_destroy_bitmap(seta1);
        al_destroy_display(janela);
        return 0;
    }

        al_set_target_bitmap(quadrado);
        al_clear_to_color(al_map_rgb(255, 0, 0));
        al_set_target_bitmap(quadrado2);
        al_clear_to_color(al_map_rgb(255, 255, 255));

        al_set_target_bitmap(seta1);
        al_clear_to_color(al_map_rgb(0,255,25));

        al_set_target_bitmap(seta2);
        al_clear_to_color(al_map_rgb(0,255,25));

        al_set_target_bitmap(al_get_backbuffer(janela));

    fila_eventos = al_create_event_queue();
    if(!fila_eventos) {
        error_msg("Falha ao criar fila de eventos");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_bitmap(quadrado);
        return 0;
    }

    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_clear_to_color(al_map_rgb(0,0,0));
    al_flip_display();
    al_start_timer(timer);

    return 1;
}

int rodaTela(void){

    int vez;
    vez = 0;
    //define quando a tela sera atualizada
    int desenha = 1;
    //quando o loop principal deve encerrar
    int sair = 0;
    //posicao do quadrado e quanto ele andara a cada disparo do timer, para coordenada X e Y
    int X[2], Y[2];
    int dirX[2], dirY[2];

    X[0] = 15;  Y[0] = 15;
    dirX[0] = 5; dirY[0] = 5;

    X[1] = 100;  Y[1] = 100;
    dirX[1] = 3; dirY[1] = 3;


    if (!inicializar()){
        return -1;
    }

    while(!sair){
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);

        // -- EVENTOS --
        //se o tipo do evento for o disparo de um timer
        if(evento.type == ALLEGRO_EVENT_TIMER){
            X[0] += roboX[0][vez];
            Y[0] += roboY[0][vez];

            X[1] += roboX[1][vez];
            Y[1] += roboY[1][vez];

            if (vez >= turno){
                sair = 1;
            }
            vez++;
        }
        else if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            sair = 1;
        }

        // -- ATUALIZA TELA --
        if(desenha && al_is_event_queue_empty(fila_eventos)) {

            al_clear_to_color(al_map_rgb(0,0,0));

            //desenha o quadrado na tela nas posicoes X e Y
            al_draw_bitmap(quadrado, X[0], Y[0], 0);
            al_draw_bitmap(seta1, X[0] + 40, Y[0],0);
            al_draw_bitmap(quadrado2, X[1], Y[1], 0);
            al_draw_bitmap(seta2, X[1], Y[1] + 40 ,0);
            al_flip_display();

            //zera flag para nao entrar aqui no proximo loop
            desenha = 0;
        }
    }

    al_destroy_bitmap(quadrado);
    al_destroy_bitmap(quadrado2);
    al_destroy_timer(timer);
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);

    return 0;
}



int main()
{

    ply[0] = new Robo(5 , 10);
    ply[1] = new Robo(10, 5);

    criaServer();
    Sleep(10);
    criaT1();
    criaT2();
    Sleep(10);
    iniciaUsers();
    system("cls");
    start = true;
    while(!gameover){

        //printGame();

        if(checkP1(0)){
            gameover = true;
        }
        else if(checkP1(1)){
            gameover = true;
        }
        cout<<"to no while gameover e " << gameover << endl;
    }

    cout<<"sai do while"<< endl;

    if(checkP1(0)){
        cout << "player 2 venceu" << endl;
    }
    else {
        cout << "player 1 venceu" << endl;
    }


    int i;
    for(i =0; i <= turno; i++){

        cout<< "robo 1 X" << roboX[0][i] << "\t robo 1 Y" << roboY[0][i] << endl;
        cout<< "robo 2 X" << roboX[1][i] << "\t robo 2 Y" << roboY[1][i] << "\t for nro:" << i << endl;
    }
    Sleep(1000);


    return 0;
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
