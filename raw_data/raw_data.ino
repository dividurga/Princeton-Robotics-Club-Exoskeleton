////////////////////////////////////////////////////////////////////////
// Arduino Bluetooth Interface with Mindwave
// 
// This is example code provided by NeuroSky, Inc. and is provided
// license free.
//
// This modification allows view data trough serial monitor
// Further modification displays average of byteread values for a "smoother graph"
////////////////////////////////////////////////////////////////////////

#define LED 13
#define BAUDRATE 57600
#define DEBUGOUTPUT 0

#define powercontrol 10

// checksum variables
byte  generatedChecksum = 0;
byte  checksum = 0; 
int   payloadLength = 0;
byte  payloadData[64] = {0};
byte  poorQuality = 0;
byte  attention = 0;
byte  meditation = 0;
int attentionAvg[4];
int count;

// system variables
long    lastReceivedPacket = 0;
boolean bigPacket = false;

//////////////////////////
// Microprocessor Setup //
//////////////////////////
void setup(){
  pinMode(LED, OUTPUT);
  Serial1.begin(BAUDRATE);           // Serial port 1 (ATMEGA2560)
  Serial.begin(BAUDRATE);           // USB
}

////////////////////////////////
// Read data from Serial UART //
////////////////////////////////
byte ReadOneByte() {
  int ByteRead;
  while(!Serial1.available());
  ByteRead = Serial1.read();
  
  #if DEBUGOUTPUT  
    Serial.print((char)ByteRead);   // echo the same byte out the USB serial (for debug purposes)
  #endif
  
  return ByteRead;
}

/////////////
//MAIN LOOP//
/////////////
void loop() {
  // Look for sync bytes
  if(ReadOneByte() == 170) {
    if(ReadOneByte() == 170) {
      payloadLength = ReadOneByte();
      if(payloadLength > 169)                      //Payload length can not be greater than 169
      return;

      generatedChecksum = 0;        
      for(int i = 0; i < payloadLength; i++) {  
        payloadData[i] = ReadOneByte();            //Read payload into memory
        generatedChecksum += payloadData[i];
      }   

      checksum = ReadOneByte();                      //Read checksum byte from stream      
      generatedChecksum = 255 - generatedChecksum;   //Take one's compliment of generated checksum

        if(checksum == generatedChecksum) {    

        poorQuality = 200;
        attention = 0;
        meditation = 0;

        for(int i = 0; i < payloadLength; i++) {    // Parse the payload
          switch (payloadData[i]) {
          case 2:
            i++;            
            poorQuality = payloadData[i];
            bigPacket = true;            
            break;
          case 4:
            i++;
            attention = payloadData[i];                        
            break;
          case 5:
            i++;
            meditation = payloadData[i];
            break;
          case 0x80:
            i = i + 3;
            break;
          case 0x83:
            i = i + 25;      
            break;
          default:
            break;
          } // switch
        } // for loop

#if !DEBUGOUTPUT
        // *** Add your code here ***
        if(bigPacket) {
          if(poorQuality == 0)  digitalWrite(LED, HIGH);
          else  digitalWrite(LED, LOW);
          //Serial.print("Attention: ");
          attentionAvg[count] = attention; 
          //Serial.println(attention);
         // Serial.print("\n");
          //Serial.print("Meditation: ");
          //Serial.println(meditation);
          //Serial.print("\n");
          count++;
          if (count == 3) {
            Serial.print(0); // To freeze the lower limit
Serial.print(" ");
Serial.print(100); // To freeze the upper limit
Serial.print(" ");
            int sum =0;
            for (int i=0; i<3; i++){
               sum += attentionAvg[i];
            }
            //printing the average of the last four values
            Serial.println(sum/4);
            count = 0;
          }
        }
#endif        
        bigPacket = false;        
      }
      else {
        // Checksum Error
      }  // end if else for checksum
    } // end if read 0xAA byte
  } // end if read 0xAA byte
}
