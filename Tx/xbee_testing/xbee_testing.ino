#include "xbee_serial_array.h"
#include "RcTrainer.h"

//#include <Servo.h>



uint8_t wakeup = 9;
xbee xbeeSerial;
RcTrainer tx;

//0x0013a200, 0x4182da6b
//0x0013A200, 0x4182DA22
char xbeedir[8] = {0x00, 0x13, 0xA2, 0x00, 0x41, 0x82, 0xda, 0x22};
String anal;
String response;
uint8_t luz = 5;
uint8_t x;
uint8_t y;
uint8_t* packet = new uint8_t[2];

//Servo motor;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  xbeeSerial.begin(19200, xbeedir, wakeup);
  Serial.println("online");

}

// the loop function runs over and over again until power down or reset
void loop() {
//  xbeeSerial.sendata("Test");

//    x = map(analogRead(0),0,1023,0,180);
//    y = map(analogRead(1),0,1023,0,180);

    x = map(tx.getChannel(2),0,1023,0,180);
    y = map(tx.getChannel(1),-1023,1023,0,180);
    
    Serial.print("x:\t"); Serial.print(x); Serial.print("\ty:\t"); Serial.println(y);
    packet[0] = x; packet[1] = y;
    xbeeSerial.sendata(packet);
    
//  Serial.println("Sent");
//  delay(10);
//  xbeeSerial.xbee_in_array();//siempre recive datos


  while (Serial2.available()){
    Serial.print(Serial2.read(),HEX);
  }
  Serial.println();


//  if (xbeeSerial.xbee_detected()) {
//    response = xbeeSerial.getdata();
//    Serial.print("Response: ");
//    Serial.println(response);
//    analogWrite(44,atoi(response.c_str()));
//    if (response == "anal") {
//      Serial.println("Recibido");
//      anal = String(analogRead(luz));
//      xbeeSerial.sendata(anal);
//    }
//  }
  delay(30);
}
