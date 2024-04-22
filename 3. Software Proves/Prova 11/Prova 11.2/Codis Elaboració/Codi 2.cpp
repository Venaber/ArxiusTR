// Importo la llibreria <Arduino.h> perquè estic utilitzant platformIO IDE amb VSCode, en altres IDEs casos no cal incloure'l (e.g. Arduino IDE)
#include <Arduino.h>

// Importo una llibreria que em facilitarà la utilització del motor
#include <Servo.h>


// Defineixo el pin al qual està connectat el motor
#define PWMPin 3

// Creo una instància de la classe Servo per a utilitzar el motor A2212
Servo ESC;


void setup() {
  ESC.attach(PWMPin, 1000, 2000); // Inicialitzo la configuració de l'ESC
  ESC.writeMicroseconds(1000);
   
  delay(10000);
}


void loop() {
  ESC.writeMicroseconds(map(analogRead(A0), 0, 1023, 1000, 2000)); // Vario la velocitat a través del joystick
}