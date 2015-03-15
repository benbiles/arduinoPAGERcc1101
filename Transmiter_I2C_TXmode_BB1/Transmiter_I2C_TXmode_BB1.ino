#include "EEPROM.h"
#include "cc1101.h"    //cc1101.flushTxFifo ();   // NOT SURE if ever NEEDED?
#include <Wire.h>  // I2C arduino library ( comes with arduino )
#include <LiquidCrystal_I2C.h>

// set the LCD address to 0x27 for a 20 chars 4 line display
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

CC1101 cc1101;

// counter to get increment in each loop
byte counter;
byte b;
byte syncWord = 199; // syncWord must be same ( pairing ? )

//LCD SCREEN SETUP

// SETUP HERE

void setup() 
{
  lcd.begin(20,4);   // initialize the lcd for 20 chars 4 lines, turn on backlight

  // SPLASH SCREEN one
  lcd.setCursor(0,0);
  lcd.print("Long Range Pager..");
  lcd.setCursor(0,1);
  lcd.print("Version 0.1");
  lcd.setCursor(0,2);
  lcd.print("ben biles & jack F.");
  lcd.setCursor(0,3);
  lcd.print("READY");
  delay(800);
  lcd.clear();
  // SPLASH SCREEN one END
    
// reset the counter
counter=0;

// initialize the RF Chip
lcd.print("initializing...");
cc1101.init();
cc1101.setSyncWord(&syncWord, false);
cc1101.setCarrierFreq(CFREQ_433);
cc1101.disableAddressCheck();
// cc1101.setTxPowerAmp(PA_LowPower);  if you want LOW power
delay(500);

//SPLASH SCREEN 2 - RF module status
lcd.setCursor(0,0);
lcd.print("CC1101_PARTNUM "); //cc1101=0
lcd.print(cc1101.readReg(CC1101_PARTNUM, CC1101_STATUS_REGISTER));
lcd.setCursor(0,1);
lcd.print("CC1101_VERSION "); //cc1101=4
lcd.print(cc1101.readReg(CC1101_VERSION, CC1101_STATUS_REGISTER));
lcd.setCursor(0,2);
lcd.print("CC1101_MARCSTATE ");
lcd.print(cc1101.readReg(CC1101_MARCSTATE, CC1101_STATUS_REGISTER) & 0x1f);
lcd.setCursor(0,4);
lcd.print("device initialized");
delay(800);
lcd.clear();
}
/// end void setup


// send_data() becomes a function to be used in void loop() 

void send_data() 
{

  CCPACKET data;
  data.length=2;
  byte blinkCount=counter++;
  byte blinkCount2=blinkCount*2;
  data.data[0]=blinkCount;
  data.data[1]=blinkCount2;
  
  delay(800);

// If there is data then print the packet number else print sent failed????
if(cc1101.sendData(data)){

lcd.setCursor(0,0);
lcd.print(blinkCount);
lcd.setCursor(4,0);
lcd.print(":A packets sent");

lcd.setCursor(0,1);
lcd.print(blinkCount2);
lcd.setCursor(4,1);
lcd.print(":B packets sent");

lcd.blink();
}else{
lcd.print("  sent failed :(");
}
}

// send data ( does this send all the data in state machine?? )

void loop()
{
send_data();
delay(2000);
}
