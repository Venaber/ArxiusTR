// Treball de Recerca: Elaboració d'un Dron | Alumne: Fabio Mora Melero; Tutor: Albert Esteve | Institut Josep Lluís Sert 2022-2023


// Importo la llibreria <Arduino.h> perquè estic utilitzant platformIO IDE amb VSCode, en altres IDEs casos no cal incloure'l (e.g. Arduino IDE)
#include <Arduino.h>


void setup() {
  // És necessari establir els pins digitals en OUTPUT
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT); // Motor 1
  pinMode(3, OUTPUT); // Motor 3
}


void loop() {
  digitalWrite(4, HIGH); // És el senyal que utilitzo per establir el sentit de gir, el muntatge físic afecta

  for (int i = 0; i < 255; i++) { // Incremento la velocitat de 0 al 255, el rang és el del senyal PWM
    // Genero els senyals PWM
    analogWrite(5, i);
    analogWrite(3, i);
    delay(10); // M'espero 10 mil·lisegons
  }

  for (int i = 255; i > 0; i--) { // Disminueixo la velocitat de 255 a 0, el rang és el del senyal PWM
    // Genero els senyals PWM
    analogWrite(5, i);
    analogWrite(3, i);
    delay(10); // M'espero 10 mil·lisegons
  }  
}