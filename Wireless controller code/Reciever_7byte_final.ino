#include <RH_ASK.h>   // Käytetään Radiohead kirjastoa
#include <SPI.h>      // ^

RH_ASK driver;

uint8_t buf[7];
uint8_t bit[8];

void setup()
{
    Serial.begin(9600);

    // Jos driver ei vastaa, annetaan viesti
    if (!driver.init())
         Serial.println("init failed");
}

void loop()
{
  uint8_t buflen = sizeof(buf);   // buflen koko tavuina
  
  // Antaa true jos vastaanottaa viestin
  // Tallentaa viestin buf -muuttujaan
  if (driver.recv((uint8_t*)buf, &buflen))  
  {
    // Luetaan buf[6] bitit
    // tallennetaan näppäinten tila bit[] muuttujaan
    for(int i=0; i<8; i++){
      bit[i] = (buf[6] >> i) & 1U;
    }

    // Printataan buf[0]-buf[5]
    for(int i = 0; i<6; i++){
    Serial.print(buf[i]);
    Serial.print(",");  
    }

    // Printataan buf[6] kaikki bitit
    // Tallennettu bit[] listaan
    for(int i = 0; i<7; i++){
      Serial.print(bit[i]);
      Serial.print(",");
    }Serial.print(bit[7]);
  }
}
