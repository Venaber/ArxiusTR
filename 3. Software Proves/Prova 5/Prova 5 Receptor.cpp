// Importo la llibreria <Arduino.h> perquè estic utilitzant platformIO IDE amb VSCode, en altres IDEs casos no cal incloure'l (e.g. Arduino IDE)
#include <Arduino.h>

// Importo unes llibreries que em facilitaran la utilització del mòdul NRF24L01. No és necessari incloure les llibreries SPI i nRF24L01 perquè ja ho fa la RF24 però no ho sabia
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


// Primerament realitzo unes accions per establir les bases amb les quals seré capaç de beneficiar-me completament de la llibreria RF24
// Creo una instància de la classe RF24 per fer servir les funcions d'aquesta classe i utilitzar el mòdul NRF24L01 amb bastant facilitat
RF24 radio(3, 4); // He d'especificar els pins CE i CSN respectivament

// Estableixo una adreça que serà inclosa pel transmissor en les dades i amb la qual el receptor identificarà les dades i decidirà si processar-les o ignorar-les
const byte address[6] = "12345"; // L'adreça ha de ser un array de 5 bytes, però cal especificar que sigui de 6 elements o no especificar la llargada perquè automàticament cpp inclou un null terminator

// Defineixo els pins connectats amb el L298N
#define ENA 6
#define IN1 7
#define IN2 8


void setup() {
  // Inicialitzo el mòdul NRF24L01 i l'estableixo com a receptor
  radio.begin(); // Inicialitzo el mòdul
  radio.openReadingPipe(0, address); // Proporciono l'adreça amb què les dades han d'estar identificades per a que el receptor les processi
  radio.startListening(); // Estableixo el mòdul com a receptor

  // Declaro els pins connectats amb el L298N com output
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
}


void loop() {
  if (radio.available()) {
    int radioMessage = 0;
    radio.read(&radioMessage, sizeof(radioMessage)); // Emmagatzemo les dades rebudes
    
    // Estableixo el sentit de gir del motor
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);

    // Estableixo la velocitat del motor
    analogWrite(ENA, int(radioMessage));
  }
}