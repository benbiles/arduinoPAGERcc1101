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
byte syncWord = 199;   // sender and reciever must have same!!! like ip address?

//LCD SCREEN SETUP

// SETUP HERE
void setup() 
{
  lcd.begin(20,4);   // initialize the lcd for 20 chars 4 lines, turn on backlight
// reset the counter
counter=0;

// initialize the RF Chip
lcd.print("initializing...");
cc1101.init();
cc1101.setSyncWord(&syncWord, false);
cc1101.setCarrierFreq(CFREQ_433);
cc1101.disableAddressCheck();
cc1101.setTxPowerAmp(PA_LongDistance);  // if you want HIGH POWER
delay(500);
lcd.clear();
}
/// end void setup

// send_data() becomes a function to be used in void loop() 

void send_data() 
{
  
  cc1101.flushTxFifo ();  // is this needed???
  
  CCPACKET data;
  data.length=2;
  byte blinkCount=counter++;
  byte blinkCount2=blinkCount*2;
  
  data.data[0]=blinkCount;
  data.data[1]=blinkCount2;
  
  if(cc1101.sendData(data)){
    
    delay(1000);

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
lcd.setCursor(0,3);
lcd.print("  sent failed :(");
delay(200);
lcd.setCursor(0,3);
lcd.print("                 ");
}
}

// send data ( does this send all the data in state machine?? )

void loop()
{
send_data();
delay(2000);
}
