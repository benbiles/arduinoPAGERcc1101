# arduinoPAGERcc1101

A Arduino UNO 80 character pager project with 80 character screen using cc1101 RF module to transmit message.

wiring up the RF module. Generally the CC1101 RF modules are 3.3v NOT 5V so logic level translator is needed with arduino
UNO

I got wiring info here:

http://labalec.fr/erwan/?p=497

We are using I2C version of 80 character screen to save pins on arduino for SPI connectiomn to RF module !


Using the cc1101 RF module we can send 4 lines of text in 4 20 byte packets to another arduino

we are using the Liquid crystal library and I2C interface for screen

we are using the PANSTAMP library for the CC1101 module with SPI

We will be adding a USB host for keyboard next ( still looking for a smaller qwerty blackberry style keyboard instead ! )

Also will be writing some kind of menu system for creating / sending messages..

I will post a full wiring diagram when we work out a satifactory keyboard! currently working on a USB host for any qwerty
keyboard. this is too large to use as a pager !!

currently the PAGER message is manually entered into the array in the code :(

Instructions.

1, wire up 2 arduino Uno's with 80 character screens with I2C
2, connect CC1101 RF modules with SPI
3, get Liquidcrystal library
4, get PanStamp Library ( correct version ) will upload here soon
5, load code on TX arduino
6, load code on RX arduino

7, change text in array to whatever you like?

Text is sent in 4 packets.. one line at a time. if the reciever is switched on as TX arduino is mid transmit it will possibley recieve line 2 , 3 or 4 before line 1 ? re transmit interval set by simple delay
















