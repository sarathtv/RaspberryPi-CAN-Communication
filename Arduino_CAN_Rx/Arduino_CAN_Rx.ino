/*
Rpi will send a CAN msg with ID 0x13
check the value of the first byte 
if it is 0x01 - ACTIVATE RELAY
if it is 0x00 - DEACTIVATE RELAY
*/
#include <CAN.h>
#include <Arduino.h>



const int RelayPin =  8;
float temperature,humidity;
unsigned char buff[8];
bool prev_status=false;
bool current_stat =false;
void setup() {
  pinMode(RelayPin, OUTPUT);
  digitalWrite(RelayPin, 1);
  Serial.begin(9600);
  while (!Serial);
  Serial.println("CAN Receiver Callback");
  // start the CAN bus at 500 kbps
  if (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }
   // register the receive callback
  CAN.onReceive(onReceive);
}
void loop() {
  //check if buff[0]&0x01 == 0x01
  if(current_stat!=prev_status)
  {
    prev_status=current_stat;
    digitalWrite(RelayPin, !current_stat);
   
  }
  
  
}
void onReceive(int packetSize) {
  long  id= CAN.packetId();
 if (id == 0x13)
 {
  //Serial.print("ID 12");
  int availbyte=CAN.available();
  for (int i=0;i<availbyte;i++){
    buff[i]=(unsigned char)CAN.read();
    Serial.print(buff[i],HEX);
    Serial.print("\t");
  }
  Serial.println();
    current_stat = (buff[0] & 0x01);
 }

}
