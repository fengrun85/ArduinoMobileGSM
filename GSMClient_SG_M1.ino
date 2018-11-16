/*

  Udp NTP Client

  Get the time from a Network Time Protocol (NTP) time server
  Demonstrates use of UDP sendPacket and ReceivePacket
  For more on NTP time servers and the messages needed to communicate with them,
  see http://en.wikipedia.org/wiki/Network_Time_Protocol

  created 4 Sep 2010
  by Michael Margolis
  modified 9 Apr 2012
  by Tom Igoe

  modified 6 Dec 2017 ported from WiFi101 to MKRGSM
  by Arturo Guadalupi
 
  This code is in the public domain.

*/

#include <MKRGSM.h>
#include <Wire.h>
#include <SeeedOLED.h>
#include <RTCZero.h>

#include "arduino_secrets.h"
// Please enter your sensitive data in the Secret tab or arduino_secrets.h
// PIN Number
const char PINNUMBER[]     = SECRET_PINNUMBER;
// APN data
const char GPRS_APN[]      = SECRET_GPRS_APN;
const char GPRS_LOGIN[]    = SECRET_GPRS_LOGIN;
const char GPRS_PASSWORD[] = SECRET_GPRS_PASSWORD;



// initialize the library instance
GSMClient client;
GPRS gprs;
GSM gsmAccess;

// A UDP instance to let us send and receive packets over UDP
GSMUDP Udp;

//For RTC Time
RTCZero rtc;
unsigned long epoch = 0;
const byte timeZone = 8; // Setting for GMT offset in hours
const unsigned short SECS_PER_HOUR = 3600;
unsigned long localSgTime = 0;


void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  /* while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  } */

  Wire.begin();
  SeeedOled.init();  //initialze SEEED OLED display

  SeeedOled.clearDisplay();          //clear the screen and set start position to top left corner
  SeeedOled.setNormalDisplay();      //Set display to normal mode (i.e non-inverse mode)
  SeeedOled.setPageMode();           //Set addressing mode to Page Mode
  SeeedOled.setTextXY(0,0);          //Set the cursor to Xth Page, Yth Column  
  SeeedOled.putString("Screen init"); //Print the String

  Serial.println("Starting Arduino GPRS NTP client.");
  // connection state
  bool connected = false;

  // After starting the modem with GSM.begin()
  // attach the shield to the GPRS network with the APN, login and password
  while (!connected) {
    if ((gsmAccess.begin(PINNUMBER) == GSM_READY) &&
        (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD) == GPRS_READY)) {
      connected = true;
    } else {
      Serial.println("Not connected");
      delay(1000);
    }
  }

  Serial.println("\nStarting connection to server...");

  epoch = gsmAccess.getTime();
  Serial.print("GSM Epoch : ");
  Serial.println(epoch);
  
  localSgTime = epoch + (timeZone * SECS_PER_HOUR);

  rtc.begin(); // initialize RTC
  rtc.setEpoch(localSgTime); // Setting epoch to SG Time
}

void loop()
{
 
  Serial.print(rtc.getDay());
  Serial.print("/");
  Serial.print(rtc.getMonth());
  Serial.print("/");
  Serial.print(rtc.getYear());
  Serial.print("\t");

  // ...and time
  print2digits(rtc.getHours());
  Serial.print(":");
  print2digits(rtc.getMinutes());
  Serial.print(":");
  print2digits(rtc.getSeconds());

  Serial.println();


  SeeedOled.putString("Hello World!"); //Print the String
  // wait ten seconds before asking for the time again
  delay(30000);
}


void print2digits(int number) {
  if (number < 10) {
    Serial.print("0");
  }
  Serial.print(number);
}