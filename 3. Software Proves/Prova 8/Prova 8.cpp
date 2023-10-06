// Treball de Recerca: Elaboració d'un Dron | Alumne: Fabio Mora Melero; Tutor: Albert Esteve | Institut Josep Lluís Sert 2022-2023


// Importo la llibreria <Arduino.h> perquè estic utilitzant platformIO IDE amb VSCode, en altres IDEs casos no cal incloure'l (e.g. Arduino IDE)
#include <Arduino.h>

// Importo una llibreria que em facilitarà la utilització del mòdul GPS
#include <SoftwareSerial.h>


// Estableixo uns pins de la Nano per a realitzar la comunicació UART, el 2 i 3 respectivament com RX i TX des del punt de vista de la Nano
int RXPin = 2; 
int TXPin = 3;

// Creo una instància per a facilitar-me la utilització del mòdul GPS
SoftwareSerial gpsSerial(RXPin, TXPin); // RX i TX respecte a l'Arduino Nano


void setup() {
  Serial.begin(9600); // Inicialitzo la comunicació a 9600 baudrate perquè és la velocitat adequada en aquest cas

  gpsSerial.begin(9600); // El mòdul GPS es comunica predeterminadament a 9600 bauds
}


void loop() {
  // Sempre que hi hagi nova informació es mostrarà en el monitor sèrie
  while (gpsSerial.available())
    Serial.write(gpsSerial.read());
}