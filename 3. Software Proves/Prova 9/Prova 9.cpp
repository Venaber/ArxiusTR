// Treball de Recerca: Elaboració d'un Dron | Alumne: Fabio Mora Melero; Tutor: Albert Esteve | Institut Josep Lluís Sert 2022-2023


// Importo la llibreria <Arduino.h> perquè estic utilitzant platformIO IDE amb VSCode, en altres IDEs casos no cal incloure'l (e.g. Arduino IDE)
#include <Arduino.h>

// Importo una llibreria que em facilitarà la utilització del HC-06
#include <SoftwareSerial.h>


// Creo una instància per a facilitar-me la utilització del mòdul HC-06
SoftwareSerial digitalSerial(6, 7); // RX i TX respecte a l'Arduino Nano


void setup() {
  Serial.begin(9600); // Inicialitzo la comunicació a 9600 baudrate perquè és la velocitat adequada en aquest cas

  digitalSerial.begin(9600); // Em comunicaré amb el HC-06 a 9600 bauds
}


void loop() {
  if (digitalSerial.available()) { // La informació rebuda en el mòdul Bluetooth s'envia al monitor sèrie
    Serial.write(digitalSerial.read());
    Serial.println();
  }

  if (Serial.available()) { // La informació del monitor sèrie s'envia al mòdul Bluetooth
    digitalSerial.write(Serial.read());
  }
}