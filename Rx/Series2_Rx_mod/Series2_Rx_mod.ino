/**
 * Copyright (c) 2009 Andrew Rapp. All rights reserved.
 *
 * This file is part of XBee-Arduino.
 *
 * XBee-Arduino is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * XBee-Arduino is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with XBee-Arduino.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Code adapted for the purposes of the Calvin Mars Rover Design Team by
 * @author: Joel Muyskens
 * @author: Levi Vande Kamp
 * @author: Duncan Van Keulen
 * 
 * This code is intended to work on an Arduino on the rover (recieving)
 * connected by PWM
 */
 
#include <XBee.h>
#include <Servo.h>

/*
This example is for Series 2 XBee
Receives a ZB RX packet and sets a PWM value based on packet data.
Error led is flashed if an unexpected packet is received
*/

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();

// create reusable response objects for responses we expect to handle 
ZBRxResponse rx = ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();

// create Servo objects for controlling the motors
Servo motor1;
Servo motor2;
Servo motor3;
Servo motor4;
Servo motor5;
Servo motor6;

// initialize variables for the values of the left and right sides
int left;
int right;

void setup() {

  // start serial
  Serial.begin(9600);
  Serial2.begin(19200);
  xbee.begin(Serial2);
  motor1.attach(7);
  motor2.attach(6);
  motor3.attach(5);
  motor4.attach(8);
  motor5.attach(9);
  motor6.attach(10);
}

/* function for setting the motor value of the right side (tank drive)
 * precondition: recieved a motor value
 * postcondition: motor controllers will recieve PWM value and turn the motors
 */
void rightWrite(int value){
  motor1.write(value);
  motor2.write(value);
  motor3.write(value);
}

/* function for setting the motor value of the left side (tank drive)
 * precondition: recieved a motor value
 * postcondition: motor controllers will recieve PWM value and turn the motors
 */
void leftWrite(int value){
  motor4.write(value);
  motor5.write(value);
  motor6.write(value);
}

// continuously reads packets, looking for ZB Receive or Modem Status
void loop() {
    
    xbee.readPacket();
    
    if (xbee.getResponse().isAvailable()) {
      // got something
      if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
        // got a zb rx packet
        
        // now fill our zb rx class
        xbee.getResponse().getZBRxResponse(rx);
        
        if (rx.getOption() == ZB_PACKET_ACKNOWLEDGED) {
            // the sender got an ACK
        } else {
            // we got it (obviously) but sender didn't get an ACK
        }
        // set dataLed PWM to value of the first byte in the data
        Serial.println(rx.getData(0),DEC);
        left = rx.getData(0);
        right = rx.getData(1);
        if (left >= 88 && left <= 92){
          left = 90;
        }
        if (right >= 88 && right <= 92){
          right = 90;
        }
        Serial.print("Left:\t"); Serial.print(left); Serial.print("\tRight\t"); Serial.println(right);
        leftWrite(left);
        rightWrite(right);

      } else if (xbee.getResponse().getApiId() == MODEM_STATUS_RESPONSE) {
        xbee.getResponse().getModemStatusResponse(msr);
        // the local XBee sends this response on certain events, like association/dissociation
        
        if (msr.getStatus() == ASSOCIATED) {
          // yay this is great.  flash led
        } else if (msr.getStatus() == DISASSOCIATED) {
          // this is awful.. flash led to show our discontent
        } else {
          // another status
        }
      } else {
      	// not something we were expecting
      }
    } else if (xbee.getResponse().isError()) {
      //nss.print("Error reading packet.  Error code: ");  
      //nss.println(xbee.getResponse().getErrorCode());
    }
}
