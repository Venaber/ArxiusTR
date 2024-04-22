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

// Declaro el pin que comprova l'estat del polsador
#define pulsador 6


void setup() {
  // Inicialitzo el mòdul i l'estableixo com a transmissor
  radio.begin(); // Inicialitzo el mòdul
  radio.openWritingPipe(address); // Proporciono l'adreça amb la qual les dades seran identificades
  radio.stopListening(); // Estableixo el mòdul com a transmissor

  // Estableixo el pin que comprova l'estat del polsador com input
  pinMode(pulsador, INPUT);
}


void loop() {
  // Comprovo l'estat del polsador i depenent d'aquest envio un missatge de text específic
  if (digitalRead(pulsador) == HIGH) {
    const char text[] = "PULSANDO"; // Creo un array amb el text a enviar sense especificar la llargada d'aquesta
    radio.write(&text, sizeof(text)); // Envio el missatge
  } else if (digitalRead(pulsador) == LOW) { // Utilitzo l'else if com a decisió personal però és una mala pràctica perquè pels humans és redundant, poc intuïtiu i confós
    const char text[] = "NO PULSANDO"; // Creo un array amb el text a enviar sense especificar la llargada d'aquesta
    radio.write(&text, sizeof(text)); // Envio el missatge
  }
}