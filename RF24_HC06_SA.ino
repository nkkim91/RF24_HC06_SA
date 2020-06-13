/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/**
 * Channel scanner
 *
 * Example to detect interference on the various channels available.
 * This is a good diagnostic tool to check whether you're picking a
 * good channel for your application.
 *
 * Inspired by cpixip.
 * See http://arduino.cc/forum/index.php/topic,54795.0.html
 */

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"
#include <SoftwareSerial.h>

SoftwareSerial BT(7, 8);

//
// Hardware configuration
//

// Set up nRF24L01 radio on SPI bus plus pins 7 & 8

RF24 radio(9,10);

char bt_tx_buf[256];

//
// Channel info
//

const uint8_t num_channels = 126;
uint8_t values[num_channels];

//
// Setup
//
char ch; // stores incoming character from other device
int ch_id, signal_strength;


void setup(void)
{
  //
  // Print preamble
  //
  BT.begin(9600);
  Serial.begin(115200);
  printf_begin();

  //
  // Setup and configure rf radio
  //

  radio.begin();
  radio.setAutoAck(false);

  // Get into standby mode
  radio.startListening();
  radio.stopListening();

//  radio.printDetails();
}

//
// Loop
//

const int num_reps = 100;

void loop(void)
{
  int i, j;
  
  // Clear measurement values
  memset(values,0,sizeof(values));
  
  // Scan all channels num_reps times
  for(i=0; i < num_reps; i++ )
  {
    for( j= num_channels; j > 0; j--)
    {
      // Select this channel
      radio.setChannel(j);

      // Listen for a little
      radio.startListening();
      delayMicroseconds(128);
      radio.stopListening();

      // Did we get a carrier?
      if ( radio.testCarrier() ){
        ++values[j];
      }
    }
  }

  for(ch_id=0; ch_id < num_channels; ch_id++) {
      sprintf(bt_tx_buf,"#*%03d:%03d", ch_id % 128, values[ch_id]*10);
      BT.write(bt_tx_buf);
//      Serial.write(bt_tx_buf);
      delay(50);
  }
//  delay(500);
}

// vim:ai:cin:sts=2 sw=2 ft=cpp
