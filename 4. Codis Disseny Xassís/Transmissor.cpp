// En el cas del transmissor, el codi per a l'elaboració no varia


// Importo la llibreria <Arduino.h> perquè estic utilitzant platformIO IDE amb VSCode, en altres IDEs casos no cal incloure'l (e.g. Arduino IDE)
#include <Arduino.h>

// Primer importo una llibreria per a la comunicació amb els mòduls ràdio
#include <RF24.h>


// Pins per a la comunicació NRF24L01
RF24 radio(7, 8); // Pin CE i Pin CSN

const byte address[6] = "12345"; // L'adreça amb què es comunicaran els mòduls ràdio

// Declaro els pins del joystick
#define VRx1 4 // Controla Yaw
#define VRy1 5 // Controla Throttle
#define VRx2 2 // Controla Roll
#define VRy2 3 // Controla Pitch


void setup() {
  // Inicialitzo el mòdul NRF24L01 i l'estableixo com a transmissor
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX); // Estableix la potència del senyal del NRF24L01, en alguns casos amb MAX funciona, però més endavant decideixo canviar-lo per MIN perquè si no la Nano no pot proporcionar suficient corrent
  radio.stopListening();

  pinMode(VRx1, INPUT);
  pinMode(VRy1, INPUT);
  pinMode(VRx2, INPUT);
  pinMode(VRy2, INPUT);  
}


void loop() {
  // Llegeixo els valors dels potenciòmetres dels joysticks
  int valorX1 = analogRead(VRx1); 
  int valorY1 = analogRead(VRy1); 
  int valorX2 = analogRead(VRx2); 
  int valorY2 = analogRead(VRy2); 


  // Mapejo el valor dels joysticks a un valor comprensible pels ESCs
  int throttle = map(valorY1, 0, 1023, 1500, 1000); // Per a les proves utilitzo un rang de 1000-1500, però en el programa final he d'utilitzar 2000
  int yaw = map(valorX1, 0, 1023, -100, 100); 
  int roll = map(valorX2, 0, 1023, -100, 100);
  int pitch = map(valorY2, 0, 1023, 100, -100); 


  // Faig ús de l'algoritme Motor Mixing per a obtenir la velocitat de cada motor
  int ESC1 = throttle + pitch + roll + yaw; // Motor a dalt a l'esquerra (CCW)
  int ESC2 = throttle + pitch - roll - yaw; // Motor a dalt a la dreta (CW)
  int ESC3 = throttle - pitch + roll - yaw; // Motor a baix a l'esquerra (CCW)
  int ESC4 = throttle - pitch - roll + yaw; // Motor a baix a la dreta (CW)

  int velocitats[4] = {ESC1, ESC2, ESC3, ESC4}; 

  radio.write(velocitats, sizeof(velocitats)); // Envio la informació
}