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
byte chan=1;

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

//-------- Write characters on the display ------------------ 
  lcd.setCursor(0,0); //Start at character 4 on line 0
  lcd.print("Long Range Pager..");
  lcd.setCursor(0,1);
  lcd.print("Version 0.1"); // Print text on 2nd Line
  lcd.setCursor(0,2);
  lcd.print("ben biles & jack F."); //Print 20 characters on 3rd line
  lcd.setCursor(0,3);
  lcd.print("READY");
  delay(800);
  lcd.clear();
  
// initialize the RF Chip
cc1101.init();

cc1101.setSyncWord(&syncWord, false);  // turn this on to check network address?
cc1101.setCarrierFreq(CFREQ_433);
cc1101.disableAddressCheck(); //if not specified, will only display "packet received"
//cc1101.setTxPowerAmp(PA_LowPower);

lcd.clear();
lcd.setCursor(0,0);
lcd.print("CC1101_PARTNUM "); //cc1101=0
lcd.print(cc1101.readReg(CC1101_PARTNUM, CC1101_STATUS_REGISTER));
lcd.setCursor(0,1);
lcd.print("CC1101_VERSION "); //cc1101=4
lcd.print(cc1101.readReg(CC1101_VERSION, CC1101_STATUS_REGISTER));
lcd.setCursor(0,2);
lcd.print("CC1101_MARCSTATE ");
lcd.print(cc1101.readReg(CC1101_MARCSTATE, CC1101_STATUS_REGISTER) & 0x1f);

attachInterrupt(0, cc1101signalsInterrupt, FALLING);   // need to find out what this does !! BB, initialise rf?

lcd.setCursor(0,3);
lcd.print("device initialized");
delay(800);
lcd.clear();
}
// END CC1101 RF MODULE SETUP


// READ LQI values from RF module and print to LCD screen
void ReadLQI()
{
byte lqi=0;
byte value=0;
lqi=(cc1101.readReg(CC1101_LQI, CC1101_STATUS_REGISTER));
value = 0x3F - (lqi & 0x3F);
lcd.setCursor(0,2);
lcd.print("LQI quality ");
lcd.print(value);
}

//READ RSSI values from RF module and print to LCD screen & serial console

void ReadRSSI()
{
byte rssi=0;
byte value=0;

rssi=(cc1101.readReg(CC1101_RSSI, CC1101_STATUS_REGISTER));

if (rssi >= 128)
{
value = 255 - rssi;
value /= 2;
value += 74;
}
else
{
value = rssi/2;
value += 74;
}
lcd.setCursor(0,3);
lcd.print("RSSI quality ");
lcd.print(value);
}

void loop()
{

// BLINK Packet Recieved if wanted

if(packetAvailable){
// lcd.setCursor(0,0); // set cursor on lcd
// lcd.print("Packet Recieved "); // print packet recieved to LCD
// delay(200);
// lcd.setCursor(0,0);
// lcd.print("                    ");

// Disable wireless reception interrupt
detachInterrupt(0);

ReadRSSI();
ReadLQI();
// clear the flag
packetAvailable = false;

CCPACKET packet;

if(cc1101.receiveData(&packet) > 0){
if(!packet.crc_ok) {
lcd.setCursor(0,1);
lcd.print("crc not ok");
delay(200);
lcd.setCursor(0,1);
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


