//Esse é o código Original usado quando havia todas as peças funcinoais(como foi dito no relatório, algumas peças queimaram)
//O código usado para a demonstração é uma variante deste, com algumas breves modificações

//incluindo bibliotecas
#include <Servo.h>

// Botões
#define Shootbutton 0
#define Modebutton 2

// Buzzer
#define buzzer 3

// Servos
Servo servoY; // pino 4
Servo servoX; // pino 5
Servo servoshoot; // pino 11

// CC motores
#define motorCW1 9 
#define motorCW2 10 
#define motorCCW1 12
#define motorCCW2 13

// Potênciometros
#define PotX A4
#define PotY A5

// Sensor Ultrassônico
#define echoPin A0
#define trigPin A1
long duracao;
long distancia_cm=0;
int minimumRange=5; //tempo de resposta do sensor
int maximumRange=200;

// Contadores auxiliares
int contador_modo=1;
int cont1=0;
int cont2=0;
int municao=12; //quantidade de dardos no cartucho

// Definindo variáveis auxiliares dos servos motores
int eixoX = 0;
int eixoY = 0;
int velocidadeX = 5;
int velocidadeY = 10;

void setup(){
	Serial.begin(9600); 
  
  	servoX.attach(5); servoX.write(0);   // posição inicial eixo X
    servoY.attach(4); servoY.write(45);  // posição inicial eixo Y  
  	servoshoot.attach(11); servoshoot.write(0); // servo do disparo zerado
  
  	pinMode(trigPin, OUTPUT); // define o pino trigger como saída
  	pinMode(echoPin, INPUT);  // define o pino echo como entrada
  
  	pinMode(motorCW1,OUTPUT); // motor horário 1
  	pinMode(motorCW2,OUTPUT); // motor horário 2
  	pinMode(motorCCW1,OUTPUT); // motor anti-horário 1
  	pinMode(motorCCW2,OUTPUT); // motor anti-horário 2
  
  	pinMode(Shootbutton,INPUT_PULLUP); // botão de disparo
  	pinMode(Modebutton,INPUT_PULLUP);  // botão de troca de modo
  
  	pinMode(buzzer, OUTPUT); // buzzer
  
  	pinMode(PotX, INPUT); // potenciômetro X
  	pinMode(PotY, INPUT); // potenciômetro Y
}

void loop(){
  	// Verificação dos modos
  	if(contador_modo==1){
     	Serial.println("Modo manual ativado");
  		delay(1000);
      	while(contador_modo==1){
        	modo_manual(); 
        }
    } 
  	else if(contador_modo==2){
      	Serial.println("Modo automatico ativado");
  		delay(1000);
      	while(contador_modo==2){
    		modo_automatico(); 
        
      	}
   	}
}
// Modo automático
void modo_automatico(){
  	int distancia = lerSonar(); // lê distância do sonar 
   	Serial.print("distancia em cm: ");
   	Serial.println(distancia);
  
  	// Dispara enquanto alvo está próximo
  	while (distancia < 80){
  		disparo();
    	distancia = lerSonar();
    	Serial.print("distancia em cm: ");
   	    Serial.println(distancia);
    	delay(500); 
  	}
  
  	// Varredura automática do servo
  	servoX.write(eixoX);
  	servoY.write(eixoY);
  	eixoX+=velocidadeX;
  	if (eixoX >= 180 || eixoX <= 0){
  		velocidadeX = -velocidadeX;
    	eixoY+=velocidadeY;
    	if (eixoY >= 90 || eixoY <= 0){
    		velocidadeY = -velocidadeY;
    	}
  	}
  
	// Troca o modo
	if(digitalRead(Modebutton)==LOW){
    	pressionar_botao_modo();
  	}
}

// Modo manual
void modo_manual(){
  	// leitura dos potenciômetros
  	int valY = analogRead(PotY);
    int valX = analogRead(PotX);
    valY = map(valY, 0, 1023, 0, 90);
    valX = map(valX, 0, 1023, 0, 180);
    servoY.write(valY);
    servoX.write(valX);
    delay(5);
  
  	// Verifica se estar no limite dos servo
    if (valY == 90 || valY == 0){cont1++;}
  	else{cont1 = 0;}
    if (valX == 180 || valX == 0){cont2++;}
 	else{cont2 = 0;}
  	if (cont1 == 1 || cont2 == 1){apito();}
  
  	// Botão de disparo
  	if(digitalRead(Shootbutton)==LOW){
    	disparo();
    	delay(100);
  	}
  
  	// Botão de troca de modo
  	if(digitalRead(Modebutton)==LOW){
    	pressionar_botao_modo();
  	}
}

// Disparo
void disparo(){
  	int pos;
  
	// liga os motores
	digitalWrite(motorCW1, 1);
	digitalWrite(motorCW2, 0);
  	digitalWrite(motorCCW1,1);
	digitalWrite(motorCCW2,0);
  
	delay(800);
  
  	// Movimento do servo de disparo
	for(pos = 0; pos <=90; pos++){
  		servoshoot.write(pos);
  		delay(1);
  	}
  	delay(500);
  	for(pos = 90; pos >= 0; pos--){
    	servoshoot.write(pos);
  		delay(1);
  	}
  	delay(800);
  
  	// Desliga os motores
  	digitalWrite(motorCW1, 0);
	digitalWrite(motorCW2, 0);
  	digitalWrite(motorCCW1,0);
	digitalWrite(motorCCW2,0);
  
  	// Diminui uma munição no pente
  	municao--;
  	if (municao==0){
		sem_municao(); // alerta sem munição
    }
}

void sem_municao(){
  while(municao==0){
  	apito_sem_municao(); // apito repetitivo
  	if (digitalRead(Shootbutton)==LOW){municao=8;} // recarrega
  	delay(1000);
  }
}

// Verificação do Sensor Ultrassônico
int lerSonar(){   
   digitalWrite(trigPin,LOW); // não envia som
   delayMicroseconds(2);
   digitalWrite(trigPin,HIGH); // envia pulso
   delayMicroseconds(10);
   digitalWrite(trigPin,LOW); // aguarda retorno
   duracao = pulseIn(echoPin,HIGH); // tempo do eco
   distancia_cm = duracao/57; // converte para cm
   delay(30); 
   return distancia_cm; // retorna distância
}

// Apito do limite dos servos
void apito(){  
    tone(buzzer, 1500);
    delay(200); 
    noTone(buzzer);
  	delay(50);
}

// Apito alvo detectado
void apito_alvo_encontrado(){
	for(int i=0;i<2;i++){
      apito(); 
      delay(50);
    }
}

// Apito sem munição
void apito_sem_municao(){
	for(int i=0;i<4;i++){
      apito(); 
      delay(250);
    }
}

// Função troca o modo
void pressionar_botao_modo(){
  	contador_modo++;
    if(contador_modo==3){contador_modo=1;} // volta para o 1º modo
    delay(100);
}
