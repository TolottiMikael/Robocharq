#ifndef arquivoTeste
#define arquivoTeste
#include <iostream>
#include "arduino.cpp"

int main() {
	
	std::cout << "to vivo" << std::endl;
	Arduino *ard = new Arduino();
	
	ard->pinMode(2, 1);
	ard->pinMode(3, 0);
	
	ard->pinMode(6, 1);
	ard->pinMode(7, 0);
	Sensor *a = new Sensor(2 , 3);
	Sensor *b = new Sensor(6 , 7);
	
	
	int leitura;
	
	ard->digitalWrite(2, 1);
	leitura = ard->digitalRead(3);
	std::cout<< "leitura de : " <<leitura << std::endl;
	
	ard->digitalWrite(6, 0);
	leitura = ard->digitalRead(7);
	std::cout<< "leitura de : " <<leitura << std::endl;
	
	
}
#endif