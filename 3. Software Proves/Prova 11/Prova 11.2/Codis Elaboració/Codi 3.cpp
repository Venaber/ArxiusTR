// Treball de Recerca: Elaboració d'un Dron | Alumne: Fabio Mora Melero; Tutor: Albert Esteve | Institut Josep Lluís Sert 2022-2023


// Importo la llibreria <Arduino.h> perquè estic utilitzant platformIO IDE amb VSCode, en altres IDEs casos no cal incloure'l (e.g. Arduino IDE)
#include <Arduino.h>

// Importo una llibreria que em facilitarà la utilització del motor
#include <Servo.h>


// Creo una instància de la classe Servo per a utilitzar el motor A2212
Servo ESC;


void setup() {
  ESC.attach(3, 1000, 2000); // Inicialitzo la configuració de l'ESC

  delay(5000);
}


void loop() {
  for (int i = 1000; i <= 2000; i += 5) {  // Incremento la velocitat fins que arriba a la màxima
    ESC.writeMicroseconds(i);
    delay(200);
  }

  ESC.writeMicroseconds(1000); // Paro els motors en sec

  delay(5000);
}