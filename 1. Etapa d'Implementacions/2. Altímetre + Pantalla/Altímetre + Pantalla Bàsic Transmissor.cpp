/****  Llibreries  ****/
// Importo la llibreria <Arduino.h> perquè estic utilitzant platformIO IDE amb VSCode, en altres IDEs casos no cal incloure'l (e.g. Arduino IDE)
#include <Arduino.h>

// Importo unes llibreries que em facilitaran la utilització dels mòduls i els ESCs
#include <RF24.h>
#include <Adafruit_GFX.h> // Per a la 3.2'' TFT LCD Touch Screen
#include <Adafruit_ILI9341.h> // Per a la 3.2'' TFT LCD Touch Screen
#include <XPT2046_Touchscreen.h> // Per a la 3.2'' TFT LCD Touch Screen


/****  Joysticks i Polsos ESCs  ****/
// Defineixo els pins connectats amb cada ESC
#define VRy1 4 // Control Yaw
#define VRx1 5 // Control Throttle
#define VRy2 2 // Control Roll
#define VRx2 3 // Control Pitch

// Declaro unes arrays que contindran el valor dels joysticks i les velocitats dels motors designats per l'amplada del pols
int josyticks[4], velocitats[4];

// Declaro variables pels moviments
int yaw, throttle, roll, pitch;


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

  int interval2 = 2000;
  unsigned long counter2 = interval2;

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
float ackBMP280; // Variable per emmagatzemar l'alçada, és important que sigui float


/****  3.2'' TFT LCD Touch Screen   ****/
unsigned long temps2;

#define pantallaCS 10
#define pantallaDC 9
#define pantallaRST 6
#define pantallaT_CS 5 // Per la funcionalitat tàctil

// Instància de la classe Adafruit_ILI9341 i XPT2056_Touchscreen per a utilitzar la pantalla i de manera tàctil
Adafruit_ILI9341 pantallaGraphics(pantallaCS, pantallaDC, pantallaRST);
XPT2046_Touchscreen pantallaTouch(pantallaT_CS); 

// Creo unes variables per a modificar la part gràfica de la pantalla
int dimensionsX = 200;
int dimensionsY = 50;

bool mostrat;


// Declaro unes variables per utilitzar el millis() en comptes del delay()
const long intervalPantalla = 2000;
unsigned long counterPantalla = 0; 
unsigned long tempsPantalla;

// Creo una funció per a mostrar dades de la pantalla i tenir el codi organitzat
void pantalla() {
  tempsPantalla = millis();

  if(tempsPantalla - counterPantalla >= intervalPantalla && pantallaTouch.getPoint().z > 100) {
    counterPantalla = tempsPantalla;
    pantallaGraphics.fillRect(100, 40, 330, 30, ILI9341_BLACK);

    pantallaGraphics.setCursor(100, 50);
    pantallaGraphics.print("A:");
    pantallaGraphics.setCursor(135, 50);
    pantallaGraphics.print(ackBMP280);
    pantallaGraphics.setCursor(190, 50);
    pantallaGraphics.print(" m");

    mostrat = true;
  } else if (tempsPantalla - counterPantalla >= 2000 && mostrat == true) { // S'esborra el missatge dos segons després d'haver sigut mostrat
    pantallaGraphics.fillRect(100, 40, 330, 30, ILI9341_BLACK);
    
    mostrat = false;
  }
}


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

  // Segueixo la inicialització del NRF24L01 i l'inicialitzo com a transmissor
  radio.setPALevel(RF24_PA_MIN);
  radio.openWritingPipe(address);
  radio.enableAckPayload(); // Habilito l'ús de l'Acknowledge Payload amb missatges customitzables
  radio.stopListening();
    
  
  /****  Joysticks  ****/
  pinMode(VRx1, INPUT);
  pinMode(VRy1, INPUT);
  pinMode(VRx2, INPUT);
  pinMode(VRy2, INPUT);


  /****  3.2'' TFT LCD Touch Screen  ****/
  // Inicialitzo la pantalla 3.2'' TFT LCD Touch Screen
  pinMode(pantallaCS, OUTPUT);
  pinMode(pantallaT_CS, OUTPUT);
  digitalWrite(pantallaCS, HIGH);
  digitalWrite(pantallaT_CS, HIGH);

  pantallaGraphics.begin();
  pantallaGraphics.setRotation(1); // 0: Orientació Predeterminada, 1: Gir 90º Horari, 2: Gir 180º, 3: Gir 90º Antihorari

  // Mostro un títol
  pantallaGraphics.fillScreen(ILI9341_BLACK);
  pantallaGraphics.setTextSize(2);
  pantallaGraphics.setTextColor(ILI9341_RED);
  pantallaGraphics.setCursor(0, 10);
  pantallaGraphics.print("TR Pantalla");
  
  pantallaGraphics.setTextColor(ILI9341_WHITE);
  pantallaGraphics.setCursor(pantallaGraphics.width()/2 - (16*6*2)/2, pantallaGraphics.height()/2 - (7*2)/2);
  pantallaGraphics.print("TOT INICIALITZAT");

  temps2 = millis();
  while(temps2 + 4000 >= millis()); // Mostro el missatge durant 4 segons

  // Creo un rectangle que em servirà per mostrar informació
  pantallaGraphics.fillRect(pantallaGraphics.width()/2 - dimensionsX/2, pantallaGraphics.height()/2 - dimensionsY/2, dimensionsX, dimensionsY, ILI9341_GREEN);
  pantallaGraphics.setCursor(pantallaGraphics.width()/2 - (16*6*2)/2, pantallaGraphics.height()/2 - (7*2)/2);
  pantallaGraphics.print("Prem la pantalla");
  

  /****  User LED  ****/
  // Mostro informació a través de l'User LED
  #ifdef userLED
    temps = millis(); 
    while(temps + 500 >= millis());

    temps = millis(); 
    while(temps + 2000 >= millis()) {
      digitalWrite(userLED, HIGH);
    }

    digitalWrite(userLED, LOW);
  #endif
}


void loop() {
  /****  Joystick i Polsos ESCs  ****/
  // Llegeixo els valors dels potenciòmetres dels joysticks
  josyticks[0] = analogRead(VRx1); // valorX del joystick a l'esquerra. valorX1, yaw
  josyticks[1] = analogRead(VRy1); // valorY del joystick a l'esquerra. valorY1, throttle
  josyticks[2] = analogRead(VRx2); // valorX del joystick a la dreta. valorX2, roll
  josyticks[3] = analogRead(VRy2); // valorY del joystick a la dreta. valorY2, pitch

  // Canvio el rang dels valors a l'adequat per als ESCs
  yaw = map(josyticks[0], 0, 1023, -100, 100); 
  throttle = map(josyticks[1], 0, 1023, 1887, 900); 
  roll = map(josyticks[2], 0, 1023, -100, 100);
  pitch = map(josyticks[3], 0, 1023, 100, -100); 

  // Motor Mixing Algorithm. Més informació: http://www.rcindia.org/multirotors/arduino-quadcopter/new/#new
  velocitats[0] = throttle + pitch + roll + yaw; // Motor a dalt a l'esquerra (CCW) (Motor 1)
  velocitats[1] = throttle + pitch - roll - yaw; // Motor a dalt a la dreta (CW) (Motor 2)
  velocitats[2] = throttle - pitch + roll - yaw; // Motor a baix a l'esquerra (CW) (Motor 3)
  velocitats[3] = throttle - pitch - roll + yaw; // Motor a baix a la dreta (CWW) (Motor 4)


  /****  NRF24L01  ****/
  // Envio el missatge i realitzo altres accions
  if(radio.write(velocitats, sizeof(velocitats))) { // Envio el missatge
    if(radio.isAckPayloadAvailable()) { // Comprovo si hi ha algun missatge de resposta del receptor
      radio.read(&ackBMP280, sizeof(ackBMP280));

      /****  3.2'' TFT LCD Touch Screen  ****/
      pantalla(); // Crido a la funció que controla la pantalla
    }
    

    /****  Monitor Sèrie  ****/
    // Si la variable monitorSerie és definida mostro dades en el monitor sèrie
    #ifdef monitorSerie
      Serial.print("Enviat. "); 
      Serial.print(velocitats[0]);
      Serial.print(" ");
      Serial.print(velocitats[1]);
      Serial.print(" ");
      Serial.print(velocitats[2]);
      Serial.print(" ");
      Serial.println(velocitats[3]);
    #endif


    /****  User LED  ****/
    // Si la variable userLED és definida faig parpellejar el LED blau durant 1 segon
    #ifdef userLED // En el vídeo no està activat
      if (millis() >= counter2) {
          counter2 += interval2;

          userLEDOnOff();
      }
    #endif
  }
}