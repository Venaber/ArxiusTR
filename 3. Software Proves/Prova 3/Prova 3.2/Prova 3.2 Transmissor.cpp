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

// Defineixo el pin a què està connectat el potenciòmetre y del joystick
#define VRy A3


void setup() {
  // Inicialitzo el mòdul i l'estableixo com a transmissor
  radio.begin(); // Inicialitzo el mòdul
  radio.openWritingPipe(address); // Proporciono l'adreça amb la qual les dades seran identificades
  radio.stopListening(); // Estableixo el mòdul com a transmissor

  // Estableixo el pin que controla la lluminositat del LED com input
  pinMode(VRy, INPUT);
}


void loop() {
  // Llegeixo el valor actual del potenciòmetre i modifico el rang del valor
  int valorY = analogRead(VRy);
  int valorYMapped = map(valorY, 0, 1023, 0, 255); // Comença en un rang 0 - 1023 perquè és un valor analògic
 
  // Estableixo el valor mapejat en una nova variable i l'envio. No cal que creí una nova variable, però personalment opto per fer-ho
  const int text = valorYMapped;
  radio.write(&text, sizeof(text)); // Envio el missatge
}