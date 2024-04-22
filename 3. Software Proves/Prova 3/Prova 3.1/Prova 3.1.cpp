// Importo la llibreria <Arduino.h> perquè estic utilitzant platformIO IDE amb VSCode, en altres IDEs casos no cal incloure'l (e.g. Arduino IDE)
#include <Arduino.h>


// Defineixo el pin als que estan connectats cada potenciòmetre del joystick
#define VRx A2
#define VRy A3


void setup() {
  Serial.begin(9600); // Inicialitzo la comunicació a baudrate 9600 perquè és la velocitat adequada en aquest cas
}


void loop() {
  // Llegeixo el valor actual de cada potenciòmetre
  int valorX = analogRead(VRx);
  int valorY = analogRead(VRy);

  // Mostro els valors en el monitor sèrie
  Serial.print("Valor X:");
  Serial.print(valorX);
  Serial.print(" "); // Deixo un espai entre l'anterior i el següent text
  Serial.print("Valor Y:");
  Serial.print(valorY);
  Serial.println(); // Imprimeixo una nova línia
}