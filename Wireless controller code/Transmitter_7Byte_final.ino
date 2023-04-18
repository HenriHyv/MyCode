#include <RH_ASK.h>             // RadioHead libraryt
#include <SPI.h>                // ^


RH_ASK driver;                  // RH_ASK driver -muuttujaan

uint8_t msg[7];     // Lähetettävä viesti
uint8_t napitTavu;  // Nappien tilan tallentava tavu
uint8_t bitit[8];   // napitTavu -muuttujasta luetaan bitit tähän listaan


// Nappien pinnit
const int nappaimet[] = {2,3,7,8,9,10,11,13};

//Joystickkien pinnit
const int JoyStick_pin = 4;
const int X_pin = A1;
const int Y_pin = A0;
const int JoyStick_pin2 = 5;
const int X_pin2 = A3;
const int Y_pin2 = A4;

// Joystick vasen
int xc;                       // x-akselin suunta
int yc;                       // y-akselin suunta
int JSButton;                 // Joystickin klikki

//Joystick oikea
int xc2;                      // x
int yc2;                      // y
int JSButton2;                // klikki




void setup()
{
  Serial.begin(9600); // baudinopeus

  // Napit input modelle
  for(int i=0; i<8; i++){
    pinMode(nappaimet[i], INPUT_PULLUP);
    digitalWrite(nappaimet[i], HIGH);
  }
  

  // Joystick napit input modelle
  pinMode(JoyStick_pin, INPUT);
  digitalWrite(JoyStick_pin, HIGH);   // Napit HIGH -modeen. Kun nappia painetaan, se maadoittuun ja menee LOW
  pinMode(JoyStick_pin2, INPUT);
  digitalWrite(JoyStick_pin2, HIGH);
  
  
  // jos driver ei vastaa, printataan viesti
  if (!driver.init())
         Serial.println("init failed");
}



void loop()
{
  // Luetaan nappien tila 
  // Asetetaan napitTavu muuttujaan bitit erikseen
  for(int i=0; i<8; i++){
    SetBit(digitalRead(nappaimet[i]), i);
  }
  

  //luetaan joystickien y- ja x-arvot
  xc = LueJoystickArvo(X_pin);    // js1, x-arvo
  yc = LueJoystickArvo(Y_pin);    // js1, y-arvo
  xc2 = LueJoystickArvo(X_pin2);  // js2, x-arvo
  yc2 = LueJoystickArvo(Y_pin2);  // js2, y-arvo

  // Joystickien napit luetaan
  JSButton = digitalRead(JoyStick_pin);
  JSButton2 = digitalRead(JoyStick_pin2);

  // kutsutaan sendData funktiota
  sendData(xc, yc, JSButton, xc2, yc2, JSButton2, napitTavu);


}



// Lukee joystickien arvot ja kääntää ne luvuiksi 0, 1, tai 2
int LueJoystickArvo(int joystickArvo)
{
  int jsSuunta;
  int jsRaw = analogRead(joystickArvo);    // lukee yhden akselin arvon

  // Joystick keskellä, kun jsRaw == 512
  // Määrittää kummalla puolella akselia joystick on
  if (jsRaw < 500) {         
    jsSuunta = 0;             
  } else if (jsRaw >530) {   
    jsSuunta = 2;
  } else {
    jsSuunta = 1;
  }

  return jsSuunta;
}

// Asettaa jokaisen bitin erikseen uint8_t napitTavu -muuttujaan
void SetBit (int nappi, int napinLuku)
{
  // Jos nappia on painettu, asetetaan bitti 1
  if(nappi == 1){
    napitTavu |= 1UL << napinLuku;
  }
  // Muuten asetetaan bitti 0
  else{
    napitTavu &= ~(1UL << napinLuku);
  }
}



// Muuttaa lähetettävän datan uint8_t -muotoon ja lähettää radiolähettimellä
// Voisi tehdä myös listana, ja lähettää for-loopilla
void sendData(int xArvo1, int yArvo1, bool jsButton1, int xArvo2, int yArvo2, bool jsButton2, uint8_t napitTavu)
{
  // Annetaan arvot msg[] muuttujaan
  msg[0] = xArvo1;
  msg[1] = yArvo1;
  msg[2] = jsButton1;
  msg[3] = xArvo2;
  msg[4] = yArvo2;
  msg[5] = jsButton2;
  msg[6] = napitTavu;

  // Lähetetään msg[]
  driver.send((uint8_t*)msg, 7);
  driver.waitPacketSent();

  // Asetetaan näppäimet takaisin HIGH
  for(int i=0; i<8; i++){
    digitalWrite(nappaimet[i], HIGH);
  }



  /* DEBUGGAUSTA VARTEN, tulostetaan lähetettävä data serial monitoriin */
  for(int i=0; i<8; i++){
    bitit[i] = (napitTavu >> i) & 1U;
  }

  for(int i = 0; i<6; i++){
    Serial.print(msg[i]);
    Serial.print(",");  
  }

  for(int i = 0; i<7; i++){
    Serial.print(bitit[i]);
    Serial.print(",");
  }Serial.println(bitit[7]);
  /* DEBUGGAUSTA VARTEN */
}