// Treball de Recerca: Elaboració d'un Dron | Alumne: Fabio Mora Melero; Tutor: Albert Esteve | Institut Josep Lluís Sert 2022-2023

// Aquest codi el vaig fer a partir del "Receptor 1.cpp" i vaig modificar el que jo pensava que m'estava causant problemes


// Importo la llibreria <Arduino.h> perquè estic utilitzant platformIO IDE amb VSCode, en altres IDEs casos no cal incloure'l (e.g. Arduino IDE)
#include <Arduino.h>


// Després  importo unes altres llibreries
#include <RF24.h> // Per a la comunicació amb els mòduls ràdio
#include <Servo.h> // Per a controlar la velocitat dels motors


// Creo una instància de la classe Servo per cada ESC per a beneficiar-me de la llibreria Servo
Servo ESC1, ESC2, ESC3, ESC4;

// Declaro els pins per a controlar la velocitat dels motors
#define PWM1 3
#define PWM2 6
#define PWM3 9
#define PWM4 10

// Pins per a la comunicació NRF24L01
RF24 radio(7, 8); // Pin CE i Pin CSN

const byte address[6] = "12345"; // L'adreça amb què es comunicaran els mòduls ràdio

// Array que contindrà les velocitats de cada motor per enviar-les
int velocitats[4];


void setup() {
  // Inicialitzo el mòdul NRF24L01 i l'estableixo com a receptor
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX); // Estableix la potència del senyal del NRF24L01, en alguns casos amb MAX funciona, però més endavant decideixo canviar-lo per MIN perquè si no la Nano no pot proporcionar suficient corrent
  radio.startListening(); 


  // Control ESCs, vaig elidir algunes parts respecte al codi "Receptor 1.cpp"
  ESC1.attach(PWM1, 1000, 2000);
  ESC2.attach(PWM2, 1000, 2000);
  ESC3.attach(PWM3, 1000, 2000);
  ESC4.attach(PWM4, 1000, 2000);
  
  while (millis() < 7000) {} // M'espero un temps per a que els motors es puguin inicialitzar correctament
}


void loop() {
  if (radio.available()) {
    radio.read(velocitats, sizeof(velocitats)); // Emmagatzemo les dades rebudes 

    int ESC1Pulse = velocitats[0];
    int ESC2Pulse = velocitats[1];
    int ESC3Pulse = velocitats[2];
    int ESC4Pulse = velocitats[3];

    // Estableixo la velocitat de cada motor
    ESC1.writeMicroseconds(ESC1Pulse);
    ESC2.writeMicroseconds(ESC2Pulse);
    ESC3.writeMicroseconds(ESC3Pulse);
    ESC4.writeMicroseconds(ESC4Pulse);
  } else {
    // En qualsevol cas que no es rebi senyal, els motors han de parar
    ESC1.writeMicroseconds(1000);
    ESC2.writeMicroseconds(1000);
    ESC3.writeMicroseconds(1000);
    ESC4.writeMicroseconds(1000);
  }
}