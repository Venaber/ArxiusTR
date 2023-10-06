// Treball de Recerca: Elaboració d'un Dron | Alumne: Fabio Mora Melero; Tutor: Albert Esteve | Institut Josep Lluís Sert 2022-2023


// Importo la llibreria <Arduino.h> perquè estic utilitzant platformIO IDE amb VSCode, en altres IDEs casos no cal incloure'l (e.g. Arduino IDE)
#include <Arduino.h>

// Importo unes llibreries que em facilitaran la utilització del mòdul BMP280
#include <Wire.h> // Comunicació I2C (és inclós per Adafruit_BMP280)
#include <Adafruit_BMP280.h> // Per a facilitar l'us del BMP280


// Defineixo els pins que utilitzaré en la comunicació I2C
#define SCL 5 // Vaig connectar-lo al D5 però ara el muntatge era diferent
#define SDA 4

// Creo una instància per a facilitar-me l'ús del BMP280
Adafruit_BMP280 sensorBMP280;

// Declaro unes variables per utilitzar millis() en comptes del delay()
int pause = 2000;
unsigned long counter = pause;


void setup() {
  Serial.begin(9600); // Inicialitzo la comunicació a 9600 baudrate perquè és la velocitat adequada en aquest cas

  // Si el BMP280 no s'ha inicialitzat mostro un missatge i evito que el programa avanci
  if (!sensorBMP280.begin()) {
    Serial.print("No s'ha trobat el sensor");
    while(1);
  }
}


// Creo la funció loop on només he de cridar a unes funcions de la llibreria Adafruit_BMP280
void loop() {
  // Mostro les dades en el monitor sèrie cada 2 segons
  if (millis() >= counter) {
    counter += pause;

    Serial.print("T: ");
    Serial.print(sensorBMP280.readTemperature());
    Serial.print("P: ");
    Serial.print(sensorBMP280.readPressure());
    Serial.print("A: ");
    Serial.println(sensorBMP280.readAltitude(1017.12)); // Cal especificar la previsió local de pressió atmosfèrica respecte al nivell del mar. Es pot utilitzar: https://barometricpressure.app/
  }
}