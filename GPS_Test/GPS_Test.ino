/*
 * SODAQ GPS test
 */

#include <Arduino.h>
#include "SodaqUBloxGPS.h"

#define MySerial        SERIAL_PORT_MONITOR

#define DISPLAY_INTERVAL 10000

void setup()
{
    delay(3000);
    while (!SerialUSB) {
        // Wait for USB to connect
    }

    MySerial.begin(9600);

    digitalWrite(LED_RED, HIGH);
    pinMode(LED_RED, OUTPUT);
    digitalWrite(LED_GREEN, HIGH);
    pinMode(LED_GREEN, OUTPUT);
    digitalWrite(LED_BLUE, HIGH);
    pinMode(LED_BLUE, OUTPUT);

    MySerial.println("SODAQ LoRaONE test_gps is starting ...\n");

    sodaq_gps.init(GPS_ENABLE);
    sodaq_gps.on();
    delay(500);

    // This is for debugging to see more details, more messages
    // Use this in combination with setDiag()
    //sodaq_gps.setMinNumOfLines(10);

    // Uncomment the next line if you want to see the incoming $GPxxx messages
    //sodaq_gps.setDiag(MySerial);
    
}

void loop()
{
    byte led = LED_RED;
    long nextShowTime = millis() + DISPLAY_INTERVAL;
    
    while(1) {
        if (sodaq_gps.task()) {
            digitalWrite(led, LOW);
        } else {
            digitalWrite(led, HIGH);
        }
        
        if (sodaq_gps.hasFix()) {
            digitalWrite(LED_RED, HIGH);
            led = LED_GREEN;
        } else {
            digitalWrite(LED_GREEN, HIGH);
            led = LED_RED;
        }

        if (millis() >= nextShowTime) {
            show();
            nextShowTime = millis() + DISPLAY_INTERVAL;
        }
    }

}

void show()
{
    if (sodaq_gps.hasFix()) {
        MySerial.println(String(" datetime = ") + sodaq_gps.getDateTimeString());
        MySerial.println(String(" lat = ") + String(sodaq_gps.getLat(), 7));
        MySerial.println(String(" lon = ") + String(sodaq_gps.getLon(), 7));
        MySerial.println(String(" num sats = ") + String(sodaq_gps.getNumberOfSatellites()));
        MySerial.println(String(" speed = ") + String(sodaq_gps.getSpeed()));
        MySerial.println(String(" HDOP = ") + String(sodaq_gps.getHDOP()));
    } else {
        MySerial.println("No Fix");
        MySerial.println(String(" GPS datetime = ") + sodaq_gps.getDateTimeString());
    }
}


