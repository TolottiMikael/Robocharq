#ifndef CLIENT
#define CLIENT

#include <iostream>
#include <windows.h>
#include "cliente.cpp"


using namespace std;


void setup(){
    cout<< "estou no setup" << endl;
}

void loop(){
    cout<< "leituras hahahaha" << endl;
    cout << "leitura da Fre : " << leituraFre() << endl;
    cout << "leitura da esquerda : " << leituraEsq() << endl;
    cout << "leitura da direita : " << leituraDir() << endl;
    Sleep(1000);
}


#endif // CLIENT
