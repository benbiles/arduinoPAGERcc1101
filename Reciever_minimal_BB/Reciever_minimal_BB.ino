#include "EEPROM.h"
#include "cc1101.h"
#include <Wire.h>  // I2C arduino library ( comes with arduino )
#include <LiquidCrystal_I2C.h>

// set the LCD address to 0x27 for a 20 chars 4 line display
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address


// The connection to the hardware chip CC1101 the RF Chip
CC1101 cc1101;

byte b;
byte i;
byte syncWord = 199;
long counter=0;
byte chan=0;

// a flag that a wireless packet has been received
boolean packetAvailable = false;


/* Handle interrupt from CC1101 (INT0) gdo0 on pin2 */
void cc1101signalsInterrupt(void){
// set the flag that a package is available
packetAvailable = true;
}

// SETUP HERE

void setup()
{  
  lcd.begin(20,4);   // initialize the lcd for 20 chars 4 lines, turn on backlight
  lcd.clear();
  
// initialize the RF Chip
cc1101.init();
cc1101.setSyncWord(&syncWord, false);
cc1101.setCarrierFreq(CFREQ_433);
cc1101.disableAddressCheck(); //if not specified, will only display "packet received"
//cc1101.setTxPowerAmp(PA_LowPower);


attachInterrupt(0, cc1101signalsInterrupt, FALLING);   // need to find out what this does !! BB, initialise rf?
lcd.setCursor(0,1);
lcd.print("device initialized");
delay(800);
lcd.clear();
}

void loop()
{
// BLINK Packet Recieved if wanted
if(packetAvailable){ 
  detachInterrupt(0);
// clear the flag
packetAvailable = false;

CCPACKET packet;

if(cc1101.receiveData(&packet) > 0){
if(!packet.crc_ok) {
lcd.setCursor(0,3);
lcd.print("crc not ok");
delay(200);
lcd.setCursor(0,3);
lcd.print("          ");
}

if(packet.length > 0){

lcd.setCursor(0,0);
lcd.print(packet.data[0]);
lcd.setCursor(4,0);
lcd.print(":A packets RX");
lcd.blink();

lcd.setCursor(0,1);
lcd.print(packet.data[1]);
lcd.setCursor(4,1);
lcd.print(":B packets RX");
lcd.blink();

}
}
}
// Enable wireless reception interrupt
attachInterrupt(0, cc1101signalsInterrupt, FALLING);
}


