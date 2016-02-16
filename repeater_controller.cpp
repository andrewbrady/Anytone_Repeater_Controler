#include <SoftwareSerial.h>

SoftwareSerial mySerial(50, 8); // RX, TX
int PTTPin = 42;
int SpeakerPin = 31;

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  Serial.println("KD0PBZ - Repeater Controller");
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
 
  pinMode(PTTPin, OUTPUT);
  pinMode(SpeakerPin, OUTPUT);

  /*
  digitalWrite(PTTPin, HIGH);
  Serial.println("TXON");
  delay(10000);
  digitalWrite(PTTPin, LOW);
  Serial.println("TXOFF");
  */
}


void loop() // run over and over
{
 
  //All we are going todo is read from serial (for the time being)
  byte read_byte = mySerial.read();  
  byte read_bytes[13];
  int read_byte_index = 0;
 
  //Start Byte ('S')
  if(read_byte == 0x53){

    Serial.print("RX -->:");
    Serial.print(read_byte,HEX);
    Serial.print(" ");
 
    read_bytes[read_byte_index] = read_byte;
    read_byte_index++;
   
    if(read_byte != 0xFF){
      while(1 == 1){     
        read_byte = mySerial.read();
        Serial.print(read_byte,HEX);
        Serial.print(" ");  
     
          read_bytes[read_byte_index] = read_byte;
        read_byte_index++;
       
        if(read_byte == 0x06){//This would indicate the end of the message
          Serial.println(" <--");
          break;
        } 
      }
    }
   
    Serial.print("RX:");
   
    for(int i = 0; i < sizeof(read_bytes); i = i + 1){ //Read the "Word"
          Serial.print(i);
          Serial.print("=>");
          Serial.print(read_bytes[i],HEX);        
          Serial.print(" ");
        }      
        Serial.println();
   
    //Now we decide how to parse or if its simple we can take action
    // In the Case of RX we are just going to send out RXON or RXOFF
    if(read_bytes[2] == 0x01){
          Serial.println("RXON");
          digitalWrite(PTTPin, HIGH);
          Serial.println("TXON");
     
        } else if(read_bytes[2] == 0x00){
          Serial.println("RXOFF");

          //Make a beep
          delay(500);
          for(int i=0; i<250; i++){
            digitalWrite(SpeakerPin, HIGH);
            delayMicroseconds(400);
            digitalWrite(SpeakerPin, LOW);
            delayMicroseconds(400);
          }
          delay(20);
          for(int i=0; i<250; i++){
            digitalWrite(SpeakerPin, HIGH);
            delayMicroseconds(600);
            digitalWrite(SpeakerPin, LOW);
            delayMicroseconds(600);
          }
         
          digitalWrite(PTTPin, LOW);
          Serial.println("TXOFF");
     
        } else {
      Serial.println("Unknown Message");
    }
   
   
  }
}
