// Treball de Recerca: Elaboració d'un Dron | Alumne: Fabio Mora Melero; Tutor: Albert Esteve | Institut Josep Lluís Sert 2022-2023


// Importo la llibreria <Arduino.h> perquè estic utilitzant platformIO IDE amb VSCode, en altres IDEs casos no cal incloure'l (e.g. Arduino IDE)
#include <Arduino.h>

// Importo unes llibreries que em facilitaran la utilització del mòdul NRF24L01 i dels motors
#include <RF24.h>
#include <Servo.h>


// Creo una instància de la classe Servo per cada ESC
Servo ESC1, ESC2, ESC3, ESC4;

// Defineixo els pins connectats amb cada ESC
#define PWM1 3
#define PWM2 6
#define PWM3 9
#define PWM4 10

// Primerament realitzo unes accions per establir les bases amb les quals seré capaç de beneficiar-me completament de la llibreria RF24
// Creo una instància de la classe RF24 per fer servir les funcions d'aquesta classe i utilitzar el mòdul NRF24L01 amb bastant facilitat
RF24 radio(7, 8); // He d'especificar els pins CE i CSN respectivament

// Estableixo una adreça que serà inclosa pel transmissor en les dades i amb la qual el receptor identificarà les dades i decidirà si processar-les o ignorar-les
const byte address[6] = "12345"; // L'adreça ha de ser un array de 5 bytes, però cal especificar que sigui de 6 elements o no especificar la llargada perquè automàticament cpp inclou un null terminator

// Creo una array que contindrà els valors de les velocitats per als motors
int velocitats[4];


void setup() {
  // Inicialitzo el mòdul i l'estableixo com a receptor
  radio.begin(); // Inicialitzo el mòdul
  radio.openReadingPipe(0, address); // Proporciono l'adreça amb què les dades han d'estar identificades per a que el receptor les processi
  radio.setPALevel(RF24_PA_MIN); // És necessari que el posi en MIN perquè si no l'Arduino no pot proporcionar suficient corrent i no funcionarà
  radio.startListening(); // Estableixo el mòdul com a receptor


  // Estableixo els pins per al senyal com OUTPUT
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);
  pinMode(PWM3, OUTPUT);
  pinMode(PWM4, OUTPUT);

  // Inicialitzo els motors
  ESC1.attach(PWM1, 1000, 2000);
  delay(1000);
  ESC2.attach(PWM2, 1000, 2000);
  delay(1000);
  ESC3.attach(PWM3, 1000, 2000);
  delay(1000);
  ESC4.attach(PWM4, 1000, 2000);
  delay(1000);
  
  
  ESC1.writeMicroseconds(1000);
  ESC2.writeMicroseconds(1000);
  ESC3.writeMicroseconds(1000);
  ESC4.writeMicroseconds(1000);

  delay(5000);
}


void loop() {
  if (radio.available()) { // Comprovo si les dades que han arribat tenen l'adreça especificada
    radio.read(velocitats, sizeof(velocitats)); // Emmagatzemo les dades rebudes

    int ESC1Pulse = velocitats[0];
    int ESC2Pulse = velocitats[1];
    int ESC3Pulse = velocitats[2];
    int ESC4Pulse = velocitats[3];

    // Estableixo les velocitats corresponents a cada motor
    ESC1.writeMicroseconds(ESC1Pulse);
    ESC2.writeMicroseconds(ESC2Pulse);
    ESC3.writeMicroseconds(ESC3Pulse);
    ESC4.writeMicroseconds(ESC4Pulse);
  }
}