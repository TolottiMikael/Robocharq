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
    if(leituraFre() == 0){
        vira(15);
    }
    else {
        anda();
    }
}


#endif // CLIENT
