#ifndef FileGuard
#define FileGuard
#include <iostream>

/*
	Flags das tensões das portas
*/
	int estados[18];

/*

protótipo das funções das classes

*/

class PortaArd{
public:
    //construtor que pede número da porta, se permite PWM e se é analógica
    PortaArd(int n, bool p, bool a);
    void setState(int n);
    int getState();
	void setMode(int m);
	int getNumber();
private:
    int output;
    int number;
	//mode é o INPUT/OUTPUT, onde 0 -> INPUT
	int mode;
    bool analog;
    bool pwmAble;
};

class Arduino{
public:
	Arduino();
	int digitalRead(int nP);
	void digitalWrite(int nP, int o);
	void pinMode(int nP, int modo);

	int analogRead(int nP);
	void analogWrite(int nP, int o);

private:
	PortaArd *portas[18];
};

class Sensor{
	public:
	static int objectCount;
	Sensor(int p1, int p2);
	int isMeding();
	private:
	PortaArd *port[2];
};

class PonteH{
	public:
	PonteH(int n, int p1, int p2);
	void ativa();
	private:
	int ponteNumber;
	PortaArd *port[2];
};

/*
					Alguns objetos "espelhos" para uso
*/
	Sensor *sense[10];
	
int meding(){
	int i;
	int r;
	r = 0;
	for (i = 0; i < Sensor::objectCount; i++)
	{
		r += sense[i]->isMeding();
	}
}
	


/*

códigos dos métodos

*/

PonteH::PonteH(int n, int p1, int p2){
	
	this->ponteNumber = n;
	//input
	this->port[0] = new PortaArd(p1, 0, 0);
	
	//output
	this->port[1] = new PortaArd(p2, 0, 0);
	
}

//sensor
Sensor::Sensor(int p1, int p2){
	
	//input
	this->port[0] = new PortaArd(p1, 0, 0);
	
	//output
	this->port[1] = new PortaArd(p2, 0, 0);
	
	sense[objectCount] = this;
	objectCount++;
}

int Sensor::objectCount = 0;

int Sensor::isMeding(){
	
	
	if(estados[this->port[0]->getNumber() ] > 0){
		estados[this->port[1]->getNumber() ] = 1;
		return 1;
	}
	return 0;
}

Arduino::Arduino(){

	//define os INPUT/OUTPUT
	#define INPUT 0
	#define OUTPUT 1
	#define A0 14
	#define A1 15
	#define A2 16
	#define A3 17
	#define A5 18


	//seta portas
	int i;
	for(i = 0; i < 18; i++){
		if(i < 14){
			portas[i] = new PortaArd(i, 0, 0);
		}
		else{
			portas[i] = new PortaArd(i, 0, 1);
		}
		
		estados[i] = 0;
		
	}
}

int Arduino::digitalRead(int nP){
	
	return this->portas[nP]->getState();
}

int Arduino::analogRead(int nP){

	return this->portas[nP]->getState();
}

void Arduino::analogWrite(int nP, int o){
	if(o > 255){
		o = 255;
	}
	else if(o < 0) {
		o = 0;
	}
	this->portas[nP]->setState(o);
	estados[nP] = o;
	meding();
}


void Arduino::digitalWrite(int nP, int o){
	if(o > 0){
		o = 1;
	}
	else if(o < 0) {
		o = 0;
	}
	this->portas[nP]->setState(o);
	estados[nP] = o;
	
	meding();
}

void Arduino::pinMode(int nP, int modo){
	this->portas[nP]->setMode(modo);
}


//métodos das portas
PortaArd::PortaArd(int n, bool p, bool a){
	this->number = n;
	this->pwmAble = p;
	this->analog = a;
	this->output = 0;
	this->mode = 0;
}

void PortaArd::setMode(int m){
	if (m > 0){
		m = 1;
	}
	else {
	m = 0;
	}

	this->mode = m;
}

void PortaArd::setState(int n){
if(this->mode == 0){
		std::cout<<"Nao permitido para porta de entrada" << std::endl;
	}
else{
	
	if(this->pwmAble){
		estados[ this->number ] = n;
	}
	else {
		if(n > 0){
			//5 volts
			estados[ this->number ] = 5;
		}
		else {
			estados[ this->number ] = 0;
		}
	}
}
}

int PortaArd::getState(){
if(this->mode == 1){
	std::cout<<"Nao permitido para porta de saida" << std::endl;
}
else{
	
	if(analog){
		return estados[ this->number ];
	}
	else{
		if(estados[ this->number ] > 0){
			return 1;
		}
		else {
			return 0;
		}
	}

}
}

int PortaArd::getNumber(){
	return this->number;
}


//métodos do arduino






#endif