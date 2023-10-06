// Treball de Recerca: Elaboració d'un Dron | Alumne: Fabio Mora Melero; Tutor: Albert Esteve | Institut Josep Lluís Sert 2022-2023


// Importo la llibreria <Arduino.h> perquè estic utilitzant platformIO IDE amb VSCode, en altres IDEs casos no cal incloure'l (e.g. Arduino IDE)
#include <Arduino.h>

// Importo unes llibreries que em facilitaran la utilització dels mòduls
#include <SPI.h> // Per ambdós sensors
#include <Adafruit_BMP280.h> // Per al BMP280
#include <Adafruit_Sensor.h> // Per al BMP280
#include <Adafruit_GFX.h> // Per a la part de la programació gràfica de la pantalla
#include <Adafruit_ILI9341.h> // Per a la part de la programació gràfica de la pantalla
#include <XPT2046_Touchscreen.h> // Per a la part de la programació tàctil de la pantalla


// Defineixo els pins del BMP280
#define BMPSCL 13
#define BMPMOSI 12
#define BMPMISO 11
#define BMPCSB 4

// Defineixo els pins de la pantalla
#define pantallaCS 10
#define pantallaDC 9
#define pantallaRST 8
#define pantallaT_CS 7 // Per la funcionalitat tàctil


// Creo una instància de la classe Adafruit_BMP280 per a utilitzar el BMP280, i d'Adafruit_ILI9341 i XPT2056_Touchscreen per a utilitzar la pantalla i de manera tàctil
Adafruit_BMP280 BMP280(BMPCSB);
Adafruit_ILI9341 pantallaGraphics(pantallaCS, pantallaDC, pantallaRST);
XPT2046_Touchscreen pantallaTouch(pantallaT_CS);


// Declaro i inicialitzo les dimensions per a crear un rectangle
int dimensionsX = 200;
int dimensionsY = 50;

// Declaro però no inicialitzo una variable per emmagatzemar si s'ha mostrat un missatge o no
bool mostrat;

// Declaro unes variables per utilitzar el millis() en comptes de delay()
unsigned long previousMillis = 0;
const long interval = 2000;

// Declaro unes variables que emmagatzemaran valors del sensor BMP280
float pressio; // Pascals (Pa)
float temperatura; // Graus Celsius (ºC)
int alcada; // Metres (m)

// Creo una funció per esborrar text de la pantalla fàcilment, cal que declari les variables en el global scope per accedir a elles posteriorment
int16_t x1, y1; // Coordenades de la vora a dalt a l'esquerra
uint16_t amText, alText; // Mesures del text en píxels

void mostrarNouText(int x, int y, const char* text, int color = ILI9341_BLACK) {
  pantallaGraphics.getTextBounds(text, x, y, &x1, &y1, &amText, &alText);
  pantallaGraphics.fillRect(0, y, amText, alText, color);
  pantallaGraphics.setCursor(x, y); 
  pantallaGraphics.print(text);
}


void setup() {
  // Inicialitzo els pins per a utilitzar la pantalla
  pinMode(pantallaCS, OUTPUT);
  pinMode(pantallaT_CS, OUTPUT);
  digitalWrite(pantallaCS, HIGH);
  digitalWrite(pantallaT_CS, HIGH);

  // Inicialitzo la pantalla i estableixo l'orientació
  pantallaGraphics.begin();
  pantallaGraphics.setRotation(1); // 0: Orientació Predeterminada, 1: Gir 90º Horari, 2: Gir 180º, 3: Gir 90º Antihorari

  // Estableixo el color de fons i mostro un text en color vermell
  pantallaGraphics.fillScreen(ILI9341_BLACK);
  pantallaGraphics.setTextSize(2);
  
  pantallaGraphics.setTextColor(ILI9341_RED);
  pantallaGraphics.setCursor(0, 10);
  pantallaGraphics.print("TR Test");

  // Mostro una seqüència de missatges indicant l'estat d'inicialització del BMP280
  pantallaGraphics.setTextColor(ILI9341_WHITE);
  pantallaGraphics.setCursor(0, 40);
  const char* text = "Inicialitzant BMP280...";
  pantallaGraphics.print(text);

  while (millis() < 4000); // Dono 4 segons per a llegir el missatge

  // Inicialitzo el BMP280 i depenent del resultat de la inicialització el codi segueix o no
  if (!BMP280.begin()) { 
    mostrarNouText(0, 40, "No s'ha pogut inicialitzar el BMP280.");
    while(1);
  } else if (BMP280.begin())  {
    mostrarNouText(0, 40, "BMP280 inicialitzat Correctament.");
  }
  
  // M'espero 8 segons i esborro el text anterior
  while (millis() < 8000);
  mostrarNouText(0, 40, "                                ");

  // Creo un rectangle que em servirà per mostrar informació
  pantallaGraphics.fillRect(pantallaGraphics.width()/2 - dimensionsX/2, pantallaGraphics.height()/2 - dimensionsY/2, dimensionsX, dimensionsY, ILI9341_GREEN);
  pantallaGraphics.setCursor(pantallaGraphics.width()/2 - (16*6*2)/2, pantallaGraphics.height()/2 - (7*2)/2);
  pantallaGraphics.print("Prem la pantalla");

  pantallaGraphics.setTextSize(1); // Canvio la mesura per al pròxim nou text
}


void loop() {
  unsigned long currentMillis = millis(); // Per utilitzar el millis() en comptes de delay()
  
  // Permeto que la pantalla sigui clicat cada 2 segons, un cop clicat es mostra un missatge per 2 segons i s'esborra automàticament, encara que es cliqui durant aquests dos segons, el missatge no s'actualitzarà
  if (currentMillis - previousMillis >= interval && pantallaTouch.getPoint().z > 100) { 
    previousMillis = currentMillis;
    pantallaGraphics.fillRect(100, 10, 330, 60, ILI9341_BLACK);
    pressio = BMP280.readPressure();
	  temperatura = BMP280.readTemperature();

	  alcada = BMP280.readAltitude(1017.27); // Cal especificar la previsió local de pressió atmosfèrica respecte al nivell del mar. Es pot utilitzar: https://barometricpressure.app/

    // Mostro els valors del BMP280 en la pantalla
    pantallaGraphics.setCursor(100, 10);
    pantallaGraphics.print("P:");
    pantallaGraphics.setCursor(120, 10);
	  pantallaGraphics.print(pressio);
    pantallaGraphics.setCursor(170, 10);
	  pantallaGraphics.print(" Pa");
    
    pantallaGraphics.setCursor(100, 30);
    pantallaGraphics.print("T:");
    pantallaGraphics.setCursor(120, 30);
	  pantallaGraphics.print(temperatura);
    pantallaGraphics.setCursor(146, 30);
	  pantallaGraphics.print(" C");

    pantallaGraphics.setCursor(100, 50);
    pantallaGraphics.print("A:");
    pantallaGraphics.setCursor(120, 50);
	  pantallaGraphics.print(alcada);
    pantallaGraphics.setCursor(128, 50);
	  pantallaGraphics.print(" m");


    mostrat = true; // Amb true represento que el missatge ha sigut mostrat
  } else if (currentMillis - previousMillis >= 2000 && mostrat == true) { // Després de dos segons s'eborra el missatge de la pantalla
    pantallaGraphics.fillRect(100, 10, 330, 60, ILI9341_BLACK);
    
    mostrat = false; // Represento amb true que el missatge ha sigut ocultat
  }
}