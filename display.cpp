#include "display.h"
#include <SoftwareWire.h>
SoftwareWire Wire(7, 8);   // SDA ja SCL uudelleen määritys koska tehtävän annossa käskettiin käyttää A4 ja A5 muuhun
//lcd kirjastoja jotka tukevat SoftwareWireä
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>
hd44780_I2Cexp lcd; //luodaan näyttö olio

//määritellään pinnit

int nollaus = 13;        //srclr
int siirtoKello = 12;   //srcclk
int lukitusKello = 11;  //rclk
int ulos = 10;          //oe
int sarjaTulo = 9;     //ser
const int wakeUpPin = 6; //hetätykseen

//määritellään mitkä ledit minkäkin numeron kohdalla pitää sytyttää
uint8_t numerot[] = {
  0b00111111, //0
  0b00000110, //1
  0b01011011, //2
  0b01001111, //3
  0b01100110, //4
  0b01101101, //5
  0b01111101, //6
  0b00000111, //7
  0b01111111, //8
  0b01101111  //9
};

void initializeDisplay(void)
{
//määritellään pinnit outputeiksi
pinMode(nollaus, OUTPUT);
pinMode(siirtoKello, OUTPUT);
pinMode(lukitusKello, OUTPUT);
pinMode(ulos, OUTPUT);
pinMode(sarjaTulo, OUTPUT);

//määritellään lähtöarvot
digitalWrite(nollaus, HIGH);
digitalWrite(siirtoKello, LOW);
digitalWrite(lukitusKello, LOW);
digitalWrite(ulos, LOW);
digitalWrite(sarjaTulo, LOW);
Wire.begin();
lcd.begin(16, 2);
lcd.print("SPEDEN SPELIT V2.0!!");
delay(1000);

//alustetaan lcd näyttö

}

void writeByte(uint8_t bits,bool last)
{
for (int i = 7; i >= 0; i--)
{
  digitalWrite(sarjaTulo, (bits >> i) & 1);
  digitalWrite(siirtoKello,HIGH);
  delay(1);
  digitalWrite(siirtoKello,LOW);
}
if (last)
{
  digitalWrite(lukitusKello,HIGH);
  delay(1);
  digitalWrite(lukitusKello,LOW);
}
}
void writeHighAndLowNumber(uint8_t kymmenet,uint8_t ykkoset)

{
  writeByte(numerot[kymmenet], false);
  writeByte(numerot[ykkoset], true);
}

void showOnSevenSegment(uint16_t numero)

{
  uint8_t kymmenet = (numero / 10) % 10;
  uint8_t ykkoset = numero % 10;
  writeHighAndLowNumber(kymmenet, ykkoset);
}

void showOnLcd(uint16_t numero)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("tulos:");
  lcd.setCursor(0,1);
  lcd.print(numero); 

}


void showNumber(uint16_t numero)
{
  showOnSevenSegment(numero);
  showOnLcd(numero);

}

void showScoreBoard(uint16_t scores[3]) 
{ 
  writeHighAndLowNumber(0,0);
  lcd.clear();
  lcd.setCursor(5,0);
  lcd.print("1#");
  lcd.print(scores[0]);
  lcd.setCursor(1,1);
  lcd.print("2#");
  lcd.print(scores[1]);
  lcd.setCursor(9,1);
  lcd.print("3#");
  lcd.print(scores[2]);

}
void textOnLcd(String teksti)
{
  int lcdLeveys=16;
  
  int tekstiPituus = teksti.length();
  if (tekstiPituus > 16)
  {
  for (int i = 0; i <= tekstiPituus; i++) 
  {
    lcd.setCursor(0,0);
    String nayttoTeksti = teksti.substring(i);
    if (nayttoTeksti.length() < lcdLeveys) 
    {
      nayttoTeksti += teksti.substring(0, lcdLeveys - nayttoTeksti.length());
    }
    lcd.print(nayttoTeksti);
    delay(300);
  }
  }
  else
  {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(teksti);
  }

}

void lcdPowerSave(bool enable) 
{
  if (enable) {
    lcd.noBacklight();
    lcd.clear();
  } else {
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("SPEDEN SPELIT V2.0");
  }
}

void sevenSegmentPowerSave(bool enable) {
  if (enable) {
    digitalWrite(ulos, HIGH);     
    digitalWrite(nollaus, LOW);   
    delay(1);
    digitalWrite(nollaus, HIGH);  
  } else {
    digitalWrite(ulos, LOW);      
  }
}


void enterSleepMode() 
{
  lcdPowerSave(true);
  sevenSegmentPowerSave(true); 
  pinMode(wakeUpPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(wakeUpPin), wakeUp, FALLING);

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_cpu();

  sleep_disable();
  detachInterrupt(digitalPinToInterrupt(wakeUpPin));
  lcdPowerSave(false);
  sevenSegmentPowerSave(false);
}

void wakeUp() 
{

}


