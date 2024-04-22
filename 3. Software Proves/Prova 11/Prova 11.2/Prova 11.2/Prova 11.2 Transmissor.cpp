// Importo la llibreria <Arduino.h> perquè estic utilitzant platformIO IDE amb VSCode, en altres IDEs casos no cal incloure'l (e.g. Arduino IDE)
#include <Arduino.h>

// Importo una llibreria que em facilitarà la utilització del mòdul NRF24L01
#include <RF24.h>


// Primerament realitzo unes accions per establir les bases amb les quals seré capaç de beneficiar-me completament de la llibreria RF24
// Creo una instància de la classe RF24 per fer servir les funcions d'aquesta classe i utilitzar el mòdul NRF24L01 amb bastant facilitat
RF24 radio(7, 8); // He d'especificar els pins CE i CSN respectivament

// Estableixo una adreça que serà inclosa pel transmissor en les dades i amb la qual el receptor identificarà les dades i decidirà si processar-les o ignorar-les
const byte address[6] = "12345"; // L'adreça ha de ser un array de 5 bytes, però cal especificar que sigui de 6 elements o no especificar la llargada perquè automàticament cpp inclou un null terminator

// Defineixo els pins del joysticks
#define VRx1 4 // Controla el Scroll
#define VRy1 5 // Controla el Pitch
#define VRx2 2 // Controla el Yaw
#define VRy2 3 // Controla el Throttle


void setup() {
  // Inicialitzo el mòdul i l'estableixo com a transmissor
  radio.begin(); // Inicialitzo el mòdul
  radio.openWritingPipe(address); // Proporciono l'adreça amb la qual les dades seran identificades
  radio.setPALevel(RF24_PA_MIN); // És necessari que el posi en MIN perquè si no l'Arduino no pot proporcionar suficient corrent i no funcionarà
  radio.stopListening(); // Estableixo el mòdul com a transmissor

  // Estableixo els pins connectats amb els joysticks com INPUT
  pinMode(VRx1, INPUT);
  pinMode(VRy2, INPUT);
  pinMode(VRx2, INPUT);
  pinMode(VRy2, INPUT);
}

void loop() { 
  // Obtinc el valor dels potenciòmetres actual de cada joystick
  int valorX1 = analogRead(VRx1); // valorX del joystick a l'esquerra
  int valorY1 = analogRead(VRy1); // valorY del joystick a l'esquerra
  int valorX2 = analogRead(VRx2); // valorX del joystick a la dreta
  int valorY2 = analogRead(VRy2); // valorY del joystick a la dreta
  
  // Obtinc els rangs adequats
  int throttle = map(valorY1, 0, 1023, 1500, 1000); // L'adequat seria el rang 1000-2000, però de moment per a comprovacions utilitzo 1500
  int yaw = map(valorX1, 0, 1023, -100, 100); 
  int roll = map(valorX2, 0, 1023, -100, 100);
  int pitch = map(valorY2, 0, 1023, 100, -100);


  // Obtinc les velocitats de cada motor amb el Motor Mixing Algorithm. Més informació: http://www.rcindia.org/multirotors/arduino-quadcopter/new/#new
  int ESC1 = throttle + pitch + roll + yaw; // Motor a dalt a l'esquerra (CCW)
  int ESC2 = throttle + pitch - roll - yaw; // Motor a dalt a la dreta (CW)
  int ESC3 = throttle - pitch + roll - yaw; // Motor a baix a l'esquerra (CCW)
  int ESC4 = throttle - pitch - roll + yaw; // Motor a baix a la dreta (CW)

  int velocitats[4] = {ESC1, ESC2, ESC3, ESC4}; // Creo un paquet de dades
  radio.write(velocitats, sizeof(velocitats)); // Envio les dades
}