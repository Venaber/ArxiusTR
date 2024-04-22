/****  Llibreries  ****/
// Importo la llibreria <Arduino.h> perquè estic utilitzant platformIO IDE amb VSCode, en altres IDEs casos no cal incloure'l (e.g. Arduino IDE)
#include <Arduino.h>

// Importo unes llibreries que em facilitaran la utilització dels mòduls i els ESCs
#include <RF24.h> // Per al NRF24L01
#include <Servo.h> // Per controlar els motors
#include <Wire.h> // Per I2C (BMP280)
#include <Adafruit_BMP280.h> // Per al BMP280


/****  ESCs  ****/
// Cal canviar els valors de les següents definicions en la llibreria Servo. Descomentant aquestes no els redefinirà perquè la llibreria ja haurà sigut compilada
// #define MIN_PULSE_WIDTH 1000
// #define MAX_PULSE_WIDTH 1887
// #define DEFAULT_PULSE_WIDTH 1000
// #define REFRESH_INTERVAL 6000

// Creo una instància de la classe Servo per cada ESC per facilitar el control d'aquests
Servo ESC1, ESC2, ESC3, ESC4;

// Variables utilitzades més endavant
int ESC1Pulse, ESC2Pulse, ESC3Pulse, ESC4Pulse;

// Defineixo els pins connectats amb cada ESC
#define Motor1 3
#define Motor2 6
#define Motor3 9
#define Motor4 10

// Declaro un array que contindrà els polsos dels ESCs per establir la velocitat dels motors
int velocitats[4]; 

unsigned long temps2;


/****  NRF24L01  ****/
// Primerament realitzo unes accions per establir les bases amb les quals seré capaç de beneficiar-me completament de la llibreria RF24
// Creo una instància de la classe RF24 per fer servir les funcions d'aquesta classe i utilitzar el mòdul NRF24L01 amb bastant facilitat
RF24 radio(7, 8); // CE, CSN

// Estableixo una adreça que serà inclosa pel transmissor en les dades i amb la qual el receptor identificarà les dades i decidirà si processar-les o ignorar-les
const byte address[6] = "12345"; // Ha de ser un array de 5 bytes


/****  Monitor Sèrie  ****/
// Defineixo o no una variable per mostrar dades en el monitor sèrie
// #define monitorSerie true


/****  User LED  ****/
#define userLED 2 // Pin del LED blau

// Si la variable userLED és definida creo unes variables i una funció
#ifdef userLED
  // Variables per a fer parpellejar el LED utilitzant millis() i no delay()
  bool userLEDEstat = false;

  unsigned long temps; 

  int interval = 100;
  unsigned long counter = interval;

  // Funció per a fer parpellejar el LED
  void userLEDOnOff() {
    if (userLEDEstat == false) {
      digitalWrite(userLED, HIGH);
      userLEDEstat = !userLEDEstat;
    } else if (userLEDEstat == true) {
      digitalWrite(userLED, LOW);
      userLEDEstat = !userLEDEstat;
    }
  }
#endif


/****  BMP280  ****/
// Cal canviar el valor de la següent definició en la llibreria Adafruit_BMP280. Descomentant aquesta no el redefinirà perquè la llibreria ja haurà sigut compilada
// #define BMP280_ADDRESS 0x76

Adafruit_BMP280 bmp; // Instància per a facilitar l'ús del BMP280
float ackBMP280; // Variable per emmagatzemar l'alçada, és important que sigui float
float puntInicial;


void setup() {
  /****  Monitor Sèrie  ****/
  // Si la variable monitorSerie és definida inicialitzo el monitor sèrie
  #ifdef monitorSerie
  Serial.begin(115200); // La velocitat adequada en aquest cas és 115200 bauds o més
  #endif


  /****  NRF24L01 + User LED  ****/
  // Depenent si el NRF24L01 s'ha pogut inicialitzar executo una seqüència determinada d'il·luminació del LED blau
  if (!radio.begin()) {
    #ifdef monitorSerie
    Serial.println(F("NRF24L01 No Inicialitzat"));
    #endif

    while (1) {
      #ifdef userLED
        if (millis() >= counter) {
          counter += interval;

          userLEDOnOff();
        }
      #endif
    }  
  } else if (radio.begin()) {
    #ifdef monitorSerie
    Serial.println("NRF24L01 Inicialitzat");
    #endif

    #ifdef userLED
      for(int i = 0; i < 4; i++) {
        temps = millis(); 
        while(temps + 500 >= millis()) {
          digitalWrite(userLED, HIGH);
        }

        temps = millis(); 
        while(temps + 500 >= millis()) {
          digitalWrite(userLED, LOW);
        }
      }
    #endif
  }

  // Segueixo la inicialització del NRF24L01 i l'inicialitzo com a receptor
  radio.setPALevel(RF24_PA_MIN);
  radio.openReadingPipe(1, address);
  radio.enableAckPayload(); // Habilito l'ús de l'Acknowledge Payload amb missatges customizables per enviar les dades del BMP280
  radio.startListening();


  /****  BMP280  ****/
  // Si el BMP280 no s'ha pogut inicialitzar executo una seqüència d'il·luminació del LED blau
  if(!bmp.begin(BMP280_ADDRESS)) {
    #ifdef monitorSerie
    Serial.println(F("BMP280 No Inicialitzat"));
    #endif

    while (1) {
      #ifdef userLED
        if (millis() >= counter) {
          counter += interval;

          userLEDOnOff();
        }
      #endif
    }  
  }

  // Configuro el BMP280 intentant conservar un bon temps de resposta
  bmp.setSampling(
    Adafruit_BMP280::MODE_NORMAL,   // "Operating Mode"
    Adafruit_BMP280::SAMPLING_X1,   // "Temp. oversampling"
    Adafruit_BMP280::SAMPLING_X4,   // "Pressure oversampling"
    Adafruit_BMP280::FILTER_X4,     // "Filtering"
    Adafruit_BMP280::STANDBY_MS_500 // "Standby time"
  );

  puntInicial = bmp.readAltitude(1013.25); // Emmagatzemo l'alçada on s'ha inicialitzat el BMP280, en aquest cas no cal especificar la pressió actual al nivell del mar

  
  /****  ESCs  ****/
  // Els valors prèviament esmentats han d'estar canviats en la llibreria Servo, igualment, per seguretat especifico velocitat 0 en inicialitzar els motors
  ESC1.writeMicroseconds(1000);
  ESC2.writeMicroseconds(1000);
  ESC3.writeMicroseconds(1000);
  ESC4.writeMicroseconds(1000);
  
  // Inicialitzo els ESCs
  ESC1.attach(Motor1, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  ESC2.attach(Motor2, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  ESC3.attach(Motor3, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  ESC4.attach(Motor4, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
}


void loop() {
  /****  NRF24L01  ****/
  if (radio.available()) {
    radio.read(velocitats, sizeof(velocitats)); // Emmagatzemo les dades rebudes


    /****  ESCs  ****/
    // Reassigno les velocitats dels motors
    ESC1Pulse = velocitats[0];
    ESC2Pulse = velocitats[1];
    ESC3Pulse = velocitats[2];
    ESC4Pulse = velocitats[3];

    // Estableixo les velocitats dels motors
    ESC1.writeMicroseconds(ESC1Pulse);
    ESC2.writeMicroseconds(ESC2Pulse);
    ESC3.writeMicroseconds(ESC3Pulse);
    ESC4.writeMicroseconds(ESC4Pulse);


    /****  BMP280  ****/
    // Utilitzo els ACK (Acknowledgement) Payloads per enviar l'alçada i així optimitzar el temps de resposta
    ackBMP280 = bmp.readAltitude(1013.25) - puntInicial; // No cal especificar la pressió actual al nivell del mar, la variació és proporcional
    radio.writeAckPayload(1, &ackBMP280, sizeof(ackBMP280));


    /****  Monitor Sèrie  ****/
    // Si la variable monitorSerie és definida mostro dades en el monitor sèrie
    #ifdef monitorSerie
      Serial.print("Rebut. ");
      Serial.print("ESC 1: "); 
      Serial.print(ESC1Pulse);
      Serial.print(" ESC 2:");
      Serial.print(ESC2Pulse);
      Serial.print(" ESC 3:");
      Serial.print(ESC3Pulse);
      Serial.print(" ESC 4:");
      Serial.println(ESC4Pulse);
    #endif
  } else { // Si no es rep senyal durant més d'un segon els motors es paren
    /****  ESCs  ****/
    temps2 = millis();
    if(temps2 + 1000 > millis()) {
      ESC1.writeMicroseconds(1000);
      ESC2.writeMicroseconds(1000);
      ESC3.writeMicroseconds(1000);
      ESC4.writeMicroseconds(1000);
    }
  }
}