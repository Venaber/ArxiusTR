// Importo la llibreria <Arduino.h> perquè estic utilitzant platformIO IDE amb VSCode, en altres IDEs casos no cal incloure'l (e.g. Arduino IDE)
#include <Arduino.h>

// Importo una llibreria que em facilitarà la utilització del HC-06 i la d'ArduinoBlue
#include <SoftwareSerial.h>
#include <ArduinoBlue.h>


// Creo una instància per a facilitar-me la utilització del mòdul HC-06
SoftwareSerial digitalSerial(6, 7); // RX i TX respecte a l'Arduino Nano
ArduinoBlue phone(digitalSerial); // Necessari per a utilitzar l'aplicació


void setup() {
  // Inicialitzo la comunicació amb el monitor sèrie i amb el mòdul HC-06
  Serial.begin(9600); // Inicialitzo la comunicació a 9600 baudrate perquè és la velocitat adequada en aquest cas

  digitalSerial.begin(9600); // Em comunicaré amb el HC-06 a 9600 bauds

  Serial.println("Llest per Operar"); // Mostro un missatge en el monitor sèrie

  phone.sendMessage("Bluetooth Connectat");
}


void loop() {
  // Intentava obtenir dades fent ús de la llibreria ArduinoBlue i amb l'aplicació però no ho vaig aconseguir, no descarto problemes tècnics o de programació
  Serial.print("Global Read: ");
  Serial.println(digitalSerial.read());

  Serial.print("Button: ");
  Serial.println(phone.getButton());

  Serial.print("Slider: ");
  Serial.println(phone.getSliderVal());

  Serial.print("Text: ");
  Serial.println(phone.getText());

  phone.sendMessage("Bluetooth Connectat - Test15"); // Envio un missatge al mòbil que era l'única cosa que em funcionava
  

  delay(3000);
}