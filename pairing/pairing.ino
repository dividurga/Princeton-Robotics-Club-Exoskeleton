#include <SoftwareSerial.h>
SoftwareSerial BT(11,10); //Rx/Tx
 
void setup(){
  BT.begin(38400);
  Serial.begin(9600);
}
 
void loop(){
  if( BT.available() ) Serial.write( BT.read() );
  if( Serial.available() ) BT.write(Serial.read());
}
//E0:7D:EA:E5:FF:B9