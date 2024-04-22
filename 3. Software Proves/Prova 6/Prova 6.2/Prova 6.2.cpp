// Importo la llibreria <Arduino.h> perquè estic utilitzant platformIO IDE amb VSCode, en altres IDEs casos no cal incloure'l (e.g. Arduino IDE)
#include <Arduino.h>

// Importo una llibreria que em facilitarà la utilització del mòdul BMP280
#include <Adafruit_BMP280.h>


// Cal canviar el valor de la següent definició en la llibreria Adafruit_BMP280. Descomentant aquesta no el redefinirà perquè la llibreria ja haurà sigut compilada.
// #define BMP280_ADDRESS 0x76

// Creo una instància per a facilitar-me l'ús del BMP280
Adafruit_BMP280 bmp;

// Declaro una funció per a emmagatzemar l'alçada a què s'ha inicialitzat el mòdul, és important que sigui float
float puntInicial;


void setup() {
  Serial.begin(9600); // Inicialitzo la comunicació a 9600 baudrate perquè és la velocitat adequada en aquest cas
  Serial.println(F("Prova BMP280"));

  // Depenent de si s'ha pogut inicialitzar o no el mòdul, mostro uns missatges i paro l'execució del codi fent-lo entrar en un bucle
  if (!bmp.begin(BMP280_ADDRESS)) {
    Serial.println(F("No s'ha pogut trobar un sensor BMP280 vàlid, comprova les connexions o prova amb una altra adreça!"));
    Serial.print("L'adreça es: 0x"); Serial.println(bmp.sensorID(),16);
    Serial.print("  Una adreça 0xFF probablement representa una adreça invàlida, d'un BMP 180 o d'un BMP 085\n");
    Serial.print("  Una adreça 0x56-0x58 representa una adreça d'un BMP 280,\n");
    Serial.print("  Una adreça 0x60 representa una adreça d'un BME 280.\n");
    Serial.print("  Una adreça 0x61 representa una adreça d'un BME 680.\n");
    while(1); // El codi no pot seguir, s'executa el bucle constantment
  }

  // Configuro el BMP280 intentant conservar un bon temps de resposta
  bmp.setSampling(
    Adafruit_BMP280::MODE_NORMAL,     // "Operating Mode"
    Adafruit_BMP280::SAMPLING_X2,     // "Temp. oversampling"
    Adafruit_BMP280::SAMPLING_X16,    // "Pressure oversampling"
    Adafruit_BMP280::FILTER_X16,      // "Filtering."
    Adafruit_BMP280::STANDBY_MS_500   // "Standby time."
  ); 

  puntInicial = bmp.readAltitude(1013.25); // Emmagatzemo l'alçada inicial, en aquest cas no cal especificar la pressió actual
}


void loop() {
  Serial.print(F("Altitud respecte el punt d'incialització:"));
  Serial.print(bmp.readAltitude(1013.25) - puntInicial);  // Obtinc la diferència d'alçades, en aquest cas no cal especificar la pressió actual
  Serial.println(" m");
  delay(1000);
}