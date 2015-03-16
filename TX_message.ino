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
byte packet_num = 1;
//make the message array and make it into HEX code so I can transmit???
// char pagertext[2] = {"A"};  make an array ?? 

char arrayA[81] = { " alright! welcome to the begining of the awsome long ranger pager project :) !!!" };
byte arrayB[81] = {};

// SETUP HERE
void setup() 
{
  lcd.begin(20,4);   // initialize the lcd for 20 chars 4 lines, turn on backlight

// load char arrayA into BYTE arrayB

 {
 for( int i = 1 ; i < 81 ; ++i ){
    arrayB[ i ] = arrayA[ i ];
  }
}

counter=1;  // start counter at zero !

// initialize the RF Chip
lcd.print("initializing......");
cc1101.init();
cc1101.setSyncWord(&syncWord, false);
cc1101.setCarrierFreq(CFREQ_433);
cc1101.disableAddressCheck();
cc1101.setTxPowerAmp(PA_LongDistance);  // if you want HIGH POWER
delay(500);
lcd.clear();
}
 // cc1101.flushTxFifo ();  // is this needed???
/// end void setup



// send_data() becomes a function to be used in void loop() 
void send_data()
{
  CCPACKET data;
  data.length=21; //packet data size
  packet_num=counter++; // make packet_num a byte and load it with counter value
  data.data[0]=packet_num; // packet address
  
  
  // load character 'byte numbers' to packet( there are 4 packets 1 -4 defined by packet_num )
   
 if (packet_num == 1)
   {
 for( int i = 1 ; i < 21 ; ++i ){
    data.data[i]=arrayB[i];
  }
}
   
 if (packet_num == 2)
   {
 for( int i = 21 ; i < 41 ; ++i ){
    data.data[i-20]=arrayB[i];
  }
}
 
 if (packet_num == 3)
   {
 for( int i = 41 ; i < 61 ; ++i ){
    data.data[i-40]=arrayB[i];
  }
}
 
 if (packet_num == 4)
   {
 for( int i = 61 ; i < 81 ; ++i ){
    data.data[i-60]=arrayB[i];
  }
}
 
  
// print the characters from the byte char number to screen in 20 characters at a time
// Print LCD screen lines 1 & 2

   if(cc1101.sendData(data)){  ///This sends the data ' including the packet address ?
  delay(20); 
 }
   
  // print it too the LCD screen 
     lcd.setCursor(0,packet_num-1);
 {
 for( int i = 1 ; i < 21 ; ++i ){
    lcd.write(data.data[i]);
  }
  
  // print packet number
  lcd.setCursor(19,4);
  lcd.print(packet_num);
 
if (counter >= 5)
{
  counter=1;
  delay(10000);  //wait 10 seconds between send
  lcd.clear();     // clear screen before each send
  
}
  
}}
   
// send data with the send_data() function described above.,

void loop()
{
send_data();
delay(10);
}
