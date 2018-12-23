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
    anda();
    Sleep(1000);
    vira(56);

}


#endif // CLIENT
