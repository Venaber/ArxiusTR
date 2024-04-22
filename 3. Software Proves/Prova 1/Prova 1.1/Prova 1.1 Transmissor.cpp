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


void setup() {
  // Inicialitzo el mòdul i l'estableixo com a transmissor
  radio.begin(); // Inicialitzo el mòdul
  radio.openWritingPipe(address); // Proporciono l'adreça amb la qual les dades seran identificades
  radio.stopListening(); // Estableixo el mòdul com a transmissor
}


void loop() {
  // Especifico el missatge i l'envio
  const char text[] = "Hello World"; // Creo un array sense especificar la llargada d'aquesta entre els claudàtors ([]) perquè així tindrà la llargada del missatge a transmetre més el null terminator
  radio.write(&text, sizeof(text)); // Envio el missatge
  
  delay(1000); // M'espero 1 segon abans de tornar a enviar el mateix missatge
}