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

// Defineixo el pin amb el qual controlaré l'estat del LED
#define LEDPin 7 


void setup() {
  // Inicialitzo el mòdul i l'estableixo com a receptor
  radio.begin(); // Inicialitzo el mòdul
  radio.openReadingPipe(0, address); // Proporciono l'adreça amb què les dades han d'estar identificades per a que el receptor les processi
  radio.startListening(); // Estableixo el mòdul com a receptor

  // Estableixo el pin que controla l'estat del LED com output
  pinMode(LEDPin, OUTPUT);
}


void loop() {
  if (radio.available()) { // Comprovo si han arribat dades amb l'adreça especificada
    char text[32] = {0}; 
    radio.read(&text, sizeof(text)); // Emmagatzemo les dades rebudes

    // Comprovo si hi ha alguna diferència entre el text rebut i el requerit per encendre o apagar el LED
    if (strcmp(text, "PULSANDO") == 0) {
      digitalWrite(LEDPin, HIGH);
    } else if (strcmp(text, "NO PULSANDO") == 0) {
      digitalWrite(LEDPin, LOW);
    }
  }
}