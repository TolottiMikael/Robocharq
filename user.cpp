#ifndef CLIENT
#define CLIENT

#include <iostream>
#include "cliente.cpp"
#include <windows.h>



using namespace std;


void setup(){
    cout<< "estou no setup" << endl;
}

void loop(){
    while(leituraFre() == 0){
        vira(1);
        cout << "viraaa" << endl;
    }
    cout << " anda !!!" << endl;
    anda();

}


#endif // CLIENT
